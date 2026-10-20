// CTCTavernGame::DrawBox(const CEnginePrimitivePrepareInfo&) -- draws the main-game score box: nine box handles, the box placed and sized from the tavern-game def in render-target units, on layer 0x47
// Leaf: ?DrawBox@CTCTavernGame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EC990, 177 bytes (`ret 4`). Ego_r.pdb
// `?DrawBox@CTCTavernGame@@IAEXABVCEnginePrimitivePrepareInfo@@@Z`.
// Callees: CDefClassBase::GetSubDef<CTavernGameDef> 0x005ED4A0 (landed as GetSubDef_005ed4a0) through the inline
//          CTCTavernGame::GetDef (out-of-line copy 0x005EA000); vector<CEnginePrimitiveHandle>::Resize(long) 0x00443F90;
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
struct CEnginePrimitiveHandle;

struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct C2DBoxF {
    C2DVector TopLeft;
    C2DVector BottomRight;
    C2DBoxF(float left, float top, float right, float bottom) : TopLeft(left, top), BottomRight(right, bottom) {}
};
struct C2DExtentsI {
    long X, Y;
    C2DExtentsI() {}
};
struct CDisplayManager {
    const C2DExtentsI GetRenderTargetDimensions();   // 0x009BEDC0
};
extern CDisplayManager* g_pDisplayDevice;      // 0x013B8390

struct CRetailHandleVector {                    // STLport vector<CEnginePrimitiveHandle> (12 bytes)
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    void Resize(long count);                    // 0x00443F90
};
struct CEnginePrimitivePrepareInfo;
enum EEngine2DLayer { ENGINE_2D_LAYER_TAVERN_SCORE_BOX = 0x47 };
void __fastcall DrawBox(CEnginePrimitivePrepareInfo* info, CRetailHandleVector* handles, const C2DBoxF& box,
                        EEngine2DLayer layer);   // 0x0048E0A2

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CTavernGameDef : CDefVirtual {
    long m_count;                               // +0x04
    unsigned char _pad_0x08[0x180];
    float MainGameScoreBoxX;                    // +0x188
    float MainGameScoreBoxY;                    // +0x18c
    float MainGameScoreBoxWidthXbox;            // +0x190
    float MainGameScoreBoxWidthPC;              // +0x194
    float MainGameScoreBoxHeight;               // +0x198
    void Release() { if (--m_count == 0) Destroy(); }
};
struct CDefPointer {
    CTavernGameDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->Release(); }
    CTavernGameDef* operator->() const { return Object; }
};
struct CDefinitionManager {
    CTavernGameDef* PeekDefFromGlobalIndex(long index);   // stand-in only
};
struct CDefClassBase {
    virtual long* GetSubDefIndex(const char* name);       // stand-in only (retail: slot 14, CCharString name)
    unsigned char       _pad_0x04[0x18];
    CDefinitionManager* m_pManager;             // +0x1c
    bool GetSubDef_005ed4a0(CDefPointer& out);  // 0x005ED4A0 (GetSubDef<CTavernGameDef>)
};

// Stand-in for the same-module GetSubDef<CTavernGameDef> body (0x005ED4A0, landed separately; not graded here): compiled
// above the caller without inlining so VC7.1 sees, as retail's whole-program build did, that the def pointer it fills
// does not escape.
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef_005ed4a0(CDefPointer& out)
{
    long* index = GetSubDefIndex("CTavernGameDef");
    if (index == 0)
        return false;
    CTavernGameDef* def = m_pManager->PeekDefFromGlobalIndex(*index);
    if (out.Object != def) {
        if (out.Object)
            out.Object->Release();
        out.Object = def;
        if (def)
            ++def->m_count;
    }
    return true;
}
#pragma auto_inline(on)

struct CThing {
    unsigned char  _pad_0x00[0x70];
    CDefClassBase* PDef;                        // +0x70
};

struct CTCTavernGame {
    virtual void v00();
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x48];
    CRetailHandleVector BoxHandles;             // +0x50
    void GetDef(CDefPointer* out) { ParentThing->PDef->GetSubDef_005ed4a0(*out); }   // out of line 0x005EA000
    void DrawBox(const CEnginePrimitivePrepareInfo& info);
};

void CTCTavernGame::DrawBox(const CEnginePrimitivePrepareInfo& info)
{
    CDefPointer def;
    GetDef(&def);
    BoxHandles.Resize(9);
    C2DExtentsI size = g_pDisplayDevice->GetRenderTargetDimensions();
    ::DrawBox(const_cast<CEnginePrimitivePrepareInfo*>(&info), &BoxHandles,
              C2DBoxF(def->MainGameScoreBoxX * (float)size.Y, def->MainGameScoreBoxY * (float)size.Y,
                      (def->MainGameScoreBoxX + def->MainGameScoreBoxWidthPC) * size.X,
                      (def->MainGameScoreBoxY + def->MainGameScoreBoxHeight) * (float)size.Y),
              ENGINE_2D_LAYER_TAVERN_SCORE_BOX);
}