#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef __int64 s64;
typedef unsigned __int64 u64;
#else
typedef long long s64;
typedef unsigned long long u64;
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)

// Recording output stream: every Write* call appends (tag, value).
struct LogEntry { char tag; u64 v; };
struct COut {
    LogEntry e[400];
    int n;
    unsigned char raw[512];
    int rawn;
    COut() : n(0), rawn(0) {}
    void add(char t, u64 v) { e[n].tag = t; e[n].v = v; ++n; }
    void WriteEBOOL(bool v) { add('b', v ? 1 : 0); }
    void WriteLong(long v) { add('l', (u64)(s64)v); }
    void WriteULong(unsigned long v) { add('u', (u64)v); }
    void WriteFloat(float f) { add('f', (u64)(s64)(f * 1000.0f)); }
    void WriteUInt64(u64 v) { add('q', v); }
    void Write(const void* data, unsigned long size) { add('w', size); memcpy(raw + rawn, data, size); rawn += (int)size; }
    bool is(int i, char t, u64 v) const { return i < n && e[i].tag == t && e[i].v == v; }
};

// Memory input stream with the retail inline Read (64-bit range check, chunk fast path, overflow slow path).
struct CIn {
    unsigned long StreamPos;
    long StreamSize;
    const unsigned char* CurrentSourcePtr;
    unsigned long SourceChunkStreamPos;
    long SourceChunkBytesRemaining;
    const unsigned char* base;
    int overflowCalls;
    void Init(const unsigned char* data, long size, long chunk)
    {
        base = data; StreamPos = 0; StreamSize = size; CurrentSourcePtr = data; SourceChunkStreamPos = 0;
        SourceChunkBytesRemaining = chunk; overflowCalls = 0;
    }
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
    {
        ++overflowCalls;
        memcpy(buffer, base + StreamPos, size);
        StreamPos += size;
        CurrentSourcePtr = base + StreamPos;
        SourceChunkBytesRemaining = StreamSize - (long)StreamPos;
    }
    static bool IsValidPosition(s64 position) { return position >= 0 && position <= 0x7fffffff; }
    void Read(void* buffer, long size)
    {
        s64 newPosition = (s64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    long ReadLong() { int v = 0; Read(&v, 4); return v; }
    unsigned long ReadULong() { unsigned int v = 0; Read(&v, 4); return v; }
    float ReadFloat() { float v = 0; Read(&v, 4); return v; }
    unsigned char ReadByte() { unsigned char v = 0; Read(&v, 1); return v; }
};
static void put32(unsigned char* p, int v) { memcpy(p, &v, 4); }
static void putf(unsigned char* p, float v) { memcpy(p, &v, 4); }
// Minimal value-semantics string standing in for CCharString (a null data pointer = the empty string).
struct CStr {
    char* PStringData;
    CStr() : PStringData(0) {}
    CStr(const char* t, long len = -1) : PStringData(0) { set(t, len); }
    CStr(const CStr& o) : PStringData(0) { set(o.c_str(), -1); }
    ~CStr() { delete[] PStringData; }
    void set(const char* t, long len)
    {
        delete[] PStringData; PStringData = 0;
        if (len < 0) len = (long)strlen(t);
        if (len == 0) return;
        PStringData = new char[len + 1]; memcpy(PStringData, t, len); PStringData[len] = 0;
    }
    const char* c_str() const { return PStringData ? PStringData : ""; }
    CStr& operator=(const CStr& o) { if (this != &o) set(o.c_str(), -1); return *this; }
    CStr& operator=(const char* t) { set(t, -1); return *this; }
    CStr& operator+=(const char* t) { char b[512]; strcpy(b, c_str()); strcat(b, t); set(b, -1); return *this; }
    CStr& operator+=(const CStr& o) { return *this += o.c_str(); }
    bool operator==(const char* t) const { return strcmp(c_str(), t) == 0; }
    bool operator!=(const char* t) const { return strcmp(c_str(), t) != 0; }
};
static CStr operator+(const CStr& l, const char* r) { CStr s(l); s += r; return s; }
static CStr operator+(const CStr& l, const CStr& r) { CStr s(l); s += r; return s; }
static CStr operator+(const char* l, const CStr& r) { CStr s(l); s += r; return s; }
static CStr GFIntToCharString(long v) { char b[32]; std::sprintf(b, "%ld", v); return CStr(b); }

struct CStringParser { const char* token; };
struct CDefinitionManager { long GetDefGlobalIndexFromName(const CStr& name) { return strcmp(name.c_str(), "SWORD") == 0 ? 17 : 0; } };
struct CPersistContext {
    CDefinitionManager* PDefinitionManager; CStr lastError;
    void Error(const CStringParser&, const CStr& message) { lastError = message; }
};
struct CStrTraits { CStr DefaultVal; void TransferIn(CPersistContext*, CStringParser* parser, CStr* value) { *value = parser->token; } };
struct CDefIndex { long Val; };
struct Traits { void TransferIn(CPersistContext* context, CStringParser* parser, CDefIndex* value)
{
    CStrTraits traits;
    CStr name;
    traits.TransferIn(context, parser, &name);
    if (name != "" && name != "NULL") {
        CDefinitionManager* manager = context->PDefinitionManager;
        long index = manager->GetDefGlobalIndexFromName(name);
        value->Val = index;
        if (index == 0)
            context->Error(*parser, "Unknown definition symbol " + name);
    }
} };
int main()
{
    CDefinitionManager dm; CPersistContext ctx; ctx.PDefinitionManager = &dm;
    CStringParser p; CDefIndex v; Traits t;
    p.token = "SWORD"; v.Val = -1; t.TransferIn(&ctx, &p, &v); CHECK(v.Val == 17 && ctx.lastError == "");
    p.token = "NULL"; v.Val = -1; t.TransferIn(&ctx, &p, &v); CHECK(v.Val == -1);
    p.token = ""; t.TransferIn(&ctx, &p, &v); CHECK(v.Val == -1);
    p.token = "AXE"; t.TransferIn(&ctx, &p, &v); CHECK(v.Val == 0 && ctx.lastError == "Unknown definition symbol AXE");
    if (g_fail) { std::printf("00a3e0d0_TEST FAIL\n"); return 1; }
    std::printf("00a3e0d0_TEST PASS\n");
    return 0;
}