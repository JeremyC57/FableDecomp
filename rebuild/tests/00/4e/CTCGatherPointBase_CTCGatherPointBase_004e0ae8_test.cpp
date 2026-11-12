#include <cstdio>
#include <cstdlib>
// Behaviour test for CTCGatherPointBase::~CTCGatherPointBase (0x004E0AE8): links the row object (its DestroyOne emission helper runs the
// implicit destructor): the Users vector (+0x0C; here empty with an allocated block) is released, then the CTCBase base is destroyed.
static int g_base = 0;
static const void* g_baseThis = 0;
class CTCBase {
public:
    virtual ~CTCBase();
};
CTCBase::~CTCBase() { ++g_base; g_baseThis = this; }
class CTCGatherPointBase;
void DestroyOne(CTCGatherPointBase* object);
int main()
{
    void* buf[8] = { 0 };
    void* block = std::malloc(16);
    buf[3] = block;                                     // Users._M_start (+0x0c)
    buf[4] = block;                                     // Users._M_finish (+0x10): no element
    buf[5] = (char*)block + 16;                         // Users._M_end_of_storage (+0x14)
    DestroyOne((CTCGatherPointBase*)buf);
    if (g_base != 1 || g_baseThis != (void*)buf) { std::printf("BAD_BASE\n"); return 1; }
    void* zero[8] = { 0 };
    DestroyOne((CTCGatherPointBase*)zero);
    if (g_base != 2 || g_baseThis != (void*)zero) { std::printf("BAD_BASE2\n"); return 1; }
    std::printf("DESTVAL_004e0ae8_OK\n");
    return 0;
}
