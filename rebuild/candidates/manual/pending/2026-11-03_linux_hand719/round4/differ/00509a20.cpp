// CWorldMap::GetMapNavigationAreaUpdateInfo(long map, const C2DBoxI& area, const CCharString& section, CNavigationAreaUpdate& update) -- collects one map area's navigation lines (plain + switchable blockages), ACTION_POINT dummies, TRANSITION_POINT helpers and topology weights
// Grade: DIFFER(3220v3120) -- 754/937 insns exact, 871/937 offset-masked (overlay $S/w22/agent719c/hdr). Residue by section: [prologue] `this` copied to ebx (retail: ecx stored to its slot, ebx = area); [switchable loop] VC7.1 inlines the implicit CSwitchableBlockage copy ctor in the vector push_back growth path (retail calls 0x005199E0), fast path `add edi,8` v retail `lea esi,[edi+8]`, plus one extra spill: this is most of the +100 bytes and the 0x12c v 0x128 frame; [frame] slot order differs (this/lastName swapped; helperNames/dummyNames v filter/actionPoint group); [topology] ebx=this/esi=area/map reloaded v retail ebx=area/esi=map/edi=this, index (w*j+data)+i v retail (w*j+i)+data, retail hoists the area TLX/BRX loads above the IsPrefNavAt je, outer-loop TLX reload rotated; [list/clear/non-switchable/action points/transition points] match except alignment padding
// Leaf: ?GetMapNavigationAreaUpdateInfo@CWorldMap@@QBEXJABVC2DBoxI@@ABVCCharString@@AAVCNavigationAreaUpdate@@@Z
// Flags: /Gr
#define FABLE_RETAIL_CMAP
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE
#define FABLE_RETAIL_C2DLINEF
#define FABLE_RETAIL_CNAVIGATIONTRANSITIONPOINT
#define FABLE_RETAIL_CNAVIGATIONPOSITION
#define FABLE_RETAIL_CNAVIGATIONAREAUPDATE
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
#include "engine/CMap.h"
#include "engine/CTCGraphicAppearance.h"
#include "engine/CNavigationAreaUpdate.h"
#include "engine/CThingFilter_OverlappingArea.h"
#include "engine/CThingFilter_HasNavigationInformation.h"
#include "engine/CThingFilter_IsInQuestOrNullQuest.h"
#include "engine/CFilter_Bind3.h"

// FableWin.pdb 0x01C903B0 ?GFGetMapThingUIDFromThingUID@@YI_K_K@Z (inline helper; landed callers spell it as the 40-bit mask).
inline unsigned __int64 __fastcall GFGetMapThingUIDFromThingUID(unsigned __int64 uid) { return uid & 0xFFFFFFFFFFull; }


// Same-TU stand-in body for the CWorldMap TU's GetAllThings instantiation 0x00516200 (lever: VC7.1 reads the callee's effects).
#pragma auto_inline(off)
template<class T,class Container,class Filter>
__declspec(noinline) void CThingSearchTools::GetAllThings(const Filter& filter,Container& result) const {
    const std::list<CClass*>& things=PeekTypeList(0);
    for(std::list<CClass*>::const_iterator it=things.begin();it!=things.end();++it) {
        T* thing=static_cast<T*>(*it);
        if(thing->IsIncludedInThingScans() && filter(*thing))
            result.push_back(thing);
    }
}
#pragma auto_inline(on)

extern CCharString g_CCharString_013bac94;   // 0x013BAC94 "TRANSITION_POINT"
extern CCharString g_CCharString_013bac9c;   // 0x013BAC9C "ACTION_POINT"


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

        bool switchable = thing->HasTC(TCI_SWITCHABLE_NAVIGATION);
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

    {
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
}
