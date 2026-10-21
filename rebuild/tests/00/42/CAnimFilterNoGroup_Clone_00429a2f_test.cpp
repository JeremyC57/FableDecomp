// behaviour test for 0x00429A2F
#include <stdio.h>
class CAnimFilterBase {
public:
    virtual ~CAnimFilterBase() {}
    virtual CAnimFilterBase* Clone() const = 0;
    virtual int Kind() const { return 0; }
};
class CAnimFilterNoGroup : public CAnimFilterBase {
public:
    virtual CAnimFilterBase* Clone() const;
    virtual int Kind() const { return 7; }
};

CAnimFilterBase* CAnimFilterNoGroup::Clone() const
{
    return new CAnimFilterNoGroup;
}
int main()
{
    CAnimFilterNoGroup f; CAnimFilterBase* c = f.Clone(); int ok = 1;
    ok &= c != 0 && c != &f && c->Kind() == 7 && sizeof(CAnimFilterNoGroup) == 4;
    delete c;
    printf(ok ? "00429a2f_TEST PASS\n" : "00429a2f_TEST FAIL\n");
    return ok ? 0 : 1;
}