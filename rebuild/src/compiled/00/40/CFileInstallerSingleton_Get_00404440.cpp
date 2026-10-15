#include "fable_file_installer.h"

// CFileInstallerSingleton::Get() -- the one CFileInstaller, created on first use
// Retail: Fable.exe (TLC, VC7.1) @ 0x00404440, 69 bytes. Callees: operator new, CFileInstaller ctor, CCountedPointer<CFileInstaller>::operator=.
//
// The file installer copies and verifies the game's data files (it drives the first-run install and the
// "installing" progress bar). There is exactly one of them, held by a static counted pointer; the first
// Get() constructs it and hands it to the pointer, later calls just return it. The counted pointer is
// what lets GFUninitialise release the installer cleanly at shutdown through
// FableFileInstallerSingletonRelease.
//
// Matching: the null test reads the pointer's object word; the new installer is assigned through the
// counted pointer's operator= (which allocates the count block), and the result is re-read from the
// global rather than from the local.
CFileInstaller& FABLE_FASTCALL CFileInstallerSingleton::Get()
{
    CFileInstaller* installer = g_CFileInstallerSingleton_013CA818.object;
    if (installer == 0)
    {
        // First use: create the installer and let the counted pointer own it.
        g_CFileInstallerSingleton_013CA818 = new CFileInstaller;
    }

    return *g_CFileInstallerSingleton_013CA818.object;
}
