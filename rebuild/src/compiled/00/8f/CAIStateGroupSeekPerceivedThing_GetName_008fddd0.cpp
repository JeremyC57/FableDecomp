// CAIStateGroup_SeekPerceivedThing::GetName() -- static: the AI state group name "SG_SEEK_PERCEIVED_THING" as a CCharString
// Leaf: ?GetName@CAIStateGroup_SeekPerceivedThing@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDDD0: push esi; push -1; push "SG_SEEK_PERCEIVED_THING"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122068c registers this GetName with Alloc 0x9003d2 (pushed before it); Alloc 0x9003d2 stores the vtable of CAIStateGroup_SeekPerceivedThing into the new object; literal "SG_SEEK_PERCEIVED_THING"; PDB name set ?GetName@CAIStateGroup_SeekPerceivedThing@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_SeekPerceivedThing@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fddd0::build).
#define CAIStateGroup_SeekPerceivedThing CAIStateGroup_SeekPerceivedThingLayout
#include "engine/CAIStateGroup_SeekPerceivedThing.h"
#undef CAIStateGroup_SeekPerceivedThing

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_SeekPerceivedThing : public CAIStateGroup_SeekPerceivedThingLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_SeekPerceivedThing::GetName()
{
    return CCharString((const char*)0x01289D08, -1);   // "SG_SEEK_PERCEIVED_THING"
}
