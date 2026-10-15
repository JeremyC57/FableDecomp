#include "candidates/build_number_compiled.h"

// GFGetBuildNumber2() -- load the full build version string from full_build_version.txt, if the file exists
// Retail: Fable.exe (TLC, VC7.1) @ 0x00401F30, 162 bytes. Callees: CCharString::AssignFromWide / Destroy / ConstructFromFile,
//   CWideString::PathExists, CDiskFileWin32 ctor / Close / base cleanup, CCharString::operator=.
//
// Called once from GFInitialise. The build machine drops a `full_build_version.txt` next to the
// executable; when it is present its whole contents become the global FableFullBuildVersion string that
// the title bar and bug reports show. A retail install usually has no such file, in which case nothing
// happens and the compiled-in build number stands.
//
// The path string is built twice because the existence check and the file open each take their own
// temporary; the disk file is opened read-only (access 1, share mode 2), read in one go, and closed
// through the normal destructor sequence.
//
// Matching: CCharString and CDiskFileWin32 are modelled by the explicit Construct/Destroy helpers in the
// shared header so the temporaries are destroyed in the same order as the inlined destructors; the vtable
// store before Close reproduces the destructor's vtable reset.
void FABLE_CDECL GFGetBuildNumber2()
{
    CCharString pathText;
    CCharString fileContents;
    CDiskFileWin32 diskFile;

    // Does the version file exist at all?
    pathText.AssignFromWide(L"full_build_version.txt");
    const bool pathExists = reinterpret_cast<const CWideString*>(&pathText)->PathExists();
    pathText.Destroy();

    if (pathExists)
    {
        // Open it read-only and slurp the whole file into the global version string.
        pathText.AssignFromWide(L"full_build_version.txt");
        diskFile.Construct(*reinterpret_cast<const CWideString*>(&pathText), 1, 2);
        pathText.Destroy();

        fileContents.ConstructFromFile(reinterpret_cast<CAFile*>(&diskFile), -1);
        FableFullBuildVersion.Assign(fileContents);
        fileContents.Destroy();

        // Destructor sequence for the disk file: vtable reset, close, release the path, base cleanup.
        diskFile.Vftable = FableDiskFileWin32Vftable;
        diskFile.Close();
        diskFile.Path.Destroy();
        diskFile.BaseCleanup();
    }
}
