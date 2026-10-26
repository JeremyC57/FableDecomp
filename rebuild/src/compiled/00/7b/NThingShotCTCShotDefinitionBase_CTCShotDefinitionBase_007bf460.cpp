// NThingShot::CTCShotDefinitionBase::CTCShotDefinitionBase(CThing* thing) -- CTCBase(thing), then the shot's intelligent pointers, empty hit list and def pointers, the flags and an unset (-1) initial velocity
// Leaf: ??0CTCShotDefinitionBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BF460, 172 bytes (`ret 4`); vtable 0x0126E1A4 NThingShot::CTCShotDefinitionBase.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CBaseIntelligentPointer ctor @ 0x00A01B10 (five times) and SetPItem
// @ 0x00A01B90 (PBestCollisionThing is built from a null pointer); intelligent-pointer vtables 0x01238C6C
// (CIntelligentPointer<CThing>) and 0x01237F1C (CIntelligentPointer<const CThing>).
// Layout: Ego_r.pdb CTCShotDefinitionBase with the retail 12-byte vector (members from PTargetThing on sit 4 below the PDB):
// PEffect +0x0c, HitThingsList +0x14, PTargetThing +0x20, PShooter +0x28, PWeapon +0x30, PShotDef +0x38, PShotAbilityDef
// +0x3c, SubShotsTriggered +0x50, TargetPos +0x51 (an unaligned C3DVector in the PDB, untouched here), MadeSound +0x65,
// CanBeDeflected +0x68, InitialVelocity +0x6c, PBestCollisionThing +0x88, BestCollisionType +0x90,
// PBestCollisionHitLocation +0x94, MultiArrowShotType +0x98 (0x9c bytes).
// Shape: every member in declaration order through the initialiser list; the intelligent pointers' inline constructors call
// the out-of-line base then store their vtable; one zero register serves every zero; the hit list is the real STLport
// _Vector_base + _STLP_alloc_proxy (that places the next member's `mov ecx,edi` after the end-of-storage zero).
class CThing;
class CBaseObject;
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
    CTCBase(CThing* thing);                             // 0x00686800
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtables 0x01238C6C / 0x01237F1C
    virtual void v00();
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
    virtual void v00();
    CTCShotDefinitionBase(CThing* thing);
};

CTCShotDefinitionBase::CTCShotDefinitionBase(CThing* thing)
    : CTCBase(thing), SubShotsTriggered(false), MadeSound(false), CanBeDeflected(true), InitialVelocity(-1.0f),
      PBestCollisionThing(0), BestCollisionType(0), PBestCollisionHitLocation(0), MultiArrowShotType(false)
{
}
}