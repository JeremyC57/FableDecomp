// behaviour test for 0x007939a0 CTCUnholyPower::PerEffectInfo's scalar deleting destructor: delete runs the one destructible member's destructor
// (CIntelligentPointer<CThing> at +0xC) and then frees the object; an explicit destructor call runs the member destructor only
#include <cstdio>
#include <cstdlib>
#include <new>
static int gMemberDtors = 0;
static int gBlocksFreed = 0;
class Member { public: ~Member() { ++gMemberDtors; } unsigned int Data; };
class Holder {
public:
    unsigned char Before[0xC];                            // trivially destroyed members
    Member M;                                                   // +0xC
    static void* operator new(size_t n) { return std::malloc(n); }
    static void operator delete(void* p) { ++gBlocksFreed; std::free(p); }
    static void* operator new(size_t, void* where) { return where; }
    static void operator delete(void*, void*) { }
};
int main()
{
    int fails = 0;
    if ((unsigned int)((char*)&((Holder*)0)->M - (char*)0) != 0xC) fails |= 1;
    Holder* p = new Holder;
    delete p;
    if (gMemberDtors != 1 || gBlocksFreed != 1) fails |= 2;
    void* raw = std::malloc(sizeof(Holder));
    Holder* q = new (raw) Holder;
    q->~Holder();                                               // no delete bit: the storage stays
    if (gMemberDtors != 2 || gBlocksFreed != 1) fails |= 4;
    std::free(raw);
    if (fails) { std::printf("007939a0_TEST FAIL %d\n", fails); return 1; }
    std::printf("007939a0_TEST PASS\n");
    return 0;
}