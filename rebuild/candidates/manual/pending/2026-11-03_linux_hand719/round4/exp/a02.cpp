// CWorldMap::GetMapNavigationAreaUpdateInfo(long map, const C2DBoxI& area, const CCharString& section, CNavigationAreaUpdate& update) -- draft a01
// Leaf: ?GetMapNavigationAreaUpdateInfo@CWorldMap@@QBEXJABVC2DBoxI@@ABVCCharString@@AAVCNavigationAreaUpdate@@@Z
// Flags: /Gr
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPHYSICSMESHOBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_BACK_INSERTER
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CThingPhysical.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CPhysicsMeshObject.h"
#include "engine/CStringParser.h"
#include "engine/CThingSearchTools.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"

class C2DLineF {
public:
    C2DVector Point1;
    C2DVector Point2;
};
class CNavigationTransitionPoint {
public:
    C2DVector Position;
    long Layer1;
    long Layer2;
    CNavigationTransitionPoint(const C2DVector& position, long layer1, long layer2) : Position(position), Layer1(layer1), Layer2(layer2) { }
};
class CNavigationPosition {
public:
    C2DVector Position;
    long Layer;
    CNavigationPosition(const C2DVector& position, long layer) : Position(position), Layer(layer) { }
};
class CNavigationAreaUpdate {
public:
    class CSwitchableBlockage {
    public:
        unsigned __int64 UniqueID;
        std::list<C2DLineF> LineList;
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
class CMap {
public:
    bool IsPassableAt(const C2DCoordI& pos) const;     // 0x0081F3D0
    bool IsPrefNavAt(const C2DCoordI& pos) const;      // 0x0081F4B0
};
namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    void GetAllDummyObjectNames(std::vector<CCharString>& names) const;   // 0x004BDB40
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector& pos, CRightHandedSet& rhs, float scale) const;   // 0x004C08C0
};
}
class CThingFilter_OverlappingArea : public std::unary_function<CThing,bool> {
public:
    CThingFilter_OverlappingArea(const C2DBoxI& area) : Area(&area) { }
    bool operator()(const CThing& thing) const;
protected:
    const C2DBoxI* Area;
};
class CThingFilter_HasNavigationInformation : public std::unary_function<CThingPhysical,bool> {
public:
    bool operator()(const CThingPhysical& thing) const;
};
class CThingFilter_IsInQuestOrNullQuest : public std::unary_function<CThing,bool> {
public:
    CThingFilter_IsInQuestOrNullQuest(const CCharString& name) : QuestSectionName(name) { }
    bool operator()(const CThing& thing) const;
protected:
    CCharString QuestSectionName;
};
template<class First,class Second,class Third> class CFilter_Bind3 {
protected:
    const First& F1;
    const Second& F2;
    const Third& F3;
public:
    CFilter_Bind3(const First& a,const Second& b,const Third& c) : F1(a), F2(b), F3(c) { }
    template<class T> bool operator()(const T& thing) const { return F1(thing) && F2(thing) && F3(thing); }
};
template<class First,class Second,class Third>
CFilter_Bind3<First,Second,Third> __fastcall GFFilter_Bind3(const First& a,const Second& b,const Third& c) {
    return CFilter_Bind3<First,Second,Third>(a,b,c);
}
inline unsigned __int64 GFGetMapThingUIDFromThingUID(unsigned __int64 uid) { return uid & 0xFFFFFFFFFFull; }

extern CCharString g_CCharString_013bac94;   // 0x013BAC94 "TRANSITION_POINT"
extern CCharString g_CCharString_013bac9c;   // 0x013BAC9C "ACTION_POINT"

typedef CFilter_Bind3<CThingFilter_OverlappingArea, CThingFilter_HasNavigationInformation, CThingFilter_IsInQuestOrNullQuest> CNavigationThingFilter;

void CWorldMap::GetMapNavigationAreaUpdateInfo(long map, const C2DBoxI& area, const CCharString& section, CNavigationAreaUpdate& update) const
{
    const CThingSearchTools& searchTools = World->PeekThingSearchTools();
    std::list<const CThingPhysical*> things;
    searchTools.GetAllThings<CThingPhysical>(GFFilter_Bind3(CThingFilter_OverlappingArea(area),
        CThingFilter_HasNavigationInformation(), CThingFilter_IsInQuestOrNullQuest(section)), things);

    update.NavigationLayers.clear();

    for (std::list<const CThingPhysical*>::iterator it = things.begin(); it != things.end(); ++it)
    {
        const CThingPhysical* thing = *it;
        long layer = GetNavigationLayerAt(thing->GetPos(), thing);
        long count = thing->GetNoNavCollisionLineLists();
        if ((unsigned long)(layer + count) > update.NavigationLayers.size())
            update.NavigationLayers.resize(layer + count);

        const bool& switchable = thing->HasTC(TCI_SWITCHABLE_NAVIGATION);
        if (switchable)
        {
            for (long i = 0; i < count; ++i)
            {
                CNavigationAreaUpdate::CSwitchableBlockage blockage;
                blockage.UniqueID = GFGetMapThingUIDFromThingUID(thing->GetUniqueID());
                std::vector<C2DLineF> lines;
                thing->GetNavCollisionLineList(lines, i);
                std::copy(lines.begin(), lines.end(), std::back_inserter(blockage.LineList));
                update.NavigationLayers[layer + i].SwitchableBlockages.push_back(blockage);
            }
        }
        else
        {
            for (long i = 0; i < count; ++i)
                thing->GetNavCollisionLineList(update.NavigationLayers[layer + i].LineList, i);
        }

        std::vector<CCharString> dummyNames;
        thing->GetTCGraphicAppearance()->GetAllDummyObjectNames(dummyNames);
        long dummyIndex = 0;
        for (std::vector<CCharString>::iterator name = dummyNames.begin(); name != dummyNames.end(); ++name)
        {
            if (name->GetLength() >= 12 && name->GetSubString(0, 12) == "ACTION_POINT")
            {
                C3DVector pos;
                CRightHandedSet rhs;
                if (thing->GetTCGraphicAppearance()->GetDummyObject(*name, (*name == g_CCharString_013bac9c) ? dummyIndex : 0, 0, pos, rhs, 1.0f))
                {
                    if (area.ContainsPoint(pos.ToC2DCoordI()))
                        update.ActionPoints.push_back(CNavigationPosition(C2DVector(pos), GetNavigationLayerAt(pos, 0)));
                }
                ++dummyIndex;
            }
        }

        if (thing->GetPhysicsTC()->GetPhysicsMeshInfo() && !thing->HasTC(TCI_SWITCHABLE_NAVIGATION))
        {
            CPhysicsMeshObject meshObject = thing->GetPhysicsTC()->GetPhysicsMeshObject();
            std::vector<CCharString> helperNames;
            meshObject.GetAllHelperPointNames(helperNames);
            CCharString lastName;
            long helperIndex = 0;
            long prefixLength = g_CCharString_013bac94.GetLength();
            for (std::vector<CCharString>::iterator name = helperNames.begin(); name != helperNames.end(); ++name)
            {
                if (name->GetLength() >= prefixLength && name->GetSubString(0, prefixLength) == g_CCharString_013bac94)
                {
                    if (*name != lastName)
                    {
                        lastName = *name;
                        helperIndex = 0;
                    }
                    else
                        ++helperIndex;
                    C2DVector pos(meshObject.GetHelperPointPositionStatic(*name, helperIndex));
                    if (area.ContainsPoint(C2DCoordI((long)pos.X, (long)pos.Y)))
                    {
                        if (*name == g_CCharString_013bac94)
                            update.TransitionPoints.push_back(CNavigationTransitionPoint(pos, 0, 1));
                        else
                        {
                            CStringParser parser(*name);
                            parser.FindKeyword(g_CCharString_013bac94 + "_");
                            long layer1 = parser.ReadAsInteger() - 1;
                            parser.FindKeyword("_");
                            long layer2 = parser.ReadAsInteger() - 1;
                            update.TransitionPoints.push_back(CNavigationTransitionPoint(pos, layer1, layer2));
                        }
                    }
                }
            }
        }
    }

    update.TopologyWeights.resize(area.GetWidth() * area.GetHeight());
    C2DBoxI localArea = ClipAndConvertWorldMapAreaToLocalArea(map, area);
    const CMap& mapRef = PeekMap(map);
    for (long y = localArea.TLY, j = 0; y < localArea.BRY; ++y, ++j)
    {
        for (long x = localArea.TLX, i = 0; x < localArea.BRX; ++x, ++i)
        {
            if (mapRef.IsPassableAt(C2DCoordI(x, y)))
            {
                if (mapRef.IsPrefNavAt(C2DCoordI(x, y)))
                    update.TopologyWeights[area.GetWidth() * j + i] = 0;
                else
                    update.TopologyWeights[area.GetWidth() * j + i] = 0x80;
            }
            else
                update.TopologyWeights[area.GetWidth() * j + i] = 0xFF;
        }
    }
}
