// CAIStateGroup_MinionReactToTrespassing::GetName() -- static: the AI state group name "SG_MINION_REACT_TO_TRESPASSING" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionReactToTrespassing@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE049: push esi; push -1; push "SG_MINION_REACT_TO_TRESPASSING"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220fc4 registers this GetName with Alloc 0x9028b7 (pushed before it); Alloc 0x9028b7 -> ctor 0x900ab7 stores (last) the vtable of CAIStateGroup_MinionReactToTrespassing at this+0; literal "SG_MINION_REACT_TO_TRESPASSING"; PDB name set ?GetName@CAIStateGroup_MinionReactToTrespassing@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionReactToTrespassing@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe049::build).
#define CAIStateGroup_MinionReactToTrespassing CAIStateGroup_MinionReactToTrespassingLayout
#include "engine/CAIStateGroup_MinionReactToTrespassing.h"
#undef CAIStateGroup_MinionReactToTrespassing

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionReactToTrespassing : public CAIStateGroup_MinionReactToTrespassingLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionReactToTrespassing::GetName()
{
    return CCharString((const char*)0x0128A0C4, -1);   // "SG_MINION_REACT_TO_TRESPASSING"
}
