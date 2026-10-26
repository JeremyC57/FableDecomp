// CAIStateGroup_MinionGossip::GetName() -- static: the AI state group name "SG_MINION_GOSSIP" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionGossip@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD3B4: push esi; push -1; push "SG_MINION_GOSSIP"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121fd54 registers this GetName with Alloc 0x8fee09 (pushed before it); Alloc 0x8fee09 stores the vtable of CAIStateGroup_MinionGossip into the new object; literal "SG_MINION_GOSSIP"; PDB name set ?GetName@CAIStateGroup_MinionGossip@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionGossip@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd3b4::build).
#define CAIStateGroup_MinionGossip CAIStateGroup_MinionGossipLayout
#include "engine/CAIStateGroup_MinionGossip.h"
#undef CAIStateGroup_MinionGossip

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionGossip : public CAIStateGroup_MinionGossipLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionGossip::GetName()
{
    return CCharString((const char*)0x01288E1C, -1);   // "SG_MINION_GOSSIP"
}
