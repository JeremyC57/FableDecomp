// 005D9B90 CTCInventoryItem::GetContextSensitiveType
// Leaf: ?GetContextSensitiveType@CTCInventoryItem@@SI?AW4EContextSensitiveType@@J@Z
// Flags:
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CONTEXTSENSITIVEITEM_QUERY_PASS28
#define FABLE_RETAIL_CCONTEXTSENSITIVEITEMDEF_CLASS
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CCONTEXTSENSITIVEITEMDEF_SIZE_BODY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CACHEDEF
#define FABLE_RETAIL_CTCINVENTORYITEM_PDEF
#define FABLE_RETAIL_INVENTORY_CONTEXT_CACHE_PASS30
#include "fable_string.h"
#include "engine/CContextSensitiveItemDef.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "fable_assert.h"

EContextSensitiveType FABLE_FASTCALL CTCInventoryItem::GetContextSensitiveType(long def_index)
{
    if (CacheDef(def_index)) {
        FABLE_RETAIL_ASSERT(!MostRecentThingDef.IsNull());
        CDefPointer<const CContextSensitiveItemDef> pcontext_def;
        if (MostRecentThingDef->GetSubDef(&pcontext_def))
            return pcontext_def->Type;
    }
    return CONTEXT_SENSITIVE_NULL;
}
