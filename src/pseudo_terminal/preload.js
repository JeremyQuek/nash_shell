const { contextBridge, ipcRenderer } = require("electron");

contextBridge.exposeInMainWorld("electronAPI", {
  sendInput: (input) => ipcRenderer.send("user-input", input),
  onOutput: (callback) =>
    ipcRenderer.on("proc-output", (event, data) => callback(data)),
  onClear: (callback) => ipcRenderer.on("clear-screen", (event) => callback()),
  // Add this:
  onSetUsername: (callback) =>
    ipcRenderer.on("set-username", (event, name) => callback(name)),
});
