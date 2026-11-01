// std::_Destroy<CGraphicDataBankInfo>(CGraphicBankManager::CGraphicDataBankInfo* __pointer) -- STLport single-element destroy of a graphic-bank record: the inlined implicit ~CGraphicDataBankInfo (release GraphicDataBank, then BankHandle)
// Leaf: ??$_Destroy@VCGraphicDataBankInfo@CGraphicBankManager@@@std@@YIXPAVCGraphicDataBankInfo@CGraphicBankManager@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F88C0, 61 bytes, __fastcall (pointer in ecx), `jmp` tail; no call or address
//   reference (an unreferenced COMDAT kept by the linker, no /OPT:REF).
// Identity (chunk 662, respelled at landing 2026-11-01): the CGraphicBankManager TU holds two bodies of this shape --
//   0x009F81C0 right after the record's inline default constructor 0x009F8170 (the class-member COMDAT: the implicit
//   ~CGraphicDataBankInfo, landed in chunk 662) and this one among the list's function-template helpers (after _Construct
//   0x009F8780, which is __fastcall, so the TU builds /Gr; before erase 0x009F8920) = STLport _Destroy(_Tp*). VC7.1 emits
//   function-template instantiations after the implicit members they use (chunk 662 exp/order1.cpp, exp/order2.cpp); the
//   CMeshBankManager TU repeats the pattern (0x00A0A3F0 dtor, 0x00A0A5D0 _Destroy). This row previously carried the dtor's
//   own spelling, one name on two addresses.
// Callees: CCPPointerInfo::DeleteFunc through the block (Data in ecx), operator delete @ 0x00BFE9BC (the block),
//   CCharString::~CCharString @ 0x0099EAE0 (tail).
// Layout: Ego_r.pdb CGraphicDataBankInfo (0x38) = CGraphicDataBankInit Init +0 (0x2c, trivially destructible), CCharString
//   BankHandle +0x2c, CCountedPointer<CGraphicDataBank> GraphicDataBank +0x30.
#include "engine/CCountedPointer.h"

class CCharStringData;
class CCharString {
public:
    ~CCharString();                                             // 0x0099EAE0
    CCharStringData* PStringData;
};

class CGraphicDataBank;
class CGraphicDataBankInit { public: unsigned char _bytes[0x2c]; }; // pixel formats and flags, trivially destructible

class CGraphicBankManager {
public:
    class CGraphicDataBankInfo {                                // Ego_r.pdb, 0x38 bytes
    public:
        CGraphicDataBankInit              Init;                 // +0x00
        CCharString                       BankHandle;           // +0x2c
        CCountedPointer<CGraphicDataBank> GraphicDataBank;      // +0x30
    };
};

namespace std {
// STLport 4.6 stl/_construct.h
template <class _Tp>
inline void _Destroy(_Tp* __pointer) {
    __pointer;
    __pointer->~_Tp();
}
template void _Destroy<CGraphicBankManager::CGraphicDataBankInfo>(CGraphicBankManager::CGraphicDataBankInfo*);
}
