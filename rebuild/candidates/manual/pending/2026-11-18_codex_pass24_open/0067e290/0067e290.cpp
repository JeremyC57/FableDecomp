// CDegradableDef::Transfer -- persist degradation stages and sort parsed stages by decreasing health
// Grade: DIFFER(273v273) -- The enum transfer arm exchanges scalar CPU operands from 0x0067E2EB.
// Leaf: ?Transfer@CDegradableDef@@UAEXAAVCPersistContext@@@Z
// Flags: none
// Retail: 0067e290,273 bytes,virtual thiscall.
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

void CDegradableDef::Transfer(CPersistContext& transfer)
{
    transfer.Transfer(Degradable);
    transfer.Transfer(GraphicType);
    transfer.Transfer(InitiallyBlocksNavigation);
    transfer.TransferVector(Degradations);
    if (transfer.IsParsingText()) {
        std::sort(Degradations.begin(), Degradations.end(), CIsHealth1GTHealth2());
        std::for_each(Degradations.begin(), Degradations.end(), CSetEngineGraphicType(GraphicType));
    }
}
