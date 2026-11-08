// Behaviour test for 0x0121F590: the dynamic initializer of the anonymous-namespace CEnginePrimitiveHandle hit_pos.
// The initializer default-constructs the handle (base CBaseClass, vptr, null Data) before main and registers its destructor with
// atexit; the test checks construction order, the null Data, and that the destructor runs exactly once at exit after main.
#include <stdio.h>
#include <stdlib.h>
static int baseCtors, handleDtors, mainDone;
class CBaseClass { public: CBaseClass() { ++baseCtors; } virtual ~CBaseClass() { } };
class CEnginePrimitiveHandle : public CBaseClass {
public:
    CEnginePrimitiveHandle() : Data(0) { }
    virtual ~CEnginePrimitiveHandle() { if (!mainDone || Data != 0) abort(); ++handleDtors; }
    void* Data;
};
static void auditExit()
{
    if (handleDtors != 1) { printf("0121f590_TEST FAIL\n"); exit(1); }
    printf("0121f590_TEST PASS\n");
}
class TestAudit { public: TestAudit() { atexit(auditExit); } };
static TestAudit audit;                       // registered first, so it runs after the handle's destructor
namespace { CEnginePrimitiveHandle hit_pos; }
int main()
{
    if (baseCtors != 1 || hit_pos.Data != 0 || handleDtors != 0) { printf("0121f590_TEST FAIL\n"); return 1; }
    mainDone = 1;
    return 0;
}