#ifndef FABLE_ENGINE_COPINIONDEEDREACTIONDEF_H
#define FABLE_ENGINE_COPINIONDEEDREACTIONDEF_H
// Retail model (chunk 1015, row 0x00479466): RTTI .?AVCOpinionDeedReactionDef@@ : CParentDefClassBase : CDefClassBase (ghidra_out/rtti_bases.tsv), vtable
// 0x012361DC (rebuild/manifest/rtti-vtables.tsv: slot 18 Transfer 0x00472095, 19 Copy 0x00472046, 20 GetSizeofClass 0x00471f93).
// Members: Ego_r.pdb COpinionDeedReactionDef (88 bytes), retail offsets = Ego_r - 4 after the 0x39-byte retail CParentDefClassBase
//   (STLport vectors 12 bytes); sizeof 0x50 = the element stride of TransferVectorOfSubComponents<COpinionDeedReactionDef> 0x00479466.
// Out-of-line constructor 0x00479574 (landed ??0COpinionDeedReactionDef@@QAE@XZ) and destructor 0x0047955B (FableWin.pdb
//   ??1COpinionDeedReactionDef@@UAE@XZ, virtual through CParentDefClassBase); 0x00479519 / 0x0047952F call both for the default element.
//   CDefString needs FABLE_RETAIL_CDEFSTRINGTABLE / FABLE_RETAIL_CCHARSTRING (engine/CDefString.h).
// Virtual overrides: Ego_r.pdb / FableWin.pdb ?Transfer@COpinionDeedReactionDef@@UAEXAAVCPersistContext@@@Z, ?Copy@COpinionDeedReactionDef@@UAEXPBVCDefClassBase@@@Z,
//   ?GetSizeofClass@COpinionDeedReactionDef@@UAEKXZ (declarations only). Needs FABLE_RETAIL_DEF_CLASS_BASE (the retail def chain).
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/COpinionDeedReactionDef.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
#else
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
#include "engine/CDefString.h"
#ifdef _MSC_VER
enum EOpinionAttitudeType;                                       // Ego_r.pdb enum (MS forward declaration; enumerators not modelled)
#else
enum EOpinionAttitudeType { };
#endif
class CPersistContext;
class COpinionDeedReactionDef : public CParentDefClassBase {
public:
    COpinionDeedReactionDef();                       // 0x00479574 (??0COpinionDeedReactionDef@@QAE@XZ)
    virtual ~COpinionDeedReactionDef();                 // slot 0 0x0047200d (scalar deleting); ~ 0x0047955B (??1COpinionDeedReactionDef@@UAE@XZ)
    virtual void Transfer(CPersistContext&);                 // slot 18
    virtual void Copy(const CDefClassBase*);                 // slot 19
    virtual unsigned long GetSizeofClass();                  // slot 20
    std::vector<EOpinionAttitudeType> Attitudes;                                // +0x3C
    CDefString    Animation;                                // +0x48
    unsigned long DialogueTag;                              // +0x4C
};
FABLE_STATIC_ASSERT(sizeof(COpinionDeedReactionDef) == 0x50);
#endif // FABLE_RETAIL_DEF_CLASS_BASE
#endif // FABLE_ENGINE_COPINIONDEEDREACTIONDEF_H
