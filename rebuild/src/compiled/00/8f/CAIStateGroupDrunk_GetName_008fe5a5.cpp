// CAIStateGroup_Drunk::GetName() -- static: the AI state group name "SG_DRUNK" as a CCharString
// Leaf: ?GetName@CAIStateGroup_Drunk@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE5A5: push esi; push -1; push "SG_DRUNK"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12213fc registers this GetName with Alloc 0x90196a (pushed before it); Alloc 0x90196a stores the vtable of CAIStateGroup_Drunk into the new object; literal "SG_DRUNK"; PDB name set ?GetName@CAIStateGroup_Drunk@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_Drunk@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe5a5::build).
#define CAIStateGroup_Drunk CAIStateGroup_DrunkLayout
#include "engine/CAIStateGroup_Drunk.h"
#undef CAIStateGroup_Drunk

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_Drunk : public CAIStateGroup_DrunkLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_Drunk::GetName()
{
    return CCharString((const char*)0x0128A994, -1);   // "SG_DRUNK"
}
