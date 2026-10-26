// CTCStealth::CTCStealth(CThing* thing) -- CTCBase(thing), every flag false, speeds zero, timers and handles -1, an empty danger-things vector, then fetch the stealth sub-def
// Leaf: ??0CTCStealth
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00747820, length 102 bytes (`ret 4`); stores vtable 0x012689EC (CTCStealth, rtti-vtables.tsv).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CDefClassBase::GetSubDef<CStealthDef>(CDefPointer<const CStealthDef>*) @ 0x00748900
// (owner def +0x70).
// Layout (Ego_r.pdb CTCStealth 0x50 = retail, as the landed ~CTCStealth 0x007495D0): Viewable/Smellable/Hearable/Sneaking/SneakingToggleSet
// +0x0C..+0x10, DistanceCanBeHeardFrom +0x14, MaxSneakingSpeed +0x18, PDef +0x1C, FrameStartedWalking .. InDangerCoolOffTimer +0x20..+0x34,
// DangerThings (STLport vector, 16-byte elements) +0x38, MaxNumberOfDangerThings +0x44, RegionBasedMaxNumberOfDangerThings +0x48.
// Shape: init list in declaration order; the zeroes share eax, the -1s ecx (`or ecx,-1`); the vector as the STLport _Vector_base +
// _STLP_alloc_proxy model.
class CThing;
class CStealthDef;
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<const CStealthDef>* out) const;   // 0x00748900
};
struct CThingDefHolder {
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;                            // +0x70
};
template <class T> struct CAllocator {              // STLport allocator<T>: empty
    CAllocator() { }
};
template <class T> struct CAllocProxy : CAllocator<T> {    // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {             // STLport _Vector_base<T, allocator<T> >
    T*             _M_start;
    T*             _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
};
template<class T> struct CVector : CVectorBase<T> { // STLport vector<T> (12 bytes)
    CVector() : CVectorBase<T>(CAllocator<T>()) { }
};
struct CDangerThing {                               // pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus>, 16 bytes
    unsigned char _data[16];
};
class CTCBase { public:
    virtual ~CTCBase();
    CThingDefHolder* ParentThing;                   // +0x04
    bool ToBeRemoved;                               // +0x08
    bool IsInGlobalUpdateTCs;                       // +0x09
    CTCBase(CThing* thing);                         // 0x00686800
};
class CTCStealth : public CTCBase { public:                       // vtable 0x012689EC
    bool  Viewable;                                 // +0x0c
    bool  Smellable;                                // +0x0d
    bool  Hearable;                                 // +0x0e
    bool  Sneaking;                                 // +0x0f
    bool  SneakingToggleSet;                        // +0x10
    float DistanceCanBeHeardFrom;                   // +0x14
    float MaxSneakingSpeed;                         // +0x18
    CDefPointer<const CStealthDef> PDef;            // +0x1c
    long  FrameStartedWalking;                      // +0x20
    long  FrameStartedSneakingAndMoving;            // +0x24
    long  SneakingAndMovingDeedHandle;              // +0x28
    long  ActionPermittedToSneakID;                 // +0x2c
    long  RunningAwayFromCombatTimer;               // +0x30
    long  InDangerCoolOffTimer;                     // +0x34
    CVector<CDangerThing> DangerThings;             // +0x38
    long  MaxNumberOfDangerThings;                  // +0x44
    long  RegionBasedMaxNumberOfDangerThings;       // +0x48
    CTCStealth(CThing* thing);
    virtual ~CTCStealth();
};

CTCStealth::CTCStealth(CThing* thing)
    : CTCBase(thing)
    , Viewable(false)
    , Smellable(false)
    , Hearable(false)
    , Sneaking(false)
    , SneakingToggleSet(false)
    , DistanceCanBeHeardFrom(0)
    , MaxSneakingSpeed(0)
    , FrameStartedWalking(-1)
    , FrameStartedSneakingAndMoving(-1)
    , SneakingAndMovingDeedHandle(-1)
    , ActionPermittedToSneakID(-1)
    , RunningAwayFromCombatTimer(-1)
    , InDangerCoolOffTimer(-1)
    , MaxNumberOfDangerThings(-1)
    , RegionBasedMaxNumberOfDangerThings(-1)
{
    ParentThing->PDef->GetSubDef(&PDef);
}