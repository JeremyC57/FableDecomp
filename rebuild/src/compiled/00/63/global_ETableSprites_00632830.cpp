// GFSerialiseMapBinaryIn<long,NUISystem::ETableSprites> -- read the persisted sprite map.
// Leaf: ??$GFSerialiseMapBinaryIn@JW4ETableSprites@NUISystem@@U?$less@W4ETableSprites@NUISystem@@@std@@@@YIXAAVCMemoryDataInputStream@@AAV?$map@W4ETableSprites@NUISystem@@JU?$less@W4ETableSprites@NUISystem@@@std@@V?$allocator@U?$pair@$$CBW4ETableSprites@NUISystem@@J@std@@@4@@std@@@Z
// Retail: Fable.exe 0x00632830,196 bytes.
// Flags: /d2inlT1
// Inline-budget: /d2inlT1 -- tree clear called as native; visible body expands in004/T0; smallest positive setting.
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTTRAITS_EMPTY_DTOR
#define FABLE_RETAIL_CPERSISTTRAITS_LONG_BINARY_BODY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CUIDEF
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "engine/NUISystem_CUIDef.h"
using NUISystem::ETableSprites;
#include "engine/CPersistTraits.h"

template<class V,class K,class C>
void FABLE_FASTCALL GFSerialiseMapBinaryIn(CMemoryDataInputStream& is,std::map<K,V,C>& val_map)
{
 CPersistTraits<K> key_traits;
 CPersistTraits<V> val_traits;
 val_map.clear();
 long size=is.ReadSLONG();
 for(long c1=0;c1<size;++c1) {
  std::pair<K,V> key_val;
  key_traits.TransferBinaryIn(is,key_val.first);
  val_traits.TransferBinaryIn(is,key_val.second);
  val_map.insert(key_val);
 }
}
template void FABLE_FASTCALL GFSerialiseMapBinaryIn<long,NUISystem::ETableSprites,std::less<NUISystem::ETableSprites> >(CMemoryDataInputStream&,std::map<NUISystem::ETableSprites,long,std::less<NUISystem::ETableSprites> >&);