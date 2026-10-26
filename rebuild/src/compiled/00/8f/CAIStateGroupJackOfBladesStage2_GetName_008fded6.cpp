// CAIStateGroup_JackOfBladesStage2::GetName() -- static: the AI state group name "SG_JACK_OF_BLADES_STAGE2" as a CCharString
// Leaf: ?GetName@CAIStateGroup_JackOfBladesStage2@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDED6: push esi; push -1; push "SG_JACK_OF_BLADES_STAGE2"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220b3c registers this GetName with Alloc 0x903280 (pushed before it); Alloc 0x903280 -> ctor 0x903095 stores (last) the vtable of CAIStateGroup_JackOfBladesStage2 at this+0; literal "SG_JACK_OF_BLADES_STAGE2"; PDB name set ?GetName@CAIStateGroup_JackOfBladesStage2@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_JackOfBladesStage2@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fded6::build).
#define CAIStateGroup_JackOfBladesStage2 CAIStateGroup_JackOfBladesStage2Layout
#include "engine/CAIStateGroup_JackOfBladesStage2.h"
#undef CAIStateGroup_JackOfBladesStage2

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_JackOfBladesStage2 : public CAIStateGroup_JackOfBladesStage2Layout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_JackOfBladesStage2::GetName()
{
    return CCharString((const char*)0x01289EDC, -1);   // "SG_JACK_OF_BLADES_STAGE2"
}
