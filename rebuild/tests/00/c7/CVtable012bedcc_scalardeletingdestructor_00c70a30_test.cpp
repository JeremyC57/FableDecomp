// Behaviour test for 00c70a30: CVtable_012bedcc scalar deleting destructor -- destroy, then free through the class allocator
#include <cstdio>
#include <cstdlib>
static int g_dtor = 0, g_base = 0, g_free = 0;
class CPKObject {
public:
    virtual ~CPKObject() { }
    static void* operator new(unsigned int size) { return std::malloc(size); }
    static void operator delete(void* p) { ++g_free; std::free(p); }
};
class CVtable_012beda4 : public CPKObject { public: CVtable_012beda4() { } ~CVtable_012beda4() { ++g_base; } unsigned char _pad04[0x24]; };
class CVtable_012bedcc : public CVtable_012beda4 { public: CVtable_012bedcc() { } ~CVtable_012bedcc() { ++g_dtor; } };
int main()
{
    CPKObject* p = new CVtable_012bedcc;
    delete p;
    int ok = g_dtor == 1 && g_base == 1 && g_free == 1;
    std::printf(ok ? "00c70a30_TEST PASS\n" : "00c70a30_TEST FAIL\n");
    return ok ? 0 : 1;
}
