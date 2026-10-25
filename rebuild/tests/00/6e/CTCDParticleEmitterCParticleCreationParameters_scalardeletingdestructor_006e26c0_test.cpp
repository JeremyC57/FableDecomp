// behaviour test for 0x006e26c0 CTCDParticleEmitter::CParticleCreationParameters's scalar deleting destructor: delete runs the one destructible member's destructor
// (CParticleUpdateParameters<unsigned long, unsigned long> at +0x28) and then frees the object; an explicit destructor call runs the member destructor only
#include <cstdio>
#include <cstdlib>
#include <new>
static int gMemberDtors = 0;
static int gBlocksFreed = 0;
class Member { public: ~Member() { ++gMemberDtors; } unsigned int Data; };
class Holder {
public:
    unsigned char Before[0x28];                            // trivially destroyed members
    Member M;                                                   // +0x28
    static void* operator new(size_t n) { return std::malloc(n); }
    static void operator delete(void* p) { ++gBlocksFreed; std::free(p); }
    static void* operator new(size_t, void* where) { return where; }
    static void operator delete(void*, void*) { }
};
int main()
{
    int fails = 0;
    if ((unsigned int)((char*)&((Holder*)0)->M - (char*)0) != 0x28) fails |= 1;
    Holder* p = new Holder;
    delete p;
    if (gMemberDtors != 1 || gBlocksFreed != 1) fails |= 2;
    void* raw = std::malloc(sizeof(Holder));
    Holder* q = new (raw) Holder;
    q->~Holder();                                               // no delete bit: the storage stays
    if (gMemberDtors != 2 || gBlocksFreed != 1) fails |= 4;
    std::free(raw);
    if (fails) { std::printf("006e26c0_TEST FAIL %d\n", fails); return 1; }
    std::printf("006e26c0_TEST PASS\n");
    return 0;
}