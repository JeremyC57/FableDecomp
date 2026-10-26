// CAIStateGroup_CleanHouse::GetName() -- static: the AI state group name "SG_CLEAN_HOUSE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_CleanHouse@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDB42: push esi; push -1; push "SG_CLEAN_HOUSE"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220484 registers this GetName with Alloc 0x8ffef4 (pushed before it); Alloc 0x8ffef4 stores the vtable of CAIStateGroup_CleanHouse into the new object; literal "SG_CLEAN_HOUSE"; PDB name set ?GetName@CAIStateGroup_CleanHouse@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_CleanHouse@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdb42::build).
#define CAIStateGroup_CleanHouse CAIStateGroup_CleanHouseLayout
#include "engine/CAIStateGroup_CleanHouse.h"
#undef CAIStateGroup_CleanHouse

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_CleanHouse : public CAIStateGroup_CleanHouseLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_CleanHouse::GetName()
{
    return CCharString((const char*)0x01289A88, -1);   // "SG_CLEAN_HOUSE"
}
