// CEngineInternalPrimitiveLine::Update(const CEnginePrimitive& primitive, unsigned long flags) -- copy a line descriptor (end points, colour with its alpha, width, bank index, blend mode/op, z-buffer flag; frame reset to 0), start loading the line's graphic when it has one, and apply the game-primitive fade defaults
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC7F70, length 241 bytes (`ret 8`, virtual `?Update@CEngineInternalPrimitiveLine@@UAEXABVCEnginePrimitive@@K@Z`).
// Callees: CGraphicInfoFrame::BeginLoading(unsigned long, unsigned long) @ 0x009FE620,
// CEngineInternalPrimitiveBase::SetFadeDistanceWithGamePrimitiveDefaults(float) @ 0x00B8FD40.
// Globals: the engine @ 0x01436E98 (graphic bank pointer at +0x954, its 0x2c-byte record vector at +0x1e0) -- the bank model of
// the landed CEngineSurfaceCompositionManager::GetGraphicInfo 0x00B2B790.
// Layout: base = engine/CEngineInternalPrimitiveBase.h (EnginePrimitiveFlags +0x0c, Alpha +0x27); the line fields follow the PDB
// CEngineInternalPrimitiveLine / CEnginePrimitiveLine layouts exactly (Start +0x48, End +0x54, Colour +0x60, Width +0x64,
// BankIndex +0x68, Frame +0x6c, BlendMode +0x70, BlendOp +0x74, UseZBuffer +0x78; descriptor Start +0x0c .. UseZBuffer +0x38).
// The method is attached through a derived shell (the header is a plain layout struct), so it is graded non-virtual.
// Shape: the C3DVector members are whole POD assignments (`lea` of both ends); the base Alpha is copied from the stored
// colour's alpha byte; the frame pointer is `Get(i) ? Get(i)->m_pFrame : 0` over the inline bounds-checked bank getter (the
// second getter folds to `sbb ; and`), called even when null; both getters must be `__forceinline` (else GetFrame stays out of line).
// NF46 (round 7): the 0x01436E98 stand-in CEngineGlobal_01436e98 / g_pEngine_01436e98 is NEngineInternal::GPResourceManager
//   (?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A) on engine/CEngineResourceManager.h; +0x954 is GetGraphicBank (_GETGRAPHICBANK,
//   as 0x00B2D9F0), and the local CGraphicBank / SBankGraphic / CRetailVector / CGraphicInfoFrame models are the header CGraphicDataBank /
//   CGraphicInfo / CGraphicInfoFrame (GraphicList +0x1E0, Frames +0x28; FableWin.pdb inline PeekGraphic / PeekGraphicInfoFrame / PeekInfoFrame as
//   landed 0x00BB0970, both getters forced inline per the shape note; BeginLoading const from the _BEGINLOADING opt-in).
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_GETGRAPHICBANK
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICINFOFRAME_BEGINLOADING
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME_DTOR_FORCEINLINE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "fable_compiler.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CGraphicInfo.h"
#include "engine/CEngineInternalPrimitiveBase.h"

#include "engine/C3DVector.h"
class CRGBColour { public: unsigned char B, G, R, A; };

class CEnginePrimitive {                           // PDB, 12 bytes
public:
    long          Type;
    long          InterpolationSet;
    unsigned long ThemeChannelDesc;
};

class CEnginePrimitiveLine : public CEnginePrimitive {   // PDB, 60 bytes
public:
    C3DVector     Start;                           // +0x0c
    C3DVector     End;                             // +0x18
    CRGBColour    Colour;                          // +0x24
    float         Width;                           // +0x28
    unsigned long BankIndex;                       // +0x2c
    long          BlendMode;                       // +0x30
    long          BlendOp;                         // +0x34
    bool          UseZBuffer;                      // +0x38
};

namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98
FABLE_FORCEINLINE const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}
FABLE_FORCEINLINE const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long graphic, unsigned long frame) const
{
    return PeekGraphic(graphic) ? PeekGraphic(graphic)->PeekInfoFrame(frame) : 0;
}

class CEngineInternalPrimitiveLine : public CEngineInternalPrimitiveBase { public:
    C3DVector     Start;                           // +0x48
    C3DVector     End;                             // +0x54
    CRGBColour    Colour;                          // +0x60
    float         Width;                           // +0x64
    unsigned long BankIndex;                       // +0x68
    unsigned long Frame;                           // +0x6c
    long          BlendMode;                       // +0x70
    long          BlendOp;                         // +0x74
    bool          UseZBuffer;                      // +0x78
    void SetFadeDistanceWithGamePrimitiveDefaults(float distance);   // 0x00B8FD40
    void Update(const CEnginePrimitive& primitive, unsigned long flags);
};

void CEngineInternalPrimitiveLine::Update(const CEnginePrimitive& primitive, unsigned long flags)
{
    const CEnginePrimitiveLine& line = static_cast<const CEnginePrimitiveLine&>(primitive);
    EnginePrimitiveFlags = flags;
    Start = line.Start;
    End = line.End;
    Colour = line.Colour;
    Alpha = Colour.A;
    Width = line.Width;
    BankIndex = line.BankIndex;
    Frame = 0;
    BlendMode = line.BlendMode;
    BlendOp = line.BlendOp;
    UseZBuffer = line.UseZBuffer;
    if (BankIndex)
        NEngineInternal::GPResourceManager->GetGraphicBank((EGraphicBankNo)0)->PeekGraphicInfoFrame(BankIndex, 0)->BeginLoading(0, 0);
    SetFadeDistanceWithGamePrimitiveDefaults(0.0f);
}