// 005933C8 CDefClassBase::GetSubDef<CContextSensitiveItemDef>
// Leaf: ??$GetSubDef@VCContextSensitiveItemDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCContextSensitiveItemDef@@@@@Z
// Flags:
// Ego_r and FableWin identify the const member template. The wall-mount action
// calls this specialization after looking up the trophy object's base definition.
// Inline-budget: 00848350 calls the 162-byte specialization in the shared header.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CONTEXTSENSITIVEITEM_QUERY_PASS28
#define FABLE_RETAIL_CCONTEXTSENSITIVEITEMDEF_CLASS
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CCONTEXTSENSITIVEITEMDEF_SIZE_BODY
#include "fable_string.h"
#include "engine/CContextSensitiveItemDef.h"
#include "engine/CDefClassBase_GetSubDef.h"

// Fidelity: scaffolding, not retail. Reference the shared inline definition so this
// standalone owner supplies the same selectable copy as its callers (NF86/NF88).
namespace {
typedef bool (CDefClassBase::*ContextItemLookup)(CDefPointer<const CContextSensitiveItemDef>*) const;
const ContextItemLookup ContextItemLookupDefinition = &CDefClassBase::GetSubDef<CContextSensitiveItemDef>;
}
