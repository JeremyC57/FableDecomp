// behaviour test for 00a31f20: __uninitialized_fill_n copy-constructs n copies of a C3DGeneratorObject into
// raw storage and returns first + n (n == 0 constructs nothing and returns first).
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

struct __false_type { __false_type() {} };

static C3DGeneratorObject* uninitialized_fill_n(C3DGeneratorObject* first, unsigned long n, const C3DGeneratorObject& x, const __false_type&)
{
    C3DGeneratorObject* cur = first;
    for (; n > 0; --n, ++cur)
        new ((void*)cur) C3DGeneratorObject(x);
    return cur;
}

int main()
{
    int ok = 1;
    long i = 0;
    C3DGeneratorObject source("gen_rock");
    void* raw = ::operator new(sizeof(C3DGeneratorObject) * 3);
    C3DGeneratorObject* first = (C3DGeneratorObject*)raw;
    C3DGeneratorObject* end = 0;
    FillSource(source);
    end = uninitialized_fill_n(first, 3, source, __false_type());
    if (end != first + 3)
        ok = 0;
    for (i = 0; i < 3; ++i) {
        if (!Same(first[i], source))
            ok = 0;
    }
    if (uninitialized_fill_n(first, 0, source, __false_type()) != first)
        ok = 0;
    for (i = 0; i < 3; ++i)
        first[i].~C3DGeneratorObject();
    ::operator delete(raw);
    if (!ok) {
        std::printf("00a31f20_TEST FAIL\n");
        return 1;
    }
    std::printf("00a31f20_TEST PASS\n");
    return 0;
}