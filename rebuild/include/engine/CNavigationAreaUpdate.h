#ifndef FABLE_ENGINE_CNAVIGATIONAREAUPDATE_H
#define FABLE_ENGINE_CNAVIGATIONAREAUPDATE_H
// Both PDB type records and native0051d870/0051d800: original navigation update types.
#include "fable_compiler.h"
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/C2DLineF.h"
class CNavigationTransitionPoint;
class CNavigationPosition;
class CNavigationAreaUpdate {
public:
    class CSwitchableBlockage {
    public:
#if defined(_MSC_VER)
        unsigned __int64 UniqueID;
#else
        unsigned long long UniqueID;
#endif
        std::list<C2DLineF> LineList;
        CSwitchableBlockage() {}
        ~CSwitchableBlockage() {}
    };
    class CNavigationLayerUpdate {
    public:
        std::vector<C2DLineF> LineList;
        std::vector<CSwitchableBlockage> SwitchableBlockages;
    };
    std::vector<CNavigationLayerUpdate> NavigationLayers;
    std::vector<CNavigationTransitionPoint> TransitionPoints;
    std::vector<CNavigationPosition> ActionPoints;
    std::vector<unsigned char> TopologyWeights;
};
FABLE_STATIC_ASSERT(sizeof(CNavigationAreaUpdate::CSwitchableBlockage)==16);
FABLE_STATIC_ASSERT(sizeof(CNavigationAreaUpdate::CNavigationLayerUpdate)==24);
FABLE_STATIC_ASSERT(sizeof(CNavigationAreaUpdate)==48);
#endif
