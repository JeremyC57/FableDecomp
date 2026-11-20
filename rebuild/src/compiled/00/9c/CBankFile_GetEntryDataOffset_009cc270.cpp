// CBankFile::GetEntryDataOffset(unsigned long index) const -- the entry's file offset from the packed array or the runtime record
// Leaf: ?GetEntryDataOffset@CBankFile@@QBEKK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CC270. Callee: CPackedUIntArray::operator[] @ 0x00A629F0 (tail jump, the argument slot
//   rewritten to index-1).
// Retail bytes:  mov eax,[ecx+3Ch] ; test eax,eax ; mov eax,[esp+4] ; je R ; dec eax ; mov [esp+4],eax ; add ecx,38h ; jmp operator[] ;
//   R: lea edx,[eax+eax*2] ; mov eax,[ecx+20h] ; mov eax,[eax+edx*4] ; ret 4
// Names (NF201, 2026-11-20, q11793): Ego_r.pdb 0x00B19F70 + FableWin.pdb 0x02F91EA0 ?GetEntryDataOffset@CBankFile@@QBEKK@Z (next to
//   GetEntryDataSize 0x009CC240 = Ego_r 0x00B19F40); the body is the GetEntryDataSize shape on PackedDataOffset +0x38 / DataOffset. Was
//   the CBankFile_Retail stand-in over local CRuntimeData / CPackedUIntArray models.
// One definition (the 0x009CC240 recipe): ReadEntryData 0x009CC2A0 expands this body (NF139 opt-in FABLE_RETAIL_CBANKFILE_ENTRYDATA_INLINE,
//   the one inline definition in engine/CBankFile.h), so this row is the nf86 / nf88 pick-any scaffolding: its object emits the same
//   COMDAT as every includer.
// Fidelity: scaffolding, not retail: takes the address of the header's inline GetEntryDataOffset in an anonymous-namespace helper so this
//   object emits it out of line as a pick-any COMDAT (retail's out-of-line copy at 0x009CC270).
#define FABLE_RETAIL_CARRAY_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CArray.h"
#include "engine/CPackedUIntArray.h"
#define FABLE_RETAIL_CBANKFILE_FILENAMEINDICES
#define FABLE_RETAIL_CBANKFILE_CRUNTIMEDATA_CTOR
#define FABLE_RETAIL_CBANKFILE_PACKRUNTIMEDATA
#define FABLE_RETAIL_CBANKFILE_ENTRYDATA_INLINE
#include "engine/CBankFile.h"

namespace {
unsigned long (CBankFile::* ReferenceGetEntryDataOffset())(unsigned long) const
{
    return &CBankFile::GetEntryDataOffset;
}
}
