// Retail opt-in (Codex 2026-11-10): complete original types from both PDBs and native field/virtual offsets; see 00b396e0 NAMING_EVIDENCE.md.
#ifndef FABLE_ENGINE_CENGINEPRIMITIVEMANAGER_H
#define FABLE_ENGINE_CENGINEPRIMITIVEMANAGER_H
// Hand-written (chunk 723): CEnginePrimitiveManager : CEnginePrimitiveRenderUtil (Fable.exe RTTI), 0xC bytes: vptr, SceneList +0x04,
// Enable +0x08 (Ego_r.pdb). Base vtable 0x012A53E8, 13 slots; the virtual order below is the retail slot order (landed
// 0x00BCC5F0). Constructor out of line at 0x00BACDB0 (??0CEnginePrimitiveManager@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z).
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CArray.h"
#include "engine/CEnginePrimitiveRenderUtil.h"
class CEngineScene;
class CEngineInternalPrimitiveBase;
class CEngineSubPrimitiveBase;
class CEngineSubPrimitiveChain;
class CEnginePrimitiveTypeList;
class C3DVector;
namespace NEngineShadowRenderer { class CShadowSceneContext; }
enum ERendered;                                                          // MS forward enums
enum EEngineRenderLayer;
enum EEnginePrimitiveType;
enum ERenderPassType;
enum ESubPrimitiveType;
class CEnginePrimitiveManager : public CEnginePrimitiveRenderUtil {
public:
    CEnginePrimitiveManager(CArray<CEngineScene*>& scenes);              // 0x00BACDB0
    virtual ~CEnginePrimitiveManager();                                  // slot 0
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType type) = 0;            // slot 1
    virtual void GetSupportedTypes(CEnginePrimitiveTypeList& types) const = 0;                               // slot 2
    virtual void FrameStart();                                                                                // slot 3
    virtual void FrameEnd();                                                                                  // slot 4
    virtual void RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass);                     // slot 5
    virtual void RenderBoundingVolume(CEngineSubPrimitiveBase& sub);                                         // slot 6
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase& prim, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain) = 0;   // slot 7
    virtual ERendered RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase& prim);                     // slot 8
    virtual bool PreloadPrimitive(CEngineInternalPrimitiveBase& prim, const C3DVector& pos, unsigned long flags);   // slot 9
    virtual void Render2DPrimitive(CEngineInternalPrimitiveBase& prim, CEngineSubPrimitiveChain& chain, ESubPrimitiveType type);   // slot 10
    virtual void RenderNonsortedPrimitives();                                                                 // slot 11
    virtual void AddStaticShadowData(CEngineInternalPrimitiveBase& prim, NEngineShadowRenderer::CShadowSceneContext& context);   // slot 12
    void AddEnableConsoleVariable(const CCharString& name);              // 0x00BACD80 ?AddEnableConsoleVariable@CEnginePrimitiveManager@@QAEXABVCCharString@@@Z
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
    void AddPrimitive(CEngineInternalPrimitiveBase&,unsigned long);
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
protected:
 void AddSubPrimitive(CEngineSubPrimitiveBase*,float,unsigned char,unsigned long);
public:
 bool GetEnable()const{return Enable;}
#else
    bool GetEnable() const;
#endif                                              // ?GetEnable@CEnginePrimitiveManager@@QBE_NXZ
    void SetEnable(bool enable);
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_BLOCKONLOAD
// Retail opt-in (chunk 2028, row 0x00B9A4A0 CEnginePrimitiveManagerRepeatedStaticMeshes::RenderPrimitive): Ego_r.pdb / FableWin.pdb
    // ?GetBlockOnLoad@CEnginePrimitiveManager@@SI_NXZ (inline: unreferenced COMDAT 0x00B98C10 reads g_pEnginePrimitiveRenderer +0x04).
    // The including row defines the body.
    static bool __fastcall GetBlockOnLoad();
#endif                                         // ?SetEnable@CEnginePrimitiveManager@@QAEX_N@Z
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
protected:
    CArray<CEngineScene*>& SceneList;
private:
#else
    CArray<CEngineScene*>* SceneList;
#endif                                    // +0x04
    bool                   Enable;                                       // +0x08
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveManager) == 0xC);

#endif // FABLE_ENGINE_CENGINEPRIMITIVEMANAGER_H
