// CAIStateGroup_MinionSitOutside::GetName() -- static: the AI state group name "SG_MINION_SIT_OUTSIDE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionSitOutside@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD5E0: push esi; push -1; push "SG_MINION_SIT_OUTSIDE"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121fcb4 registers this GetName with Alloc 0x902d95 (pushed before it); Alloc 0x902d95 stores the vtable of CAIStateGroup_MinionSitOutside into the new object; literal "SG_MINION_SIT_OUTSIDE"; PDB name set ?GetName@CAIStateGroup_MinionSitOutside@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionSitOutside@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd5e0::build).
#define CAIStateGroup_MinionSitOutside CAIStateGroup_MinionSitOutsideLayout
#include "engine/CAIStateGroup_MinionSitOutside.h"
#undef CAIStateGroup_MinionSitOutside

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionSitOutside : public CAIStateGroup_MinionSitOutsideLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionSitOutside::GetName()
{
    return CCharString((const char*)0x012891B8, -1);   // "SG_MINION_SIT_OUTSIDE"
}
