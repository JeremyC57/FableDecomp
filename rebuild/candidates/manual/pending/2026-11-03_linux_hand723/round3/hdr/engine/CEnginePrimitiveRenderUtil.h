#ifndef FABLE_ENGINE_CENGINEPRIMITIVERENDERUTIL_H
#define FABLE_ENGINE_CENGINEPRIMITIVERENDERUTIL_H
// Hand-written (chunk 723): CEnginePrimitiveRenderUtil : CBaseClassNonCopyable : CBaseClass (Fable.exe RTTI), no data members
// (Ego_r.pdb). vtable 0x012A3BD0; ctor inline (out-of-line copy 0x00B8B5E0), virtual dtor slot 0 0x00B8B600. Its many helpers are
// static fastcall (SI) functions in the PDBs and are declared where a row needs them.
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#include "engine/CEnginePrimitiveThemeChannelDesc.h"
class C3DVector;
class CEngineCamera;
class CEngineLocalLightArray;
class CPixelShader;
class CVertexShader;
enum EEnginePrimitiveBlendMode;
enum EEnginePrimitiveBlendOp;
#endif
class CEnginePrimitiveRenderUtil : public CBaseClassNonCopyable {
public:
    CEnginePrimitiveRenderUtil() { }                                     // ??0CEnginePrimitiveRenderUtil@@QAE@XZ
    virtual ~CEnginePrimitiveRenderUtil();                               // slot 0  ??1CEnginePrimitiveRenderUtil@@UAE@XZ
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
    // Retail opt-in (chunk 723c): Ego_r.pdb / FableWin.pdb members with no retail out-of-line copy (RenderSingleSprite
    // 0x00BC04F0 expands them); the row that expands them defines the bodies (they reach the engine globals).
    static void __fastcall ApplyVertexShader(const CVertexShader& shader);   // ?ApplyVertexShader@CEnginePrimitiveRenderUtil@@SIXABVCVertexShader@@@Z
    static void __fastcall ApplyPixelShader(const CPixelShader& shader);     // ?ApplyPixelShader@CEnginePrimitiveRenderUtil@@SIXABVCPixelShader@@@Z
    static CEngineCamera& __fastcall GetCurrentCamera();                      // ?GetCurrentCamera@CEnginePrimitiveRenderUtil@@SIAAVCEngineCamera@@XZ
    static CEngineLocalLightArray* __fastcall FindLocalLights(const C3DVector& position, float radius,
                                                              CEnginePrimitiveThemeChannelDesc channel);
                                    // ?FindLocalLights@CEnginePrimitiveRenderUtil@@SIPAVCEngineLocalLightArray@@ABVC3DVector@@MVCEnginePrimitiveThemeChannelDesc@@@Z
    static void __fastcall SetupLocalLights(const CEngineLocalLightArray& lights);   // ?SetupLocalLights@CEnginePrimitiveRenderUtil@@SIXABVCEngineLocalLightArray@@@Z
    void SetupBlendOpState(EEnginePrimitiveBlendOp op);                   // ?SetupBlendOpState@CEnginePrimitiveRenderUtil@@QAEXW4EEnginePrimitiveBlendOp@@@Z
    // ?SetupAlphaBlendingStatesFromBlendMode@CEnginePrimitiveRenderUtil@@QAE_NW4EEnginePrimitiveBlendMode@@@Z: true = the colour
    // must be premultiplied by alpha.
    bool SetupAlphaBlendingStatesFromBlendMode(EEnginePrimitiveBlendMode mode);
#endif
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveRenderUtil) == 0x4);

#endif // FABLE_ENGINE_CENGINEPRIMITIVERENDERUTIL_H
