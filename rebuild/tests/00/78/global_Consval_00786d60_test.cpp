// behaviour test for 00786d60: placement-copy-construct a (long, CCharString, long) record into raw
// storage: key and value copied, the name deep-copied.
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

struct SKeyedNamedValue {
    long        m_key;
    CCharString m_name;
    long        m_value;
    SKeyedNamedValue(long k, const char* n, long v) : m_key(k), m_name(n), m_value(v) { }
};

static void Cons_val(SKeyedNamedValue* ptr, const SKeyedNamedValue& value)
{
    new ((void*)ptr) SKeyedNamedValue(value);
}

int main()
{
    int ok = 1;
    SKeyedNamedValue source(-17, "region_a", 88);
    void* raw = ::operator new(sizeof(SKeyedNamedValue));
    SKeyedNamedValue* dest = (SKeyedNamedValue*)raw;
    Cons_val(dest, source);
    if (dest->m_key != -17 || dest->m_value != 88)
        ok = 0;
    if (std::strcmp(dest->m_name.PStringData, "region_a") != 0 || dest->m_name.PStringData == source.m_name.PStringData)
        ok = 0;
    dest->~SKeyedNamedValue();
    ::operator delete(raw);
    if (!ok) {
        std::printf("00786d60_TEST FAIL\n");
        return 1;
    }
    std::printf("00786d60_TEST PASS\n");
    return 0;
}