// CTCCreatureNavigation::CTCCreatureNavigation(CThing* parent) -- creature navigation component: no navigator, empty navigator and steering-agent vectors, a zeroed navigation target, no thing to avoid; FrameLastNavigated -1 and CalculatingNewRoute set
// Leaf: ??0CTCCreatureNavigation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CC170, length 106 bytes (+ int3 padding). Vtable 0x0125EC24 (CTCCreatureNavigation).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10 (then the
// CIntelligentPointer<const CThing> vtable 0x01237F1C).
// Layout: Ego_r.pdb CTCCreatureNavigation with retail 12-byte vectors (everything after PossibleNavigators sits 4 bytes lower, after
// Agents 8): PNavigator +0xC, PossibleNavigators +0x10, NavigationTarget +0x1C (CNavigationInput: Destination +0, Proximity +0xC,
// MovementType +0x10, Speed +0x14), Speed +0x34, PThingToNotAvoid +0x38, Agents +0x40, NextUpdate +0x4C, FrameLastNavigated +0x50,
// CalculatingNewRoute +0x54, CanMoveThroughSolidObjects +0x55, GotNavigationTarget +0x56, ProximityForDirectApproach +0x58,
// CurrentSpeed +0x5C. The destination vector has a trivial ctor (not stored).
// Shape: the zero members are initialisers (one ebx zero register, vectors as STLport _Vector_base + alloc proxy); the -1 and
// the `true` store come last: they are body assignments.
struct CThing;
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CTCBase(CThing* parent);                    // 0x00686800
};
template <class T> struct SRetailAllocProxy {   // STLport _STLP_alloc_proxy: the end-of-storage pointer
    T* _M_data;
    SRetailAllocProxy(T* data) : _M_data(data) {}
};
template <class T> struct CRetailVector {       // STLport vector (12 bytes): _Vector_base start / finish + the alloc proxy
    T* _M_start;
    T* _M_finish;
    SRetailAllocProxy<T> _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
};
struct CNavigationInput {                       // 24 bytes
    C3DVector Destination;                      // +0x00
    float     Proximity;                        // +0x0c
    long      MovementType;                     // +0x10 EMovementType
    float     Speed;                            // +0x14
    CNavigationInput() : Proximity(0), MovementType(0), Speed(0) {}
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v01();
    CIntelligentPointerConstThing() {}
};
struct CNavigatorBase;
struct CSteeringAgent;
struct CTCCreatureNavigation : CTCBase {
    CNavigatorBase* PNavigator;                 // +0x0c
    CRetailVector<CNavigatorBase*> PossibleNavigators;      // +0x10
    CNavigationInput NavigationTarget;          // +0x1c
    float Speed;                                // +0x34
    CIntelligentPointerConstThing PThingToNotAvoid;         // +0x38
    CRetailVector<const CSteeringAgent*> Agents;            // +0x40
    long NextUpdate;                            // +0x4c
    long FrameLastNavigated;                    // +0x50
    bool CalculatingNewRoute;                   // +0x54
    bool CanMoveThroughSolidObjects;            // +0x55
    bool GotNavigationTarget;                   // +0x56
    float ProximityForDirectApproach;           // +0x58
    float CurrentSpeed;                         // +0x5c
    virtual void v01();
    CTCCreatureNavigation(CThing* parent);
};

CTCCreatureNavigation::CTCCreatureNavigation(CThing* parent)
    : CTCBase(parent)
    , PNavigator(0)
    , Speed(0)
    , NextUpdate(0)
    , CanMoveThroughSolidObjects(false)
    , GotNavigationTarget(false)
    , ProximityForDirectApproach(0)
    , CurrentSpeed(0)
{
    FrameLastNavigated = -1;
    CalculatingNewRoute = true;
}
