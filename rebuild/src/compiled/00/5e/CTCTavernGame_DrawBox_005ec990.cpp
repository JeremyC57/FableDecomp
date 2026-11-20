// CTCTavernGame::DrawBox(const CEnginePrimitivePrepareInfo&) -- draws the main-game score box: nine box handles, the box placed and sized from the tavern-game def in render-target units, on layer 0x47
// Leaf: ?DrawBox@CTCTavernGame
// Naming wave 18: PDisplayManager is GPDisplayManager (?GPDisplayManager@@3PAVCDisplayManager@@A); ChatGPT pass 10 audit, raw Ego_r public.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EC990, 177 bytes (`ret 4`). Ego_r.pdb
// `?DrawBox@CTCTavernGame@@IAEXABVCEnginePrimitivePrepareInfo@@@Z`.
// Callees: CDefClassBase::GetSubDef<CTavernGameDef> 0x005ED4A0 (landed as GetSubDef_005ed4a0) through the inline
//          CTCTavernGame::GetDef (out-of-line copy 0x005EA000); vector<CEnginePrimitiveHandle>::resize(size_type) 0x00443F90;
//          CDisplayManager::GetRenderTargetDimensions() 0x009BEDC0 on g_pDisplayDevice 0x013B8390; the nine-patch box
//          drawer 0x0048E0A2 (`__fastcall`: info in ecx, handles in edx, box and layer on the stack; declared as the landed
//          caller 0x00643EB2 spells it -- Ego_r.pdb names it CSubtitleRenderer::DrawBox(const
//          CEnginePrimitivePrepareInfo&, vector<CEnginePrimitiveHandle>&, const C2DBoxF&, EEngine2DLayer)); the def's
//          slot 1 on release.
// Layout: retail CTCTavernGame::BoxHandles +0x50 (local model; engine/CTCTavernGame.h carries PDB offsets); retail
//         CTavernGameDef sits 4 below the PDB after its PrizeScores vector: MainGameScoreBoxX +0x188, MainGameScoreBoxY
//         +0x18c, MainGameScoreBoxWidthPC +0x194, MainGameScoreBoxHeight +0x198.
// Shape: `(float)size.Y` is written at each use: VC7.1 converts it once into a compiler temporary that it homes in the
//        def pointer's dead frame word (a named float local takes a frame word of its own), while the width is used
//        through `fimul`; the 4-float C2DBoxF constructor's arguments evaluate right to left
//        onto the x87 stack and are stored into the box temporary in one run. As in GetTotalWinnings (0x005EA310) the
//        def is kept in esi to the short `dec [esi+4] ; jne` release only when VC7.1 sees the GetSubDef body (a
//        stand-in compiled above under `#pragma auto_inline(off)`); only DrawBox is graded.
class CEnginePrimitiveHandle;
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#include "stlport/fable_containers.h"

class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class C2DBoxF { public:
    C2DVector TopLeft;
    C2DVector BottomRight;
    C2DBoxF(float left, float top, float right, float bottom) : TopLeft(left, top), BottomRight(right, bottom) {}
};
class C2DExtentsI { public:
    long X, Y;
    C2DExtentsI() {}
};
class CDisplayManager { public:
    const C2DExtentsI GetRenderTargetDimensions();   // 0x009BEDC0
};
extern CDisplayManager* GPDisplayManager;      // 0x013B8390

// Name fix (NF189, 2026-11-20, q4755): the handle vector's resize 0x00443F90 is the shared STLport
//   vector<CEnginePrimitiveHandle>::resize(size_type) (stlport/fable_containers.h, declared only by
//   FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL); the header-less local CRetailHandleVector keeps its token (the box drawer
//   0x0048E0A2 is declared with it, as the landed caller 0x00643EB2 spells it) and sits on that shared base.
struct CRetailHandleVector : std::vector<CEnginePrimitiveHandle> {   // STLport vector<CEnginePrimitiveHandle> (12 bytes)
};
class CEnginePrimitivePrepareInfo;
enum EEngine2DLayer { ENGINE_2D_LAYER_TAVERN_SCORE_BOX = 0x47 };
void __fastcall DrawBox(CEnginePrimitivePrepareInfo* info, CRetailHandleVector* handles, const C2DBoxF& box,
                        EEngine2DLayer layer);   // 0x0048E0A2

// NF170 (q4278, 2026-11-20): the sub-def call 0x005ED4A0 is the shared CDefClassBase::GetSubDef<CTavernGameDef> template (Ego_r.pdb +
//   FableWin.pdb ??$GetSubDef@VCTavernGameDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCTavernGameDef@@@@@Z; the callee pushes the literal
//   'CTavernGameDef'), its noinline body visible (engine/CDefClassBase_GetSubDef.h FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE), on the
//   shared CDefPointer / CTavernGameDef (Ego_r member names) and fable_string.h. Was a local def / CDefPointer / CDefClassBase model (and
//   local string classes) with a stand-in GetSubDef body.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#include "fable_string.h"
#include "engine/CTavernGameDef.h"
#include "engine/CDefClassBase_GetSubDef.h"

class CThing { public:
    unsigned char  _pad_0x00[0x70];
    CDefClassBase* PDef;                        // +0x70
};

class CTCTavernGame { public:
    virtual void v00();
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x48];
    CRetailHandleVector BoxHandles;             // +0x50
    void GetDef(CDefPointer<const CTavernGameDef>* out) { ParentThing->PDef->GetSubDef(out); }   // out of line 0x005EA000
    void DrawBox(const CEnginePrimitivePrepareInfo& info);
};

void CTCTavernGame::DrawBox(const CEnginePrimitivePrepareInfo& info)
{
    CDefPointer<const CTavernGameDef> def;
    GetDef(&def);
    BoxHandles.resize(9);
    C2DExtentsI size = GPDisplayManager->GetRenderTargetDimensions();
    ::DrawBox(const_cast<CEnginePrimitivePrepareInfo*>(&info), &BoxHandles,
              C2DBoxF(def->MainGameScoreBoxX * (float)size.Y, def->MainGameScoreBoxY * (float)size.Y,
                      (def->MainGameScoreBoxX + def->MainGameScoreBoxWidthPC) * size.X,
                      (def->MainGameScoreBoxY + def->MainGameScoreBoxHeight) * (float)size.Y),
              ENGINE_2D_LAYER_TAVERN_SCORE_BOX);
}