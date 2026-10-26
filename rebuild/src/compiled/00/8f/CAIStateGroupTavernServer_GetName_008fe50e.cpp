// CAIStateGroup_TavernServer::GetName() -- static: the AI state group name "SG_TAVERN_SERVER" as a CCharString
// Leaf: ?GetName@CAIStateGroup_TavernServer@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE50E: push esi; push -1; push "SG_TAVERN_SERVER"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122126c registers this GetName with Alloc 0x902be4 (pushed before it); Alloc 0x902be4 -> ctor 0x9016d9 stores (last) the vtable of CAIStateGroup_TavernServer at this+0; literal "SG_TAVERN_SERVER"; PDB name set ?GetName@CAIStateGroup_TavernServer@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_TavernServer@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe50e::build).
#define CAIStateGroup_TavernServer CAIStateGroup_TavernServerLayout
#include "engine/CAIStateGroup_TavernServer.h"
#undef CAIStateGroup_TavernServer

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_TavernServer : public CAIStateGroup_TavernServerLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_TavernServer::GetName()
{
    return CCharString((const char*)0x0128A8D4, -1);   // "SG_TAVERN_SERVER"
}
