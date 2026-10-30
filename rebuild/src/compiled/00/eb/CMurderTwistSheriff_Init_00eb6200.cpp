// CMurderTwistSheriff::Init() -- make the sheriff fight the hero (best enemy target), killable, damageable, pushable and moving in actions, clear his friends-with-everything flag, then drop his old scripting handle
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB6200, 166 bytes (+ int3 padding). __fastcall, `ret`.
// Callees (CGameScriptInterface virtuals, ghidra_out/cgsi_slots_aligned.tsv): GetHero 70 (+0x118) 0x00891CA0,
//   GiveThingBestEnemyTarget 451 (+0x70c) 0x008A0260, EntitySetAsDamageable 516 (+0x810) 0x0088EAD0, EntitySetAsKillable 517
//   (+0x814) 0x0088EB30, EntitySetAsUseMovementInActions 529 (+0x844) 0x0088EC20, SetIsPushableByHero 844 (+0xd30) 0x008A6DD0
//   (CScriptThing BY VALUE: inline CBaseClass vtable 0x01231710 then CScriptThing vtable 0x01238C8C stored into the argument area,
//   PImp copied with add-ref); the handle's IsScripting 0x00CD23B9 / Release 0x00CD2770 (manifest labels
//   "C3DMeshInfo::HasPhysicsMesh" / "CMemoryDataOutputStream::Clear" are propagated; see landed CFH_BriarRose::Main 0x00D37330).
// Class: the catalog label is propagated over four byte-similar Init bodies (no vtable in .rdata references this address);
//   Ego_r.pdb CMurderTwistSheriff (56 bytes) has MyEntityHandle at +0x1c, which is the handle tested first. Byte twin of 0x00EB53A0 (another class); same prologue as 0x00EB1EC0, then
//   CScriptThing slot 67 SetFriendsWithEverythingFlag(false) 0x004AB060 on Me before the handle release.
// Shape: the nested GetHero call fetches the interface vtable once (ebx); the by-value CScriptThing copy ctor is inline;
//   the Release is a tail jump.
class CCPPointerInfo { public: long Count; };
template <class T> class CCountedPointer { public:
    T* Data;                                     // +0
    CCPPointerInfo* Info;                        // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->Count; }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0, implicit copy
struct CScriptThingImp;
class CScriptThing : public CBaseClass { public:                    // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImp> PImp;       // +0x04
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) {}
    virtual ~CScriptThing();   // slot 0 (+0x0)
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016();
    virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032();
    virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040();
    virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048();
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056();
    virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066();
    virtual void SetFriendsWithEverythingFlag(bool on);   // slot 67 (+0x10c)
};
class CScriptGameResourceObjectScriptedThingBase { public:
    unsigned char _body[0x10];
    bool IsScripting();                          // 0x00CD23B9
    void Release();                              // 0x00CD2770
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CV_MurderTwistFinalScript;
struct CQ_SunnyvaleMasterData;
struct CMurderTwistSheriff {
    void*                       __vftable;       // +0x00
    CGameScriptInterface*       Game;            // +0x04
    CScriptThing                Me;              // +0x08
    CV_MurderTwistFinalScript*  ParentClass;     // +0x14
    CQ_SunnyvaleMasterData*     PMasterData;     // +0x18
    CScriptGameResourceObjectScriptedThingBase MyEntityHandle;   // +0x1c
    void Init();
};

void CMurderTwistSheriff::Init()
{
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    Game->EntitySetAsKillable(Me, true, true);
    Game->EntitySetAsDamageable(Me, true);
    Game->SetIsPushableByHero(Me, true);
    Game->EntitySetAsUseMovementInActions(Me, true);
    Me.SetFriendsWithEverythingFlag(false);
    if (MyEntityHandle.IsScripting())
        MyEntityHandle.Release();
}