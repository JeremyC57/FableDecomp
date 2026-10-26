// Behaviour test for retail 0x007e3c70: the candidate source below is embedded verbatim (from src/007e3c70.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// CTCWillResponse::CTCWillResponse(CThing*) -- construct the TC, fetch the thing's CWillResponseDef sub-definition and
//   copy its response flags into the TC's bitfield (and the slow-time flag into the thing); without a def all
//   responses except IsNotBloodyWillable default to true
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E3C70, 0xe0 bytes. Callees: CTCBase::CTCBase(CThing*) 0x00686800 (manifest label
//   is a propagated CEngineSubPrimitive2DClockSprite twin), CDefClassBase::GetSubDef<T>(CDefPointer<const T>*) 0x007E47D0
//   (folded instance; manifest names the CAICreatureWillPowerIndicatorDef instance). Vtable 0x01270534 = CTCWillResponse.
// Layout: PDB (Ego_r) names: CTCWillResponse PDef +0x0c, flag bits +0x10 (PDB field-list order); CWillResponseDef bools
//   +0x25..+0x2c (offsets equal the PDB); CThing PBaseDef at retail +0x70 and IsAffectedByBulletTimeOverride bit 5 of
//   retail +0x93 (PDB +0x80 / +0xa3: the TLC CThing is 0x10 bytes shorter before them). Local model: engine/CTCBase.h is
//   a flat generated layout without the constructor, and no CThing/CWillResponseDef header exists.
template<class T> class CDefPointer;
class CDefClassBase { public:
    template<class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x007E47D0
};
template<class T> class CDefPointer { public: T* Object; CDefPointer() : Object(0) {} };
class CThingBaseDef : public CDefClassBase { public:};
class CThing { public:
    unsigned char _pad00[0x70];
    CDefPointer<const CThingBaseDef> PBaseDef;          // +0x70
    unsigned char _pad74[0x1f];
    unsigned char NotPausedDuringCutscenes : 1;         // +0x93 bit 0
    unsigned char DeathPersistent : 1;
    unsigned char KillOnLevelUnload : 1;
    unsigned char ActivatedScript : 1;
    unsigned char LevelIsUnloading : 1;
    unsigned char IsAffectedByBulletTimeOverride : 1;   // +0x93 bit 5
    unsigned char AddingTCsPermitted : 1;
};
class CTCBase { public:
    virtual void VFunc00();
    CThing* ParentThing;                                // +0x04
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
    CTCBase(CThing* thing);                             // 0x00686800
};
class CWillResponseDef { public:
    unsigned char _base[0x25];
    bool IsForcePushable;                               // +0x25
    bool IsLifeDrainable;                               // +0x26
    bool IsLightningable;                               // +0x27
    bool IsNotBloodyWillable;                           // +0x28
    bool IsAffectedBySlowTime;                          // +0x29
    bool IsAbleToBeLifted;                              // +0x2a
    bool IsEpicPowerExplodable;                         // +0x2b
    bool IsEpicSpellable;                               // +0x2c
};
class CTCWillResponse : public CTCBase { public:
    CDefPointer<const CWillResponseDef> PDef;           // +0x0c
    bool IsForcePushable : 1;                           // +0x10 bit 0
    bool IsLifeDrainable : 1;
    bool IsLightningable : 1;
    bool IsNotBloodyWillable : 1;
    bool IsAbleToBeLifted : 1;
    bool IsEpicPowerExplodable : 1;
    bool IsEpicSpellable : 1;                           // +0x10 bit 6
    virtual void VFunc00();
    CTCWillResponse(CThing* thing);
};
CTCWillResponse::CTCWillResponse(CThing* thing) : CTCBase(thing), PDef() {
    if (ParentThing->PBaseDef.Object->GetSubDef(&PDef)) {
        const CWillResponseDef* d = PDef.Object;
        IsForcePushable = d->IsForcePushable;
        IsLifeDrainable = d->IsLifeDrainable;
        IsLightningable = d->IsLightningable;
        IsNotBloodyWillable = d->IsNotBloodyWillable;
        IsAbleToBeLifted = d->IsAbleToBeLifted;
        IsEpicPowerExplodable = d->IsEpicPowerExplodable;
        IsEpicSpellable = d->IsEpicSpellable;
        ParentThing->IsAffectedByBulletTimeOverride = d->IsAffectedBySlowTime;
    } else {
        IsForcePushable = true;
        IsLifeDrainable = true;
        IsLightningable = true;
        IsNotBloodyWillable = false;
        IsAbleToBeLifted = true;
        IsEpicPowerExplodable = true;
        IsEpicSpellable = true;
    }
}

// ===== harness =====
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <new>
static int g_tcbase; static CThing* g_tcthing; static const CWillResponseDef* g_def; static const void* g_asked_on;
void CTCBase::VFunc00() {}
void CTCWillResponse::VFunc00() {}
CTCBase::CTCBase(CThing* t) : ParentThing(t), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { ++g_tcbase; g_tcthing = t; }
template<class T> bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const { g_asked_on = this; out->Object = (const T*)g_def; return g_def != 0; }
static unsigned char g_buf[sizeof(CTCWillResponse)];
int main() {
    static CThingBaseDef base; static CThing thing; static CWillResponseDef def;
    memset(&thing, 0, sizeof thing); thing.PBaseDef.Object = &base;
    unsigned char* t93 = (unsigned char*)&thing + 0x93;
    int ok = offsetof(CTCWillResponse, PDef) == 0x0c && sizeof(CTCWillResponse) == 0x14 && offsetof(CWillResponseDef, IsEpicSpellable) == 0x2c;
    /* no sub-def: defaults (bit 7 of the flag byte is untouched) */
    memset(g_buf, 0, sizeof g_buf); g_buf[0x10] = 0x88; g_def = 0;
    CTCWillResponse* r = new (g_buf) CTCWillResponse(&thing);
    ok = ok && g_tcbase == 1 && g_tcthing == &thing && r->ParentThing == &thing && g_asked_on == &base && r->PDef.Object == 0;
    ok = ok && g_buf[0x10] == 0xf7 && *t93 == 0;
    /* with a sub-def: each flag copied, plus the thing's bullet-time override from IsAffectedBySlowTime */
    memset(&def, 0, sizeof def); def.IsForcePushable = true; def.IsLightningable = true; def.IsNotBloodyWillable = true;
    def.IsAffectedBySlowTime = true; def.IsEpicSpellable = true; g_def = &def; *t93 = 0xc5;
    memset(g_buf, 0, sizeof g_buf); g_buf[0x10] = 0xfe;
    r = new (g_buf) CTCWillResponse(&thing);
    ok = ok && r->PDef.Object == &def && g_buf[0x10] == (0x80 | 0x01 | 0x04 | 0x08 | 0x40) && *t93 == (0xc5 | 0x20);
    def.IsAffectedBySlowTime = false; r = new (g_buf) CTCWillResponse(&thing);
    ok = ok && *t93 == 0xc5;
    printf(ok ? "007e3c70_TEST PASS\n" : "007e3c70_TEST FAIL\n"); return ok ? 0 : 1;
}