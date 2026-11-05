// std::vector<NInventory::CItem>::resize(unsigned int) -- resizes the item vector, filling new slots with a default (zeroed) CItem temporary
// Leaf: ?resize@
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pBlock->Info are the header CCountedPointer's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C2AA9, 47 bytes. Callers: NInventory::CCategory::CCategory (0x005BFD24)
// and GFSerialiseVectorBinaryIn<NInventory::CItem> (0x005C2B4F).
// Callees: vector<CItem>::resize(n, const CItem&) @ 0x005C2A13 (the `/14h` stride: erase the tail or insert copies;
//          manifest label "CArray<...>::push_back" is propagated), CCountedPointer<NInventory::CPerItemData> destructor
//          @ 0x005C04BF (the temporary's only non-trivial member).
//
// LABEL CORRECTION: "CTCInventoryStats::OnSerialise" is propagated. The 20-byte zeroed temporary whose +8 member is
// destroyed after the call is PDB NInventory::CItem {ItemType, NumberOfItems, CCountedPointer PPerItemData,
// AverageBoughtPrice}; the body is the Dinkumware one-argument `resize(n) { resize(n, T()); }`.
//
// Shape: frame pointer kept in a size TU (`push ebp ; mov ebp,esp ; sub esp,14h ... leave ; ret 4`, the argument
// re-pushed as `push dword ptr [ebp+8]`): `#pragma optimize("y", off)` + `optimize("s", on)` (GOTCHAS wave 24b). The
// five zero stores run in member order through one zero register: the CItem constructor's init list with the counted
// pointer's own inline default constructor in the middle. The container is reconstructed by hand (retail Dinkumware
// vector, 12 bytes); the DDK STL is not the retail revision.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct SCountBlock;

#include "engine/CCountedPointer.h"

namespace NInventory {
class CPerItemData;
class CItem { public:
    long ItemType;                                  // +0x00
    long NumberOfItems;                             // +0x04
    CCountedPointer<CPerItemData> PPerItemData;     // +0x08
    long AverageBoughtPrice;                        // +0x10
    CItem() : ItemType(0), NumberOfItems(0), AverageBoughtPrice(0) {}
};
}

namespace std {
template <class T> class vector {
public:
    T* _First;
    T* _Last;
    T* _End;
    void resize(unsigned int newSize, const T& value);   // 0x005C2A13
    void resize(unsigned int newSize) { resize(newSize, T()); }
};
}

template void std::vector<NInventory::CItem>::resize(unsigned int);