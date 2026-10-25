// Behaviour test for 0x00CD2725 _STL::__destroy_aux<CScriptRecord16*> (virtual destructor per element).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("00cd2725_TEST FAIL\n"); return 1; } } while (0)

static int g_dtorCount = 0;
class CScriptRecord16 {
public:
    long A, B, C;
    CScriptRecord16() : A(1), B(2), C(3) { }
    virtual ~CScriptRecord16() { ++g_dtorCount; }
};

#include <new>
// the function under test: STLport __destroy_aux(first, last, __false_type)
template <class _ForwardIterator>
void destroy_aux_under_test(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        (&*__first)->~CScriptRecord16();
}

int main()
{
    int i;
    void* raw = ::operator new(sizeof(CScriptRecord16) * 5);
    CScriptRecord16* p = (CScriptRecord16*)raw;
    for (i = 0; i < 5; ++i) new (p + i) CScriptRecord16();
    g_dtorCount = 0;
    destroy_aux_under_test(p + 1, p + 4);
    CHECK(g_dtorCount == 3);
    destroy_aux_under_test(p + 4, p + 4);
    CHECK(g_dtorCount == 3);
    destroy_aux_under_test(p, p + 1);
    destroy_aux_under_test(p + 4, p + 5);
    CHECK(g_dtorCount == 5);
    ::operator delete(raw);
    std::printf("00cd2725_TEST PASS\n");
    return 0;
}