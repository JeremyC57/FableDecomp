// NCreatureAction::CActionEventTriggerParticle::SetDataFromString(const CCharString& data) -- parse "effect, attach type[, attachment object][, drop]" event data: field 0 is the effect name, field 1 PREFIX_TYPE (WEAPON prefix sets Weapon, HELPER / DUMMY type add attachment flag 4 / 2 to the default 8), then the attachment object when either flag is set, and a present next field sets DropRatherThanAttach
// Leaf: ?SetDataFromString@CActionEventTriggerParticle@NCreatureAction@@MAE_NABVCCharString@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00870E00, 438 bytes, `ret 4`; Ego_r.pdb
//   ?SetDataFromString@CActionEventTriggerParticle@NCreatureAction@@MAE_NABVCCharString@@@Z (protected virtual).
// Callees: `anonymous namespace'::GetDataFromEventString(const CCharString&, long, CCharString&) @ 0x00870C90 (__fastcall, the
//   landed CActionEventAddObject / CActionEventCameraShake spelling), CCharString() 0x0099E4B0 / ~CCharString 0x0099EAE0,
//   CCharString::FindCharPos(char) const 0x0099E5A0, CCharString::GetSubString(unsigned long, unsigned long) const 0x0099EC70,
//   CBasicString<char>::operator==(const char*) const 0x004115A0.
// Data: "" 0x0122D70E, "WEAPON" 0x01277498, "HELPER" 0x01277490, "DUMMY" 0x01277488.
// Layout: engine/NCreatureActionEvents.h (Ego_r.pdb CActionEventTriggerParticle: AttachmentFlags +0x14, AttachmentObject +0x18,
//   EffectName +0x1c, Weapon +0x20, DropRatherThanAttach +0x21).
// Shape: the field index is one counter (constant-propagated into the first three calls, materialised for the last); the
//   attach-type string is a named local in its own block, the drop flag's string a second block; the prefix and type are compared
//   with the inline CCharString::operator==(const char*) (VC7.1 shares the type's null test across the HELPER / DUMMY tests).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_SUBSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#include <string.h>
#include "fable_string.h"
#include "engine/NCreatureActionEvents.h"

// CBasicString<T>::operator==(const T*) is a template member, so its body is visible in this TU as in retail (the COMDAT the
// linker kept is 0x004115A0; the landed same-TU sibling 0x00870FC0 carries the same body). With the body visible VC7.1 keeps the
// type's string data in esi across the calls (`mov ecx,esi`); a declaration alone loads it into ecx and copies it.

template<> bool CBasicString<char>::operator==(const char* text) const
{
    const signed char* first = (const signed char*)PData;
    const signed char* second = (const signed char*)text;
    int result = 0;
    while (*first || *second) {
        if (*first < *second) { result = -1; break; }
        if (*first > *second) { result = 1; break; }
        ++first; ++second;
    }
    return result == 0;
}

namespace { bool __fastcall GetDataFromEventString(const CCharString& data, long index, CCharString& out); }   // 0x00870C90

namespace NCreatureAction {
bool CActionEventTriggerParticle::SetDataFromString(const CCharString& data)
{
    long field = 0;
    if (!GetDataFromEventString(data, field++, EffectName))
        return false;
    Weapon = false;
    AttachmentFlags = 8;
    {
        CCharString attachType;
        if (!GetDataFromEventString(data, field++, attachType))
            return false;
        long underscore = attachType.FindCharPos('_');
        if (underscore > -1) {
            CCharString prefix = attachType.GetSubString(0, underscore);
            CCharString type = attachType.GetSubString(underscore + 1, attachType.GetLength());
            if (prefix == "WEAPON")
                Weapon = true;
            if (type == "HELPER")
                AttachmentFlags |= 4;
            else if (type == "DUMMY")
                AttachmentFlags |= 2;
        }
        if (AttachmentFlags & 6) {
            if (!GetDataFromEventString(data, field++, AttachmentObject))
                return false;
        }
    }
    {
        CCharString drop;
        if (GetDataFromEventString(data, field, drop))
            DropRatherThanAttach = true;
    }
    return true;
}
}