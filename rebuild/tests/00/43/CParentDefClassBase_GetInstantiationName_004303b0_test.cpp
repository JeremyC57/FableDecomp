#include <stdio.h>
#include <stddef.h>
static int g_copies;
#pragma pack(push, 1)
class CDefString {
public:
    long TablePos;
    CDefString(long p) : TablePos(p) {}
    CDefString(const CDefString& other) : TablePos(other.TablePos) { ++g_copies; }
};
class CDefClassBase {
public:
    virtual ~CDefClassBase() {}
    unsigned char _body[0x21];
};
class CParentDefClassBase : public CDefClassBase {
public:
    CDefString InstantiationName;
    unsigned char SubDefInfoMap[0x14];
    CParentDefClassBase() : InstantiationName(0) {}
    virtual CDefString GetInstantiationName() const;
};
#pragma pack(pop)

CDefString CParentDefClassBase::GetInstantiationName() const
{
    return InstantiationName;
}

int main()
{
    CParentDefClassBase d; d.InstantiationName.TablePos = 1234;
    CDefString n = d.GetInstantiationName();
    bool ok = n.TablePos == 1234 && offsetof(CParentDefClassBase, InstantiationName) == 0x25 && g_copies >= 1;
    d.InstantiationName.TablePos = -1;
    ok = ok && d.GetInstantiationName().TablePos == -1;
    printf(ok ? "004303b0_TEST PASS\n" : "004303b0_TEST FAIL\n");
    return ok ? 0 : 1;
}