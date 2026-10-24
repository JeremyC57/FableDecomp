// NCreatureAction::CActionEventRemoveObject::SetDataFromString(const CCharString&) -- parse the event data's first field into Hand
// Leaf: ?SetDataFromString@CActionEventRemoveObject
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00870DE0, length 23 bytes (+ int3 padding), `ret 4`, vtable 0x0125C1A0 slot 4.
// Callees: GetDataFromEventString(const CCharString&, long index, CCharString* out) @ 0x00870C90 (__fastcall: string in
//   ecx, index in edx, out pointer on the stack; landed callers 0x00870D70 / 0x00870FC0 use the same declaration).
// Name: FableWin.pdb `?SetDataFromString@CActionEventRemoveObject@NCreatureAction@@MAE_NABVCCharString@@@Z` (protected
//   virtual). Layout: Ego_r.pdb CActionEventRemoveObject {CActionEventBase 0x14 bytes, Hand +0x14}.
// Shape: `return GetDataFromEventString(...) ? true : false;` re-normalises the callee's bool as `test al,al ; setne al`;
//   the `!= false` and `!!` spellings give `neg al ; sbb eax,eax ; neg eax` instead.
#include "fable_string.h"

bool __fastcall GetDataFromEventString(const CCharString& data, long index, CCharString* out);   // 0x00870C90

namespace NCreatureAction {
struct CActionEventRemoveObject {
    void* __vftable;                               // +0x00
    unsigned char _base[0x10];                     // +0x04 CActionEventBase words (Finished, ActivationFrame, PThing, PCameraManager)
    CCharString Hand;                              // +0x14
    bool SetDataFromString(const CCharString& data);
};
}

bool NCreatureAction::CActionEventRemoveObject::SetDataFromString(const CCharString& data)
{
    return GetDataFromEventString(data, 0, &Hand) ? true : false;
}