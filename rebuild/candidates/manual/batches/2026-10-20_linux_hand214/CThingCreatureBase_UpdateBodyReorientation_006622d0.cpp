// CThingCreatureBase::UpdateBodyReorientation() -- run the body reorienter when it wants an update (and stop it once no action holds it), then level a body-set YZ pitch back to 0 once the creature is alive, idle, upright and capable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006622D0, length 186 bytes, `ret`.
// Callees: CThingBodyReorienter::IsUpdateNeeded @ 0x00863250, CThingBodyReorienter::UpdateBodyReorienation @ 0x00863E10
//   (PDB spelling), the reorienter's one-byte flag setter @ 0x00863260 (`[ecx+8] = arg`; manifest label
//   "CTCPhysicsNavigator::EnableNavigator" is propagated, real name unproven), IsLyingDown @ 0x0085F2B0 and
//   IsIncapacitated @ 0x0085F0C0 (free `__fastcall` thing predicates), CTCPhysicsBase::GetFacingAngleYZ (slot 64,
//   +0x100) / SetFacingAngleYZ (slot 67, +0x10C).
// Global: g_epsilon_0129ba3c (1e-4f).
// Layout (retail offsets; the engine header carries the Ego_r.pdb ones, so the class is local): CThing PhysicsTC +0x60,
//   Dead +0x91 bit 0; CThingCreatureBase PCurrentAction.Data +0x100 (PDB +0x11c), BodyReorienter +0x140 (PDB +0x164),
//   the second creature flag byte +0x15d (PDB +0x185) with HadFacingAngleYZSetByBodyOrientation at bit 5;
//   CCreatureActionBase Finished +0x61.
// Shape: the reorienter is reached through scoped_ptr's inline operator-> (re-read for every call); "performing any
//   action" is the landed inline IsPerformingAnyAction (0x006622A0) test, duplicated in both halves; the alive test
//   is the inline `Dead ? false : true` (`not al ; test al,1`).
#include <math.h>
#pragma intrinsic(fabs)

#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();

struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    FILL10(1) FILL10(2) FILL10(3) FILL10(4) FILL10(5)
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual float GetFacingAngleYZ();                   // slot 64 (+0x100)
    virtual void v65(); virtual void v66();
    virtual void SetFacingAngleYZ(float angle);         // slot 67 (+0x10C)
};

struct CThingBodyReorienter {
    bool IsUpdateNeeded();                              // 0x00863250
    void UpdateBodyReorienation();                      // 0x00863E10
    void SetFlag_00863260(bool value);                  // 0x00863260 (`[this+8] = value`)
};

template<class T> struct scoped_ptr {
    T* px;
    T* get() const { return px; }
    T* operator->() const { return px; }
};

struct CCreatureActionBase {
    unsigned char _pad[0x61];
    bool Finished;                                      // +0x61
};

struct CThing {
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char Dead : 1;                             // +0x91 bit 0
    unsigned char _bits91 : 7;
    unsigned char _pad92[2];
    bool IsAlive() const { return Dead ? false : true; }
};

extern const float g_epsilon_0129ba3c;                  // 0x0129BA3C (1e-4f)
bool __fastcall IsLyingDown(CThing* thing);             // 0x0085F2B0
bool __fastcall IsIncapacitated(CThing* thing);         // 0x0085F0C0

struct CThingCreatureBase : CThing {
    unsigned char _pad94[0x100 - 0x94];
    CCreatureActionBase* PCurrentAction_Data;           // +0x100 CCountedPointer<CCreatureActionBase>::Data
    unsigned char _pad104[0x140 - 0x104];
    scoped_ptr<CThingBodyReorienter> BodyReorienter;    // +0x140
    unsigned char _pad144[0x15d - 0x144];
    bool UseMovementInActions : 1;                      // +0x15d bit 0
    bool UpdateMovementVectorThisFrame : 1;             // bit 1
    bool AbortingCurrentAction : 1;                     // bit 2
    bool DemonDoor : 1;                                 // bit 3
    bool Oracle : 1;                                    // bit 4
    bool HadFacingAngleYZSetByBodyOrientation : 1;      // bit 5
    bool UnderInfluenceOfEpicSpell : 1;                 // bit 6
    bool HiddenOnMiniMap : 1;                           // bit 7
    bool IsPerformingAnyAction()
    {
        CCreatureActionBase* action = PCurrentAction_Data;
        if (action != 0) {
            if (!action->Finished)
                return true;
        }
        return false;
    }
    void UpdateBodyReorientation();
};

void CThingCreatureBase::UpdateBodyReorientation()
{
    if (BodyReorienter.get() && BodyReorienter->IsUpdateNeeded()) {
        HadFacingAngleYZSetByBodyOrientation = false;
        BodyReorienter->UpdateBodyReorienation();
        if (!IsPerformingAnyAction())
            BodyReorienter->SetFlag_00863260(false);
    }
    if (IsAlive() && HadFacingAngleYZSetByBodyOrientation) {
        if (fabs(PhysicsTC->GetFacingAngleYZ()) > g_epsilon_0129ba3c) {
            if (!IsPerformingAnyAction() && !IsLyingDown(this) && !IsIncapacitated(this)) {
                PhysicsTC->SetFacingAngleYZ(0.0f);
                HadFacingAngleYZSetByBodyOrientation = false;
            }
        }
    }
}
