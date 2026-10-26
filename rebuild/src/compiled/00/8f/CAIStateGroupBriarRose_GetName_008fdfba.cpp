// CAIStateGroup_BriarRose::GetName() -- static: the AI state group name "SG_BRIAR_ROSE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_BriarRose@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDFBA: push esi; push -1; push "SG_BRIAR_ROSE"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220cf4 registers this GetName with Alloc 0x9027e3 (pushed before it); Alloc 0x9027e3 -> ctor 0x9008c1 stores (last) the vtable of CAIStateGroup_BriarRose at this+0; literal "SG_BRIAR_ROSE"; PDB name set ?GetName@CAIStateGroup_BriarRose@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_BriarRose@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdfba::build).
#define CAIStateGroup_BriarRose CAIStateGroup_BriarRoseLayout
#include "engine/CAIStateGroup_BriarRose.h"
#undef CAIStateGroup_BriarRose

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_BriarRose : public CAIStateGroup_BriarRoseLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_BriarRose::GetName()
{
    return CCharString((const char*)0x0128A018, -1);   // "SG_BRIAR_ROSE"
}
