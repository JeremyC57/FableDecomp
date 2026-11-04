// Behaviour test for CMemoryAllocatorVariableSize::Allocate (0x00A659C0, parked DIFFER row): a model of the row -- existing chunks first, else a new chunk and retry; result past the header or 0.
#include <stdio.h>
static int g_free; static int g_chunks;
static int TryExisting(unsigned long size) { if ((unsigned long)g_free >= size) { g_free -= size; return 100; } return 0; }
static bool AddChunk() { if (g_chunks == 0) return false; --g_chunks; g_free += 64; return true; }
static int Allocate(unsigned long size) { int p = TryExisting(size); if (!p && AddChunk()) p = TryExisting(size); return p ? p + 8 : 0; }
static int Check() { g_free = 10; g_chunks = 1; int ok = Allocate(8) == 108 && Allocate(40) == 108; return ok && Allocate(100) == 0; }
int main()
{
    int ok = Check();
    printf(ok ? "00a659c0_TEST PASS\n" : "00a659c0_TEST FAIL\n");
    return ok ? 0 : 1;
}
