// behaviour test for 0x004D7DE2 CTCCreatedEntity::CTCCreatedEntity(CThing&): the base part is built from the thing first, then the intelligent-pointer member is default-constructed empty
#include <cstdio>
struct Thing { int id; };
static int gOrder[4]; static int gN = 0;
struct Base { Thing* parent; Base(Thing& t) : parent(&t) { gOrder[gN++] = 1; } virtual ~Base() { } };
struct Ptr { void* data; Ptr() : data(0) { gOrder[gN++] = 2; } virtual ~Ptr() { } };
struct Derived : Base {
    Ptr member;
    Derived(Thing& t) : Base(t) { }                         // the function under test
};
int main()
{
    int fail = 0;
    Thing t; t.id = 3;
    Derived d(t);
    if (gN != 2 || gOrder[0] != 1 || gOrder[1] != 2) fail |= 1;
    if (d.parent != &t || d.member.data != 0) fail |= 2;
    if (fail) { std::printf("004d7de2_TEST FAIL %d\n", fail); return 1; }
    std::printf("004d7de2_TEST PASS\n");
    return 0;
}