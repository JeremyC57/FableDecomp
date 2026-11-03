#ifndef FABLE_ENGINE_CTCCREATURENAVIGATION_H
#define FABLE_ENGINE_CTCCREATURENAVIGATION_H
#include "engine/CTCBase.h"
class CNavigatorBase;
#ifdef FABLE_RETAIL_CTCCREATURENAVIGATION
class C2DVector;
class C3DVector;
namespace std {
template <class _Tp> class allocator;
template <class _Tp, class _Alloc> class vector;
template <class _T1, class _T2> struct pair;
}
#endif
class CTCCreatureNavigation : public CTCBase {
private:
    CNavigatorBase* PNavigator;                              // +0x0c
    unsigned char   PossibleNavigators[0xc];                 // +0x10 vector<CNavigatorBase_*,std::allocator<CNavigatorBase_*>_>
    unsigned char   NavigationTarget[0x18];                  // +0x1c CNavigationInput
    float           Speed;                                   // +0x34
    unsigned char   PThingToNotAvoid[0x8];                   // +0x38 CIntelligentPointer<CThing_const_>
    unsigned char   Agents[0xc];                             // +0x40 vector<CSteeringAgent_const_*,std::allocator<CSteeringAgent_const_*>_>
    long            NextUpdate;                              // +0x4c
    long            FrameLastNavigated;                      // +0x50
    bool            CalculatingNewRoute;                     // +0x54
    bool            CanMoveThroughSolidObjects;              // +0x55
    bool            GotNavigationTarget;                     // +0x56
    unsigned char   _pad_0x57[0x1];                          // +0x57
    float           ProximityForDirectApproach;              // +0x58
    float           CurrentSpeed;                            // +0x5c
public:
    void InterruptNavigation();
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
    // Chunk 732 (row 0x006AA720): Ego_r.pdb / FableWin.pdb ?IsAbleToNavigateFromPosToPos@CTCCreatureNavigation@@QBE_NABVC3DVector@@0M@Z
    // (0x006CB620, out of line). StatGetInterfaceType: TCI_NAVIGATION (7; retail tests bit 7 of +0x20).
#ifdef FABLE_RETAIL_CTCBASE
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)7; }   // TCI_NAVIGATION (the landed includers have no retail CTCBase)
#endif
    bool IsAbleToNavigateFromPosToPos(const C3DVector&,const C3DVector&,float) const;   // 0x006CB620
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCCreatureNavigation)==0x60);
#endif
