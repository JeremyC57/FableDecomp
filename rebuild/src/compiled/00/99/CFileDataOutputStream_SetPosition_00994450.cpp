// CFileDataOutputStream::SetPosition(unsigned long position) -- move the output position: forward to the file's SetPosition
// Leaf: ?SetPosition@CFileDataOutputStream@@UAEXK@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00994450, 8 bytes: `mov ecx,[ecx+8] ; mov eax,[ecx] ; jmp dword ptr [eax+1h]`
//   (File->SetPosition, CAFile vtable slot 5, a virtual tail call).
// Identity (two sources): (1) slot 1 of the CFileDataOutputStream vtable 0x0129A6F8 (RTTI .?AVCFileDataOutputStream@@ : CDataOutputStream :
//   CDataStream), CDataStream's SetPosition slot (engine/CDataStream.h: ~ 0, SetPosition 1, GetPosition 2, Close 3); (2) Ego_r.pdb / FableWin.pdb
//   ?SetPosition@CFileDataOutputStream@@UAEXK@Z (Ego_r donor order SetPosition 0x00AFC550, GetPosition 0x00AFC560, Close, Write = retail
//   0x00994450 .. 0x00994480). The forwarded slot 5 is CAFile::SetPosition (engine/CAFile.h, the Ego_r / CDiskFileWin32 virtual order).
//   Callers: CBankFile::WriteFileInformation, CChecksumCacheFile::Save.
// Header: engine/CFileDataOutputStream.h (FABLE_RETAIL_FILE_DATA_OUTPUT_STREAM), engine/CDataOutputStream.h (FABLE_RETAIL_CDATAOUTPUTSTREAM),
//   engine/CDataStream.h, engine/CAFile.h.
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_FILE_DATA_OUTPUT_STREAM
#include "engine/CDataOutputStream.h"
#include "engine/CFileDataOutputStream.h"
#include "engine/CAFile.h"
void CFileDataOutputStream::SetPosition(unsigned long position)
{
    File->SetPosition(position);
}