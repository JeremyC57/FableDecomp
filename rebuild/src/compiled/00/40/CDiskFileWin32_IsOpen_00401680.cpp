
// CDiskFileWin32::IsOpen() const -- whether the Win32 handle is currently open
// Retail: Fable.exe (TLC, VC7.1) @ 0x00401680, 4 bytes. Callees: none.
//
// Reports the `Opened` flag that Open() sets and Close() clears. The bank-file and installer code check
// it before issuing reads so a failed open turns into a clean "file missing" path rather than a bad
// handle.
//
// Matching: uses the PDB layout from engine/CDiskFileWin32.h (Opened at +0x14, a byte load).
#include "engine/CDiskFileWin32.h"  // PDB layout: AccessType, Position, Length, WinFileHandle, Opened, Pathname
bool __fastcall CDiskFileWin32_IsOpen(const CDiskFileWin32* win32)
{
    return win32->Opened;
}
