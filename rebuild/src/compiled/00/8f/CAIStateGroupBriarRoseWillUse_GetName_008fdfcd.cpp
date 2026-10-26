// CAIStateGroup_BriarRoseWillUse::GetName() -- static: the AI state group name "SG_BRIAR_ROSE_WILL_USE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_BriarRoseWillUse@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDFCD: push esi; push -1; push "SG_BRIAR_ROSE_WILL_USE"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220d1c registers this GetName with Alloc 0x9027f9 (pushed before it); Alloc 0x9027f9 -> ctor 0x90090f stores (last) the vtable of CAIStateGroup_BriarRoseWillUse at this+0; literal "SG_BRIAR_ROSE_WILL_USE"; PDB name set ?GetName@CAIStateGroup_BriarRoseWillUse@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_BriarRoseWillUse@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdfcd::build).
#define CAIStateGroup_BriarRoseWillUse CAIStateGroup_BriarRoseWillUseLayout
#include "engine/CAIStateGroup_BriarRoseWillUse.h"
#undef CAIStateGroup_BriarRoseWillUse

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_BriarRoseWillUse : public CAIStateGroup_BriarRoseWillUseLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_BriarRoseWillUse::GetName()
{
    return CCharString((const char*)0x0128A028, -1);   // "SG_BRIAR_ROSE_WILL_USE"
}
