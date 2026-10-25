// CCountedPointer::Reset(T* p) -- (CCountedPointer<std::vector<CCharString> >) drop the current share through ReleaseData, then adopt p with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@V?$vector@VCCharString@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C1E91, 59 bytes (size-optimised TU, odd entry), thiscall, `ret 4`.
// Identity: CCountedPointer<std::vector<CCharString> >::Reset(std::vector<CCharString>*) -- proven; evidence: its only caller CTCInventoryBase::OnSerialise 0x005BFD5E resets this+0x80 with a new zeroed 12-byte vector (Ego_r.pdb CTCInventoryBase::SerialisedSelectedItems, CCountedPointer<vector<CCharString> > at +136; retail is 8 lower after the two 12-byte STLport vectors Components and Categories). FableWin.pdb name set holds ?Reset@?$CCountedPointer@V?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@@QAEXPAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@Z. Family G054 (CCountedPointer
//   Reset bodies; the propagated "CCountedPointer<std::vector<CCharString>>::Reset" is right only for 0x005C1E91).
// Callees: ReleaseData() 0x005C0563 (this instantiation's own COMDAT), operator new 0x00BFEA1A (12-byte count block);
//   DeleteData 0x005C1981 (static __fastcall deleter stored in the block).
// Shape: Data is stored before the null test; the block takes Data re-read through `this`; a failed new stores 0.
#pragma optimize("s", on)
void* __cdecl operator new(unsigned int size);             // 0x00BFEA1A
class CCharString;
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { public: _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage; };
}
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
    void ReleaseData();                                    // own COMDAT (0x005C0563)
    static void __fastcall DeleteData(void* data);         // own COMDAT (0x005C1981)
    void Reset(T* p)
    {
        ReleaseData();
        Data = p;
        if (p)
            Info = new CCPPointerInfo(1, &DeleteData, Data);
    }
};
template void CCountedPointer<std::vector<CCharString> >::Reset(std::vector<CCharString>*);