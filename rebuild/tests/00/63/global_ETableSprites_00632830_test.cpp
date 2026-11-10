#include <stdio.h>
#include <map>
#include <vector>
#if defined(_MSC_VER)
#define FABLE_FASTCALL __fastcall
#else
#define FABLE_FASTCALL
#endif
namespace NUISystem {enum ETableSprites {TABLE_SPRITES_TOP_LEFT=0,TABLE_SPRITES_AMOUNT=13};}
using NUISystem::ETableSprites;
static int traits_live,failures,key_reads,value_reads;
class CMemoryDataInputStream {public:std::vector<unsigned char> Data;unsigned long Position;long Remaining;int Overflows;
 CMemoryDataInputStream():Position(0),Remaining(0),Overflows(0){}
 long ReadSLONG(){if(Position+4>Data.size()){++failures;return 0;}if(Remaining<4){++Overflows;Remaining=4;}unsigned int bits=(unsigned int)Data[Position]|((unsigned int)Data[Position+1]<<8)|((unsigned int)Data[Position+2]<<16)|((unsigned int)Data[Position+3]<<24);Position+=4;Remaining-=4;return (long)(int)bits;}
};
template<class T>class CPersistTraits {public:T DefaultVal;CPersistTraits():DefaultVal(){++traits_live;}~CPersistTraits(){--traits_live;}void TransferBinaryIn(CMemoryDataInputStream& is,T& val){++key_reads;if(traits_live!=2)++failures;val=(T)is.ReadSLONG();}};
template<>void CPersistTraits<long>::TransferBinaryIn(CMemoryDataInputStream& is,long& val){++value_reads;if(traits_live!=2)++failures;long value=is.ReadSLONG();val=value;}
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
static void put(CMemoryDataInputStream& is,long v){unsigned int bits=(unsigned int)v;for(int i=0;i<4;++i)is.Data.push_back((unsigned char)(bits>>(8*i)));}
int main(){
 long values[]={-100,0,7,1234567};int cases=0;
 for(int n=0;n<8;++n)for(int pattern=0;pattern<4;++pattern)for(int chunk=0;chunk<69;++chunk){
  CMemoryDataInputStream is;put(is,n);std::map<NUISystem::ETableSprites,long> expected,got;
  for(int i=0;i<n;++i){long key=(i*(pattern+1))%5;long val=values[(i+pattern)%4];put(is,key);put(is,val);expected.insert(std::make_pair((NUISystem::ETableSprites)key,val));}
  got[(NUISystem::ETableSprites)13]=999;got[(NUISystem::ETableSprites)12]=888;is.Remaining=chunk;key_reads=value_reads=0;
  GFSerialiseMapBinaryIn<long,NUISystem::ETableSprites,std::less<NUISystem::ETableSprites> >(is,got);
  if(got!=expected||is.Position!=4UL+8UL*n||key_reads!=n||value_reads!=n||traits_live||failures)return 1;
  ++cases;
 }
 CMemoryDataInputStream negative;put(negative,-1);std::map<NUISystem::ETableSprites,long> got;got[(NUISystem::ETableSprites)1]=10;GFSerialiseMapBinaryIn<long,NUISystem::ETableSprites,std::less<NUISystem::ETableSprites> >(negative,got);if(!got.empty()||negative.Position!=4||traits_live||failures)return 1;
 if(cases!=2208)return 1;
 puts("00632830_TEST PASS");return 0;
}