// Behaviour test for NThingShot::CTCShotDefinitionBase(CThing*) (0x007BF460): the CTCBase owner, five constructed intelligent
// pointers (only PBestCollisionThing set, to null), an empty hit list, null defs, the flags and InitialVelocity -1.
#include <cstdio>
class CThing;
class CBaseObject;
static int g_ctors = 0, g_sets = 0;
class CTCBase { public:
    virtual void v00() { }
    CThing* ParentThing;                                // +0x04
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
    CTCBase(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
};
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer() : m_pItem((CThing*)0x1) { ++g_ctors; }
    void SetPItem(CBaseObject* item) { m_pItem = (CThing*)item; ++g_sets; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtables 0x01238C6C / 0x01237F1C
    virtual void v00() { }
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
template <class T> struct CAllocator {                  // STLport allocator<T> (empty)
    CAllocator() { }
};
template <class T> struct CAllocProxy : CAllocator<T> { // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {                 // STLport _Vector_base<T, allocator<T> >
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
};
template <class T> struct CRetailVector : CVectorBase<T> {   // STLport vector<T> (12 bytes)
    CRetailVector() : CVectorBase<T>(CAllocator<T>()) { }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
class CThingShotDef;
class CMeleeCombatAbilityDef;
struct CHitLocation;
class C3DVector { public: float X, Y, Z; C3DVector() { } };
namespace NThingShot {
class CTCShotDefinitionBase : public CTCBase { public:
    CIntelligentPointer<CThing> PEffect;                            // +0x0c
    CRetailVector< CIntelligentPointer<const CThing> > HitThingsList;   // +0x14
    CIntelligentPointer<const CThing> PTargetThing;                 // +0x20
    CIntelligentPointer<CThing> PShooter;                           // +0x28
    CIntelligentPointer<CThing> PWeapon;                            // +0x30
    CDefPointer<const CThingShotDef> PShotDef;                      // +0x38
    CDefPointer<const CMeleeCombatAbilityDef> PShotAbilityDef;      // +0x3c
    float AccuracyPercentage;                                       // +0x40
    float DamageMultiplier;                                         // +0x44
    float ShotBuildupTime;                                          // +0x48
    long StartFrame;                                                // +0x4c
    bool SubShotsTriggered;                                         // +0x50
    unsigned char TargetPos[12];                                    // +0x51 C3DVector (unaligned in the PDB)
    long DieOnFrame;                                                // +0x60
    bool PassedTarget;                                              // +0x64
    bool MadeSound;                                                 // +0x65
    bool KnockDown;                                                 // +0x66
    bool Decapitate;                                                // +0x67
    bool CanBeDeflected;                                            // +0x68
    float InitialVelocity;                                          // +0x6c
    C3DVector BestCollisionPos;                                     // +0x70
    C3DVector BestCollisionNormal;                                  // +0x7c
    CIntelligentPointer<CThing> PBestCollisionThing;                // +0x88
    long BestCollisionType;                                         // +0x90 ECollideType
    CHitLocation* PBestCollisionHitLocation;                        // +0x94
    bool MultiArrowShotType;                                        // +0x98
    virtual void v00() { }
    CTCShotDefinitionBase(CThing* thing);
};

CTCShotDefinitionBase::CTCShotDefinitionBase(CThing* thing)
    : CTCBase(thing), SubShotsTriggered(false), MadeSound(false), CanBeDeflected(true), InitialVelocity(-1.0f),
      PBestCollisionThing(0), BestCollisionType(0), PBestCollisionHitLocation(0), MultiArrowShotType(false)
{
}
}

int main()
{
    int bad = 0;
    CThing* owner = (CThing*)0x1234;
    NThingShot::CTCShotDefinitionBase* s = new NThingShot::CTCShotDefinitionBase(owner);
    if (s->ParentThing != owner) bad |= 1;
    if (g_ctors != 5 || g_sets != 1 || s->PBestCollisionThing.m_pItem != 0 || s->PEffect.m_pItem != (CThing*)0x1) bad |= 2;
    if (s->HitThingsList._M_start || s->HitThingsList._M_finish || s->HitThingsList._M_end_of_storage.m_data) bad |= 4;
    if (s->PShotDef.Object || s->PShotAbilityDef.Object || s->PBestCollisionHitLocation || s->BestCollisionType != 0) bad |= 8;
    if (s->SubShotsTriggered || s->MadeSound || !s->CanBeDeflected || s->MultiArrowShotType) bad |= 16;
    float minusOne = -1.0f;
    if (s->InitialVelocity != minusOne) bad |= 32;
    if (bad) { std::printf("007bf460_TEST FAIL\n"); return 1; }
    std::printf("007bf460_TEST PASS\n");
    return 0;
}