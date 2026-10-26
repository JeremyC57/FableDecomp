#include <cstdio>
#include <cstring>
#include <cstdlib>
// behaviour test: CQuestManager::LoadGameState reads a length-prefixed text in binary modes (2/3) and parses it,
// otherwise uses the context's parser; boasts are transferred either way
class CCharString { public:
    char m[64];
    CCharString(const char* text, long) { std::strncpy(m, text, 63); m[63] = 0; }
};
namespace N_00411910 { struct CByteArray {
    unsigned char* m_begin; unsigned char* m_end;
    CByteArray(unsigned long n) { m_begin = (unsigned char*)std::calloc(n, 1); m_end = m_begin + n; }
    ~CByteArray() { if (m_begin) std::free(m_begin); }
    unsigned char* GetData() { return m_end - m_begin != 0 ? m_begin : (unsigned char*)this; }
    unsigned char& operator[](long index) { return m_begin[index]; }
}; } using N_00411910::CByteArray;
class CMemoryDataInputStream { public:
    const unsigned char* src; unsigned long StreamPos;
    void ReadBuffer(void* buffer, long size) { std::memcpy(buffer, src + StreamPos, size); StreamPos += size; }
    long ReadLong() { long v = 0; std::memcpy(&v, src + StreamPos, 4); StreamPos += 4; return v; }
};
class CStringParser { public:
    char text[64];
    CStringParser(const CCharString& t, void (*)(CCharString*), const CCharString&, long) { std::strcpy(text, t.m); }
};
class CPersistContext { public: CStringParser* PParser; long Mode; CMemoryDataInputStream* PLoadStream; };
static const char g_emptyString_0122d70e[] = "";
class CQuestManager { public:
    char loaded[64]; int boastTransfers;
    void LoadGameState(CStringParser& parser) { std::strcpy(loaded, parser.text); }
    void TransferBoasts(CPersistContext&) { ++boastTransfers; }
    void LoadGameState(CPersistContext& context);
};
void CQuestManager::LoadGameState(CPersistContext& context)
{
    bool binary = context.Mode == 2 || context.Mode == 3;
    if (binary) {
        CMemoryDataInputStream* stream = context.PLoadStream;
        long length = stream->ReadLong();
        CByteArray buffer(length + 1);
        stream->ReadBuffer(buffer.GetData(), length);
        buffer[length] = 0;
        CCharString text((const char*)buffer.GetData(), -1);
        CStringParser parser(text, 0, CCharString(g_emptyString_0122d70e, -1), 0);
        LoadGameState(parser);
    } else
        LoadGameState(*context.PParser);
    TransferBoasts(context);
}
int main()
{
    unsigned char raw[16] = { 5, 0, 0, 0, 'q', 'u', 'e', 's', 't', 'X', 'X' };
    CMemoryDataInputStream stream; stream.src = raw; stream.StreamPos = 0;
    CQuestManager q; q.boastTransfers = 0; q.loaded[0] = 0;
    CPersistContext ctx; ctx.Mode = 3; ctx.PLoadStream = &stream; ctx.PParser = 0;
    q.LoadGameState(ctx);
    bool ok = std::strcmp(q.loaded, "quest") == 0 && q.boastTransfers == 1 && stream.StreamPos == 9;
    CCharString t("textmode", -1); CCharString e("", -1); CStringParser p(t, 0, e, 0);
    ctx.Mode = 1; ctx.PParser = &p;
    q.LoadGameState(ctx);
    ok = ok && std::strcmp(q.loaded, "textmode") == 0 && q.boastTransfers == 2;
    if (!ok) { std::printf("004b6380_TEST FAIL\n"); return 1; }
    std::printf("004b6380_TEST PASS\n");
    return 0;
}