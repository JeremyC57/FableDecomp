// CAIStateGroup_TavernKeeper::GetName() -- static: the AI state group name "SG_TAVERN_KEEPER" as a CCharString
// Leaf: ?GetName@CAIStateGroup_TavernKeeper@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE521: push esi; push -1; push "SG_TAVERN_KEEPER"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1221294 registers this GetName with Alloc 0x902bfd (pushed before it); Alloc 0x902bfd -> ctor 0x901783 stores (last) the vtable of CAIStateGroup_TavernKeeper at this+0; literal "SG_TAVERN_KEEPER"; PDB name set ?GetName@CAIStateGroup_TavernKeeper@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_TavernKeeper@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe521::build).
#define CAIStateGroup_TavernKeeper CAIStateGroup_TavernKeeperLayout
#include "engine/CAIStateGroup_TavernKeeper.h"
#undef CAIStateGroup_TavernKeeper

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_TavernKeeper : public CAIStateGroup_TavernKeeperLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_TavernKeeper::GetName()
{
    return CCharString((const char*)0x0128A8E8, -1);   // "SG_TAVERN_KEEPER"
}
