// Behaviour test for 0x004DA655 std::__destroy_aux<CMultiStaticMeshEntryDef*> (virtual destructor per element).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("004da655_TEST FAIL\n"); return 1; } } while (0)

static int g_dtorCount = 0;
class CSubDefClassBase { public: virtual ~CSubDefClassBase() { ++g_dtorCount; } long Mesh; };
class CMultiStaticMeshEntryDef : public CSubDefClassBase {
public:
    CMultiStaticMeshEntryDef() { Mesh = 1; }
    virtual ~CMultiStaticMeshEntryDef() { Mesh = 0; }
};

#include <new>
// the function under test: STLport __destroy_aux(first, last, __false_type)
template <class _ForwardIterator>
void destroy_aux_under_test(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        (&*__first)->~CMultiStaticMeshEntryDef();
}

int main()
{
    int i;
    void* raw = ::operator new(sizeof(CMultiStaticMeshEntryDef) * 5);
    CMultiStaticMeshEntryDef* p = (CMultiStaticMeshEntryDef*)raw;
    for (i = 0; i < 5; ++i) new (p + i) CMultiStaticMeshEntryDef();
    g_dtorCount = 0;
    destroy_aux_under_test(p + 1, p + 4);
    CHECK(g_dtorCount == 3);
    destroy_aux_under_test(p + 4, p + 4);
    CHECK(g_dtorCount == 3);
    destroy_aux_under_test(p, p + 1);
    destroy_aux_under_test(p + 4, p + 5);
    CHECK(g_dtorCount == 5);
    ::operator delete(raw);
    std::printf("004da655_TEST PASS\n");
    return 0;
}