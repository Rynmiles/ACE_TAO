# Microsoft Developer Studio Project File - Name="Bug 1393 Client Static EXE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Bug 1393 Client Static EXE - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "Bug_1393_Client_Static.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "Bug_1393_Client_Static.mak" CFG="Bug 1393 Client Static EXE - Win32 Static Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "Bug 1393 Client Static EXE - Win32 Static Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Bug 1393 Client Static EXE - Win32 Static Debug" (based on "Win32 (x86) Console Application")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Bug 1393 Client Static EXE - Win32 Static Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Static_Release"
# PROP Intermediate_Dir "Static_Release"
# PROP Target_Dir ""
# ADD CPP /nologo /W3 /GX /O2 /MD /GR /I "..\..\.." /I "..\..\..\tao" /I "..\..\..\.." /D NDEBUG=1 /D WIN32=1 /D _CONSOLE=1 /D TAO_AS_STATIC_LIBS=1 /D ACE_AS_STATIC_LIBS=1 /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG=1 /i "..\..\.." /i "..\..\..\tao" /i "..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_IFR_Clients.lib TAOs.lib ACEs.lib /libpath:"..\..\..\tao\IFR_Client" /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace" /nologo /version:1.2.8 /subsystem:console  /machine:I386 /out:"client.exe"

!ELSEIF  "$(CFG)" == "Bug 1393 Client Static EXE - Win32 Static Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Static_Debug"
# PROP Intermediate_Dir "Static_Debug"
# PROP Target_Dir ""
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /MDd /GR /Gy /I "..\..\.." /I "..\..\..\tao" /I "..\..\..\.." /D _DEBUG=1 /D WIN32=1 /D _CONSOLE=1 /D TAO_AS_STATIC_LIBS=1 /D ACE_AS_STATIC_LIBS=1 /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d _DEBUG=1 /i "..\..\.." /i "..\..\..\tao" /i "..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_IFR_Clientsd.lib TAOsd.lib ACEsd.lib /libpath:"..\..\..\tao\IFR_Client" /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace" /nologo /version:1.2.8 /subsystem:console /debug /machine:I386 /out:"client.exe"

!ENDIF

# Begin Target

# Name "Bug 1393 Client Static EXE - Win32 Static Release"
# Name "Bug 1393 Client Static EXE - Win32 Static Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=.\client.cpp
# End Source File
# End Group
# End Target
# End Project
