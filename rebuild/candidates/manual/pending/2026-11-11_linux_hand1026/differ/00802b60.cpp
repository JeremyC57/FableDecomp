// GetTargetingRangeForTarget(const CThing& target, const CThing& targeter, const CThing* carried) -- how far away the targeter may lock on to this target: the target's own targeting def gives the range for its targeting flags (plus the target's AI scratchpad offset); a scripted-hook target without one borrows the creature base template's targeting def (type 1); otherwise 0
// Grade: DIFFER(410v424) -- registers/frame only: def lives in esi (retail ebx) and targeter in ebp (retail homes it to [esp+0x10], frame 4 v 8); block structure, calls and tails identical; c2prio gives def priority 37 > target 28 under every spelling tried
// Leaf: ?GetTargetingRangeForTarget@?A0x
// Retail: Fable.exe (TLC, VC7.1) @ 0x00802B60, 424 bytes (+8 int3 to 0x00802D10), __fastcall (target in ecx, targeter in edx,
//   carried on the stack), `ret 4`, float on the x87 stack.
// Identity (proven, two sources): Ego_r.pdb 0x007FDB70 / FableWin.pdb ?GetTargetingRangeForTarget@?A0x58ed6966@@YIMABVCThing@@0PBV2@@Z,
//   the file-static of the player-targeting TU (Ego_r donor order GetTargetingRange 0x007FD880, GetTargetingRangeForTargetType
//   0x007FD8C0, GetTargetingRangeForTarget 0x007FDB70 = retail 0x00802890 / 0x008028D0 / 0x00802B60); its only caller is
//   0x00809132 (the TU's thing filter); arity: two registers + one stack word = `ret 4`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound const 0x004365B0 (the inline PeekTC of TCI_TARGETED 9 and
//   TCI_AI_SCRATCHPAD 170), GetTargetingRangeForTargetType 0x008028D0, CMainGameComponent::PeekDefinitionManager 0x00415FBC,
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("CREATURE_BASE_TEMPLATE" 0x012722F4),
//   CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, CDefinitionManager::GetDef<CThingCreatureDef> 0x00513160,
//   CDefClassBase::GetSubDef<CTargetingDef> 0x007EA960, GetTargetingRange 0x00802890 (type 1); the defs' slot 1 on release.
//   Inline: FableWin.pdb ?GetPTargetingDef@CTCTargeted@@QBE?AV?$CDefPointer@$$CBVCTargetingDef@@@@XZ, ?GetTargetingFlags@CTCTargeted@@QBEKXZ,
//   ?GetTargetingDistanceOffset@CTCAIScratchpad@@QBEMXZ (+0x7C), PeekTC<const CTCTargeted> / PeekTC<const CTCAIScratchpad>.
// Header overlay (FABLE_HDR_OVERLAY=$A/hdr): engine/CTargetingDef.h (new), FABLE_RETAIL_CTCTARGETED_PTARGETINGDEF,
//   FABLE_RETAIL_CTCAISCRATCHPAD_TARGETINGOFFSET.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTCTARGETED
#define FABLE_RETAIL_CTCTARGETED_PTARGETINGDEF
#define FABLE_RETAIL_CTCAISCRATCHPAD_TARGETINGOFFSET
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CTCTargeted.h"
#include "engine/CTCAIScratchpad.h"
#include "engine/CTargetingDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMainGameComponent.h"

namespace {
float __fastcall GetTargetingRange(const CTargetingDef& def, ETargetingType type);   // 0x00802890
float __fastcall GetTargetingRangeForTargetType(unsigned long type, const CTargetingDef& def, const CThing& targeter,
                                                const CThing* carried);             // 0x008028D0

float __fastcall GetTargetingRangeForTarget(const CThing& target, const CThing& targeter, const CThing* carried)
{
    float range;
    const CTCTargeted* targeted;
    target.PeekTC(&targeted);
    CDefPointer<const CTargetingDef> def = targeted->GetPTargetingDef();
    if (!def.IsNull()) {
        range = GetTargetingRangeForTargetType(targeted->GetTargetingFlags(), *def, targeter, carried);
        const CTCAIScratchpad* scratchpad;
        if (target.PeekTC(&scratchpad))
            range += scratchpad->GetTargetingDistanceOffset();
    } else {
        if (!target.HasTC(TCI_ACTION_USE_SCRIPTED_HOOK))
            return 0.0f;
        const CGameDefinitionManager& definitions = CMainGameComponent::Get()->PeekDefinitionManager();
        long index = definitions.GetDefGlobalIndexFromName("CREATURE_BASE_TEMPLATE");
        if (index <= 0)
            return 0.0f;
        CDefPointer<const CThingCreatureDef> creature;
        definitions.GetDef(index, &creature);
        if (creature.IsNull())
            return 0.0f;
        CDefPointer<const CTargetingDef> targeting;
        if (creature->GetSubDef(&targeting))
            range = GetTargetingRange(*targeting, (ETargetingType)1);
        else
            return 0.0f;
    }
    return range;
}
}

float __fastcall GetTargetingRangeForTarget_Use(const CThing& target, const CThing& targeter, const CThing* carried)
{
    return GetTargetingRangeForTarget(target, targeter, carried);
}
