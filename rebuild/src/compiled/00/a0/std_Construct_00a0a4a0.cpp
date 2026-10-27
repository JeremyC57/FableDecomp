// std::_Construct(CUnprovenRecord_00a0a4a0* __p, const CUnprovenRecord_00a0a4a0& __val) -- placement copy of a {CCharString, CCountedPointer} record (nothing for a null __p)
// Leaf: ??$_Construct@VCUnprovenRecord_00a0a4a0@@V1@@std@@YIXPAVCUnprovenRecord_00a0a4a0@@ABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0A4A0, 37 bytes, `__fastcall` (p ecx, value edx), plain `ret`.
// Identity (descriptive): no call, jump or data reference reaches it (an unreferenced COMDAT kept without /OPT:REF); its
//   bytes copy-construct a 12-byte non-polymorphic record {CCharString +0 (copy constructor out of line), CCountedPointer +4
//   (Data, Info, ++Info->RefCount)}; its neighbours are the graphic-bank manager's STLport helpers (0x00A0A460 list ctor of 0xC-byte elements, 0x00A0A4D0 ??_G of a {CCharString, CCountedPointer} pair, 0x00A0A650 list<CGraphicBankManager::CGraphicDataBankInfo>::erase), but no reference ties this instantiation to one container. The manifest's
//   "_Cons_val<allocator<CMeshBankManager::CMeshDataBankInfo>, ...>" is a Dinkumware name propagated across byte-identical bodies.
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30.
// Shape (family M196, representative 0x006964A0): STLport `_Construct(p, val) { new (p) T1(val); }` -- placement new's null
//   test, then the record's implicit copy constructor. /Gr TU written as explicit __fastcall.
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
class CCPPointerInfo {
public:
    long RefCount;                                        // +0x00
    void (__fastcall* DeleteFunc)(void* data);            // +0x04
    void* Data;                                           // +0x08
};
template <class T> class CCountedPointer {                 // 8 bytes: Data, Info
public:
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    T* Data;                                              // +0x00
    CCPPointerInfo* Info;                                 // +0x04
};
class CCharString {
public:
    CCharString(const CCharString& other);                // 0x0099EC30
    void* PStringData;                                    // +0x00
};
class CUnprovenPointee_00a0a4a0;                              // descriptive: the pointee is unproven
class CUnprovenRecord_00a0a4a0 {                               // descriptive: the record type is unproven
public:
    CCharString Name;                                     // +0x00
    CCountedPointer<CUnprovenPointee_00a0a4a0> Pointer;       // +0x04
};
namespace std {
template <class _T1, class _T2> inline void __fastcall _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template void __fastcall _Construct<CUnprovenRecord_00a0a4a0, CUnprovenRecord_00a0a4a0>(CUnprovenRecord_00a0a4a0* __p, const CUnprovenRecord_00a0a4a0& __val);
}