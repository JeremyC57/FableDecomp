// CAIStateGroup_WanderAroundShops::GetName() -- static: the AI state group name "SG_WANDER_AROUND_SHOPS" as a CCharString
// Leaf: ?GetName@CAIStateGroup_WanderAroundShops@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDC91: push esi; push -1; push "SG_WANDER_AROUND_SHOPS"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122059c registers this GetName with Alloc 0x902556 (pushed before it); Alloc 0x902556 -> ctor 0x9000bc stores (last) the vtable of CAIStateGroup_WanderAroundShops at this+0; literal "SG_WANDER_AROUND_SHOPS"; PDB name set ?GetName@CAIStateGroup_WanderAroundShops@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_WanderAroundShops@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdc91::build).
#define CAIStateGroup_WanderAroundShops CAIStateGroup_WanderAroundShopsLayout
#include "engine/CAIStateGroup_WanderAroundShops.h"
#undef CAIStateGroup_WanderAroundShops

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_WanderAroundShops : public CAIStateGroup_WanderAroundShopsLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_WanderAroundShops::GetName()
{
    return CCharString((const char*)0x01289BF4, -1);   // "SG_WANDER_AROUND_SHOPS"
}
