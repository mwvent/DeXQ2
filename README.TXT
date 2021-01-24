DeXQ2
-----

This project is to make a version of Quake 2 for Android that runs well as a desktop application with full mouse/keyboard play support.

I forked this project to learn about Android Applications ( I have never worked with developing for Android before ) and also because I seem to use my PC less and less these days in favour of Android but there is a lack of games that have proper mouse support.

The current status of this project is it can be sucessfully compiled and run ( I use AIDE on Android to edit and compile ) and has working mouse support.

Once I fix a few of the below bugs I will create a release APK.


Current issues
------- ------
If the application is paused in any way ( minimised, resized, covered by another window ) it will freeze
Only Left mouse button is used, need to add right and scroll wheel
The control overlay needs some rework
Clear instructions to loading paks are needed on the entry screen
A faulty Quake 2 cfg file can make the application crash at startup. The Quake 2 config file should be part managed by DeXQ2
Players with custom play models seem to crash this client when they are encountered
Removing the libs folder and attempting to recompile results in compile errors. This needs to be fixed as it seems this project is reliant on the pre-built binaries in that lib folder


Licencing
---------
This projects modified and new files are released under the GPLv3 licence https://www.gnu.org/licenses/ for the licencing on original files not modified by myself the licence information is included.




