// CAIStateGroup_SitInPub::GetName() -- static: the AI state group name "SG_SIT_IN_PUB" as a CCharString
// Leaf: ?GetName@CAIStateGroup_SitInPub@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDCFD: push esi; push -1; push "SG_SIT_IN_PUB"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220614 registers this GetName with Alloc 0x90259b (pushed before it); Alloc 0x90259b -> ctor 0x9001f2 stores (last) the vtable of CAIStateGroup_SitInPub at this+0; literal "SG_SIT_IN_PUB"; PDB name set ?GetName@CAIStateGroup_SitInPub@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_SitInPub@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdcfd::build).
#define CAIStateGroup_SitInPub CAIStateGroup_SitInPubLayout
#include "engine/CAIStateGroup_SitInPub.h"
#undef CAIStateGroup_SitInPub

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_SitInPub : public CAIStateGroup_SitInPubLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_SitInPub::GetName()
{
    return CCharString((const char*)0x01289C34, -1);   // "SG_SIT_IN_PUB"
}
