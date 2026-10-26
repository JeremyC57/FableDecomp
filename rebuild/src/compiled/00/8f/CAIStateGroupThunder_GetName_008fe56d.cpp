// CAIStateGroup_Thunder::GetName() -- static: the AI state group name "SG_THUNDER" as a CCharString
// Leaf: ?GetName@CAIStateGroup_Thunder@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE56D: push esi; push -1; push "SG_THUNDER"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12213ac registers this GetName with Alloc 0x902c3f (pushed before it); Alloc 0x902c3f -> ctor 0x9018b8 stores (last) the vtable of CAIStateGroup_Thunder at this+0; literal "SG_THUNDER"; PDB name set ?GetName@CAIStateGroup_Thunder@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_Thunder@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe56d::build).
#define CAIStateGroup_Thunder CAIStateGroup_ThunderLayout
#include "engine/CAIStateGroup_Thunder.h"
#undef CAIStateGroup_Thunder

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_Thunder : public CAIStateGroup_ThunderLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_Thunder::GetName()
{
    return CCharString((const char*)0x0128A940, -1);   // "SG_THUNDER"
}
