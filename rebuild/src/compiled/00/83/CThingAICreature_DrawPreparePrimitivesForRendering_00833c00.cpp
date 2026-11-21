// CThingAICreature::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- clear the creature's debug render handles, run the CThingCreatureBase pass, then (debug switches) draw the creature's def name, its brain's current state-group debug text and name, the stealth perception debug, lines to its home / work buildings, and the hero navigation level with the containing volume's def name; always returns false
// Leaf: ?DrawPreparePrimitivesForRendering@CThingAICreature@@UAE_NABVCEnginePrimitivePrepareInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00833C00, 2300 bytes, `ret 4`. Grade: RELOCATION_MATCH (2300/2300). No flag. CThingAICreature vtable 0x0127293C slot 36
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
// Shape:
//   - The CEnginePrimitiveLine and CEnginePrimitiveText constructors are inline header functions in the original (Ego_d.pdb lists each
//     once, under tc_graphic_appearance.obj; FableWin.pdb emits their COMDATs in several objects) with one out-of-line copy each in
//     retail (0x004C3090 / 0x004C3120). Here they are plain-inline and visible (opt-ins FABLE_RETAIL_CENGINEPRIMITIVELINE_INLINE_CTOR /
//     _TEXT_INLINE_CTOR); c2 still calls them out of line at every site, as retail does. With the bodies in view, each
//     `pos + vect` result is built with Z stored first and X / Y copied through ecx / edx, as retail does. Declaration-only bodies give
//     x87 copies (DIFFER, 164 difflines).
//   - C3DVector::operator+ is the header's plain `C3DVector(X+o.X, Y+o.Y, Z+o.Z)` form. The grouped XY opt-in breaks the integer copies
//     once the constructor bodies are visible (202).
//   - The stealth and buildings switches are CThingManager's private statics ?DrawPerceptionDebug / ?DrawLinkedBuildingsDebug@CThingManager@@0_NA
//     (Ego_d.pdb; 0x013B8A80 / 0x013B8A81, set by SetDrawThingPerception 0x0051E4A0 / SetDrawThingLinkedBuildings 0x0051E4B0), read
//     through FableWin.pdb's inline IsDrawingPerceptionDebug / IsDrawingLinkedBuildingsDebug on GetThingManager(); the current-manager
//     load is dead and drops out. The names and navigation-level switches (0x013B86E8 / 0x013B8640) keep their established descriptive
//     names (one-source Ego_d candidates GDisplayScriptNames / GDisplayNavigationLevels).
// Identity: Ego_d.pdb locals vect, def_name (const CCharString&), state_text, perception (CTCBase&), up, tracker, draw_pos, level, name;
//   line table 916..1007 (50 records): def_name before vect, push_back and the primitive call on separate lines.
// Inferred: GetHomeBuilding / GetWorkBuilding are the header's "body in the includer" inline bodies (HomeBuilding.Get()).
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVETEXT_INLINE_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVELINE_INLINE_CTOR
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHING_GETTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_DEBUGDRAWFLAGS
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_MIXED
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING
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
#define FABLE_RETAIL_CAIBRAIN_STATEGROUPTEXT
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_INSERT_DEFAULT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "stlport/fable_containers.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingManager.h"
#include "engine/ThingAlive.h"
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
#include "engine/CEnginePrimitiveLine.h"
#include "engine/CIEngine.h"
#include "engine/CRGBColour.h"
#include "engine/CTCVolumeContainmentTracker.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"

extern bool g_DebugDrawCreatureNames;                    // 0x013B86E8 descriptive (console bool)
extern bool g_DebugDrawHeroNavigationLevel;              // 0x013B8640 descriptive (established spelling, row 0x006AA2E0)
CWideString __fastcall GFIntToWideString(long value);    // 0x00405F00 ?GFIntToWideString@@YI?AVCWideString@@J@Z


inline CThingBuilding* CThingAICreature::GetHomeBuilding() const { return HomeBuilding.Get(); }
inline CThingBuilding* CThingAICreature::GetWorkBuilding() const { return WorkBuilding.Get(); }

bool CThingAICreature::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    DebugRenderHandles.clear();
    if (!CThingCreatureBase::DrawPreparePrimitivesForRendering(info))
        return false;
    if (g_DebugDrawCreatureNames) {
        const CCharString& def_name = PeekDefinitionManager().GetDefNameFromGlobalIndex(GetDefGlobalIndex()).GetString();
        C3DVector vect(0.0f, 0.0f, 2.0f);
        DebugRenderHandles.push_back();
        info.Engine.SetPersistentPrimitive(DebugRenderHandles.back(),
            CEnginePrimitiveLine(GetUniqueID(), PeekPos(), PeekPos() + vect, CRGBColour(0xff, 0xff, 0x78, 0xff), 0.05f, (EEnginePrimitiveBlendMode)0, 0, true, (EEngineInterpolationSet)0, (EEnginePrimitiveBlendOp)0), 0x40);
        DebugRenderHandles.push_back();
        info.Engine.SetPersistentPrimitive(DebugRenderHandles.back(),
            CEnginePrimitiveText(GetUniqueID(), PeekPos() + vect, 0.8f, def_name.ToWideString(), PeekComponent().PeekDebugFont(),
                                 CRGBColour(0xff, 0xff, 0x78, 0xff), false, (EEngineInterpolationSet)0), 0);
        if (!PBrain.IsNull()) {
            const CCharString* state_text = PBrain->GetCurrentStateGroupDebugText();
            if (state_text) {
                vect.Z += 0.3f;
                DebugRenderHandles.push_back();
                info.Engine.SetPersistentPrimitive(DebugRenderHandles.back(),
                    CEnginePrimitiveText(GetUniqueID(), PeekPos() + vect, 0.8f, state_text->ToWideString(), PeekComponent().PeekDebugFont(),
                                         CRGBColour(0xff, 0xff, 0x78, 0xff), false, (EEngineInterpolationSet)0), 0);
            }
            state_text = PBrain->PeekCurrentStateGroupName();
            if (state_text) {
                vect.Z += 0.3f;
                DebugRenderHandles.push_back();
                info.Engine.SetPersistentPrimitive(DebugRenderHandles.back(),
                    CEnginePrimitiveText(GetUniqueID(), PeekPos() + vect, 0.8f, state_text->ToWideString(), PeekComponent().PeekDebugFont(),
                                         CRGBColour(0xff, 0xff, 0x78, 0xff), false, (EEngineInterpolationSet)0), 0);
            }
        }
    }
    if (GetThingManager().IsDrawingPerceptionDebug() && HasTC(TCI_PERCEIVE_THING)) {
        CTCBase& perception = GetTC(TCI_PERCEIVE_THING);
        perception.DrawPreparePrimitivesForRendering(info);
    }
    if (GetThingManager().IsDrawingLinkedBuildingsDebug()) {
        C3DVector up(0.0f, 0.0f, 1.5f);
        if (HomeBuilding == WorkBuilding && GetHomeBuilding()) {
            DebugRenderHandles.push_back();
            info.Engine.SetPersistentPrimitive(DebugRenderHandles.back(),
                CEnginePrimitiveLine(0, GetPos() + up, GetHomeBuilding()->GetPos() + up, CRGBColour(0x7f, 0x7f, 0x7f, 0xff), 0.05f, (EEnginePrimitiveBlendMode)0, 0, true, (EEngineInterpolationSet)0, (EEnginePrimitiveBlendOp)0), 0);
        } else {
            if (GetHomeBuilding()) {
                DebugRenderHandles.push_back();
                info.Engine.SetPersistentPrimitive(DebugRenderHandles.back(),
                    CEnginePrimitiveLine(0, GetPos() + up, GetHomeBuilding()->GetPos() + up, CRGBColour(0xff, 0xff, 0xff, 0xff), 0.05f, (EEnginePrimitiveBlendMode)0, 0, true, (EEngineInterpolationSet)0, (EEnginePrimitiveBlendOp)0), 0);
            }
            if (GetWorkBuilding()) {
                DebugRenderHandles.push_back();
                info.Engine.SetPersistentPrimitive(DebugRenderHandles.back(),
                    CEnginePrimitiveLine(0, GetPos() + up, GetWorkBuilding()->GetPos() + up, CRGBColour(0, 0, 0, 0xff), 0.05f, (EEnginePrimitiveBlendMode)0, 0, true, (EEngineInterpolationSet)0, (EEnginePrimitiveBlendOp)0), 0);
            }
        }
    }
    if (g_DebugDrawHeroNavigationLevel) {
        C3DVector draw_pos = PTCGraphicAppearance->GetFocalPos(info.Interpolate.Current.WFInterpolate);
        draw_pos.Z += 0.7f;
        CWideString level = GFIntToWideString(GetNavigationLevelAt(GetPos()));
        info.Engine.AddPrimitive(CEnginePrimitiveText(GetUniqueID(), draw_pos, 1.0f, level, PeekComponent().PeekDebugFont(),
                                                       CRGBColour(0xff, 0xff, 0, 0xff), true, (EEngineInterpolationSet)0));
        CTCVolumeContainmentTracker* tracker;
        if (GetTC(&tracker) && IS_THING_ALIVE(tracker->GetPThingInside())
            && !tracker->GetPThingInside()->GetPBaseDef().IsNull()) {
            draw_pos.Z += 0.2f;
            CWideString name = tracker->GetPThingInside()->GetPBaseDef()->GetInstantiationName().ToWideString();
            info.Engine.AddPrimitive(CEnginePrimitiveText(GetUniqueID(), draw_pos, 0.08f, name, PeekComponent().PeekDebugFont(),
                                                           CRGBColour(0xff, 0xff, 0, 0xff), true, (EEngineInterpolationSet)0));
        }
    }
    return false;
}