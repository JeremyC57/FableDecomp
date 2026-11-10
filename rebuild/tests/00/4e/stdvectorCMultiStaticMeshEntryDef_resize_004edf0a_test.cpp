// Behaviour test for 0x004EDF0A std::vector<CMultiStaticMeshEntryDef>::resize(unsigned int): STLport's one-argument resize forwards to
// resize(n, CMultiStaticMeshEntryDef()) -- one default temporary is built, passed by reference, then destroyed; the vector grows with
// default-valued copies and shrinks by erasing the tail.
#include <stdio.h>
#include <vector>
static long g_ctor = 0, g_dtor = 0, g_twoArgCalls = 0;
static bool g_valueOk = true;
class CMultiStaticMeshEntryDef {                                          // element model: the retail def is 0x38 bytes
public:
    long Value;
    CMultiStaticMeshEntryDef() : Value(0) { ++g_ctor; }
    CMultiStaticMeshEntryDef(const CMultiStaticMeshEntryDef& other) : Value(other.Value) { }
    virtual ~CMultiStaticMeshEntryDef() { ++g_dtor; }
};
template <class _Tp> class StlportVectorModel {     // the STLport 4.6 resize pair (stl/_vector.h)
public:
    std::vector<_Tp> Items;
    void resize(unsigned int n, const _Tp& x) { ++g_twoArgCalls; if (x.Value != 0) g_valueOk = false; Items.resize(n, x); }
    void resize(unsigned int n) { resize(n, _Tp()); }
};
int main()
{
    StlportVectorModel<CMultiStaticMeshEntryDef> v;
    v.resize(4);
    bool ok = v.Items.size() == 4 && g_twoArgCalls == 1 && g_valueOk;
    long ctorAfterGrow = g_ctor;
    v.Items[3].Value = 7;
    v.resize(2);
    ok = ok && v.Items.size() == 2 && g_twoArgCalls == 2 && g_ctor == ctorAfterGrow + 1 && g_valueOk;
    if (!ok) { printf("004edf0a_TEST FAIL\n"); return 1; }
    printf("004edf0a_TEST PASS\n");
    return 0;
}
