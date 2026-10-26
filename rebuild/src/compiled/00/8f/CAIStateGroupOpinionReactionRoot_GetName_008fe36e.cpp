// CAIStateGroup_OpinionReactionRoot::GetName() -- static: the AI state group name "SG_OPINION_REACTION_ROOT" as a CCharString
// Leaf: ?GetName@CAIStateGroup_OpinionReactionRoot@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE36E: push esi; push -1; push "SG_OPINION_REACTION_ROOT"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1221104 registers this GetName with Alloc 0x901139 (pushed before it); Alloc 0x901139 -> ctor 0x90110e stores (last) the vtable of CAIStateGroup_OpinionReactionRoot at this+0; literal "SG_OPINION_REACTION_ROOT"; PDB name set ?GetName@CAIStateGroup_OpinionReactionRoot@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_OpinionReactionRoot@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe36e::build).
#define CAIStateGroup_OpinionReactionRoot CAIStateGroup_OpinionReactionRootLayout
#include "engine/CAIStateGroup_OpinionReactionRoot.h"
#undef CAIStateGroup_OpinionReactionRoot

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_OpinionReactionRoot : public CAIStateGroup_OpinionReactionRootLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_OpinionReactionRoot::GetName()
{
    return CCharString((const char*)0x0125FD40, -1);   // "SG_OPINION_REACTION_ROOT"
}
