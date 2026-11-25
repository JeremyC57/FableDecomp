// Behaviour test for 0x0078D950: links the row object's ?Release@?$CDefPointer@$$CBVCSpecialAbilitiesBerserkDef@@@@QAEXXZ (declared here, defined only there).
#include <cstdio>
class CDefPointeeBase;
class CSpecialAbilitiesBerserkDef;
template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;
    void Release();
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
    CDefPointer<const CSpecialAbilitiesBerserkDef> p;
    p.Object = reinterpret_cast<CDefPointeeBase*>(&obj);
    p.Release();
    return p.Object == 0 ? 1 : 0;
}
int main()
{
    TestPointee last, shared;
    if (!Run(last, 1) || last.IVRefCount != 0 || g_deletes != 1 || g_deleted != &last) return 1;
    if (!Run(shared, 2) || shared.IVRefCount != 1 || g_deletes != 1) return 2;
    CDefPointer<const CSpecialAbilitiesBerserkDef> empty;
    empty.Object = 0;
    empty.Release();
    if (g_deletes != 1) return 3;
    std::printf("PASS_0078d950\n");
    return 0;
}
