// CAIStateGroup_JackDragon::GetName() -- static: the AI state group name "SG_JACK_DRAGON" as a CCharString
// Leaf: ?GetName@CAIStateGroup_JackDragon@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDEB0: push esi; push -1; push "SG_JACK_DRAGON"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12211f4 registers this GetName with Alloc 0x90269c (pushed before it); Alloc 0x90269c -> ctor 0x900647 stores (last) the vtable of CAIStateGroup_JackDragon at this+0; literal "SG_JACK_DRAGON"; PDB name set ?GetName@CAIStateGroup_JackDragon@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_JackDragon@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdeb0::build).
#define CAIStateGroup_JackDragon CAIStateGroup_JackDragonLayout
#include "engine/CAIStateGroup_JackDragon.h"
#undef CAIStateGroup_JackDragon

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_JackDragon : public CAIStateGroup_JackDragonLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_JackDragon::GetName()
{
    return CCharString((const char*)0x01289EB0, -1);   // "SG_JACK_DRAGON"
}
