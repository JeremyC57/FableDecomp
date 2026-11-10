#ifndef FABLE_ENGINE_CENGINELOCALDETAILLAYERDEF_H
#define FABLE_ENGINE_CENGINELOCALDETAILLAYERDEF_H
// Retail model (chunk 1015, row 0x004798AB): RTTI .?AVCEngineLocalDetailLayerDef@@ : CParentDefClassBase : CDefClassBase (ghidra_out/rtti_bases.tsv), vtable
// 0x012362E4 (rebuild/manifest/rtti-vtables.tsv: slot 18 Transfer 0x00477cb1, 19 Copy 0x00477c65, 20 GetSizeofClass 0x00477424).
// Members: Ego_r.pdb CEngineLocalDetailLayerDef (96 bytes), retail offsets = Ego_r - 4 after the 0x39-byte retail CParentDefClassBase
//   (STLport vectors 12 bytes); sizeof 0x54 = the element stride of TransferVectorOfSubComponents<CEngineLocalDetailLayerDef> 0x004798AB.
// Out-of-line constructor 0x004799A0 (landed) and destructor 0x0047747C (??1CEngineLocalDetailLayerDef@@UAE@XZ, bound);
//   0x0047995E / 0x00479974 call both for the default element.
// Virtual overrides: Ego_r.pdb / FableWin.pdb ?Transfer@CEngineLocalDetailLayerDef@@UAEXAAVCPersistContext@@@Z, ?Copy@CEngineLocalDetailLayerDef@@UAEXPBVCDefClassBase@@@Z,
//   ?GetSizeofClass@CEngineLocalDetailLayerDef@@UAEKXZ (declarations only). Needs FABLE_RETAIL_DEF_CLASS_BASE (the retail def chain).
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CEngineLocalDetailLayerDef.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
#else
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
class CEngineLocalDetailObjectDef;
class CPersistContext;
class CEngineLocalDetailLayerDef : public CParentDefClassBase {
public:
    CEngineLocalDetailLayerDef();                    // 0x004799A0 (??0CEngineLocalDetailLayerDef@@QAE@XZ)
    virtual ~CEngineLocalDetailLayerDef();              // slot 0 0x00477460 (scalar deleting); ~ 0x0047747C (??1CEngineLocalDetailLayerDef@@UAE@XZ)
    virtual void Transfer(CPersistContext&);                 // slot 18
    virtual void Copy(const CDefClassBase*);                 // slot 19
    virtual unsigned long GetSizeofClass();                  // slot 20
    std::vector<float> SpacingFromLayer;                         // +0x3C
    std::vector<CEngineLocalDetailObjectDef> Objects;                                  // +0x48
};
FABLE_STATIC_ASSERT(sizeof(CEngineLocalDetailLayerDef) == 0x54);
#endif // FABLE_RETAIL_DEF_CLASS_BASE
#endif // FABLE_ENGINE_CENGINELOCALDETAILLAYERDEF_H
