// CAIStateGroup_BirdGroundWander::GetName() -- static: the AI state group name "SG_BIRD_GROUND_WANDER" as a CCharString
// Leaf: ?GetName@CAIStateGroup_BirdGroundWander@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD86D: push esi; push -1; push "SG_BIRD_GROUND_WANDER"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121ff5c registers this GetName with Alloc 0x8ff8ce (pushed before it); Alloc 0x8ff8ce stores the vtable of CAIStateGroup_BirdGroundWander into the new object; literal "SG_BIRD_GROUND_WANDER"; PDB name set ?GetName@CAIStateGroup_BirdGroundWander@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_BirdGroundWander@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd86d::build).
#define CAIStateGroup_BirdGroundWander CAIStateGroup_BirdGroundWanderLayout
#include "engine/CAIStateGroup_BirdGroundWander.h"
#undef CAIStateGroup_BirdGroundWander

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_BirdGroundWander : public CAIStateGroup_BirdGroundWanderLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_BirdGroundWander::GetName()
{
    return CCharString((const char*)0x01289654, -1);   // "SG_BIRD_GROUND_WANDER"
}
