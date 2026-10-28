// Behaviour test for 0055f090: a copy of the source's model and logic with its out-of-line callees stubbed.
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


static int g_cryptCalls = 0;
static char g_lastMode = -1;
static int CryptoMatic(const char* source, char* destination, int length, const char* key, char decrypt)   // stub for 0x0055E2C0
{
    ++g_cryptCalls;
    g_lastMode = decrypt;
    int keyLength = (int)std::strlen(key);
    for (int i = 0; i < length; ++i)
        destination[i] = (char)(source[i] ^ key[i % keyLength]);
    return length - 3;
}
static int EncryptBuffer(const char* source, char* destination, int length, const char* key) { return CryptoMatic(source, destination, length, key, 0); }
static int DecryptBuffer(const char* source, char* destination, int length, const char* key) { return CryptoMatic(source, destination, length, key, 1); }
struct string {                                          // STLport 12-byte string stand-in
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    string(const char* f, const char* l) { Init(f, l); }
    string(const string& s) { Init(s._M_start, s._M_finish); }
    ~string() { if (_M_start) std::free(_M_start); }
    void Init(const char* f, const char* l)              // _M_range_initialize 0x00567D00: copy and NUL-terminate
    {
        int n = (int)(l - f);
        _M_start = (char*)std::malloc(n + 1);
        std::memcpy(_M_start, f, n);
        _M_finish = _M_start + n;
        *_M_finish = 0;
        _M_end_of_storage = _M_finish + 1;
    }
    const char* c_str() const { return _M_start; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
};
static int g_vectorSize = -1;
struct vector {                                          // vector<char>(n, val) 0x00567560
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    vector(unsigned int n, const char& val)
    {
        g_vectorSize = (int)n;
        _M_start = n ? (char*)std::malloc(n) : 0;
        std::memset(_M_start, val, n);
        _M_finish = _M_end_of_storage = _M_start + n;
    }
    ~vector() { if (_M_start) std::free(_M_start); }
    char& operator[](unsigned int n) { return *(_M_start + n); }
};
static string StdEncryptString(const string& text, const string& key)
{
    vector buffer(text.size() + 32, 0);
    int length = EncryptBuffer(text.c_str(), &buffer[0], text.size(), key.c_str());
    string result(&buffer[0], &buffer[0] + length);
    return result;
}
static void RunTest()
{
    const char plain[] = "Hero of Oakvale";
    const char k[] = "LH";
    string text(plain, plain + sizeof(plain) - 1);
    string key(k, k + 2);
    string out = StdEncryptString(text, key);
    CHECK(g_cryptCalls == 1 && g_lastMode == 0 && g_vectorSize == (int)(sizeof(plain) - 1) + 32);
    CHECK(out.size() == sizeof(plain) - 1 - 3);
    for (unsigned int i = 0; i < out.size(); ++i)
        CHECK(out.c_str()[i] == (char)(plain[i] ^ k[i % 2]));
    CHECK(out.c_str()[out.size()] == 0);
    string back = StdEncryptString(out, key);                    // the XOR stand-in is its own inverse
    CHECK(back.size() == sizeof(plain) - 1 - 6 && std::memcmp(back.c_str(), plain, back.size()) == 0);
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("0055f090_TEST FAIL\n");
        return 1;
    }
    std::printf("0055f090_TEST PASS\n");
    return 0;
}