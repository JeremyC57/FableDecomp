// CWADFile::GetAlignment() const -- the alignment of the bank file this WAD entry lives in
// Leaf: ?GetAlignment@CWADFile@@UBEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A39C70, 8 bytes: `mov ecx,[ecx+4] ; jmp 0x009CBB80` (Bank, then its GetAlignment).
// Body port (NF321, 2026-11-23): defined on the real class of engine/CWADFile.h (FABLE_RETAIL_CWADFILE; was the void stand-in
//   ?GetAlignment@CWADFile_Methods@@QAEXXZ calling a made-up CBankFileAsync::Run); the retail RTTI vftable placement names this PDB member
//   here. Callee: Ego_r.pdb / FableWin.pdb ?GetAlignment@CBankFile@@QBEKXZ 0x009CBB80, which the donor FableWin.exe CWADFile::GetAlignment
//   calls; declared by the add-only opt-in FABLE_RETAIL_CBANKFILE_GETALIGNMENT_DECL and reached through engine/CBankFileAsync.h's
//   CBankFileAsync : CBankFile (Bank is the header's CBankFileAsync*).
#define FABLE_RETAIL_CWADFILE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILE_GETALIGNMENT_DECL
#include "engine/CWADFile.h"
#include "engine/CBankFileAsync.h"

long CWADFile::GetAlignment() const
{
    return Bank->GetAlignment();
}
