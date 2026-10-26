#include <stdio.h>
#include <stddef.h>
static int g_baseOp, g_vecOp, g_tailOp;
#pragma pack(push, 1)
struct CBase_00431f10 { unsigned char _body[0x25]; CBase_00431f10& operator=(const CBase_00431f10& o) { ++g_baseOp; _body[0] = o._body[0]; return *this; } };
struct CRetailVector_long { long* first; long* last; long* end; CRetailVector_long& operator=(const CRetailVector_long& o) { ++g_vecOp; first = o.first; last = o.last; end = o.end; return *this; } };
struct SAttitudeBlock {
    CRetailVector_long m_attitudes; unsigned char m_pad0c; unsigned char m_flag;
    SAttitudeBlock& operator=(const SAttitudeBlock& other) { m_attitudes = other.m_attitudes; m_flag = other.m_flag; return *this; }
};
struct CSubObject_00471e44 { unsigned char _body[0x0c]; CSubObject_00471e44& operator=(const CSubObject_00471e44& o) { ++g_tailOp; _body[0] = o._body[0]; return *this; } };
class CCardDef : public CBase_00431f10 { public:
    long m_id; SAttitudeBlock m_block; unsigned char m_pad37[5]; CSubObject_00471e44 m_tail;
    CCardDef& operator=(const CCardDef& other);
};
#pragma pack(pop)
CCardDef& CCardDef::operator=(const CCardDef& other)
{
    CBase_00431f10::operator=(other); m_id = other.m_id; m_block = other.m_block; m_tail = other.m_tail; return *this;
}
int main()
{
    CCardDef a, b; long arr[2];
    b._body[0] = 9; b.m_id = 1234; b.m_block.m_attitudes.first = arr; b.m_block.m_attitudes.last = arr + 2; b.m_block.m_attitudes.end = arr + 2; b.m_block.m_pad0c = 0x55; b.m_block.m_flag = 1; b.m_tail._body[0] = 7;
    a._body[0] = 0; a.m_id = 0; a.m_block.m_attitudes.first = 0; a.m_block.m_pad0c = 0x11; a.m_block.m_flag = 0; a.m_tail._body[0] = 0;
    CCardDef& r = (a = b);
    bool ok = &r == &a && g_baseOp == 1 && g_vecOp == 1 && g_tailOp == 1 && a.m_id == 1234 && a.m_block.m_attitudes.last == arr + 2 && a.m_block.m_flag == 1 && a.m_block.m_pad0c == 0x11 && a.m_tail._body[0] == 7
        && offsetof(CCardDef, m_id) == 0x25 && offsetof(CCardDef, m_block) == 0x29 && offsetof(CCardDef, m_tail) == 0x3c;
    if (!ok) { printf("0067c490_TEST FAIL\n"); return 1; }
    printf("0067c490_TEST PASS\n"); return 0;
}