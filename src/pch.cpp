#include"pch.h"
/*
For some background. The compiler only looks at source files (ie, *.cpp, *.c, etc) and so when it compiles them it has
to include every header and compile any code found in the headers as well. The precompiled headers option allows for
compiling all of that code (ie, the globally include'd code in StdAfx.h) once so that you don't have to do it all of the time.
Thats what StdAfx.cpp is for. The compiler compiles StdAfx.cpp with all of the code included in StdAfx.h once instead
of having to do it every time you build.
*/