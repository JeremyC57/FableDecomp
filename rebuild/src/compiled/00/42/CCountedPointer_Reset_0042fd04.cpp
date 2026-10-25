// CCountedPointer::Reset(T* p) -- (CCountedPointer<CIEngine >) drop the current share through ReleaseData, then adopt p with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCIEngine@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042FD04, 59 bytes (size-optimised TU, odd entry), thiscall, `ret 4`.
// Identity: CCountedPointer<CIEngine >::Reset(CIEngine*) -- proven; evidence: its only caller CNewFrontendGameComponent::InitialiseEngine 0x0042E204 resets this+0x58 (Ego_r.pdb CNewFrontendGameComponent::m_pEngine, CCountedPointer<CIEngine> at +88; the members before it hold no STL type, so the retail offset is the same) with the engine factory's result; DeleteData 0x0042F887 deletes through the virtual destructor. FableWin.pdb name set holds ?Reset@?$CCountedPointer@VCIEngine@@@@QAEXPAVCIEngine@@@Z. Family G054 (CCountedPointer
//   Reset bodies; the propagated "CCountedPointer<std::vector<CCharString>>::Reset" is right only for 0x005C1E91).
// Callees: ReleaseData() 0x0042F974 (this instantiation's own COMDAT), operator new 0x00BFEA1A (12-byte count block);
//   DeleteData 0x0042F887 (static __fastcall deleter stored in the block).
// Shape: Data is stored before the null test; the block takes Data re-read through `this`; a failed new stores 0.
#pragma optimize("s", on)
void* __cdecl operator new(unsigned int size);             // 0x00BFEA1A
class CIEngine;
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
    void ReleaseData();                                    // own COMDAT (0x0042F974)
    static void __fastcall DeleteData(void* data);         // own COMDAT (0x0042F887)
    void Reset(T* p)
    {
        ReleaseData();
        Data = p;
        if (p)
            Info = new CCPPointerInfo(1, &DeleteData, Data);
    }
};
template void CCountedPointer<CIEngine >::Reset(CIEngine*);