// CAIStateGroup_WifeGiveYouGift::GetName() -- static: the AI state group name "SG_WIFE_GIVE_YOU_GIFT" as a CCharString
// Leaf: ?GetName@CAIStateGroup_WifeGiveYouGift@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE718: push esi; push -1; push "SG_WIFE_GIVE_YOU_GIFT"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122158c registers this GetName with Alloc 0x901bd3 (pushed before it); Alloc 0x901bd3 stores the vtable of CAIStateGroup_WifeGiveYouGift into the new object; literal "SG_WIFE_GIVE_YOU_GIFT"; PDB name set ?GetName@CAIStateGroup_WifeGiveYouGift@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_WifeGiveYouGift@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe718::build).
#define CAIStateGroup_WifeGiveYouGift CAIStateGroup_WifeGiveYouGiftLayout
#include "engine/CAIStateGroup_WifeGiveYouGift.h"
#undef CAIStateGroup_WifeGiveYouGift

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_WifeGiveYouGift : public CAIStateGroup_WifeGiveYouGiftLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_WifeGiveYouGift::GetName()
{
    return CCharString((const char*)0x0128AC2C, -1);   // "SG_WIFE_GIVE_YOU_GIFT"
}
