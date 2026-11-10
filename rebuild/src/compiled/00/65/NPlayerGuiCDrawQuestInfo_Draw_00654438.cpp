// NPlayerGui::CDrawQuestInfo::Draw(const CEnginePrimitivePrepareInfo& prepareInfo, bool forceDraw) -- unless the GUI is in mode 1 and the draw is not forced: stack the quest-info elements down from the top-right quest-info anchor (below the clock ring while the mini-map draws), each advancing by its drawn height rounded up and cleared when it draws nothing or is the mini-game bar, frame them in a box at least the minimum height when any drew, then draw the oracle symbols
// Leaf: ?Draw@CDrawQuestInfo@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@_N@Z
// Retail-asm: 0x006544EC..0x00654506 `fld dword ptr [ebp-0Ch] ; fadd dword ptr [ebp-8] ; fistp dword ptr [ebp-4] ; fild dword ptr [ebp-4] ; fsub dword ptr [ebp+0Ch] ; fstp ; mov eax ; cmp eax ; jne ; dec dword ptr [ebp-4]` -- the engine's inline-assembly ceiling GFFloatToLongCeil (engine/GFFloatToLongCeil.h, Retail-asm-helper with its FABLE_PORTABLE twin) expanded at the height step: 0.5f / 1.0f homed as frame constants, a bare dword fistp into a named long, the fix-up on memory. VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword`: no C/C++ form gives the bare dword fistp.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00654438, 347 bytes, thiscall, ebp frame (`leave`), `ret 8`.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?Draw@CDrawQuestInfo@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@_N@Z (the only
//   two-argument Draw of the class); the body walks CDrawQuestInfo::Elements (+0x58, list<CCountedPointer<CQuestInfoElement> >),
//   tests GuiMode (+0x28) and ends in DrawOracleSymbols (Ego_r.pdb members 4 below the donor; STLport list 4 bytes).
// Callees: CPlayerGui::PeekGuiDef 0x00438570, NPlayerGui::GFAnchorToTopRight(const C2DVector&) 0x00643CA1 (CPlayerGuiDef::
//   QuestInfoPos +0x56C), CPlayerGui::IsMiniMapDrawing() const 0x00437870, the bank's virtual GetGraphicMaxHeight(unsigned long)
//   const (slot 24, +0x60; CPlayerGuiDef::ClockRingGraphic +0x30C), the element's virtual DrawAt (slot 0), Clear (slot 2) and
//   GetType (slot 3; type 6 is never drawn), CDrawQuestInfo::DrawBox(const CEnginePrimitivePrepareInfo&, const C2DBoxF&)
//   0x00643EB2 (CPlayerGuiDef::QuestInfoGraphicWidth +0x578 / QuestInfoGraphicMinHeight +0x580), DrawOracleSymbols 0x0064F6DC.
// Names: 0x00437870 reads PDrawMiniMap->Drawing (CPlayerGui +0x160 / CDrawBase +0x18) -> ?IsMiniMapDrawing@CPlayerGui@@QBE_NXZ;
//   0x00643EB2 is the DrawBox signature (prepare info, box); 0x0064F6DC tests OracleSymbols (+0x40) -> DrawOracleSymbols; bank
//   slot 24 is the max-over-frames of the frame height word (+8; slot 23 takes +6) -> GetGraphicMaxHeight. All probable.
// Label correction: catalogued as "CDrawQuestInfo::Draw" without the NPlayerGui namespace.
// Layout: engine/NPlayerGui_CDrawBase.h (PlayerGui +0x04, Bank +0x14), engine/CGraphicDataBank.h (GetGraphicMaxHeight slot 24 under
//   FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE), engine/C2DVector.h, engine/C2DBoxF.h, engine/CCountedPointer.h and STLport 4.6
//   list / vector, fable_string.h (CWideString), engine/CPlayerGuiDef.h (new FABLE_RETAIL_CPLAYERGUIDEF_QUESTINFO: QuestInfoPos as a
//   C2DVector at +0x56C; ClockRingGraphic +0x30C, QuestInfoGraphicWidth +0x578, QuestInfoGraphicMinHeight +0x580) and engine/CPlayerGui.h
//   (FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF, new _MINIMAPDRAWING IsMiniMapDrawing and _GUIMODE EPlayerGuiMode). CDrawQuestInfo and its
//   CQuestInfoElement have no shared header: modelled from Ego_r.pdb (members from +0x28 on the CDrawBase part).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_C2DVECTOR
#include "engine/C2DVector.h"
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#include "engine/C2DBoxF.h"
#include "engine/GFFloatToLongCeil.h"
// Shape: the box is the FableWin.pdb inline C2DBoxF(const C2DVector& top_left, const C2DExtentsF& extents) over a named extents
//   (opt-in FABLE_RETAIL_C2DBOXF_EXTENTS): its BRX = top_left.X + extents.X gives retail's `fld st(1) ; fadd [pos.X]` (0x0065454D);
//   the parked field-by-field box loaded pos.X first. The height step is `drawPos.Y += (float)GFFloatToLongCeil(height)`.
class CEnginePrimitivePrepareInfo;
#define FABLE_RETAIL_CPLAYERGUIDEF_QUESTINFO
#include "engine/CPlayerGuiDef.h"
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUI_MINIMAPDRAWING
#define FABLE_RETAIL_CPLAYERGUI_GUIMODE
#include "engine/CPlayerGui.h"
#define FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE
#include "engine/CGraphicDataBank.h"
class CPlayerManager;
class CWorld;
class CMainGameComponent;
class CEnginePrimitiveHandle;
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/NPlayerGui_CDrawBase.h"
namespace NPlayerGui {
C2DVector __fastcall GFAnchorToTopRight(const C2DVector& position);   // 0x00643CA1
class CDrawQuestInfo : public CDrawBase {
public:
    enum EElementType { };
    class CQuestInfoElement {
    public:
        virtual float DrawAt(const C2DVector& position, const CEnginePrimitivePrepareInfo& prepareInfo);   // slot 0
        virtual bool Update();                              // slot 1
        virtual void Clear();                               // slot 2
        virtual EElementType GetType() const;               // slot 3
    };
    void Draw(const CEnginePrimitivePrepareInfo& prepareInfo, bool forceDraw);
    void DrawBox(const CEnginePrimitivePrepareInfo& prepareInfo, const C2DBoxF& box);   // 0x00643EB2
    void DrawOracleSymbols(const CEnginePrimitivePrepareInfo& prepareInfo);            // 0x0064F6DC
    EPlayerGuiMode                GuiMode;                  // +0x28
    float                         FadeInTimer;              // +0x2C
    float                         MiniModeTimer;            // +0x30
    bool                          MiniMode;                 // +0x34
    long                          FramesUntilMiniMode;      // +0x38
    long                          MiniGameElementIndex;     // +0x3C
    std::list<long>               OracleSymbols;            // +0x40
    std::vector<CEnginePrimitiveHandle> OraclePrimitiveHandles;   // +0x44
    CWideString                   Name;                     // +0x50
    CWideString                   Text;                     // +0x54
    std::list<CCountedPointer<CQuestInfoElement> > Elements;   // +0x58
};

void CDrawQuestInfo::Draw(const CEnginePrimitivePrepareInfo& prepareInfo, bool forceDraw)
{
    if (GuiMode == (EPlayerGuiMode)1 && !forceDraw)
        return;
    const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
    C2DVector pos = GFAnchorToTopRight(def.QuestInfoPos);
    if (PlayerGui->IsMiniMapDrawing())
        pos.Y = pos.Y + (float)(long)Bank->GetGraphicMaxHeight(def.ClockRingGraphic) + 16.0f;
    C2DVector drawPos = pos;
    bool drewSomething = false;
    for (std::list<CCountedPointer<CQuestInfoElement> >::iterator it = Elements.begin(); it != Elements.end(); ++it) {
        if (it->Data) {
            if ((*it)->GetType() != (EElementType)6) {
                float height = (*it)->DrawAt(drawPos, prepareInfo);
                if (height > 0.0001f) {
                    drawPos.Y += (float)GFFloatToLongCeil(height);
                    drewSomething = true;
                }
                else
                    (*it)->Clear();
            }
            else
                (*it)->Clear();
        }
    }
    if (drewSomething) {
        C2DExtentsF size((float)def.QuestInfoGraphicWidth, drawPos.Y - pos.Y);
        C2DBoxF box(pos, size);
        if (box.BRY - pos.Y < def.QuestInfoGraphicMinHeight)
            box.BRY = pos.Y + def.QuestInfoGraphicMinHeight;
        DrawBox(prepareInfo, box);
    }
    DrawOracleSymbols(prepareInfo);
}
}   // namespace NPlayerGui