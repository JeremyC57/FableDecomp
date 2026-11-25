// Behaviour test for 0x007C65A0: links the row object's ??1?$CDefPointer@$$CBVCSpecialAbilitiesGhostSwordDef@@@@QAE@XZ (declared here, defined only there).
#include <cstdio>
class CDefPointeeBase;
class CSpecialAbilitiesGhostSwordDef;
template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;
    ~CDefPointer();
};
static int g_deletes = 0;
static const void* g_deleted = 0;
struct TestPointee {                       // the pointee layout the row relies on: vptr, then IVRefCount at +4; slot 1 = Delete
    virtual ~TestPointee() { }
    virtual void Delete() const { ++g_deletes; g_deleted = this; }
    unsigned long IVRefCount;
};
static int Run(TestPointee& obj, unsigned long count)
{
    obj.IVRefCount = count;
    CDefPointer<const CSpecialAbilitiesGhostSwordDef> p;
    p.Object = reinterpret_cast<CDefPointeeBase*>(&obj);
    { CDefPointer<const CSpecialAbilitiesGhostSwordDef> q; q.Object = p.Object; p.Object = 0; q.~CDefPointer(); if (q.Object != 0) return 0; }
    return p.Object == 0 ? 1 : 0;
}
int main()
{
    TestPointee last, shared;
    if (!Run(last, 1) || last.IVRefCount != 0 || g_deletes != 1 || g_deleted != &last) return 1;
    if (!Run(shared, 2) || shared.IVRefCount != 1 || g_deletes != 1) return 2;
    CDefPointer<const CSpecialAbilitiesGhostSwordDef> empty;
    empty.Object = 0;
    { CDefPointer<const CSpecialAbilitiesGhostSwordDef> q; q.Object = 0; }
    if (g_deletes != 1) return 3;
    std::printf("AUTO_TINY_007c65a0_TEST PASS\n");
    return 0;
}
