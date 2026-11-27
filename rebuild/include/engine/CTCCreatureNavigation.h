#ifndef FABLE_ENGINE_CTCCREATURENAVIGATION_H
#define FABLE_ENGINE_CTCCREATURENAVIGATION_H
#include "engine/CTCBase.h"
#ifdef FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#include "engine/ETCInterfaceType.h"
#endif
class CNavigatorBase;
#ifdef FABLE_RETAIL_CTCCREATURENAVIGATION_FRAMEUPDATE
// Opt-in (hand3c, row 0x006CC800 FrameUpdate): Agents typed as the Ego_r.pdb vector<const CSteeringAgent*> (12 bytes, same offset; the
// row's FrameUpdateThing functor fills it through ForEachInRoughArea 0x006CE800), the FableWin.pdb private inline
// ?GetParentCreature@CTCCreatureNavigation@@ABEAAVCThingCreatureBase@@XZ (retail reads ParentThing +0x04 in place: the creature base is
// at offset 0 of the thing) and the CTCBase slot-9 override ?FrameUpdate@CTCCreatureNavigation@@UAEXXZ (0x006CC800). Needs the retail
// CTCBase (FABLE_RETAIL_CTCBASE) and CThingCreatureBase chain. No layout change.
#include "stlport/fable_containers.h"
#include "engine/CThingCreatureBase.h"
class CSteeringAgent;
#endif
#ifdef FABLE_RETAIL_CTCCREATURENAVIGATION
class C2DVector;
class C3DVector;
namespace std {
template <class _Tp> class allocator;
template <class _Tp, class _Alloc> class vector;
template <class _T1, class _T2> struct pair;
}
#endif
#ifdef FABLE_RETAIL_AI_CREATURE_UPDATE_PASS29
#include "engine/C3DVector.h"
#include "engine/ENavigatorReturn.h"
class CNavigationInput;
#endif
class CTCCreatureNavigation : public CTCBase {
#ifdef FABLE_RETAIL_AI_CREATURE_UPDATE_PASS29
public:
    class CCreatureNavOutput {
    public:
        ENavigatorReturn NavResult;
        C3DVector MovementVector;
    };
    void SetGotNavigationTarget(bool has_target) { GotNavigationTarget = has_target; }
    bool HasGotNavigationTarget() const { return GotNavigationTarget; }
    void SetNavigationTarget(const CNavigationInput& nav_target);
    CCreatureNavOutput GetNextNavPosOnRoute(const CNavigationInput& nav_input, bool force_los,
                                          bool avoid_dynamic_obstacles, bool reduce_speed_when_near,
                                          bool fail_on_dynamic_blockage, bool ignore_path_preferability);
#endif
private:
#ifdef FABLE_RETAIL_CREATURE_NAV_ROUTE_PASS29
    static bool DrawNavRoutes;
public:
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);
    unsigned long GetNavigatorType() const {return PNavigator->GetType();}
private:
#endif
    CNavigatorBase* PNavigator;                              // +0x0c
    unsigned char   PossibleNavigators[0xc];                 // +0x10 vector<CNavigatorBase_*,std::allocator<CNavigatorBase_*>_>
    unsigned char   NavigationTarget[0x18];                  // +0x1c CNavigationInput
    float           Speed;                                   // +0x34
    unsigned char   PThingToNotAvoid[0x8];                   // +0x38 CIntelligentPointer<CThing_const_>
#ifdef FABLE_RETAIL_CTCCREATURENAVIGATION_FRAMEUPDATE
    std::vector<const CSteeringAgent*> Agents;               // +0x40
#else
    unsigned char   Agents[0xc];                             // +0x40 vector<CSteeringAgent_const_*,std::allocator<CSteeringAgent_const_*>_>
#endif
    long            NextUpdate;                              // +0x4c
    long            FrameLastNavigated;                      // +0x50
    bool            CalculatingNewRoute;                     // +0x54
    bool            CanMoveThroughSolidObjects;              // +0x55
    bool            GotNavigationTarget;                     // +0x56
    unsigned char   _pad_0x57[0x1];                          // +0x57
    float           ProximityForDirectApproach;              // +0x58
    float           CurrentSpeed;                            // +0x5c
#ifdef FABLE_RETAIL_CTCCREATURENAVIGATION_FRAMEUPDATE
    CThingCreatureBase& GetParentCreature() const { return static_cast<CThingCreatureBase&>(*ParentThing); }
public:
    virtual void FrameUpdate();                                          // slot 9 0x006CC800
#endif
public:
#ifdef FABLE_RETAIL_KID_EVADE_LIFETIMES_PASS23
    // Original lifetime declarations, fixture definitions do not replace row calls.
    CTCCreatureNavigation(CThing&);
    virtual ~CTCCreatureNavigation();
#endif
#ifdef FABLE_RETAIL_MOVETOTHINGPROXIMITY_PASS29
    class CCreatureNavOutput {public: ENavigatorReturn NavResult; C3DVector MovementVector;};
    CCreatureNavOutput GetNextNavPosOnRoute(const class CNavigationInput&,bool,bool,bool,bool,bool);
#endif
    void InterruptNavigation();
#ifdef FABLE_RETAIL_VILLAGE_PATROL_POSITION_PASS29
    float FindPosZAt(const C3DVector&) const;
#endif
#ifdef FABLE_RETAIL_GHOST_SWORD_ADD_PASS27
    void SetAsAbleToMoveThroughSolidObjects(bool);                     // 0x006CB9D0
#endif
#ifdef FABLE_RETAIL_KID_EVADE_PASS23
    // Original PDB const member and donor02b68d59; native006cb840 receives a reference output.
    bool GetNearestFleePosition(const C3DVector&,float,C3DVector&) const;
#endif
#ifdef FABLE_RETAIL_CTCCREATURENAVIGATION_VSLOTS
// agentFAM05 (2026-11-11): retail virtual override this declaration lacks, declaration only (needs the polymorphic FABLE_RETAIL_CTCBASE; no layout
// change). Slot 8 of the TLC vtable (agentFAM03 proof, re-verified by agentFAM05); spelling from the Ego_r / FableWin name sets.
    virtual bool HasFrameUpdate() const;                                 // slot 8 0x006CD560
#endif // FABLE_RETAIL_CTCCREATURENAVIGATION_VSLOTS
#ifdef FABLE_RETAIL_CTCCREATURENAVIGATION_ISNAVIGATING
    // Retail opt-in (agentNF14, row 0x006CB8F0): Ego_r.pdb ?IsCurrentlyNavigating@CTCCreatureNavigation@@QBE_NXZ (public const), right
    // after GetNearestNavigablePosTo as in the Ego_r donor order (0x00701340 / 0x00701360 = retail 0x006CB8E0 / 0x006CB8F0); it reads
    // FrameLastNavigated +0x50. Declaration only.
    bool IsCurrentlyNavigating() const;                      // 0x006CB8F0
#endif
#ifdef FABLE_RETAIL_CTCCREATURENAVIGATION_ONEPOINTLOS
    bool IsPosNavLineOfSightTo(const C3DVector&) const;
#endif
    bool GetNearbyWanderPosition(const C3DVector&,float,C3DVector&,bool) const;
    bool GetNearestNavigablePosTo(const C3DVector&,float,C3DVector&) const;
#ifdef FABLE_RETAIL_CTCCREATURENAVIGATION
    // Retail opt-in (chunk 725): FableWin.pdb members that retail compiles to PNavigator vtable forwards (CNavigatorBase
    // slots, engine/CNavigatorBase.h FABLE_RETAIL_CNAVIGATORBASE): InterruptNavigation 0x006CB6B0 (slot 18 Interrupt),
    // GetNearestNavigablePosTo 0x006CB8E0 (slot 8), IsNewDestinationGoingToResetNavigation 0x006CB990 (slot 11),
    // GetCurrentDestination 0x006CB9A0 (slot 12), CalculateRouteFromTo 0x006CB9B0 (slot 14), InitialiseFromPreCalculatedRoute
    // 0x006CB9C0 (slot 15), CalculateNavigableWidthOfLine 0x006CBA10 (slot 19).
    bool IsNewDestinationGoingToResetNavigation(const C3DVector&,float) const;
    const C3DVector& GetCurrentDestination() const;
    void CalculateRouteFromTo(const C3DVector&,const C3DVector&,std::vector<std::pair<C2DVector,unsigned char>,std::allocator<std::pair<C2DVector,unsigned char> > >&) const;
    void InitialiseFromPreCalculatedRoute(const C3DVector&,const std::vector<std::pair<C2DVector,unsigned char>,std::allocator<std::pair<C2DVector,unsigned char> > >&,const C3DVector&);
    void CalculateNavigableWidthOfLine(const C3DVector&,const C3DVector&,float&,float&);
#ifdef FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
    // Nested opt-in (chunk 760, row 0x0093D270 CAIStateGroup_MeleeCombatNearbyBase::IsRunnable): the interface type
    // (TCI_NAVIGATION 7: the row's HasTC tests bit 7 of +0x20 and looks up key 7) and FableWin.pdb / Ego_r.pdb
    // ?IsAbleToNavigateFromPosToPos@CTCCreatureNavigation@@QBE_NABVC3DVector@@0M@Z (public const, retail 0x006CB620).
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_NAVIGATION; }
    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float proximity) const;   // 0x006CB620
#endif
#ifdef FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGATORTYPE
    // Nested opt-in (w22 agent2007, row 0x00665860 CThingCreatureBase::UpdateAnimation): Ego_r.pdb / FableWin.pdb
    // ?GetNavigatorType@CTCCreatureNavigation@@QBEKXZ (public const, out of line in retail at 0x006CB6A0). Declaration only.
    unsigned long GetNavigatorType() const;                                                                 // 0x006CB6A0
#ifdef FABLE_RETAIL_CTCCREATURENAVIGATION_SETNAVIGATORTOTYPE
    // NF41 (2026-11-16, row 0x00912700 CAIStateGroup_FlyingThingBase::GetRandomFlightPos): Ego_r.pdb 0x00701A10 / FableWin.pdb
    // 0x02478060 ?SetNavigatorToType@CTCCreatureNavigation@@QAEXK@Z (public, retail 0x006CBA80). Declaration only.
    void SetNavigatorToType(unsigned long type);                                                            // 0x006CBA80
#endif
#endif
#ifdef FABLE_RETAIL_CTCCREATURENAVIGATION_LINEOFSIGHT
    // Nested opt-in (chunk 770, row 0x006CB610): Ego_r.pdb ?IsPosNavLineOfSightToPos@CTCCreatureNavigation@@QBE_NABVC3DVector@@0@Z
    // (public const), retail a PNavigator slot-10 forward (IsNavLineOfSightClear) just before IsAbleToNavigateFromPosToPos 0x006CB620.
    bool IsPosNavLineOfSightToPos(const C3DVector& from, const C3DVector& to) const;                       // 0x006CB610
#endif
#endif
#ifdef FABLE_RETAIL_MINION_CRIMINAL_UPDATE_PASS24
public:
    bool IsPosNavigable(const C3DVector& pos) const;
#endif
#ifdef FABLE_RETAIL_CTCCREATURENAVIGATION_NEARBYDIRECTED
public:
    // Opt-in (wfE11, row 0x004A3740 CWorld::UpdateRegionLoad): Ego_r.pdb
    // ?GetNearbyDirectedPosition@CTCCreatureNavigation@@QBE_NABVC3DVector@@M0MAAV2@@Z (0x006CB780, called 0x004A4ED3). Declaration only.
    bool GetNearbyDirectedPosition(const C3DVector& pos, float radius, const C3DVector& direction, float angle, C3DVector& out) const;   // 0x006CB780
#endif // FABLE_RETAIL_CTCCREATURENAVIGATION_NEARBYDIRECTED
};
FABLE_STATIC_ASSERT_AT(81, sizeof(CTCCreatureNavigation)==0x60);
#endif
