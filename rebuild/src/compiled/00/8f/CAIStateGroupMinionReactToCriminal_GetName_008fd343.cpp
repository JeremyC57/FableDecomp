// CAIStateGroup_MinionReactToCriminal::GetName() -- static: the AI state group name "SG_MINION_REACT_TO_CRIMINAL" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionReactToCriminal@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD343: push esi; push -1; push "SG_MINION_REACT_TO_CRIMINAL"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121fa0c registers this GetName with Alloc 0x901f6b (pushed before it); Alloc 0x901f6b -> ctor 0x8fec9c stores (last) the vtable of CAIStateGroup_MinionReactToCriminal at this+0; literal "SG_MINION_REACT_TO_CRIMINAL"; PDB name set ?GetName@CAIStateGroup_MinionReactToCriminal@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionReactToCriminal@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd343::build).
#define CAIStateGroup_MinionReactToCriminal CAIStateGroup_MinionReactToCriminalLayout
#include "engine/CAIStateGroup_MinionReactToCriminal.h"
#undef CAIStateGroup_MinionReactToCriminal

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionReactToCriminal : public CAIStateGroup_MinionReactToCriminalLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionReactToCriminal::GetName()
{
    return CCharString((const char*)0x01288D48, -1);   // "SG_MINION_REACT_TO_CRIMINAL"
}
