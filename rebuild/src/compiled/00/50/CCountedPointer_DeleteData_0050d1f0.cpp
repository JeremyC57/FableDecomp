#pragma optimize("s",on)
// CCountedPointer<COldFileFormatMapHeader2>::DeleteData(void* data) -- the counted pointer's per-type deleter: `delete static_cast<T*>(data)`
// Leaf: ?DeleteData@?$CCountedPointer@VCOldFileFormatMapHeader2@@@@SIXPAX@Z
// Retail: push ecx ; call 0x00BFE9BC ; pop ecx ; ret -- the pointer in ecx is handed to the scalar operator delete (0x00BFE9BC is
//   the MSVCR71 ??3@YAXPAX@Z import thunk); COldFileFormatMapHeader2 has no destructor, so the delete is the bare call.
// Name (NF94 q11431): the landed caller 0x005148D0 CReplaceSerialise<COldFileFormatMapHeader2,..>::ConvertPreviousUnknownVersionUp
//   stores this deleter in the count block and binds ?DeleteData@?$CCountedPointer@VCOldFileFormatMapHeader2@@@@SIXPAX@Z
//   (reloc_bindings); engine/CCountedPointer.h names the per-T deleter DeleteData (Ego_r: static __fastcall `SIXPAX@Z`, out of
//   line). The type is the shared CFileFormatBrush.h OLDMAPHEADER12 record (Ego_r / FableWin). Was a free
//   OperatorDelete_thunk_0050d1f0 stand-in; now the explicit instantiation, as the landed DeleteData rows (0x00BB6330 ...).
#define FABLE_RETAIL_CFILEFORMATBRUSH
#define FABLE_RETAIL_CFILEFORMAT_HEIGHTMAP_V5
#define FABLE_RETAIL_CFILEFORMAT_OLDMAPHEADER12
#include "engine/CFileFormatBrush.h"
#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<COldFileFormatMapHeader2>::DeleteData(void*);
