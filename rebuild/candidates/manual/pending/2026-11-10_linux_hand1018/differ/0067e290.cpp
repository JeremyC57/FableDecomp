// CDegradableDef::Transfer(CPersistContext& context) -- persist Degradable, GraphicType, InitiallyBlocksNavigation and the Degradations vector; after a text parse the degradations are sorted by health and each one's engine graphic takes the def's graphic type
// Grade: DIFFER(273v273, oracle cut to 273) -- expression-temporary rotation only: c2prio --rotation gives the GraphicType arms (line 36) edx/eax where retail has eax (save value) / edx (load stream), and the InitiallyBlocksNavigation arms and the sort comparator push follow one register behind; retail behaves as if the enum save value took eax by hint (one rotation pick at line 36, not two). Stream accessors and a complete enum leave the rotation unchanged
// Leaf: ?Transfer@CDegradableDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0067E290, 273 bytes, virtual thiscall `ret 4` (CDegradableDef vtable slot 18, rtti-vtables.tsv); speed TU.
//   Split: the catalog row (592 bytes) runs on past the `ret 4` at 0x0067E39E into the int3 padding and the next bodies (SPLITS.tsv).
// Identity (proven): Ego_r.pdb / FableWin.pdb ?Transfer@CDegradableDef@@UAEXAAVCPersistContext@@@Z; CDegradableDef's vtable slot 18 is
//   0x0067E290; members per Ego_r.pdb CDegradableDef (Degradable +0x25, GraphicType +0x28, InitiallyBlocksNavigation +0x2C, Degradations +0x30).
// Callees: CPersistContext::CheckCRC 0x00404500 (x4), CPersistTraits<bool>::TransferBinaryIn 0x00403EB0, CDataOutputStream::WriteEBOOL 0x00993E30,
//   CPersistTraits<EEngineGraphicType>::TransferBinaryIn (ICF-folded into the 4-byte reader 0x00632470), CDataOutputStream::WriteSLONG 0x00993EB0,
//   GFSerialiseVectorBinaryIn<CDegradableInfo> 0x0067F430 / GFSerialiseVectorBinaryOut<CDegradableInfo> 0x0067E720 (__fastcall, Ego_r.pdb names),
//   std::sort<CDegradableInfo*, CDegradableDef::CIsHealth1GTHealth2> 0x0067F3D0 (__fastcall: first ecx, last edx, the empty comparator pushed).
// Shape: four inline CPersistContext::Transfer<T>(T&) / TransferVector<T>(vector&) (engine/CPersistContext.h FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BINARY_BODY),
//   then IsParsingText(): sort + for_each(CSetEngineGraphicType) expanded in place (FABLE_RETAIL_STLP_FOR_EACH_BODY).
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
#define FABLE_RETAIL_CPERSISTTRAITS_BOOL_BINARYOUT
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
        CIsHealth1GTHealth2 compare;
        std::sort(Degradations.begin(), Degradations.end(), compare);
        std::for_each(Degradations.begin(), Degradations.end(), CSetEngineGraphicType(GraphicType));
    }
}
