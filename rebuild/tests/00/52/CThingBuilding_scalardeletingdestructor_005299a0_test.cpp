#include <cstdio>

// Mirror the source's class shape so we can drive its logic in a standalone test.
class CThingBuilding
{ public:
    int scalarCalled;
    int deleteCalled;
    void ScalarDtor();
    void DeleteHelper();
    void* ScalarDeletingDestructor(unsigned int flags);
};

void CThingBuilding::ScalarDtor()  { scalarCalled = 1; }
void CThingBuilding::DeleteHelper(){ deleteCalled = 1; }

void* CThingBuilding::ScalarDeletingDestructor(unsigned int flags)
{
    this->ScalarDtor();
    if (flags & 1)
        this->DeleteHelper();
    return this;
}

int main()
{
    // flags bit0 clear: dtor runs, no delete
    CThingBuilding a;
    a.scalarCalled = 0; a.deleteCalled = 0;
    void* ra = a.ScalarDeletingDestructor(0);
    bool ok = (ra == &a) && a.scalarCalled == 1 && a.deleteCalled == 0;

    // flags bit0 set: dtor runs, delete runs
    CThingBuilding b;
    b.scalarCalled = 0; b.deleteCalled = 0;
    void* rb = b.ScalarDeletingDestructor(1);
    ok = ok && (rb == &b) && b.scalarCalled == 1 && b.deleteCalled == 1;

    // even flag value with bit0 clear -> no delete
    CThingBuilding c;
    c.scalarCalled = 0; c.deleteCalled = 0;
    c.ScalarDeletingDestructor(2);
    ok = ok && c.scalarCalled == 1 && c.deleteCalled == 0;

    if (ok) { printf("VDD_OK\n"); return 0; }
    printf("VDD_FAIL\n"); return 1;
}