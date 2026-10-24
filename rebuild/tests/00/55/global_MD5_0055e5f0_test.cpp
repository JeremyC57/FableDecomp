// Behaviour test for 0055e5f0: a copy of the source's model and logic with its out-of-line callees stubbed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_fail = 0;
static void Check(bool ok, int line)
{
    if (!ok) {
        g_fail = 1;
        std::printf("check failed at line %d\n", line);
    }
}
#define CHECK(c) Check((c) ? true : false, __LINE__)

typedef unsigned char md5_byte_t;
typedef struct md5_state_s {
    unsigned int count[2];
    unsigned int abcd[4];
    md5_byte_t buf[64];
} md5_state_t;
static const char* g_appended = 0;
static int g_appendedLength = -1, g_inits = 0;
static void md5_init(md5_state_t* pms) { ++g_inits; pms->count[0] = 0; }                // stubs for the md5.c entry points
static void md5_append(md5_state_t* pms, const md5_byte_t* data, int nbytes)
{
    g_appended = (const char*)data;
    g_appendedLength = nbytes;
    pms->count[0] += nbytes;
}
static void md5_finish(md5_state_t* pms, md5_byte_t digest[16])                         // a recognisable fake digest
{
    for (int i = 0; i < 16; ++i)
        digest[i] = (md5_byte_t)(i * 17 + pms->count[0]);
}
struct CCharStringData {
    const char* m_pData;
    long m_length;
};
class CCharString {
public:
    CCharStringData* m_pData;
    char Buffer[64];
    CCharStringData Data;
    CCharString() : m_pData(0) { Buffer[0] = 0; }
    CCharString(const char* s) : m_pData(&Data) { std::strcpy(Buffer, s); Data.m_pData = Buffer; Data.m_length = (long)std::strlen(s); }
    CCharString(const CCharString& o) : m_pData(0)
    {
        Buffer[0] = 0;
        if (o.m_pData) { std::strcpy(Buffer, o.m_pData->m_pData); Data.m_pData = Buffer; Data.m_length = o.m_pData->m_length; m_pData = &Data; }
    }
    ~CCharString() { }
    const CCharString& operator+=(const char* text)
    {
        if (!m_pData) { Buffer[0] = 0; Data.m_pData = Buffer; Data.m_length = 0; m_pData = &Data; }
        std::strcat(Buffer, text);
        Data.m_length = (long)std::strlen(Buffer);
        return *this;
    }
    long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetString() const { return m_pData == 0 ? "" : m_pData->m_pData; }
};
#ifdef _MSC_VER
#define snprintf _snprintf
#endif
static CCharString MD5(const CCharString& text)
{
    CCharString result;
    md5_state_t state;
    md5_init(&state);
    md5_append(&state, (const md5_byte_t*)text.GetString(), text.GetLength());
    md5_byte_t digest[16];
    md5_finish(&state, digest);
    for (int i = 0; i < 16; ++i) {
        char hex[3];
        snprintf(hex, 3, "%02x", digest[i]);
        hex[2] = 0;
        result += hex;
    }
    return result;
}
static void RunTest()
{
    CCharString password("secret");
    CCharString h = MD5(password);
    CHECK(g_inits == 1 && g_appendedLength == 6 && std::strcmp(g_appended, "secret") == 0);
    CHECK(h.GetLength() == 32);
    CHECK(std::strcmp(h.GetString(), "061728394a5b6c7d8e9fb0c1d2e3f405") == 0);
    CCharString empty;                                    // no data block: "" and length 0
    CCharString e = MD5(empty);
    CHECK(g_appendedLength == 0 && g_appended[0] == 0 && std::strcmp(e.GetString(), "00112233445566778899aabbccddeeff") == 0);
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("0055e5f0_TEST FAIL\n");
        return 1;
    }
    std::printf("0055e5f0_TEST PASS\n");
    return 0;
}