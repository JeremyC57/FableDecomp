// CAIStateGroup_MinionReactToCrime::GetName() -- static: the AI state group name "SG_MINION_REACT_TO_CRIME" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionReactToCrime@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE081: push esi; push -1; push "SG_MINION_REACT_TO_CRIME"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122103c registers this GetName with Alloc 0x9028cd (pushed before it); Alloc 0x9028cd -> ctor 0x900b41 stores (last) the vtable of CAIStateGroup_MinionReactToCrime at this+0; literal "SG_MINION_REACT_TO_CRIME"; PDB name set ?GetName@CAIStateGroup_MinionReactToCrime@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionReactToCrime@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe081::build).
#define CAIStateGroup_MinionReactToCrime CAIStateGroup_MinionReactToCrimeLayout
#include "engine/CAIStateGroup_MinionReactToCrime.h"
#undef CAIStateGroup_MinionReactToCrime

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionReactToCrime : public CAIStateGroup_MinionReactToCrimeLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionReactToCrime::GetName()
{
    return CCharString((const char*)0x0128A13C, -1);   // "SG_MINION_REACT_TO_CRIME"
}
