#include <cstdio>
#include <cstring>
// Behaviour copy of C3DClothPrimitive::LoadBinary (0x00AC3BD0): indices, pooled program bytes, program read, release on failure.
struct Pool {
    bool allowAlloc; int allocs, frees; unsigned char block[64];
    Pool() : allowAlloc(true), allocs(0), frees(0) {}
    bool Allocate(Pool** owner, long* handle, unsigned int size) { if (!allowAlloc || size > 64) return false; ++allocs; *owner = this; *handle = 7; return true; }
    void* GetPointer(long) { return block; }
    void Free(long) { ++frees; }
};
struct ScopedHandle { Pool* pool; long handle; ScopedHandle() : pool(0), handle(-1) {} ~ScopedHandle() { if (pool) pool->Free(handle); } };
struct Stream { const unsigned char* p; Stream(const unsigned char* d) : p(d) {}
    unsigned int ReadULong() { unsigned int v; std::memcpy(&v, p, 4); p += 4; return v; }
    void ReadData(void* b, unsigned int n) { std::memcpy(b, p, n); p += n; } };
struct Program { bool ok; bool Read(const unsigned char* data, unsigned int n) { return n > 0 && data[0] == 'P'; } };
static int g_programsAlive = 0;
struct Cloth {
    Program* program; unsigned int PrimitiveIndex, MaterialIndex;
    bool LoadBinary(Stream& in, Pool* pool) {
        PrimitiveIndex = in.ReadULong(); MaterialIndex = in.ReadULong();
        unsigned int size = in.ReadULong();
        ScopedHandle h;
        if (!pool->Allocate(&h.pool, &h.handle, size)) return false;
        unsigned char* data = (unsigned char*)h.pool->GetPointer(h.handle);
        in.ReadData(data, size);
        program = new Program; ++g_programsAlive;
        if (!program->Read(data, size)) { delete program; --g_programsAlive; program = 0; return false; }
        return true;
    }
};
int main() {
    unsigned char good[] = { 1,0,0,0, 2,0,0,0, 2,0,0,0, 'P','x' };
    unsigned char bad[]  = { 3,0,0,0, 4,0,0,0, 1,0,0,0, 'Q' };
    Pool pool; Cloth c; c.program = 0;
    Stream s(good); bool r1 = c.LoadBinary(s, &pool);
    bool ok = r1 && c.PrimitiveIndex == 1 && c.MaterialIndex == 2 && c.program != 0 && pool.allocs == 1 && pool.frees == 1;
    Cloth d; d.program = 0; Stream t(bad); bool r2 = d.LoadBinary(t, &pool);
    ok = ok && !r2 && d.program == 0 && d.MaterialIndex == 4 && pool.frees == 2 && g_programsAlive == 1;
    Pool none; none.allowAlloc = false; Cloth e; e.program = 0; Stream u(good); bool r3 = e.LoadBinary(u, &none);
    ok = ok && !r3 && none.frees == 0 && e.program == 0;
    delete c.program;
    if (!ok) { std::printf("00ac3bd0_TEST FAIL\n"); return 1; }
    std::printf("00ac3bd0_TEST PASS\n"); return 0;
}