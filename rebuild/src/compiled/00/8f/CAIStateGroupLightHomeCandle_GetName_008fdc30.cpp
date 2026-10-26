// CAIStateGroup_LightHomeCandle::GetName() -- static: the AI state group name "SG_HOME_CANDLE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_LightHomeCandle@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDC30: push esi; push -1; push "SG_HOME_CANDLE"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122063c registers this GetName with Alloc 0x90251c (pushed before it); Alloc 0x90251c -> ctor 0x900008 stores (last) the vtable of CAIStateGroup_LightHomeCandle at this+0; literal "SG_HOME_CANDLE"; PDB name set ?GetName@CAIStateGroup_LightHomeCandle@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_LightHomeCandle@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdc30::build).
#define CAIStateGroup_LightHomeCandle CAIStateGroup_LightHomeCandleLayout
#include "engine/CAIStateGroup_LightHomeCandle.h"
#undef CAIStateGroup_LightHomeCandle

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_LightHomeCandle : public CAIStateGroup_LightHomeCandleLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_LightHomeCandle::GetName()
{
    return CCharString((const char*)0x01289B78, -1);   // "SG_HOME_CANDLE"
}
