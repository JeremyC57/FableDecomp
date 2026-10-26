// CAIStateGroup_BuyFromSeller::GetName() -- static: the AI state group name "SG_BUY_FROM_SELLER" as a CCharString
// Leaf: ?GetName@CAIStateGroup_BuyFromSeller@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDD39: push esi; push -1; push "SG_BUY_FROM_SELLER"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220704 registers this GetName with Alloc 0x8fdd23 (pushed before it); Alloc 0x8fdd23 -> ctor 0x919a40 stores (last) the vtable of CAIStateGroup_BuyFromSeller at this+0; literal "SG_BUY_FROM_SELLER"; PDB name set ?GetName@CAIStateGroup_BuyFromSeller@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_BuyFromSeller@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdd39::build).
#define CAIStateGroup_BuyFromSeller CAIStateGroup_BuyFromSellerLayout
#include "engine/CAIStateGroup_BuyFromSeller.h"
#undef CAIStateGroup_BuyFromSeller

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_BuyFromSeller : public CAIStateGroup_BuyFromSellerLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_BuyFromSeller::GetName()
{
    return CCharString((const char*)0x01289C64, -1);   // "SG_BUY_FROM_SELLER"
}
