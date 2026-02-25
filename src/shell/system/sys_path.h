#include <string.h>
#include <vector>

#ifdef __APPLE__
    inline std::vector<std::string> system_paths = {
        "/bin",
        "/sbin",
        "/usr/bin",
        "/usr/sbin",
        "/usr/local/bin"
    };

    python_path = {

    }

    go_path = {

    }

    java_path = {

    }
    

#endif