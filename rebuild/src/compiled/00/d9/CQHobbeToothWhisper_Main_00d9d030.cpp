// CQ_HobbeToothWhisper::Main() -- the hobbe-tooth whisper entity: until the contest flags the end, turn the whisperer to the hero's faction when the hero hits it; then take it under scripted control and forever hunt the nearest hobbe (or a drinking one), draining its health while it lives, else walk to the hobbe spawn point
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D9D030, 1216 bytes (int3 padded).
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, IsActiveThreadTerminating 0x00F35B30, ~CScriptThing 0x004AA840,
//   CScriptThing::operator= 0x008AB980; scripted-thing handle (vtable 0x0127094C) over CBaseObject 0x0099A380, IsValid
//   0x00CD23B9, Release 0x00CD2770, MoveToThing 0x007E7300, dtor 0x007E74D0; CScriptThing slots 21 MsgIsHitBy, 41
//   MsgIsHitBySpecialAbilityFrom, 42 MsgIsHitByAnySpecialAbilityFrom; impl IsAlive (slot 75, inline null-checked);
//   CGameScriptInterface slots 7, 8 StartScriptingEntity, 77 GetNearestWithScriptName, 265 ModifyThingHealth,
//   451 GiveThingBestEnemyTarget, 597 EntitySetThingAsEnemyOfThing, 601 EntitySetInFaction.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (contest script: +0x58 contest over, +0x5D whisper hit).
// Shape: the hit test is a named bool (three flag-guarded temporaries); the hunt loop is `while (!IsActiveThreadTerminating())`
//   ending in NewScriptFrame (the handle model of the landed CSafeTrader::Main 0x00E4A280).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CScriptThingVT {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020();
    virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54)
    virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040();
    virtual bool MsgIsHitBySpecialAbilityFrom(long ability, const CCharString& name) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const;   // slot 42 (+0xa8)
    virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052();
    virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062();
    virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072();
    virtual void v073(); virtual void v074();
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
};
class CScriptThing : public CScriptThingVT { public:                  // 12 bytes, vtable 0x01238C8C {vptr, PImp.Data, PImp.Info}
    void* PImpData;
    void* PImpInfo;
    virtual ~CScriptThing();                                    // 0x004AA840
    CScriptThing& operator=(const CScriptThing& other); // 0x008AB980
};
struct CScriptThingImp {
    virtual void i00(); virtual void i01(); virtual void i02(); virtual void i03(); virtual void i04(); virtual void i05(); virtual void i06(); virtual void i07(); virtual void i08(); virtual void i09();
    virtual void i10(); virtual void i11(); virtual void i12(); virtual void i13(); virtual void i14(); virtual void i15(); virtual void i16(); virtual void i17(); virtual void i18(); virtual void i19();
    virtual void i20(); virtual void i21(); virtual void i22(); virtual void i23(); virtual void i24(); virtual void i25(); virtual void i26(); virtual void i27(); virtual void i28(); virtual void i29();
    virtual void i30(); virtual void i31(); virtual void i32(); virtual void i33(); virtual void i34(); virtual void i35(); virtual void i36(); virtual void i37(); virtual void i38(); virtual void i39();
    virtual void i40(); virtual void i41(); virtual void i42(); virtual void i43(); virtual void i44(); virtual void i45(); virtual void i46(); virtual void i47(); virtual void i48(); virtual void i49();
    virtual void i50(); virtual void i51(); virtual void i52(); virtual void i53(); virtual void i54(); virtual void i55(); virtual void i56(); virtual void i57(); virtual void i58(); virtual void i59();
    virtual void i60(); virtual void i61(); virtual void i62(); virtual void i63(); virtual void i64(); virtual void i65(); virtual void i66(); virtual void i67(); virtual void i68(); virtual void i69();
    virtual void i70(); virtual void i71(); virtual void i72(); virtual void i73(); virtual void i74();
    virtual bool IsAlive() const;                       // slot 75 (+0x12c)
};
static inline bool IsThingImpAlive(const CScriptThing& t)
{
    CScriptThingImp* imp = (CScriptThingImp*)t.PImpData;
    return imp && imp->IsAlive();
}
class CCPPointerInfo { public:                         // CCountedPointer's shared block (Ego_r.pdb CCPPointerInfo)
    long RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);         // +4 destroys the pointee
    void* Data;                                        // +8
};
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
template <class T> class CCountedPointer { public:     // retail CCountedPointer<T>: Data +0, Info +4; the release
    T*              Data;                              // dtor is 0x007E74D0's inline PImp release (wave 6 GRO rule:
    CCPPointerInfo* Info;                              // VC7.1's per-site inlining of the implicit STB dtor follows
    CCountedPointer() : Data(0), Info(0) {}            // from this size)
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
enum EScriptEntityMoveType;
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CHobbeToothContest {
    unsigned char _pad[0x58];
    bool ContestOver;                                   // +0x58
    unsigned char _pad59[4];
    bool WhisperHit;                                    // +0x5D
};
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
struct CQ_HobbeToothWhisper : CEntityScriptBase {
    CHobbeToothContest* ParentClass;                    // +0x14
    void Main();
};

void CQ_HobbeToothWhisper::Main()
{
    while (!ParentClass->ContestOver) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        bool hit = Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO"));
        if (hit) {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->WhisperHit = true;
            Game->EntitySetInFaction(Me, "FACTION_HERO");
        }
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase handle;
    if (handle.IsValid())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, (EScriptAIPriority)1)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {
      {
        CScriptThing hobbe = Game->GetNearestWithScriptName(Me, "Q_HobbeToothHobbe");
        if (!IsThingImpAlive(hobbe)) {
            if (IsActiveThreadTerminating())
                return;
            hobbe = Game->GetNearestWithScriptName(Me, "DrinkingHobbe");
        }
        if (IsThingImpAlive(hobbe)) {
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetThingAsEnemyOfThing(Me, hobbe);
            Game->GiveThingBestEnemyTarget(Me, hobbe);
            handle.MoveToThing(hobbe, 0.0f, (EScriptEntityMoveType)1, false, true, false, true);
            while (IsThingImpAlive(hobbe)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
                Game->ModifyThingHealth(Me, 1.0f, false);
            }
            if (IsActiveThreadTerminating())
                return;
        } else {
            if (IsActiveThreadTerminating())
                return;
            CScriptThing spawn = Game->GetNearestWithScriptName(Me, "SpawnNewHobbe");
            if (handle.IsValid())
                handle.Release();
            while (!Game->StartScriptingEntity(Me, handle, (EScriptAIPriority)4)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            handle.MoveToThing(spawn, 0.0f, (EScriptEntityMoveType)1, true, true, false, true);
            if (handle.IsValid())
                handle.Release();
            while (!Game->StartScriptingEntity(Me, handle, (EScriptAIPriority)1)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
      }
        Game->NewScriptFrame();
    }
}