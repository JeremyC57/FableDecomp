#define FABLE_RETAIL_STLP_CHAR_LONG_ERASE_OWNER_PASS26
#define FABLE_RETAIL_STLP_TREE_MERASE_NOINLINE
// Leaf: ?OpenStaticMapsUsingCommonRetailHeader@CEngineWorldMap@@AAEXJ@Z
// Flags: (none)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLD
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CENGINEWORLDMAP_OPENSTATICMAPS
#define FABLE_RETAIL_CENGINEWORLDMAP_STATICMAPFILE
#define FABLE_RETAIL_CSTATICMAPBANKFILE_CLASS
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE
#define FABLE_RETAIL_CBANKFILE_READENTRYDATA
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_ORIGINAL_READ_SCOPE
#define FABLE_RETAIL_CENGINEMAP_OPENRETAILSTATICMAP_PASS26
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CBankFileAsync.h"
#include "engine/CStaticMapBankFile.h"
#include "engine/CEngineMap.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CMemoryDataInputStream.h"

#include <cstdio>
#include <cstring>
static int errors=0,reads=0,opens=0,lookups=0,stream_dtors=0;static unsigned long last_entry=0;
#define CHECK(x) do{if(!(x)){++errors;std::printf("FAIL %d\n",__LINE__);}}while(0)
struct FixtureString {char value[32];FixtureString(const char*s=""){CHECK(std::strlen(s)<32);std::strcpy(value,s);}bool operator==(const FixtureString&s)const{return std::strcmp(value,s.value)==0;}};
namespace std {template<class K,class V> struct FixtureMap {pair<K,V> entries[8];int count;FixtureMap():count(0){}void insert(const pair<K,V>&v){for(int i=0;i<count;++i)if(entries[i].first==v.first)return;CHECK(count<8);entries[count++]=v;}long lookup(const char*s)const{for(int i=0;i<count;++i)if(entries[i].first==FixtureString(s))return entries[i].second;return-1;}};}
struct FixtureBank {unsigned char bytes[256];unsigned size;unsigned long GetEntryDataSize(unsigned long e)const{last_entry=e;++lookups;return size;}void ReadEntryData(unsigned long e,unsigned long offset,void*out,unsigned long n){CHECK(e==last_entry&&offset==0&&n==size);++reads;std::memcpy(out,bytes,n);}};
struct FixtureStream {const unsigned char*data;unsigned size,pos;FixtureStream(const void*p,unsigned long n):data((const unsigned char*)p),size(n),pos(0){}~FixtureStream(){++stream_dtors;}long ReadSLONG(){CHECK(pos+4<=size);long v;std::memcpy(&v,data+pos,4);pos+=4;return v;}FixtureString ReadNullTerminatedString(){unsigned at=pos;while(pos<size&&data[pos])++pos;CHECK(pos<size);++pos;return FixtureString((const char*)data+at);}};
struct FixtureMapOwner {int calls;const char*name;long expected;unsigned expected_pos;FixtureMapOwner(const char*n,long e):calls(0),name(n),expected(e),expected_pos(0){}void OpenRetailStaticMap(FixtureBank&bank,FixtureStream&stream,std::FixtureMap<FixtureString,long>&m){++calls;++opens;CHECK(m.lookup(name)==expected&&stream.pos==expected_pos&&stream.size==bank.size);}};
struct FixtureWorld {FixtureBank StaticMapFile;CArray<FixtureMapOwner*>MapList;void OpenStaticMapsUsingCommonRetailHeader(long);};
#define CEngineWorldMap FixtureWorld
#define CCharString FixtureString
#define CMemoryDataInputStream FixtureStream
#define map FixtureMap
void CEngineWorldMap::OpenStaticMapsUsingCommonRetailHeader(long header_entry)
{
    long data_size = StaticMapFile.GetEntryDataSize(header_entry);
    CArray<unsigned char> data(data_size);
    StaticMapFile.ReadEntryData(header_entry, 0, data.GetAsCArray(), data_size);
    CMemoryDataInputStream stream(data.GetAsCArray(), data.size());
    std::map<CCharString,long> stream_pos_lookup_map;
    long map_count = stream.ReadSLONG();
    for(long mp=0;mp<map_count;++mp)
    {
        CCharString name = stream.ReadNullTerminatedString();
        long pos = stream.ReadSLONG();
        stream_pos_lookup_map.insert(std::pair<CCharString,long>(name,pos));
    }
    for(long n=0;n<MapList.size();++n)
        if(MapList[n])
            MapList[n]->OpenRetailStaticMap(StaticMapFile,stream,stream_pos_lookup_map);
}

#undef CEngineWorldMap
#undef CCharString
#undef CMemoryDataInputStream
#undef map
static void AddLong(FixtureBank&b,long n){std::memcpy(b.bytes+b.size,&n,4);b.size+=4;}
static void AddName(FixtureBank&b,const char*s,long n){unsigned z=(unsigned)std::strlen(s)+1;std::memcpy(b.bytes+b.size,s,z);b.size+=z;AddLong(b,n);}
int main(){for(int mode=0;mode<6;++mode){FixtureWorld w;w.StaticMapFile.size=0;long count=mode==0?-1:mode==1?0:mode==2?1:3;AddLong(w.StaticMapFile,count);if(count>0){AddName(w.StaticMapFile,"a",17);if(count>1){AddName(w.StaticMapFile,"b",-31);AddName(w.StaticMapFile,mode==3?"a":"c",99);}}FixtureMapOwner a("a",count>0?17:-1),b("b",count>1?-31:-1);a.expected_pos=b.expected_pos=w.StaticMapFile.size;if(mode!=5){w.MapList.push_back(&a);w.MapList.push_back(0);w.MapList.push_back(&b);if(mode==4)w.MapList.push_back(&a);}reads=opens=lookups=stream_dtors=0;w.OpenStaticMapsUsingCommonRetailHeader(19+mode);CHECK(reads==1&&lookups==1&&last_entry==(unsigned long)(19+mode)&&stream_dtors==1);CHECK(a.calls==(mode==5?0:mode==4?2:1)&&b.calls==(mode==5?0:1)&&opens==a.calls+b.calls);}std::puts(errors?"00b420f0_TEST FAIL":"00b420f0_TEST PASS");return errors?1:0;}