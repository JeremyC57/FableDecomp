// CAIStateGroup_HoverKeepInArea::GetName() -- static: the AI state group name "SG_HOVER_KEEP_IN_AREA" as a CCharString
// Leaf: ?GetName@CAIStateGroup_HoverKeepInArea@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE1A3: push esi; push -1; push "SG_HOVER_KEEP_IN_AREA"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122095c registers this GetName with Alloc 0x9029c6 (pushed before it); Alloc 0x9029c6 stores the vtable of CAIStateGroup_HoverKeepInArea into the new object; literal "SG_HOVER_KEEP_IN_AREA"; PDB name set ?GetName@CAIStateGroup_HoverKeepInArea@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_HoverKeepInArea@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe1a3::build).
#define CAIStateGroup_HoverKeepInArea CAIStateGroup_HoverKeepInAreaLayout
#include "engine/CAIStateGroup_HoverKeepInArea.h"
#undef CAIStateGroup_HoverKeepInArea

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_HoverKeepInArea : public CAIStateGroup_HoverKeepInAreaLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_HoverKeepInArea::GetName()
{
    return CCharString((const char*)0x0128A2F8, -1);   // "SG_HOVER_KEEP_IN_AREA"
}
