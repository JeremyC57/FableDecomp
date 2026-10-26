// CAIStateGroup_IceTrollIcicleAssault::GetName() -- static: the AI state group name "SG_ICE_TROLL_ICICLE_ASSAULT" as a CCharString
// Leaf: ?GetName@CAIStateGroup_IceTrollIcicleAssault@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE43E: push esi; push -1; push "SG_ICE_TROLL_ICICLE_ASSAULT"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220ac4 registers this GetName with Alloc 0x902b5e (pushed before it); Alloc 0x902b5e -> ctor 0x9013ac stores (last) the vtable of CAIStateGroup_IceTrollIcicleAssault at this+0; literal "SG_ICE_TROLL_ICICLE_ASSAULT"; PDB name set ?GetName@CAIStateGroup_IceTrollIcicleAssault@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_IceTrollIcicleAssault@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe43e::build).
#define CAIStateGroup_IceTrollIcicleAssault CAIStateGroup_IceTrollIcicleAssaultLayout
#include "engine/CAIStateGroup_IceTrollIcicleAssault.h"
#undef CAIStateGroup_IceTrollIcicleAssault

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_IceTrollIcicleAssault : public CAIStateGroup_IceTrollIcicleAssaultLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_IceTrollIcicleAssault::GetName()
{
    return CCharString((const char*)0x0128A7B4, -1);   // "SG_ICE_TROLL_ICICLE_ASSAULT"
}
