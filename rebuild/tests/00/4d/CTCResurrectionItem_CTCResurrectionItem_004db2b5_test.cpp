// behaviour test for 0x004DB2B5 CTCResurrectionItem::CTCResurrectionItem(CThing&): the base records the owning thing, the def pointer starts null
#include <cstdio>
struct Thing { int Id; };
struct TCBase { Thing* Parent; bool ToBeRemoved; bool InGlobal; TCBase(Thing& t) : Parent(&t), ToBeRemoved(false), InGlobal(false) { } virtual ~TCBase() { } };
struct DefPointer { void* Object; DefPointer() : Object(0) { } };
struct TC : public TCBase { DefPointer PDef; TC(Thing& t) : TCBase(t) { } };
int main()
{
    int fail = 0;
    Thing t; t.Id = 3;
    TC c(t);
    if (c.Parent != &t || c.PDef.Object != 0) fail |= 1;
    if (fail) { std::printf("004db2b5_TEST FAIL %d\n", fail); return 1; }
    std::printf("004db2b5_TEST PASS\n");
    return 0;
}