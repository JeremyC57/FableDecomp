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