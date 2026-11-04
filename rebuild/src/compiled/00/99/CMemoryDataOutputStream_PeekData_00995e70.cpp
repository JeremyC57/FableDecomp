// CMemoryDataOutputStream::PeekData() -- pointer to the bytes written so far: the first element of the Data array, or the array object itself while it is empty
// Leaf: ?PeekData@CMemoryDataOutputStream@@QBEPBXXZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00995E70, 15 bytes, thiscall, plain `ret` (one int3, then
//   CopyDataToString at 0x00995E80).
// Identity (proven): Ego_r.pdb ?PeekData@CMemoryDataOutputStream@@QBEPBXXZ; the donor order PeekData 0x00AFDE10 ->
//   CopyDataToString 0x00AFDE20 -> Write(const CMemoryDataOutputStream&) 0x00AFDE50 -> CMemoryDataInputStream ctor 0x00AFDE80
//   is retail's 0x00995E70 -> 0x00995E80 -> 0x00995EB0 -> 0x00995EE0 with the same gaps; 44 callers, the landed ones
//   (CDefStringTable::GetString / WriteTable, NAnimationEvents::CManager::SaveBinary, CParticleProgram::OptimiseConstraints)
//   all call it as the stream's PeekData.
// Shape: the body is CArray<char>::GetAsCArray() const inlined (FableWin.pdb ?GetAsCArray@?$CArray@D@@QBEPBDXZ):
//   `size() > 0 ? &front() : GetZeroSizeArray()` (an empty() test compiles to `cmp`, retail subtracts: the size test) with the private GetZeroSizeArray() (?GetZeroSizeArray@?$CArray@D@@ABEPBDXZ)
//   returning the array object itself -- hence `lea eax,[ecx+8]` kept as the empty result.
// Layout: CDataOutputStream (vptr +0, CCharString NewLine +4), Data CArray<char> +0x08 (STLport vector, 12 bytes),
//   CurrentPosition +0x14 -- engine/CMemoryDataOutputStream.h has this layout but types Data as opaque bytes, so the class
//   is modelled locally with the real CArray<char> member (Header debt in NOTES.md); the CDataOutputStream base is local
//   too because engine/CDataOutputStream.h flattens the vptr into a data member.
#include "engine/CArray.h"


class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
};

class CDataOutputStream {
public:
    virtual ~CDataOutputStream();                       // slot 0
    CCharString NewLine;                                // +0x04
};

class CMemoryDataOutputStream : public CDataOutputStream {
public:
    CArray<char>  Data;                                 // +0x08
    unsigned long CurrentPosition;                      // +0x14
    const void* PeekData() const;
};

const void* CMemoryDataOutputStream::PeekData() const
{
    return Data.GetAsCArray();
}