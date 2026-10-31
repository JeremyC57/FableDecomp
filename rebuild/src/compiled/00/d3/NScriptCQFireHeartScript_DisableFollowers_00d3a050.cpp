// NScript::CQ_FireHeartScript::DisableFollowers() -- collect the hero's followers, give each a scripted-thing handle, and one by one take scripted control of each follower (releasing a stale handle, retrying a script frame at a time), fade it out and hide its emote icon
// Leaf: ?DisableFollowers@CQ_FireHeartScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D3A050, 330 bytes (+ int3 padding), plain `ret`.
// Identity: Ego_r.pdb 00d473e0 / FableWin.pdb ?DisableFollowers@CQ_FireHeartScript@NScript@@QAEXXZ.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940 (engine/CScriptBase.h); the scripted-thing handle's ctor 0x007E72A0 /
//   dtor 0x007E74D0 / IsValid() const 0x00CD23B9 / Release 0x00CD2770 (engine/CScriptGameResourceObjectScriptedThingBase.h
//   names); vector<CScriptGameResourceObjectScriptedThingBase>::resize(size_type, const T&) 0x00CD3CCB (its body calls the
//   bound erase 0x00CD3228 and _M_fill_insert 0x00CD378D: STLport resize); through Game (+0x40, engine/CGameScriptInterface.h):
//   slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 231 GetFollowingEntityList, 510 EntitySetAlpha,
//   530 EntitySetAsDisplayingEmoteIcon.
// Layout: CQ_FireHeartScript as the landed Init 0x00D347B0 (retail = Ego_r.pdb less 0x54 here): Followers
//   (vector<CScriptThing>) +0x11C (PDB +0x170), FollowerHandles (vector<CScriptGameResourceObjectScriptedThingBase>) +0x128
//   (PDB +0x180). The handle is the row-local model of the landed RunCeremonyCutscene 0x00D49D50 / map operator[] 0x00CD3D2E:
//   retail calls the ctor/dtor COMDATs 0x007E72A0 / 0x007E74D0 here, while the shared header's implicit members expand inline.
// Shape: `long count = Followers.size()` (signed /12); the handles are resized through STLport's inline resize(n) (the default-element temporary); a counted loop
//   with both element strides strength-reduced (ebp += 16, ebx += 12), counter and count in frame words. The stale-handle
//   release reads the handle array through a NAMED begin() pointer (`handles[i].IsValid()` / `handles[i].Release()`): VC7.1
//   loads the start into eax and forms the spilled element address with `lea ecx,[eax+ebp]` as retail does (a handle pointer,
//   a reference, direct operator[] calls and begin()+i all give `mov ecx,[esi+128h] ; add ecx,ebp`).
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
    void DisableFollowers();
};

void CQ_FireHeartScript::DisableFollowers()
{
    Game->GetFollowingEntityList(Game->GetHero(), Followers);
    long count = Followers.size();
    FollowerHandles.resize(count);
    for (long i = 0; i < count; ++i) {
        if (IsActiveThreadTerminating())
            return;
        CScriptGameResourceObjectScriptedThingBase* handles = FollowerHandles.begin();
        if (handles[i].IsValid())
            handles[i].Release();
        while (!Game->StartScriptingEntity(Followers[i], FollowerHandles[i], (EScriptAIPriority)4)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->EntitySetAlpha(Followers[i], 0.0f, true);
        Game->EntitySetAsDisplayingEmoteIcon(Followers[i], false);
    }
    if (IsActiveThreadTerminating())
        return;
}
}