// CCountedPointer::Reset(T* p) -- (CCountedPointer<NInventory::CPerItemData >) drop the current share through ReleaseData, then adopt p with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCPerItemData@NInventory@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C1946, 59 bytes (size-optimised TU, odd entry), thiscall, `ret 4`.
// Identity: CCountedPointer<NInventory::CPerItemData >::Reset(NInventory::CPerItemData*) -- proven; evidence: its callers (NInventory item code 0x005BEC85 / 0x005BECD7 / 0x005BEE35 / 0x005BEEFE ...) reset the item's +0x08 pointer (Ego_r.pdb CItem::PPerItemData, CCountedPointer<NInventory::CPerItemData>) with a new CPerItemData (default ctor 0x005C14A6: Augmentations vector, counters, -1 ref count field; copy ctor 0x005C14C4). FableWin.pdb name set holds ?Reset@?$CCountedPointer@VCPerItemData@NInventory@@@@QAEXPAVCPerItemData@NInventory@@@Z. Family G054 (CCountedPointer
//   Reset bodies; the propagated "CCountedPointer<std::vector<CCharString>>::Reset" is right only for 0x005C1E91).
// Callees: ReleaseData() 0x005C04BF (this instantiation's own COMDAT), operator new 0x00BFEA1A (12-byte count block);
//   DeleteData 0x005C18E3 (static __fastcall deleter stored in the block).
// Shape: Data is stored before the null test; the block takes Data re-read through `this`; a failed new stores 0.
#pragma optimize("s", on)
void* __cdecl operator new(unsigned int size);             // 0x00BFEA1A
namespace NInventory { class CPerItemData; }
class CCPPointerInfo {                                     // 12 bytes
public:
    unsigned long RefCount;                                // +0x00
    void (__fastcall* DeleteFunc)(void* data);             // +0x04
    void* Data;                                            // +0x08
    CCPPointerInfo(unsigned long refCount, void (__fastcall* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }
};
template <class T> class CCountedPointer {
public:
    T* Data;                                               // +0x00
    CCPPointerInfo* Info;                                  // +0x04
    void ReleaseData();                                    // own COMDAT (0x005C04BF)
    static void __fastcall DeleteData(void* data);         // own COMDAT (0x005C18E3)
    void Reset(T* p)
    {
        ReleaseData();
        Data = p;
        if (p)
            Info = new CCPPointerInfo(1, &DeleteData, Data);
    }
};
template void CCountedPointer<NInventory::CPerItemData >::Reset(NInventory::CPerItemData*);