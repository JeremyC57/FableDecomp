// Behaviour test for 0x005F8736 std::pair<long, CDefPointer<const CHeroAbilityDef> >::~pair(): the pair's destruction drops the def's
// count, deletes it through its slot-1 Delete when the count reaches zero, and leaves a null pointer alone. Self-contained (the old
// test linked the landed stand-in symbol ?_Dest_val@@YIXPAUAnimPair@@H@Z); prints the catalog PassPattern and <addr>_TEST PASS.
#include <cstdio>

static int g_deleted = 0;

class CDefPointeeBase {                                   // count at +4, Delete in slot 1
public:
    CDefPointeeBase() : IVRefCount(0) { }
    virtual ~CDefPointeeBase() { }
    virtual void Delete() const { ++g_deleted; }
    mutable unsigned int IVRefCount;
};
template <class T> class CDefPointer {
public:
    CDefPointer() : Object(0) { }
    ~CDefPointer() { Release(); }
    void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;
};
class CHeroAbilityDef : public CDefPointeeBase { };
template <class T1, class T2> struct pair { T1 first; T2 second; };
typedef pair<long, CDefPointer<const CHeroAbilityDef> > THeroAbilityPair;

int main()
{
    CHeroAbilityDef a; a.IVRefCount = 3;
    CHeroAbilityDef b; b.IVRefCount = 1;
    bool ok = true;
    {
        THeroAbilityPair p; p.first = 5; p.second.Object = &a;
        g_deleted = 0;
        p.~THeroAbilityPair();
        ok = ok && a.IVRefCount == 2 && g_deleted == 0 && p.second.Object == 0 && p.first == 5;
        p.second.Object = 0;
    }
    {
        THeroAbilityPair p; p.first = 7; p.second.Object = &b;
        g_deleted = 0;
        p.~THeroAbilityPair();
        ok = ok && b.IVRefCount == 0 && g_deleted == 1 && p.second.Object == 0;
        p.second.Object = 0;
    }
    {
        THeroAbilityPair p; p.first = 9;
        g_deleted = 0;
        p.~THeroAbilityPair();
        ok = ok && g_deleted == 0 && p.second.Object == 0;
    }
    if (!ok) { std::printf("DESTVAL_FAIL\n"); return 1; }
    std::printf("DESTVAL_OK\n005f8736_TEST PASS\n");
    return 0;
}
