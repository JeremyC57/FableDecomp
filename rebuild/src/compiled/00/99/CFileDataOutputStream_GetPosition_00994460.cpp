// CFileDataOutputStream::GetPosition() const -- the output position: the file's GetPosition
// Leaf: ?GetPosition@CFileDataOutputStream@@UBEKXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00994460, 8 bytes: `mov ecx,[ecx+8] ; mov eax,[ecx] ; jmp dword ptr [eax+2h]`
//   (File->GetPosition, CAFile vtable slot 7, a virtual tail call).
// Identity (two sources): (1) slot 2 of the CFileDataOutputStream vtable 0x0129A6F8 (RTTI .?AVCFileDataOutputStream@@ : CDataOutputStream :
//   CDataStream), CDataStream's GetPosition slot (engine/CDataStream.h: ~ 0, SetPosition 1, GetPosition 2, Close 3); (2) Ego_r.pdb / FableWin.pdb
//   ?GetPosition@CFileDataOutputStream@@UBEKXZ (Ego_r donor order SetPosition 0x00AFC550, GetPosition 0x00AFC560, Close, Write = retail
//   0x00994450 .. 0x00994480). The forwarded slot 7 is CAFile::GetPosition (engine/CAFile.h, the Ego_r / CDiskFileWin32 virtual order).
//   Callers: CBankFile::WriteFileInformation, CChecksumCacheFile::Save.
// Header: engine/CFileDataOutputStream.h (FABLE_RETAIL_FILE_DATA_OUTPUT_STREAM), engine/CDataOutputStream.h (FABLE_RETAIL_CDATAOUTPUTSTREAM),
//   engine/CDataStream.h, engine/CAFile.h.
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_FILE_DATA_OUTPUT_STREAM
#include "engine/CDataOutputStream.h"
#include "engine/CFileDataOutputStream.h"
#include "engine/CAFile.h"
unsigned long CFileDataOutputStream::GetPosition() const
{
    return File->GetPosition();
}