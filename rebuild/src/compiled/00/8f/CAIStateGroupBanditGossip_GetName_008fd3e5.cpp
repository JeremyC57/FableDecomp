// CAIStateGroup_BanditGossip::GetName() -- static: the AI state group name "SG_BANDIT_GOSSIP" as a CCharString
// Leaf: ?GetName@CAIStateGroup_BanditGossip@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD3E5: push esi; push -1; push "SG_BANDIT_GOSSIP"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121fd7c registers this GetName with Alloc 0x8fee49 (pushed before it); Alloc 0x8fee49 stores the vtable of CAIStateGroup_BanditGossip into the new object; literal "SG_BANDIT_GOSSIP"; PDB name set ?GetName@CAIStateGroup_BanditGossip@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_BanditGossip@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd3e5::build).
#define CAIStateGroup_BanditGossip CAIStateGroup_BanditGossipLayout
#include "engine/CAIStateGroup_BanditGossip.h"
#undef CAIStateGroup_BanditGossip

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_BanditGossip : public CAIStateGroup_BanditGossipLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_BanditGossip::GetName()
{
    return CCharString((const char*)0x01288E78, -1);   // "SG_BANDIT_GOSSIP"
}
