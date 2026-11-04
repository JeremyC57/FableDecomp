// NCreatureAction::CActionEventExplosion::SetDataFromString(const CCharString& data) -- parse "explosion def, attach type[, attachment object]" event data: field 0 names the explosion definition, field 1 is PREFIX_TYPE (HELPER clears Dummy, DUMMY sets it, ACTION_HELPER sets UseActionHelper, anything else fails), field 2 (unless an action helper) the attachment object; always reports false
// Leaf: ?SetDataFromString@CActionEventExplosion@NCreatureAction@@MAE_NABVCCharString@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00871290, 408 bytes, `ret 4`; Ego_r.pdb
//   ?SetDataFromString@CActionEventExplosion@NCreatureAction@@MAE_NABVCCharString@@@Z (protected virtual).
// Callees: `anonymous namespace'::GetDataFromEventString(const CCharString&, long, CCharString&) @ 0x00870C90 (__fastcall, the
//   landed CActionEventAddObject / CActionEventCameraShake spelling), CCharString() 0x0099E4B0 / ~CCharString 0x0099EAE0,
//   CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName const 0x009AD410,
//   CCharString::FindCharPos(char) const 0x0099E5A0, CCharString::GetSubString(unsigned long, unsigned long) const 0x0099EC70,
//   CBasicString<char>::operator==(const char*) const 0x004115A0.
// Data: "" 0x0122D70E, "HELPER" 0x01277490, "DUMMY" 0x01277488, "ACTION_HELPER" 0x01277524.
// Layout: engine/NCreatureActionEvents.h (Ego_r.pdb CActionEventExplosion: AttachmentObject +0x14, ExplosionDefIndex +0x18,
//   Dummy +0x1c, UseActionHelper +0x1d).
// Shape: the definition name is a local in its own block; the type is compared with the inline CCharString::operator==(const char*)
//   three times (VC7.1 shares the one null test: strcmp against "" in the null arm, the CBasicString call in the other).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_SUBSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#include <string.h>
#include "fable_string.h"
#include "engine/CGameDefinitionManager.h"
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
bool CActionEventExplosion::SetDataFromString(const CCharString& data)
{
    {
        CCharString defName;
        if (!GetDataFromEventString(data, 0, defName))
            return false;
        ExplosionDefIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(defName);
    }
    {
        CCharString attachType;
        if (!GetDataFromEventString(data, 1, attachType))
            return false;
        long underscore = attachType.FindCharPos('_');
        CCharString prefix = attachType.GetSubString(0, underscore);
        CCharString type = attachType.GetSubString(underscore + 1, attachType.GetLength());
        if (type == "HELPER")
            Dummy = false;
        else if (type == "DUMMY")
            Dummy = true;
        else if (type == "ACTION_HELPER")
            UseActionHelper = true;
        else
            return false;
    }
    if (UseActionHelper)
        return false;
    GetDataFromEventString(data, 2, AttachmentObject);
    return false;
}
}