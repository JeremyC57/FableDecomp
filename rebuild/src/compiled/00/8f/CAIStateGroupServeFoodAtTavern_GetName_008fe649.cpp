// CAIStateGroup_ServeFoodAtTavern::GetName() -- static: the AI state group name "SG_SERVE_FOOD_AT_TAVERN" as a CCharString
// Leaf: ?GetName@CAIStateGroup_ServeFoodAtTavern@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE649: push esi; push -1; push "SG_SERVE_FOOD_AT_TAVERN"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122144c registers this GetName with Alloc 0x901a49 (pushed before it); Alloc 0x901a49 stores the vtable of CAIStateGroup_ServeFoodAtTavern into the new object; literal "SG_SERVE_FOOD_AT_TAVERN"; PDB name set ?GetName@CAIStateGroup_ServeFoodAtTavern@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_ServeFoodAtTavern@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe649::build).
#define CAIStateGroup_ServeFoodAtTavern CAIStateGroup_ServeFoodAtTavernLayout
#include "engine/CAIStateGroup_ServeFoodAtTavern.h"
#undef CAIStateGroup_ServeFoodAtTavern

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_ServeFoodAtTavern : public CAIStateGroup_ServeFoodAtTavernLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_ServeFoodAtTavern::GetName()
{
    return CCharString((const char*)0x0128AA60, -1);   // "SG_SERVE_FOOD_AT_TAVERN"
}
