// Behaviour test for 0x00632B90 GFSerialiseMapBinaryIn<CCharString, unsigned long, less<unsigned long> > (self-contained C++03
// model: clear the map, read the count, then per entry a value-initialised pair read key (ulong) then value (null-terminated
// string) and inserted with map::insert, so a repeated key keeps its first value).
#include <stdio.h>
#include <string.h>
#include <string>
#include <map>

static int g_fail = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); g_fail = 1; } }

class CMemoryDataInputStream {
public:
    const unsigned char* P;
    const unsigned char* End;
    CMemoryDataInputStream(const unsigned char* p, const unsigned char* e) : P(p), End(e) { }
    void Read(void* buffer, long size) { if (P + size <= End) { memcpy(buffer, P, size); P += size; } else memset(buffer, 0, size); }
    long ReadSLONG() { int v; Read(&v, 4); return v; }
    std::string ReadNullTerminatedString() { std::string s; while (P < End && *P) s += (char)*P++; if (P < End) ++P; return s; }
};

static void GFSerialiseMapBinaryIn(CMemoryDataInputStream& is, std::map<unsigned long, std::string>& m)
{
    m.clear();
    long count = is.ReadSLONG();
    for (long i = 0; i < count; ++i) {
        std::pair<unsigned long, std::string> entry;
        unsigned int key; is.Read(&key, 4); entry.first = key;      // CPersistTraits<unsigned long>::TransferBinaryIn 0x0040F8A0
        entry.second = is.ReadNullTerminatedString();               // CPersistTraits<CCharString>::TransferBinaryIn (inline)
        m.insert(entry);
    }
}

static unsigned char* PutU32(unsigned char* p, unsigned int v) { memcpy(p, &v, 4); return p + 4; }
static unsigned char* PutStr(unsigned char* p, const char* s) { size_t n = strlen(s) + 1; memcpy(p, s, n); return p + n; }

int main()
{
    unsigned char buf[64]; unsigned char* p = buf;
    p = PutU32(p, 3);
    p = PutU32(p, 7); p = PutStr(p, "seven");
    p = PutU32(p, 2); p = PutStr(p, "two");
    p = PutU32(p, 7); p = PutStr(p, "again");
    std::map<unsigned long, std::string> m; m[99] = "stale";
    CMemoryDataInputStream in(buf, p);
    GFSerialiseMapBinaryIn(in, m);
    Check(m.size() == 2, "clears, then inserts unique keys");
    Check(m.find(99) == m.end(), "old entries are cleared");
    Check(m[7] == "seven" && m[2] == "two", "a repeated key keeps its first value");
    Check(in.P == p, "consumes every record");
    unsigned char z[4]; PutU32(z, 0);
    CMemoryDataInputStream in2(z, z + 4);
    GFSerialiseMapBinaryIn(in2, m);
    Check(m.empty(), "a zero count leaves the map empty");
    if (g_fail) return 1;
    printf("00632b90_TEST PASS\n");
    return 0;
}