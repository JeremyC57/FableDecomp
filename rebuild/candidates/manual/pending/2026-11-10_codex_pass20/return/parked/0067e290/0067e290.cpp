// CDegradableDef::Transfer -- persist degradation stages and sort parsed stages by decreasing health
// Grade: DIFFER(273v273) -- 10 strict instruction hunks; enum EAX/EDX swap begins+5b, then bool temporary rotation and sort operand order; same273bytes/102instructions
// Leaf: ?Transfer@CDegradableDef@@UAEXAAVCPersistContext@@@Z
// Retail: 0067e290,273 bytes,virtual thiscall.
// Flags: none
#define FABLE_RETAIL_CDEGRADABLEDEF_ORIGINAL_FUNCTORS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTCONTEXT_ISPARSINGTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BINARY_BODY
#define FABLE_RETAIL_CPERSISTTRAITS_BOOL_CLASS
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_STLP_FOR_EACH_BODY
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "engine/CDegradableDef.h"
#include "engine/CPersistContext.h"

void CDegradableDef::Transfer(CPersistContext& context)
{
    context.Transfer(Degradable);
    context.Transfer(GraphicType);
    context.Transfer(InitiallyBlocksNavigation);
    context.TransferVector(Degradations);
    if (context.IsParsingText()) {
        std::sort(Degradations.begin(), Degradations.end(), CIsHealth1GTHealth2());
        std::for_each(Degradations.begin(), Degradations.end(), CSetEngineGraphicType(GraphicType));
    }
}
