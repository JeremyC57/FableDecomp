// Behaviour test for CBaseClass::~CBaseClass (0x0099A2E0): a derived object's destruction ends with the CBaseClass vtable
// re-installed (the base destructor's only work), so a virtual call made from the base destructor sees CBaseClass.
#include <stdio.h>
#include <new>
static int g_seen = 0;
class CBaseClass {
public:
    CBaseClass() {}
    virtual ~CBaseClass() { g_seen = Id(); }
    virtual int Id() const { return 1; }
};
class CDerived : public CBaseClass {
public:
    virtual int Id() const { return 2; }
};
int main()
{
    void* storage[4];
    CDerived* d = new (storage) CDerived;
    if (d->Id() != 2) { printf("FABLETLC_CBASE_RESTORE_A_BEHAVIOR FAIL code=1\n"); return 1; }
    d->~CDerived();
    if (g_seen != 1) { printf("FABLETLC_CBASE_RESTORE_A_BEHAVIOR FAIL code=2\n"); return 1; }
    printf("FABLETLC_CBASE_RESTORE_A_BEHAVIOR PASS\n");
    return 0;
}
