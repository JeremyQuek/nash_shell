const { app, BrowserWindow, ipcMain } = require("electron");
const { spawn } = require("child_process");
const path = require("path");

let mainWindow;
let shellProc;
let shellBin = "../src/shell/shell";

const os = require("os"); 

//tdl:
// Display current directory
// Make the shell wait until parent completes
// allow continuous writes to cout

function createWindow() {
  mainWindow = new BrowserWindow({
    width: 900,
    height: 600,
    webPreferences: { preload: path.join(__dirname, "preload.js") },
  });

  mainWindow.loadFile("index.html");

  // Send the username once the page finishes loading
  mainWindow.webContents.on("did-finish-load", () => {
    const userName = os.userInfo().username.toUpperCase();
    mainWindow.webContents.send("set-username", userName);
  });

  // Start your binary
  //todo fix shell env path
  shellProc = spawn(shellBin);

  shellProc.stdout.on("data", (data) => {
    mainWindow.webContents.send("proc-output", data.toString());
  });

  
  shellProc.stderr.on("data", (data) => {
    // We send it to the same "proc-output" channel so it shows up in your terminal UI
    mainWindow.webContents.send("proc-output", data.toString());
  });



  // Receive input from the UI and send to the binary
  ipcMain.on("user-input", (event, input) => {
    const command = input.trim();
    const lowerCmd = command.toLowerCase();

    if (lowerCmd === "clear") {
      mainWindow.webContents.send("clear-screen");
    }
    // Example: If you want to handle specific commands and error on others
    else if (lowerCmd === "exit" || lowerCmd === "quit" || lowerCmd === "q") {
      app.quit();
    } else {
      if (shellProc && !shellProc.killed) {
        shellProc.stdin.write(input + "\n");
      }
    }
  });
}

app.whenReady().then(createWindow);

// 1. Handle the window closing
app.on("window-all-closed", () => {
  if (shellProc) {
    shellProc.stdin.pause(); // Stop accepting input
    shellProc.kill("SIGTERM"); // Politely ask it to die
  }

  // On macOS, it's standard to stay active, but for a terminal,
  // you likely want it to quit entirely:
  if (process.platform !== "darwin") {
    app.quit();
  }
});

// 2. The absolute final safety net
app.on("will-quit", () => {
  if (shellProc) {
    shellProc.kill("SIGKILL"); // Force kill if it's still hanging
  }
});
