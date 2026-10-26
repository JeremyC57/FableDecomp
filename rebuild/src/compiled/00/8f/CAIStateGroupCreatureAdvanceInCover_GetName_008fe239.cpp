// CAIStateGroup_CreatureAdvanceInCover::GetName() -- static: the AI state group name "SG_CREATURE_ADVANCE_IN_COVER" as a CCharString
// Leaf: ?GetName@CAIStateGroup_CreatureAdvanceInCover@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE239: push esi; push -1; push "SG_CREATURE_ADVANCE_IN_COVER"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220de4 registers this GetName with Alloc 0x9033a4 (pushed before it); Alloc 0x9033a4 stores the vtable of CAIStateGroup_CreatureAdvanceInCover into the new object; literal "SG_CREATURE_ADVANCE_IN_COVER"; PDB name set ?GetName@CAIStateGroup_CreatureAdvanceInCover@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_CreatureAdvanceInCover@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe239::build).
#define CAIStateGroup_CreatureAdvanceInCover CAIStateGroup_CreatureAdvanceInCoverLayout
#include "engine/CAIStateGroup_CreatureAdvanceInCover.h"
#undef CAIStateGroup_CreatureAdvanceInCover

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_CreatureAdvanceInCover : public CAIStateGroup_CreatureAdvanceInCoverLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_CreatureAdvanceInCover::GetName()
{
    return CCharString((const char*)0x0128A414, -1);   // "SG_CREATURE_ADVANCE_IN_COVER"
}
