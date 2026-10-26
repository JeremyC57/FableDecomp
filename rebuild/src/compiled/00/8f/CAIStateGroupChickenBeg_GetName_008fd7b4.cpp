// CAIStateGroup_ChickenBeg::GetName() -- static: the AI state group name "SG_CHICKEN_BEG" as a CCharString
// Leaf: ?GetName@CAIStateGroup_ChickenBeg@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD7B4: push esi; push -1; push "SG_CHICKEN_BEG"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121fe94 registers this GetName with Alloc 0x8ff7a0 (pushed before it); Alloc 0x8ff7a0 stores the vtable of CAIStateGroup_ChickenBeg into the new object; literal "SG_CHICKEN_BEG"; PDB name set ?GetName@CAIStateGroup_ChickenBeg@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_ChickenBeg@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd7b4::build).
#define CAIStateGroup_ChickenBeg CAIStateGroup_ChickenBegLayout
#include "engine/CAIStateGroup_ChickenBeg.h"
#undef CAIStateGroup_ChickenBeg

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_ChickenBeg : public CAIStateGroup_ChickenBegLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_ChickenBeg::GetName()
{
    return CCharString((const char*)0x01289500, -1);   // "SG_CHICKEN_BEG"
}
