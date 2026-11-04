// NScript::CQ_FireHeartScript::RestoreFollowers() -- one by one, unless the script thread is terminating: release each follower's stale scripted-thing handle, fade the follower back in and show its emote icon again
// Leaf: ?RestoreFollowers@CQ_FireHeartScript@NScript@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D361F0, 186 bytes (+ int3 padding), plain `ret` / tail jump. Ego_r.pdb ?RestoreFollowers@CQ_FireHeartScript@NScript@@QAEXXZ.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CScriptGameResourceObjectScriptedThingBase::IsValid() const 0x00CD23B9 /
//   Release 0x00CD2770; Game (+0x40) slots 510 EntitySetAlpha, 530 EntitySetAsDisplayingEmoteIcon.
// Shape: the landed same-TU sibling DisableFollowers 0x00D3A050 model verbatim (Followers +0x11C, FollowerHandles +0x128, the
//   handle array read through a named begin() pointer); the closing IsActiveThreadTerminating() test is the tail jump.
class CThing;
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 16 bytes
    void* PImpData;                                     // +0x08
    void* PImpInfo;                                     // +0x0c
    CScriptGameResourceObjectScriptedThingBase();       // 0x007E72A0
    virtual ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    bool IsValid() const;                               // 0x00CD23B9
    void Release();                                     // 0x00CD2770
};
namespace std {
template <> class vector<CScriptThing, allocator<CScriptThing> > {
public:
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    CScriptThing* begin() { return _M_start; }
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    CScriptThing& operator[](unsigned int n) { return *(begin() + n); }
};
template <> class vector<CScriptGameResourceObjectScriptedThingBase, allocator<CScriptGameResourceObjectScriptedThingBase> > {
public:
    CScriptGameResourceObjectScriptedThingBase* _M_start;
    CScriptGameResourceObjectScriptedThingBase* _M_finish;
    CScriptGameResourceObjectScriptedThingBase* _M_end_of_storage;
    CScriptGameResourceObjectScriptedThingBase* begin() { return _M_start; }
    CScriptGameResourceObjectScriptedThingBase& operator[](unsigned int n) { return *(begin() + n); }
    void resize(unsigned int new_size, const CScriptGameResourceObjectScriptedThingBase& x);   // 0x00CD3CCB
    void resize(unsigned int new_size) { resize(new_size, CScriptGameResourceObjectScriptedThingBase()); }   // STLport 4.6 inline
};
}
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptBase.h"

namespace NScript {
class CQ_FireHeartScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    unsigned char         _pad48[0x11c - 0x48];
    std::vector<CScriptThing, std::allocator<CScriptThing> > Followers;                       // +0x11c
    std::vector<CScriptGameResourceObjectScriptedThingBase, std::allocator<CScriptGameResourceObjectScriptedThingBase> > FollowerHandles;   // +0x128
    void RestoreFollowers();
};

void CQ_FireHeartScript::RestoreFollowers()
{
    long count = Followers.size();
    for (long i = 0; i < count; ++i) {
        if (IsActiveThreadTerminating())
            return;
        CScriptGameResourceObjectScriptedThingBase* handles = FollowerHandles.begin();
        if (handles[i].IsValid())
            handles[i].Release();
        Game->EntitySetAlpha(Followers[i], 1.0f, true);
        Game->EntitySetAsDisplayingEmoteIcon(Followers[i], true);
    }
    if (IsActiveThreadTerminating())
        return;
}
}