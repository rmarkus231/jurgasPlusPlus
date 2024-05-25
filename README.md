# jurgas++
C++ recreation/rebuild of the infamous UT student program.

Does not include the webscraper part, just the optimizations of the search feature and data storage mainly.

Compiling the program required wxWidgets. I had it in my C:\ directory so that is also the one listed in the visual studio project and sln file.

The sqlite used i the program is included with the files.

The DLL's included in the directory are needed for the program to function on windows.


Compiled with:

- wxWidgets 3.2.4
- Microsoft Visual Studio 2022 version 17.10.0
- C++14


On both:

- Intel core i5-8265U
- Ryzen 9 5900x

To build with visual studio:

- git clone or get download the .zip
- download and compile libraries for wxWidgets.
- link them in the solution file.
- makesure the runtime environments are set to multitreaded dll and multithreaded dll debug, set to that by defualt but incase it is changed. For a guide on how, I reccomend this [guide](https://www.codtronic.com/blog/windows/building-wxwidgets-applications-using-microsoft-visual-studio/).
- include wxwidgets directories in the solution and make sure the linker can see the correct dll's.

To build elsewhere:

-idk, figure it out ¯\_(ツ)_/¯
