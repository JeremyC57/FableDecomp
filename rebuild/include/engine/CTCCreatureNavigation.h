#ifndef FABLE_ENGINE_CTCCREATURENAVIGATION_H
#define FABLE_ENGINE_CTCCREATURENAVIGATION_H
#include "engine/CTCBase.h"
class CNavigatorBase;
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
};
FABLE_STATIC_ASSERT(sizeof(CTCCreatureNavigation)==0x60);
#endif
