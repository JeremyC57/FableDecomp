// CAIStateGroup_MinionKidAnnoy::GetName() -- static: the AI state group name "SG_MINION_KID_ANNOY" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionKidAnnoy@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD420: push esi; push -1; push "SG_MINION_KID_ANNOY"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121fdcc registers this GetName with Alloc 0x901fe7 (pushed before it); Alloc 0x901fe7 -> ctor 0x8feed3 stores (last) the vtable of CAIStateGroup_MinionKidAnnoy at this+0; literal "SG_MINION_KID_ANNOY"; PDB name set ?GetName@CAIStateGroup_MinionKidAnnoy@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionKidAnnoy@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd420::build).
#define CAIStateGroup_MinionKidAnnoy CAIStateGroup_MinionKidAnnoyLayout
#include "engine/CAIStateGroup_MinionKidAnnoy.h"
#undef CAIStateGroup_MinionKidAnnoy

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionKidAnnoy : public CAIStateGroup_MinionKidAnnoyLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionKidAnnoy::GetName()
{
    return CCharString((const char*)0x01288EF8, -1);   // "SG_MINION_KID_ANNOY"
}
