// CScriptThing::SetFriendsWithEverythingFlag(bool flag) -- script-handle proxy: forwards to the same virtual on the implementation object (PImp) when there is one
// Retail: Fable.exe @ 0x004AB060, vtable 0x01238C8C slot 67 (+0x10c); a null test then a tail jump through the
// implementation's vtable (`ret 4` when PImp is null). Same proxy shape as landed 0x004AAF40.
// Layout: engine/CScriptThing.h (+0x04 PImp, a CCountedPointer<CScriptThing>); the class is modelled with its virtuals
// so the slot offset comes from the declaration order (CScriptThing.h is a data-only header).
class CCPPointerInfo;
template <class T> class CCountedPointer { public:
    T*              Data;
    T* operator->() const { return Data; }              // inline accessors: a direct `.Data->` read loads into eax
    bool IsValid() const { return Data != 0; }
    CCPPointerInfo* Info;
};
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

void CScriptThing::SetFriendsWithEverythingFlag(bool flag)
{
    if (PImp.IsValid())
        PImp->SetFriendsWithEverythingFlag(flag);
}