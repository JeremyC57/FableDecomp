// behaviour test for 009c6640: the counted-pointer deleter destroys both wide strings of the record
// (second, then first) and frees it; a null object is ignored.
#include <cstdio>
#include <new>

static long g_order[4];
static long g_destroyed = 0;

class CWideString {
public:
    long m_tag;
    CWideString() : m_tag(0) { }
    ~CWideString() { if (g_destroyed < 4) g_order[g_destroyed] = m_tag; ++g_destroyed; }
};

struct SCountedWideStringPair {
    CWideString m_first;
    CWideString m_second;
    static void DeleteObject(SCountedWideStringPair* object)
    {
        if (object) {
            object->~SCountedWideStringPair();
            operator delete(object);
        }
    }
};

int main()
{
    int ok = 1;
    void* raw = ::operator new(sizeof(SCountedWideStringPair));
    SCountedWideStringPair* object = new (raw) SCountedWideStringPair;
    object->m_first.m_tag = 1;
    object->m_second.m_tag = 2;
    SCountedWideStringPair::DeleteObject(object);
    if (g_destroyed != 2 || g_order[0] != 2 || g_order[1] != 1)
        ok = 0;
    SCountedWideStringPair::DeleteObject(0);
    if (g_destroyed != 2)
        ok = 0;
    if (!ok) {
        std::printf("009c6640_TEST FAIL\n");
        return 1;
    }
    std::printf("009c6640_TEST PASS\n");
    return 0;
}