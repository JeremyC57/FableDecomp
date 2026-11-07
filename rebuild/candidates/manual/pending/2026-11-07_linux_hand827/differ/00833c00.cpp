// CThingAICreature::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- clear the creature's debug render handles, run the CThingCreatureBase pass, then (debug switches) draw the creature's def name, its brain's current state-group debug text and name, the stealth perception debug, lines to its home / work buildings, and the hero navigation level with the containing volume's def name; always returns false
// Grade: DIFFER(2296v2300) -- 608 difflines (684 when regraded unchanged on today's headers): info.Engine is still cached in ebp where retail keeps info in ebp and re-reads [ebp+34h] at each call (retail uses ebx for the vtable / font, ours keeps bl = 0xff), which shifts the register picks of every block, and the frame slots shift by one dword (the debug flag at retail [esp+48h], ours [esp+44h]); the line block is scoped and the name-text block unscoped (colour slot sharing), and the line top is an explicit C3DVector temporary (retail's interleaved X / Y integer moves)
// Next idea: the remaining engine caching: a per-call `info.Engine->` spelling that VC7.1 cannot CSE (a reference-to-info local per block, or the calls as members of a small inline helper taking info by reference); the 1-dword slot shift is probably the same root
// Leaf: ?DrawPreparePrimitivesForRendering@CThingAICreature@@UAE_NABVCEnginePrimitivePrepareInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00833C00, 2300 bytes, `ret 4`; CThingAICreature vtable 0x0127293C slot 36
//   (rtti-vtables.tsv; CThingCreatureBase slot 36 = 0x00663C40, which the body calls first).
// Name proof: vtable 0x0127293C (CThingAICreature, RTTI) slot 36 holds 0x00833C00; slot 36 of the CThingCreatureBase vtable
//   0x0125AC0C is DrawPreparePrimitivesForRendering (landed 0x00663C40, called at 0x00833C2B with the same argument); Ego_r.pdb
//   and FableWin.pdb both list ?DrawPreparePrimitivesForRendering@CThingAICreature@@UAE_NABVCEnginePrimitivePrepareInfo@@@Z.
// Callees: std::list<CEnginePrimitiveHandle>::clear 0x00834F90, CThingCreatureBase::DrawPreparePrimitivesForRendering 0x00663C40,
//   CThing::PeekDefinitionManager 0x004C7990, CDefinitionManager::GetDefNameFromGlobalIndex 0x009ACCC0,
//   CDefStringTable::GetString 0x009D49B0 (GDefStringTable 0x013CA828), list insert(end()) 0x007464A0,
//   CThingPhysical::PeekPos 0x006A4D00, CEnginePrimitiveLine 0x004C3090, CEnginePrimitiveText 0x004C3120,
//   CCharString::ToWideString 0x0099E480, ~CWideString 0x0099B510, ~CCharString 0x0099EAE0, CThing::PeekComponent 0x004C7A70,
//   CAIBrain::GetCurrentStateGroupDebugText 0x0088B7E0 / PeekCurrentStateGroupName 0x0088B7D0 (donor order, bodies checked),
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPos 0x004C73D0, LowerBound 0x0040F020,
//   CTCGraphicAppearance::GetFocalPos(float) 0x004C1060, CThingCreatureBase::GetNavigationLevelAt 0x00661D20,
//   GFIntToWideString 0x00405F00, CDefString::ToWideString 0x00415D90, CIEngine::AddPrimitive (slot 20) /
//   SetPersistentPrimitive (slot 23).
// Layout (retail): CThing UID +0x18, UsingComponents +0x20, Components +0x44, PBaseDef +0x70, PTCGraphicAppearance +0x64;
//   CThingAICreature HomeBuilding +0x168, WorkBuilding +0x170, DebugRenderHandles +0x194 (STLport list, 4 bytes),
//   PBrain +0x1A8 (CCountedPointer<CAIBrain>::Data); CMainGameComponent debug font +0x16144.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGAICREATURE_WORKBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_DEBUGRENDER
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CAIBRAIN_STATEGROUPTEXT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST_INSERT_DEFAULT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "stlport/fable_containers.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIBrain.h"
#include "fable_string.h"
#include "fable_game.h"
#include "engine/CCountedPointer.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CAIBrain.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefString.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitiveText.h"
#include "engine/CIEngine.h"
#include "engine/CRGBColour.h"
#include "engine/CTCVolumeContainmentTracker.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"

extern bool g_DebugDrawCreatureNames;                    // 0x013B86E8 descriptive (console bool)
extern bool g_DebugDrawStealth;                          // 0x013B8A80 descriptive (established spelling, row 0x006AA2E0)
extern bool g_DebugDrawBuildings;                        // 0x013B8A81 descriptive (console bool)
extern bool g_DebugDrawHeroNavigationLevel;              // 0x013B8640 descriptive (established spelling, row 0x006AA2E0)
CWideString __fastcall GFIntToWideString(long value);    // 0x00405F00 ?GFIntToWideString@@YI?AVCWideString@@J@Z
template<class T> inline bool __fastcall IS_THING_ALIVE(const T& thing) { return thing && !thing->IsDead(); }   // FableWin ??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z

// No shared header: Ego_r.pdb CEnginePrimitiveLine : CEnginePrimitive, sizeof 0x3C (the landed CShape::EditorDrawHighlightPoints
// 0x007617A0 and NShape::CTrack 0x00761620 rows bind the same constructor 0x004C3090).
class CEnginePrimitiveLine : public CEnginePrimitive {
public:
    CEnginePrimitiveLine(unsigned __int64 id, const C3DVector& start, const C3DVector& end, const CRGBColour& colour, float width,
                         long blendMode, unsigned long flags, bool depthTest, long interpolationSet, long blendOp);   // 0x004C3090
    unsigned char _body[0x3c - sizeof(CEnginePrimitive)];
};

CThingBuilding* CThingAICreature::GetHomeBuilding() const { return HomeBuilding.Get(); }
CThingBuilding* CThingAICreature::GetWorkBuilding() const { return WorkBuilding.Get(); }

bool CThingAICreature::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    DebugRenderHandles.clear();
    if (!CThingCreatureBase::DrawPreparePrimitivesForRendering(info))
        return false;
    if (g_DebugDrawCreatureNames) {
        CCharString name = PeekDefinitionManager().GetDefNameFromGlobalIndex(GetDefGlobalIndex()).GetString();
        float height = 2.0f;
        DebugRenderHandles.push_back();
        {
        CRGBColour lineColour(0xff, 0xff, 0x78, 0xff);
        const C3DVector& linePos = PeekPos();
        info.Engine->SetPersistentPrimitive(DebugRenderHandles.back(),
            CEnginePrimitiveLine(GetUniqueID(), PeekPos(), C3DVector(linePos.X, linePos.Y, linePos.Z + height), lineColour, 0.05f, 0, 0, true, 0, 0), 0x40);
        }
        DebugRenderHandles.push_back();
            CRGBColour colour(0xff, 0xff, 0x78, 0xff);
            const CFontBank& font = PeekComponent().PeekDebugFont();
            const C3DVector& pos = PeekPos();
            C3DVector textPos(pos.X, pos.Y, pos.Z + height);
            info.Engine->SetPersistentPrimitive(DebugRenderHandles.back(),
                CEnginePrimitiveText(GetUniqueID(), textPos, 0.8f, name.ToWideString(), font, colour, false, (EEngineInterpolationSet)0), 0);
        if (!PBrain.IsNull()) {
            const CCharString* debugText = PBrain->GetCurrentStateGroupDebugText();
            if (debugText) {
                height += 0.3f;
                DebugRenderHandles.push_back();
                CRGBColour colour(0xff, 0xff, 0x78, 0xff);
                const CFontBank& font = PeekComponent().PeekDebugFont();
                const C3DVector& pos = PeekPos();
                C3DVector textPos(pos.X, pos.Y, pos.Z + height);
                info.Engine->SetPersistentPrimitive(DebugRenderHandles.back(),
                    CEnginePrimitiveText(GetUniqueID(), textPos, 0.8f, debugText->ToWideString(), font, colour, false, (EEngineInterpolationSet)0), 0);
            }
            const CCharString* stateGroupName = PBrain->PeekCurrentStateGroupName();
            if (stateGroupName) {
                height += 0.3f;
                DebugRenderHandles.push_back();
                CRGBColour colour(0xff, 0xff, 0x78, 0xff);
                const CFontBank& font = PeekComponent().PeekDebugFont();
                const C3DVector& pos = PeekPos();
                C3DVector textPos(pos.X, pos.Y, height + pos.Z);
                info.Engine->SetPersistentPrimitive(DebugRenderHandles.back(),
                    CEnginePrimitiveText(GetUniqueID(), textPos, 0.8f, stateGroupName->ToWideString(), font, colour, false, (EEngineInterpolationSet)0), 0);
            }
        }
    }
    if (g_DebugDrawStealth && HasTC(TCI_PERCEIVE_THING))
        GetTC(TCI_PERCEIVE_THING).DrawPreparePrimitivesForRendering(info);
    if (g_DebugDrawBuildings) {
        if (GetHomeBuilding() == GetWorkBuilding()) {
            if (GetHomeBuilding()) {
                DebugRenderHandles.push_back();
                CRGBColour colour(0x7f, 0x7f, 0x7f, 0xff);
                const C3DVector& buildingPos = GetHomeBuilding()->GetPos();
                C3DVector start(buildingPos.X, buildingPos.Y, buildingPos.Z + 1.5f);
                const C3DVector& myPos = GetPos();
                C3DVector end(myPos.X, myPos.Y, myPos.Z + 1.5f);
                info.Engine->SetPersistentPrimitive(DebugRenderHandles.back(),
                    CEnginePrimitiveLine(0, start, end, colour, 0.05f, 0, 0, true, 0, 0), 0);
            }
        } else {
            if (GetHomeBuilding()) {
                DebugRenderHandles.push_back();
                CRGBColour colour(0xff, 0xff, 0xff, 0xff);
                const C3DVector& buildingPos = GetHomeBuilding()->GetPos();
                C3DVector start(buildingPos.X, buildingPos.Y, buildingPos.Z + 1.5f);
                const C3DVector& myPos = GetPos();
                C3DVector end(myPos.X, myPos.Y, myPos.Z + 1.5f);
                info.Engine->SetPersistentPrimitive(DebugRenderHandles.back(),
                    CEnginePrimitiveLine(0, start, end, colour, 0.05f, 0, 0, true, 0, 0), 0);
            }
            if (GetWorkBuilding()) {
                DebugRenderHandles.push_back();
                CRGBColour colour(0, 0, 0, 0xff);
                const C3DVector& buildingPos = GetWorkBuilding()->GetPos();
                C3DVector start(buildingPos.X, buildingPos.Y, buildingPos.Z + 1.5f);
                const C3DVector& myPos = GetPos();
                C3DVector end(myPos.X, myPos.Y, myPos.Z + 1.5f);
                info.Engine->SetPersistentPrimitive(DebugRenderHandles.back(),
                    CEnginePrimitiveLine(0, start, end, colour, 0.05f, 0, 0, true, 0, 0), 0);
            }
        }
    }
    if (g_DebugDrawHeroNavigationLevel) {
        C3DVector focal = PTCGraphicAppearance->GetFocalPos(*(const float*)&info.Interpolate[4]);
        focal.Z += 0.7f;
        CWideString level = GFIntToWideString(GetNavigationLevelAt(GetPos()));
        CRGBColour colour(0xff, 0xff, 0, 0xff);
        info.Engine->AddPrimitive(CEnginePrimitiveText(GetUniqueID(), focal, 1.0f, level, PeekComponent().PeekDebugFont(), colour, true,
                                                       (EEngineInterpolationSet)0));
        CTCVolumeContainmentTracker* tracker;
        if (GetTC(&tracker) && IS_THING_ALIVE(tracker->GetPThingInside())
            && !tracker->GetPThingInside()->GetPBaseDef().IsNull()) {
            focal.Z += 0.2f;
            CWideString defName = tracker->GetPThingInside()->GetPBaseDef()->GetInstantiationName().ToWideString();
            info.Engine->AddPrimitive(CEnginePrimitiveText(GetUniqueID(), focal, 0.08f, defName, PeekComponent().PeekDebugFont(),
                                                           colour, true, (EEngineInterpolationSet)0));
        }
    }
    return false;
}
