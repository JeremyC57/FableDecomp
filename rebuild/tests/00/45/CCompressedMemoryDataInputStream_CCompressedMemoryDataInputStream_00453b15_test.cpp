#include <cstdio>

struct CCompressedMemoryDataInputStream {
    void* vtbl;
    char pad[0xa4];
    void* buf_a8;
    char pad2[0x8];
    void* buf_b4;
};

static int g_freed = 0;
static void* g_last_free = 0;
static int g_base_called = 0;

// free spy (retail calls the MSVCR71 free thunk 0x00BFEA14): records only while armed around the call
// under test; any other free (CRT start-up/exit) is left as a harmless leak
static int g_armed = 0;
extern "C" void __cdecl free(void* p) { if (g_armed) { g_freed++; g_last_free = p; } }
void __fastcall CBase_dtor_009fc550(void* self) { g_base_called++; }

void __fastcall CCompressedMemoryDataInputStream_dtor(CCompressedMemoryDataInputStream* self)
{
    if (self->buf_b4)
        free(self->buf_b4);
    if (self->buf_a8)
        free(self->buf_a8);
    self->vtbl = (void*)0x1230ba0;
    CBase_dtor_009fc550(self);
}

int main()
{
    CCompressedMemoryDataInputStream o;
    o.vtbl = 0;
    o.buf_a8 = (void*)0x1111;
    o.buf_b4 = (void*)0x2222;
    g_armed = 1; CCompressedMemoryDataInputStream_dtor(&o); g_armed = 0;
    if (g_freed == 2 && o.vtbl == (void*)0x1230ba0 && g_base_called == 1) {
        // also test null path
        g_freed = 0; g_base_called = 0;
        CCompressedMemoryDataInputStream o2;
        o2.vtbl = 0; o2.buf_a8 = 0; o2.buf_b4 = 0;
        g_armed = 1; CCompressedMemoryDataInputStream_dtor(&o2); g_armed = 0;
        if (g_freed == 0 && g_base_called == 1 && o2.vtbl == (void*)0x1230ba0) {
            std::printf("CCompressedMemoryDataInputStream_00453b15_TEST PASS\n");
            return 0;
        }
    }
    std::printf("FAIL\n");
    return 1;
}