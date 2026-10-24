// behaviour test for 00a31e70: placement-copy-construct a C3DGeneratorObject into raw storage: the 52-byte
// transform block copied word for word, the name deep-copied, bank index and flag copied.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>

class CCharString {
public:
    char* PStringData;
    explicit CCharString(const char* text) { PStringData = (char*)std::malloc(std::strlen(text) + 1); std::strcpy(PStringData, text); }
    CCharString(const CCharString& other) { PStringData = (char*)std::malloc(std::strlen(other.PStringData) + 1); std::strcpy(PStringData, other.PStringData); }
    ~CCharString() { std::free(PStringData); }
};

struct SGeneratorTransform {
    long m_words[13];
};

struct C3DGeneratorObject {
    SGeneratorTransform Transform;
    CCharString         ObjectName;
    unsigned long       BankIndex;
    bool                UseLocalOrigin;
    explicit C3DGeneratorObject(const char* name) : ObjectName(name), BankIndex(0), UseLocalOrigin(false) { std::memset(&Transform, 0, sizeof(Transform)); }
};

static int Same(const C3DGeneratorObject& a, const C3DGeneratorObject& b)
{
    long i = 0;
    for (i = 0; i < 13; ++i) {
        if (a.Transform.m_words[i] != b.Transform.m_words[i])
            return 0;
    }
    if (std::strcmp(a.ObjectName.PStringData, b.ObjectName.PStringData) != 0 || a.ObjectName.PStringData == b.ObjectName.PStringData)
        return 0;
    return a.BankIndex == b.BankIndex && a.UseLocalOrigin == b.UseLocalOrigin;
}

static void FillSource(C3DGeneratorObject& source)
{
    long i = 0;
    for (i = 0; i < 13; ++i)
        source.Transform.m_words[i] = i * i - 5;
    source.BankIndex = 321;
    source.UseLocalOrigin = true;
}

static void Cons_val(C3DGeneratorObject* ptr, const C3DGeneratorObject& value)
{
    new ((void*)ptr) C3DGeneratorObject(value);
}

int main()
{
    int ok = 1;
    C3DGeneratorObject source("gen_tree");
    void* raw = ::operator new(sizeof(C3DGeneratorObject));
    C3DGeneratorObject* dest = (C3DGeneratorObject*)raw;
    FillSource(source);
    Cons_val(dest, source);
    if (!Same(*dest, source))
        ok = 0;
    dest->~C3DGeneratorObject();
    ::operator delete(raw);
    if (!ok) {
        std::printf("00a31e70_TEST FAIL\n");
        return 1;
    }
    std::printf("00a31e70_TEST PASS\n");
    return 0;
}