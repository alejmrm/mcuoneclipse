mkdir debug
cd debug
cmake -G "Eclipse CDT4 - Unix Makefiles" -DCMAKE_MAKE_PROGRAM=make -DCMAKE_BUILD_TYPE=Debug -DCMAKE_ECLIPSE_VERSION="4.5" -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE -DCMAKE_ECLIPSE_MAKE_ARGUMENTS=-j8 -DCMAKE_ECLIPSE_EXECUTABLE="C:\NXP\MCUXpressoIDE_11.6.0_8187\ide\mcuxpressoide.exe" ../src
cd ..