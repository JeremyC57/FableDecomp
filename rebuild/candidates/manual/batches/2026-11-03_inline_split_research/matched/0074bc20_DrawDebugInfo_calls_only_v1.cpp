#define FABLE_RETAIL_CRGBCOLOUR
// CTCCreatureGenerator::DrawDebugInfo(const CEnginePrimitivePrepareInfo& info) -- when the creature-generator debug display is on, draw
// Grade: DIFFER(1415v1290) (best of 12 compiled attempts; residue in NOTES.md)
// "Points (Open/Hidden): <open> / <hidden>" two units above the generator and a pair of 0.05-wide lines per cached position (open: green
// to the point and light green one unit up; hidden: red and pink), then clear the persistent handles left over
// Leaf: ?DrawDebugInfo@CTCCreatureGenerator@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCCREATUREGENERATOR
#define FABLE_RETAIL_CTCSHAPEMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CIENGINE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCCreatureGenerator.h"
#include "engine/CMainGameComponent.h"
#include "engine/CIEngine.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitiveLine.h"
#include "engine/CEnginePrimitiveText.h"

extern bool g_CreatureGeneratorDrawDebug;   // 0x013BAD58 (descriptive; .bss, read only here and by FrameUpdate 0x0074C226)

// FableWin.pdb ?GFIntToWideString@@YI?AVCWideString@@J@Z: a header inline (out-of-line COMDAT 0x00405F00), expanded here.
inline CWideString __fastcall GFIntToWideString(long value)
{
    CWideString text;
    text.Sprintf(L"%d", value);
    return text;
}

void CTCCreatureGenerator::DrawDebugInfo(const CEnginePrimitivePrepareInfo& info)
{
    if (!g_CreatureGeneratorDrawDebug)
        return;
    C3DVector pos = GetParentThing().GetPos();
    DebugPrimitives.resize((HiddenPositions.size() + OpenPositions.size()) * 2 + 1);
    CWideString text = L"Points (Open/Hidden): " + GFIntToWideString(OpenPositions.size()) + L" / " +
                       GFIntToWideString(HiddenPositions.size());
    C3DVector text_pos(pos.X, pos.Y, pos.Z + 2.0f);
    const CFontBank& font = PeekComponent().PeekDebugFont();
    unsigned __int64 id = GetParentThing().GetUniqueID();
    info.Engine->SetPersistentPrimitive(DebugPrimitives[0],
        CEnginePrimitiveText(id, text_pos, 1.0f, text,
                             font, CRGBColour(255, 255, 255, 255), true, EEngineInterpolationSet(0)), 0);
    long index = 1;
    if (!OpenPositions.empty()) {
    for (std::vector<C3DVector>::const_iterator it = OpenPositions.begin(); it != OpenPositions.end(); ++it) {
        info.Engine->SetPersistentPrimitive(DebugPrimitives[index++],
            CEnginePrimitiveLine(GetParentThing().GetUniqueID(), pos, *it, CRGBColour(0, 255, 0, 255), 0.05f,
                                 EEnginePrimitiveBlendMode(0), 0, true, EEngineInterpolationSet(0), EEnginePrimitiveBlendOp(0)), 0);
        info.Engine->SetPersistentPrimitive(DebugPrimitives[index++],
            CEnginePrimitiveLine(GetParentThing().GetUniqueID(), *it, C3DVector(it->X, it->Y, it->Z + 1.0f), CRGBColour(128, 255, 128, 255),
                                 0.05f, EEnginePrimitiveBlendMode(0), 0, true, EEngineInterpolationSet(0), EEnginePrimitiveBlendOp(0)), 0);
    }
    }
    for (std::vector<C3DVector>::const_iterator it = HiddenPositions.begin(); it != HiddenPositions.end(); ++it) {
        info.Engine->SetPersistentPrimitive(DebugPrimitives[index++],
            CEnginePrimitiveLine(GetParentThing().GetUniqueID(), pos, *it, CRGBColour(255, 0, 0, 255), 0.05f,
                                 EEnginePrimitiveBlendMode(0), 0, true, EEngineInterpolationSet(0), EEnginePrimitiveBlendOp(0)), 0);
        info.Engine->SetPersistentPrimitive(DebugPrimitives[index++],
            CEnginePrimitiveLine(GetParentThing().GetUniqueID(), *it, C3DVector(it->X, it->Y, it->Z + 1.0f), CRGBColour(255, 128, 128, 255),
                                 0.05f, EEnginePrimitiveBlendMode(0), 0, true, EEngineInterpolationSet(0), EEnginePrimitiveBlendOp(0)), 0);
    }
    for (std::vector<CEnginePrimitiveHandle>::iterator handle = DebugPrimitives.begin() + index; handle != DebugPrimitives.end(); ++handle)
        handle->Clear();
}
