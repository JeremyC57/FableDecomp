// Behaviour test for CPersistContext::TransferMap<CCharString,long> (0x0047280C): CheckCRC("") always, then mode 2 reads
// the map from PLoadStream, mode 3 writes it to PSaveStream, any other mode does nothing.
#include <cstdio>
#include <cstring>
namespace std {
    template <class T> struct less;
    template <class T> class allocator;
    template <class T1, class T2> struct pair;
    template <class K, class V, class C, class A> class map { public: int Words[3]; };
}
class CCharString;
class CMemoryDataInputStream;
class CDataOutputStream;
typedef std::map<long, CCharString, std::less<long>, std::allocator<std::pair<const long, CCharString> > > TMap;

template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryIn(CMemoryDataInputStream& stream,
    std::map<K, V, C, std::allocator<std::pair<const K, V> > >& value);
template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryOut(CDataOutputStream& stream,
    const std::map<K, V, C, std::allocator<std::pair<const K, V> > >& value);

class CPersistContext {
public:
    enum EMode { MODE_0, MODE_1, MODE_BINARY_LOAD, MODE_BINARY_SAVE };
    unsigned char           _pad00[0x18];
    EMode                   Mode;
    bool                    Binary;
    unsigned char           _pad1d[3];
    void*                   PStringToSaveTo;
    CMemoryDataInputStream* PLoadStream;
    CDataOutputStream*      PSaveStream;
    void CheckCRC(const char* name);
    template <class V, class K>
    void TransferMap(std::map<K, V, std::less<K>, std::allocator<std::pair<const K, V> > >& value);
};

static int g_crc = 0, g_in = 0, g_out = 0;
static const void* g_stream = 0;
static const void* g_map = 0;
static const char* g_name = 0;
void CPersistContext::CheckCRC(const char* name) { ++g_crc; g_name = name; }
template <> void __fastcall GFSerialiseMapBinaryIn<CCharString, long, std::less<long> >(CMemoryDataInputStream& s, TMap& v)
{ ++g_in; g_stream = &s; g_map = &v; }
template <> void __fastcall GFSerialiseMapBinaryOut<CCharString, long, std::less<long> >(CDataOutputStream& s, const TMap& v)
{ ++g_out; g_stream = &s; g_map = &v; }

static int run(CPersistContext& c, TMap& m, int mode)
{
    g_crc = g_in = g_out = 0; g_stream = 0; g_map = 0; g_name = 0;
    c.Mode = (CPersistContext::EMode)mode;
    c.TransferMap(m);
    if (g_crc != 1 || g_name == 0) return 0;
    return g_name[0] == 0;
}

int main()
{
    int loadStream = 1, saveStream = 2;
    TMap m;
    const void* mapStorage = &m;
    CPersistContext c;
    std::memset(&c, 0, sizeof c);
    c.PLoadStream = (CMemoryDataInputStream*)&loadStream;
    c.PSaveStream = (CDataOutputStream*)&saveStream;
    if (!run(c, m, 2) || g_in != 1 || g_out != 0 || g_stream != &loadStream || g_map != mapStorage) { std::printf("FAIL load\n"); return 1; }
    if (!run(c, m, 3) || g_in != 0 || g_out != 1 || g_stream != &saveStream || g_map != mapStorage) { std::printf("FAIL save\n"); return 1; }
    for (int mode = 0; mode < 6; ++mode) {
        if (mode == 2 || mode == 3) continue;
        if (!run(c, m, mode) || g_in != 0 || g_out != 0) { std::printf("FAIL mode %d\n", mode); return 1; }
    }
    std::printf("0047280c_TEST PASS\n");
    return 0;
}
