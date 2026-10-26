#include <cstdio>
#include <new>
static int g_baseCopies = 0;
class CDefClassBase { public:
    unsigned char _body[0x21];
    CDefClassBase() { for (int i = 0; i < 0x21; ++i) _body[i] = (unsigned char)i; }
    CDefClassBase(const CDefClassBase& other) { for (int i = 0; i < 0x21; ++i) _body[i] = other._body[i]; ++g_baseCopies; }
    virtual ~CDefClassBase() { }
    virtual int Kind() const { return 1; }
};
static int g_vecCopies = 0;
struct CByteVector {
    unsigned char* m_first; unsigned char* m_last; unsigned char* m_end;
    CByteVector() : m_first(0), m_last(0), m_end(0) { }
    CByteVector(const CByteVector& o) : m_first(o.m_first), m_last(o.m_last), m_end(o.m_end) { ++g_vecCopies; }
};
struct CDefWithByteVector : CDefClassBase {
    CByteVector m_data;
    virtual int Kind() const { return 7; }
};
void Construct_DefWithByteVector_00d34ec0(CDefWithByteVector* p, const CDefWithByteVector& value)
{
    if (p) new (p) CDefWithByteVector(value);   // VC null-tests the placement result itself
}
int main()
{
    unsigned char bytes[4] = { 1, 2, 3, 4 };
    CDefWithByteVector src;
    src.m_data.m_first = bytes; src.m_data.m_last = bytes + 4; src.m_data.m_end = bytes + 4;
    static double storage[16];
    CDefWithByteVector* dst = (CDefWithByteVector*)(void*)storage;
    Construct_DefWithByteVector_00d34ec0(dst, src);
    if (g_baseCopies != 1 || g_vecCopies != 1 || dst->Kind() != 7 || dst->m_data.m_first != bytes || dst->m_data.m_last != bytes + 4
        || dst->_body[0x20] != 0x20) { std::printf("00d34ec0_TEST FAIL\n"); return 1; }
    Construct_DefWithByteVector_00d34ec0(0, src);
    if (g_baseCopies != 1 || g_vecCopies != 1) { std::printf("00d34ec0_TEST FAIL\n"); return 1; }
    std::printf("00d34ec0_TEST PASS\n");
    return 0;
}