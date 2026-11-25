// Behaviour test for 0x007C7F80: links the row object's ??1?$CDefPointer@$$CBVCTurncoatDef@@@@QAE@XZ (declared here, defined only there).
#include <cstdio>
class CDefPointeeBase;
class CTurncoatDef;
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
    CDefPointer<const CTurncoatDef> p;
    p.Object = reinterpret_cast<CDefPointeeBase*>(&obj);
    { CDefPointer<const CTurncoatDef> q; q.Object = p.Object; p.Object = 0; q.~CDefPointer(); if (q.Object != 0) return 0; }
    return p.Object == 0 ? 1 : 0;
}
int main()
{
    TestPointee last, shared;
    if (!Run(last, 1) || last.IVRefCount != 0 || g_deletes != 1 || g_deleted != &last) return 1;
    if (!Run(shared, 2) || shared.IVRefCount != 1 || g_deletes != 1) return 2;
    CDefPointer<const CTurncoatDef> empty;
    empty.Object = 0;
    { CDefPointer<const CTurncoatDef> q; q.Object = 0; }
    if (g_deletes != 1) return 3;
    std::printf("PASS_007c7f80\n");
    return 0;
}
