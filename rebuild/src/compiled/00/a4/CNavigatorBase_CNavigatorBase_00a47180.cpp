// CNavigatorBase::~CNavigatorBase() -- unregister from the navigator manager's class lists, delete the steering agent and the external interface, clear both pointers, then ~CBaseObject
// Leaf: ??1CNavigatorBase@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A47180, 79 bytes, thiscall, tail `jmp ~CBaseObject`.
// Identity (proven): stores vtable 0x0129D4A4 first (RTTI .?AVCNavigatorBase@@ : CClass : CBaseObject : CBaseClass); callers are
//   that vtable's slot-0 deleting dtor 0x00A47E80 and the derived destructors (0x00A19B23, 0x00A1CBDF: CNavigatorAStar /
//   CNavigatorFlyer). Ego_r (0x00B9E620) and FableWin (0x03241EC0) spell ??1CNavigatorBase@@UAE@XZ; reloc_bindings binds it here.
// Callees: CClass::Unregister(CClassFactory&) 0x00A37160 (both PDBs ?Unregister@CClass@@QAEXAAVCClassFactory@@@Z; `this` is the
//   navigator, the argument its manager -- CNavigatorManager : CClassFactory by RTTI; retail inlines the factory's
//   UnregisterAllocatedClass into it), CSteeringAgent::~CSteeringAgent 0x00A1C950 (Ego_r / FableWin ??1CSteeringAgent@@QAE@XZ,
//   non-virtual), operator delete 0x00BFE9BC, the external interface's virtual deleting dtor (slot 23 of
//   CNavigatorExternalInterface: its 23 pure query slots come first), ~CBaseObject 0x0099A430 (protected virtual ??1CBaseObject@@MAE@XZ;
//   CClass's compiler-generated destructor adds nothing).
// Layout: engine/CNavigatorBase.h (retail opt-in FABLE_RETAIL_CNAVIGATORBASE, corrected in this chunk) over engine/CClass.h
//   (FABLE_RETAIL_CCLASS, corrected in this chunk): PInterface +0x14, OnRoute +0x18, Interrupted +0x19, NavigationManager +0x1C,
//   SteeringAgent +0x20. engine/CClassFactory.h and engine/CNavigatorManager.h flatten their vptr (header debt): the two factory
//   classes are declared here (RTTI CNavigatorManager : CClassFactory : CBaseClassNonCopyable).
// Shape: user destructor `Unregister(*NavigationManager); delete SteeringAgent; delete PInterface; SteeringAgent = 0;
//   PInterface = 0;` -- the zeroing written after both deletes (the per-pointer `delete p; p = 0;` order moves the first store
//   ahead of the second test, probe 2).
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#define FABLE_RETAIL_CNAVIGATORBASE
#include "engine/CNavigatorBase.h"

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector;
}   // namespace std

class CClassFactory : public CBaseClassNonCopyable {            // RTTI CClassFactory : CBaseClassNonCopyable; engine/CClassFactory.h is flattened
};
class CNavigatorManager : public CClassFactory {                // RTTI CNavigatorManager : CClassFactory
};
class CSteeringAgent {
public:
    ~CSteeringAgent();                                          // 0x00A1C950
};

class CNavigatorExternalInterface {                             // RTTI: no base; vtable 0x01241D74 (slots 0-22 and 24 pure)
public:
    virtual float FindPosZAt(const C3DVector&, bool) const = 0;                         //  0 (FableWin definition order of the
    virtual float FindPosZAtIncludingThemes(const C3DVector&, bool) const = 0;          //  1  Ground / Obstacle overrides)
    virtual bool IsMovementBlockedAt(const C3DVector&, const C3DVector&, bool&, bool&) const = 0;   // 2
    virtual float GetTerrainDesirabilityAt(const C3DVector&) const = 0;                 //  3
    virtual float GetTerrainHeightAt(const C3DVector&) const = 0;                       //  4
    virtual float GetThemeHeightAt(const C3DVector&) const = 0;                         //  5
    virtual long GetNavigationLevelAt(const C3DVector&) const = 0;                      //  6
    virtual float PeekNavigatorRadius(bool) const = 0;                                  //  7
    virtual C2DVector PeekNavigatorOrientation() const = 0;                             //  8
    virtual C2DVector PeekNavigatorPosition2D() const = 0;                              //  9
    virtual C3DVector PeekNavigatorPosition3D() const = 0;                              // 10
    virtual float PeekNavigatorMaxSpeed(const C3DVector&) const = 0;                    // 11
    virtual const std::vector<const CSteeringAgent*>& GetNavigatorObstacles() const = 0;   // 12-13 (an overload pair)
    virtual std::vector<const CSteeringAgent*>& GetNavigatorObstacles() = 0;
    virtual std::vector<const CSteeringAgent*>* GetPNavigatorObstacles() = 0;           // 14
    virtual const CSteeringAgent* GetPThingToNotAvoid() const = 0;                      // 15
    virtual float GetProximityForDirectApproach() const = 0;                            // 16
    virtual long GetConstantFPS() const = 0;                                            // 17
    virtual float GetMinPathCost() const = 0;                                           // 18
    virtual float GetMaxPathCost() const = 0;                                           // 19
    virtual bool IsPlayer() const = 0;                                                  // 20
    virtual bool IsDead() const = 0;                                                    // 21
    virtual float GetForwardNavigationWidthExtension() const = 0;                       // 22
    virtual ~CNavigatorExternalInterface();                                             // 23 (FableWin ??1CNavigatorExternalInterface@@UAE@XZ)
    virtual CNavigatorExternalInterface* Clone() const = 0;                             // 24
};

CNavigatorBase::~CNavigatorBase()
{
    Unregister(*NavigationManager);
    delete SteeringAgent;
    delete PInterface;
    SteeringAgent = 0;
    PInterface = 0;
}