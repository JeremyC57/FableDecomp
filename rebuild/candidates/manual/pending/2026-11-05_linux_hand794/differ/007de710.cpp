// CTCWallMount::AttachObject(CThing& object, long boughtForAmount) -- mount a trophy: place it so its WEAPON_FOCUS dummy sits on the mount's WALL_MOUNT_DUMMY_OBJECT, freeze its physics, take it out of limbo, detach particles, drop targeting, count the hero's mounted trophies, persist both things and fire the wall-mount effects
// Grade: DIFFER(2574v2578) -- only x87 order in the matrix block: retail emits each PostMultiply sum k3, k2, k1 with the memory operand loaded first (`fld [a.E13] ; fmul st(2)`), mine k1 first; retail spills the inverse scale with fstp before its first use and schedules the `rep movsd` copy of objectMatrix before the fdivr
// Next idea: another SetInverseOfUniformScaleRotationTranslation / PostMultiply body spelling (the out-of-line PostMultiply 0x00C1CF20 is k1..k3 in source; writing k3..k1 did not flip the inlined order: ATTEMPTS a4), or an explicit SetMultiply into a temporary
// Leaf: ?AttachObject@CTCWallMount@@QAE_NAAVCThing@@J@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_POSTMULTIPLY
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/CMatrix3x4.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CWorld.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"

class CTCParticleAttacher : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_PARTICLE_ATTACHER; }
    void DetachParticles();                                              // 0x007F8860
};
class CTCTargeted : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_TARGETED; }
    void SetTargetable(bool value) { Targetable = value; }               // FableWin ?SetTargetable@CTCTargeted@@QAEX_N@Z (inline)
private:
    unsigned long TargetingFlags;                                        // +0x0c (Ego_r.pdb)
    bool Targetable;                                                     // +0x10
};
class CTCWallMountEffects : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_WALL_MOUNT_EFFECTS; }
    void OnPutOnMount();                                                 // 0x007DF3E0 (Ego_r.pdb ?OnPutOnMount@CTCWallMountEffects@@QAEXXZ; follows HasEffect 0x007DF3D0 as in Ego_r)
};
class CTCHeroStatsTrophies : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_HERO_STATS; }
    unsigned char _pad_0x0c[0xd4 - 0x0c];
    long NumMountedTrophies;                                             // +0xd4
};

class CTCWallMount : public CTCBase {
public:
    bool AttachObject(CThing& object, long boughtForAmount);
private:
    long BoughtForAmount;                                                // +0x0c
    CIntelligentPointer<CThing> Trophy;                                  // +0x10
};

template<class T> inline bool __fastcall IS_THING_ALIVE(T* const& thing) { return thing && !thing->IsDead(); }

bool CTCWallMount::AttachObject(CThing& object, long boughtForAmount)
{
    Trophy = &object;
    bool attach = true;
    CRightHandedSet objectRH = object.GetPhysicsTC()->GetRHSet();
    C3DVector objectPos = object.GetPhysicsTC()->GetPosition();
    C3DVector wallPos;
    CRightHandedSet wallRH;
    NGraphicAppearance::CTCGraphicAppearance* wallAppearance = GetParentThing().GetTCGraphicAppearance();
    bool failed = !(wallAppearance->GetDummyObject("WALL_MOUNT_DUMMY_OBJECT", 0, 0, wallPos, wallRH, 1.0f)
                    || wallAppearance->GetDummyObject("WALL_MOUNT_DUMMY_OBJECT ", 0, 0, wallPos, wallRH, 1.0f));
    if (failed)
        attach = false;
    C3DVector focusPos;
    CRightHandedSet focusRH;
    NGraphicAppearance::CTCGraphicAppearance* objectAppearance = object.GetTCGraphicAppearance();
    if (!(objectAppearance->GetDummyObject("WEAPON_FOCUS_01", 0, 0, focusPos, focusRH, 1.0f)
          || objectAppearance->GetDummyObject("WEAPON_FOCUS_02", 0, 0, focusPos, focusRH, 1.0f))) {
    } else if (attach) {
        CMatrix3x4 objectMatrix;
        objectMatrix.Set(objectRH.GetRight(), objectRH.GetForward(), objectRH.GetUp(), objectPos);
        CMatrix3x4 focusMatrix;
        focusMatrix.Set(focusRH.GetRight(), focusRH.GetForward(), focusRH.GetUp(), focusPos);
        CMatrix3x4 wallMatrix;
        wallMatrix.Set(wallRH.GetRight(), wallRH.GetForward(), wallRH.GetUp(), wallPos);
        CMatrix3x4 focusInverse;
        focusInverse.SetInverseOfUniformScaleRotationTranslation(focusMatrix);
        objectMatrix.PostMultiply(focusInverse);
        objectMatrix.PostMultiply(wallMatrix);
        object.GetPhysicsTC()->ResetPosition(C3DVector(objectMatrix.E41, objectMatrix.E42, objectMatrix.E43));
        object.GetPhysicsTC()->ResetRHSet(CRightHandedSet(C3DVector(objectMatrix.E31, objectMatrix.E32, objectMatrix.E33),
                                                          C3DVector(objectMatrix.E21, objectMatrix.E22, objectMatrix.E23)));
    }
    object.GetPhysicsTC()->EnablePhysics(false);
    object.SetInLimbo(false);
    CTCParticleAttacher* attacher;
    if (object.GetTC(&attacher))
        attacher->DetachParticles();
    CTCTargeted* targeted;
    if (object.GetTC(&targeted))
        targeted->SetTargetable(false);
    CThing* hero = (CThing*)GetWorld().PeekMyPlayer()->PlayerCharacter.Get();
    CTCHeroStatsTrophies* stats;
    if (IS_THING_ALIVE(hero) && hero->GetTC(&stats))
        ++stats->NumMountedTrophies;
    BoughtForAmount = boughtForAmount;
    GetParentThing().SetAsGameStatePersisted(true);
    Trophy.Get()->SetAsGameStatePersisted(true);
    CTCWallMountEffects* effects;
    if (object.GetTC(&effects))
        effects->OnPutOnMount();
    return true;
}
