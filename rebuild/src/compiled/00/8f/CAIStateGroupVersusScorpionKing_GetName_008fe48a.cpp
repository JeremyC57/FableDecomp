// CAIStateGroup_VersusScorpionKing::GetName() -- static: the AI state group name "SG_VERSUS_SCORPION_KING" as a CCharString
// Leaf: ?GetName@CAIStateGroup_VersusScorpionKing@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE48A: push esi; push -1; push "SG_VERSUS_SCORPION_KING"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220d6c registers this GetName with Alloc 0x902ba2 (pushed before it); Alloc 0x902ba2 -> ctor 0x90143e stores (last) the vtable of CAIStateGroup_VersusScorpionKing at this+0; literal "SG_VERSUS_SCORPION_KING"; PDB name set ?GetName@CAIStateGroup_VersusScorpionKing@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_VersusScorpionKing@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe48a::build).
#define CAIStateGroup_VersusScorpionKing CAIStateGroup_VersusScorpionKingLayout
#include "engine/CAIStateGroup_VersusScorpionKing.h"
#undef CAIStateGroup_VersusScorpionKing

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_VersusScorpionKing : public CAIStateGroup_VersusScorpionKingLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_VersusScorpionKing::GetName()
{
    return CCharString((const char*)0x0128A7FC, -1);   // "SG_VERSUS_SCORPION_KING"
}
