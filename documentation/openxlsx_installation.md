# Installation of OpenXLSX
### Clone repo
```shell
git clone https://github.com/troldal/OpenXLSX.git
cd OpenXLSX/OpenXLSX
```

#### Create cmake build directory
```shell
mkdir build
cd build
cmake .. -A x64
```

#### Build and install
```shell
# For Debug
cmake --build . --target OpenXLSX --config Debug
cmake --install . --prefix path_to_install_debug

# For Release
cmake --build . --target OpenXLSX --config Release
cmake --install . --prefix path_to_install_release
```
e.g.
```shell
cmake --build . --target OpenXLSX --config Debug
cmake --install . --prefix "C:\Program Files\OpenXLSX.Library\Debug"

cmake --build . --target OpenXLSX --config Release
cmake --install . --prefix "C:\Program Files\OpenXLSX.Library\Release"
```

### Setup visual studio config
To use the OpenXLSX library in your Visual Studio project, you need to configure your project to recognize the library's include headers and link against the library binaries. Here’s a step-by-step guide to set up your Visual Studio project with OpenXLSX:

##### 1. Open Your Project in Visual Studio
Open your existing project or create a new one in Visual Studio.

##### 2. Configure Project Properties
Once your project is open, you need to configure it to recognize the OpenXLSX library.

##### a. Open Project Properties
Right-click on your project in the Solution Explorer and select `Properties`.

##### b. Set Configuration
Ensure that the Configuration is set to `All Configurations` unless you want to specify different settings for Debug and Release.

##### c. Add Include Directories
Navigate to `Configuration Properties -> VC++ Directories -> Include Directories`. Add the path to the `include` directory of the OpenXLSX library, likely something like:
```
C:\Program Files (x86)\OpenXLSX.Library\include
```

##### d. Add Library Directories
Navigate to `Configuration Properties -> VC++ Directories -> Library Directories`. Add the path to the `lib` directory of the OpenXLSX library, likely something like:
```
C:\Program Files (x86)\OpenXLSX.Library\lib
```

##### e. Link against the Library
Navigate to `Configuration Properties -> Linker -> Input -> Additional Dependencies`. Add the name of the OpenXLSX library file, which is likely something like:
```
OpenXLSX.lib
```

#### 3. Write Code
Now, you should be able to include OpenXLSX headers in your source files and use the library, like so:
```cpp
#include <OpenXLSX/OpenXLSX.hpp>

int main() {
    // Your code using OpenXLSX here
}
```

#### 4. Build and Run
After configuring, you can build and run your project. If there are any issues, double-check the paths and names you have entered in the project properties.

#### Note:
- The exact names of the include and library directories may vary based on how OpenXLSX is installed, so adjust them as per your installation.
- If you are building a 64-bit application, make sure to install the 64-bit version of the library, or vice versa for 32-bit applications, to avoid conflicts.
- The steps might slightly vary depending on the version of Visual Studio you are using, but they should be somewhat similar.