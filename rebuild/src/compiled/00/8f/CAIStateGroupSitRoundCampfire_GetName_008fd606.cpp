// CAIStateGroup_SitRoundCampfire::GetName() -- static: the AI state group name "SG_SIT_ROUND_CAMPFIRE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_SitRoundCampfire@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD606: push esi; push -1; push "SG_SIT_ROUND_CAMPFIRE"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121fd04 registers this GetName with Alloc 0x90221d (pushed before it); Alloc 0x90221d -> ctor 0x8ff38b stores (last) the vtable of CAIStateGroup_SitRoundCampfire at this+0; literal "SG_SIT_ROUND_CAMPFIRE"; PDB name set ?GetName@CAIStateGroup_SitRoundCampfire@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_SitRoundCampfire@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd606::build).
#define CAIStateGroup_SitRoundCampfire CAIStateGroup_SitRoundCampfireLayout
#include "engine/CAIStateGroup_SitRoundCampfire.h"
#undef CAIStateGroup_SitRoundCampfire

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_SitRoundCampfire : public CAIStateGroup_SitRoundCampfireLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_SitRoundCampfire::GetName()
{
    return CCharString((const char*)0x012891E8, -1);   // "SG_SIT_ROUND_CAMPFIRE"
}
