// CAIStateGroup_CreatureTakeCover::GetName() -- static: the AI state group name "SG_CREATURE_RANGED_COMBAT_USE_COVER" as a CCharString
// Leaf: ?GetName@CAIStateGroup_CreatureTakeCover@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE2AD: push esi; push -1; push "SG_CREATURE_RANGED_COMBAT_USE_COVER"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220dbc registers this GetName with Alloc 0x9033e4 (pushed before it); Alloc 0x9033e4 stores the vtable of CAIStateGroup_CreatureTakeCover into the new object; literal "SG_CREATURE_RANGED_COMBAT_USE_COVER"; PDB name set ?GetName@CAIStateGroup_CreatureTakeCover@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_CreatureTakeCover@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe2ad::build).
#define CAIStateGroup_CreatureTakeCover CAIStateGroup_CreatureTakeCoverLayout
#include "engine/CAIStateGroup_CreatureTakeCover.h"
#undef CAIStateGroup_CreatureTakeCover

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_CreatureTakeCover : public CAIStateGroup_CreatureTakeCoverLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_CreatureTakeCover::GetName()
{
    return CCharString((const char*)0x0128A514, -1);   // "SG_CREATURE_RANGED_COMBAT_USE_COVER"
}
