// CTCPreCalculatedNavigationRoute::CalculateRouteFromTo(const C3DVector& from, const C3DVector& to, vector<pair<C2DVector,uchar> >& route) -- when the game is running and both ends lie in the same world-map region, spawn the placeholder navigation creature at the owner's position, let its navigation component compute the route, then kill the creature
// Leaf: ?CalculateRouteFromTo@CTCPreCalculatedNavigationRoute@@ABEXABVC3DVector@@0AAV?$vector@U?$pair@VC2DVector@@E@std@@V?$allocator@U?$pair@VC2DVector@@E@std@@@2@@std@@@Z
// Name (NF154, PRIORITY q4533): the navigation call 0x006CB9B0 is ?CalculateRouteFromTo@CTCCreatureNavigation@@QBEXABVC3DVector@@0AAV?$vector@U?$pair@VC2DVector@@E@std@@V?$allocator@U?$pair@VC2DVector@@E@std@@@2@@std@@@Z
//   (Ego_r.pdb 0x00701430 / FableWin.pdb 0x0247B150; PNavigator slot 14 forward; engine/CTCCreatureNavigation.h), was the stand-in
//   ?CalculateRouteFromTo@CTCNavigationBase@@QBEXABVC3DVector@@0AAUCRoute@@@Z. Its route argument is this row's own: the row takes the PDB
//   signature (Ego_r.pdb 0x007F3F10 / FableWin.pdb 0x026C1C20, private const, STLport vector<pair<C2DVector, unsigned char> >&; was
//   ?CalculateRouteFromTo@CTCPreCalculatedNavigationRoute@@QAEXABVC3DVector@@0AAUCRoute@@@Z), and its only caller 0x008006D0 is respelled
//   with it. The local CTCBase model is replaced by engine/CTCBase.h's retail branch (PeekComponent / PeekWorldMap declared there).
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00800540, 400 bytes (`ret 0xc`).
// Callees: CTCBase accessors 0x00686D60 (`[0x013B8A1C]+0x1c`, its virtual slot 9 gates the body) and 0x00686D20 (world map,
//          `[0x013B8A1C]+0x28`); CWorldMap::GetMapNumberAt (virtual slot 16, +0x40); CWorldMap::GetRegionNumberMapIsIn 0x004FC190;
//          CCharString ctor 0x0099EBF0 / copy 0x0099EC30 / dtor 0x0099EAE0; CGameDefinitionManager::Get 0x0044C6B0;
//          CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410; CThing::GetPos 0x004C73D0;
//          CThingAICreature::Create 0x00833800 (__fastcall); CVectorMap::LowerBound const 0x004365B0 (interface 7); the
//          component's route call 0x006CB9B0 (catalog "CScriptThing::operator==", a byte twin; descriptive name); CThing::Kill 0x004C9B80.
// Layout: CThingAICreatureInit as the PDB (0x1c, landed ctor 0x004FACD0, here inlined); CThing interface bitset +0x20, map +0x44.
// Shape: the def name and the two `""` names are named locals in blocks (brain declared first); the init record is a
//          temporary of the Create call (inline ctor); the navigation lookup is the ternary GetInterface without a test.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/C3DVector.h"
class C2DVector { public: float X, Y; };
class CTCBase;

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThingPhysical;
class CThingBuilding;
class CThingAICreatureInit { public:
    CThingPhysical* OriginalThing;                      // +0x00
    CThingPhysical* PreviousThing;                      // +0x04
    CCharString     ScriptName;                         // +0x08
    CCharString     OverridingBrainName;                // +0x0c
    bool            PlayerCharacter;                    // +0x10
    bool            Drawable;                           // +0x11
    bool            PlaceAtEndOfList;                   // +0x12
    bool            EnableCreatureAutoPlacing;          // +0x13
    CThingBuilding* HomeBuilding;                       // +0x14
    CThingBuilding* WorkBuilding;                       // +0x18
    CThingAICreatureInit(CThingPhysical* original, CThingPhysical* previous, bool playerCharacter,
                         const CCharString& scriptName, const CCharString& overridingBrainName, bool drawable)
        : OriginalThing(original), PreviousThing(previous), ScriptName(scriptName), OverridingBrainName(overridingBrainName),
          PlayerCharacter(playerCharacter), Drawable(drawable), PlaceAtEndOfList(true), EnableCreatureAutoPlacing(false),
          HomeBuilding(0), WorkBuilding(0) {}
};

class CThingAICreature : public CThing {
public:
    static CThingAICreature* __fastcall Create(long defIndex, const C3DVector& pos, long flags, const CThingAICreatureInit& init);   // 0x00833800
};
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();        // 0x0044C6B0
};
class CMainGameComponent { public:                                  // partial view: CTCBase::PeekComponent() 0x00686D60 ([0x013B8A1C]+0x1c)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool IsEditorActive() const;                // slot 9 (+0x24) 0x004197B0 (FableWin.pdb ?IsEditorActive@CMainGameComponent@@UBE_NXZ; was IsRunning)
};
class CWorldMap { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetMapNumberAt(const C3DVector& pos) const;   // slot 16 (+0x40)
    long GetRegionNumberMapIsIn(long mapNumber) const;        // 0x004FC190
};
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_COMPONENT
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#include "engine/CTCBase.h"
#include "engine/CTCCreatureNavigation.h"
class CTCPreCalculatedNavigationRoute : public CTCBase {
private:
    void CalculateRouteFromTo(const C3DVector& from, const C3DVector& to, std::vector<std::pair<C2DVector, unsigned char>, std::allocator<std::pair<C2DVector, unsigned char> > >& route) const;
};

void CTCPreCalculatedNavigationRoute::CalculateRouteFromTo(const C3DVector& from, const C3DVector& to, std::vector<std::pair<C2DVector, unsigned char>, std::allocator<std::pair<C2DVector, unsigned char> > >& route) const
{
    if (!PeekComponent().IsEditorActive())
        return;
    long fromRegion = PeekWorldMap().GetRegionNumberMapIsIn(PeekWorldMap().GetMapNumberAt(from));
    if (fromRegion != PeekWorldMap().GetRegionNumberMapIsIn(PeekWorldMap().GetMapNumberAt(to)))
        return;
    long defIndex;
    {
        CCharString name("CREATURE_YOURE_ONLY_INTERESTED_IN_ME_FOR_MY_NAVIGATION_PLEASE_DONT_DELETE_ME");
        defIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
    }
    CThingAICreature* creature;
    {
        CCharString brain("");
        CCharString script("");
        creature = CThingAICreature::Create(defIndex, ParentThing->GetPos(), 4,
            CThingAICreatureInit(0, 0, false, script, brain, true));
    }
    const CTCCreatureNavigation* navigation;
    creature->PeekTC(TCI_NAVIGATION, &navigation);
    navigation->CalculateRouteFromTo(from, to, route);
    creature->Kill(false);
}