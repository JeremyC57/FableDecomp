// Behaviour test for CPersistContext::Transfer<CFireEffectCreationSet> (0x004E59E1): CheckCRC(""), a default traits value
// is built, mode 2 reads value.ContainmentMap from PLoadStream, mode 3 writes it to PSaveStream, any other mode does
// neither; the traits value is always destroyed (map, then base).
#include <cstdio>
#include <cstring>

static char g_seq[32] = "";
static const void* g_stream = 0;
static const void* g_map = 0;
static const char* g_name = 0;

class CMemoryDataInputStream;
class CDataOutputStream;
class CDefString;
template <class T> class CCompareDefString;
namespace std {
template <class T> class allocator;
template <class T1, class T2> struct pair;
template <class K, class V, class C, class A> class map {
public:
    unsigned char _body[12];
    ~map();
};
}
typedef std::map<CDefString, long, CCompareDefString<long>, std::allocator<std::pair<const CDefString, long> > >
    TContainmentMap;
template <> TContainmentMap::~map() { std::strcat(g_seq, "m"); }

template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryIn(CMemoryDataInputStream& stream,
    std::map<K, V, C, std::allocator<std::pair<const K, V> > >& value);
template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryOut(CDataOutputStream& stream,
    const std::map<K, V, C, std::allocator<std::pair<const K, V> > >& value);
template <> void __fastcall GFSerialiseMapBinaryIn<long, CDefString, CCompareDefString<long> >(CMemoryDataInputStream& s,
    TContainmentMap& v) { std::strcat(g_seq, "I"); g_stream = &s; g_map = &v; }
template <> void __fastcall GFSerialiseMapBinaryOut<long, CDefString, CCompareDefString<long> >(CDataOutputStream& s,
    const TContainmentMap& v) { std::strcat(g_seq, "O"); g_stream = &s; g_map = &v; }

class CInitBaseClass {
public:
    unsigned char _body[8];
    ~CInitBaseClass();
};
CInitBaseClass::~CInitBaseClass() { std::strcat(g_seq, "b"); }

class CFireEffectCreationSet : public CInitBaseClass {
public:
    TContainmentMap ContainmentMap;
    CFireEffectCreationSet();
};
CFireEffectCreationSet::CFireEffectCreationSet() { std::strcat(g_seq, "c"); }

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
    protected: void CheckCRC(const char* name); public:
    template <class T> void Transfer(T& value);
};
void CPersistContext::CheckCRC(const char* name) { std::strcat(g_seq, "N"); g_name = name; }

int main()
{
    static unsigned char raw[sizeof(CFireEffectCreationSet)];
    CFireEffectCreationSet& value = *(CFireEffectCreationSet*)raw;
    int loadStream = 1, saveStream = 2;
    CPersistContext c;
    std::memset(&c, 0, sizeof c);
    c.PLoadStream = (CMemoryDataInputStream*)&loadStream;
    c.PSaveStream = (CDataOutputStream*)&saveStream;
    c.Mode = CPersistContext::MODE_BINARY_LOAD;
    c.Transfer(value);
    if (std::strcmp(g_seq, "NcImb") || g_stream != &loadStream || g_map != &value.ContainmentMap || !g_name || g_name[0]) {
        std::printf("004e59e1_TEST FAIL load %s\n", g_seq); return 1;
    }
    g_seq[0] = 0; c.Mode = CPersistContext::MODE_BINARY_SAVE; c.Transfer(value);
    if (std::strcmp(g_seq, "NcOmb") || g_stream != &saveStream || g_map != &value.ContainmentMap) {
        std::printf("004e59e1_TEST FAIL save %s\n", g_seq); return 1;
    }
    g_seq[0] = 0; c.Mode = CPersistContext::MODE_1; c.Transfer(value);
    if (std::strcmp(g_seq, "Ncmb")) { std::printf("004e59e1_TEST FAIL other %s\n", g_seq); return 1; }
    std::printf("004e59e1_TEST PASS\n");
    return 0;
}
