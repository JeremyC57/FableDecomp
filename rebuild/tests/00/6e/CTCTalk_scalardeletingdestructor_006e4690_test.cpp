// behaviour test for 0x006E4690 CTCTalk's scalar deleting destructor: deleting through a base pointer runs the members' destructors in reverse declaration order, then the base destructor, exactly once each, and frees the block; destroying without the delete bit (an explicit destructor call) runs the same chain and frees nothing
#include <cstdio>
#include <cstdlib>
#include <new>
static int gOrder[8];
static int gCount = 0;
static int gFreed = 0;
static void Log(int id) { if (gCount < 8) gOrder[gCount] = id; ++gCount; }
class Base {
public:
    virtual ~Base() { Log(9); }
    static void* operator new(size_t n) { return std::malloc(n); }
    static void operator delete(void* p) { ++gFreed; std::free(p); }
    static void* operator new(size_t, void* where) { return where; }
    static void operator delete(void*, void*) { }
    int BaseData[3];
};
class MemberA { public: ~MemberA() { Log(1); } int Data; };
class MemberB { public: ~MemberB() { Log(2); } int Data[2]; };
class Derived : public Base {                     // implicit destructor, like CTCTalk
public:
    MemberA First;
    long Plain;
    MemberB Second;
};
int main()
{
    int fails = 0;
    Base* p = new Derived;
    delete p;
    if (gCount != 3 || gOrder[0] != 2 || gOrder[1] != 1 || gOrder[2] != 9) fails |= 1;
    if (gFreed != 1) fails |= 2;
    gCount = 0; gFreed = 0;
    void* raw = std::malloc(sizeof(Derived));
    Derived* d = new (raw) Derived;
    d->~Derived();
    if (gCount != 3 || gOrder[0] != 2 || gOrder[2] != 9 || gFreed != 0) fails |= 4;
    std::free(raw);
    if (fails) { std::printf("006e4690_TEST FAIL %d\n", fails); return 1; }
    std::printf("006e4690_TEST PASS\n");
    return 0;
}