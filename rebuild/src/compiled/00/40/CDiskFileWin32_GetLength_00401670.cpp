
// CDiskFileWin32::GetLength() const -- size of the open file in bytes
// Retail: Fable.exe (TLC, VC7.1) @ 0x00401670, 4 bytes. Callees: none.
//
// CDiskFileWin32 wraps a Win32 file handle for the CAFile interface. The file's length is read once when
// the file is opened and cached in the `Length` member, so this accessor is a plain load.
//
// Matching: uses the PDB layout from engine/CDiskFileWin32.h (Length at +0x0C).
#include "engine/CDiskFileWin32.h"  // PDB layout: AccessType, Position, Length, WinFileHandle, Opened, Pathname
long __fastcall CDiskFileWin32_GetLength(const CDiskFileWin32* self) {
    return self->Length;
}
