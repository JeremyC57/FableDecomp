// Behaviour test for 0098b420: src/0098b420.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CShaderRenderManager::AllocateLights(unsigned long, unsigned long)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CShaderRenderManager::AllocateLights(unsigned long numLights, unsigned long numEntriesPerLight) -- grow the light table (keeping the existing lights, defaulting new ones) and the per-light entry buffer from the frame pool, and size the light-constant vector
// Shape: the STLport vector<CLight>(n) ctor 0x0098CCC0 and __copy 0x0098C2F0 are template instantiations of this TU, so
//   they are defined here (noinline): with their bodies visible VC7.1 caches the old-lights start in edi for the final free,
//   homes the first copy's tag in the dead numLights argument slot and lays the resize value over the old-lights vector
//   (0x48 frame). The pool guard is a nested conditional (count ? (64-bit product > 0xFFFFFFFF ? 0 : Allocate) : 0),
//   which drops the never-taken jb of the && spelling.
// Leaf: AllocateLights
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
struct random_access_iterator_tag { random_access_iterator_tag() {} };
struct CLight {                                    // 0x38 bytes
    unsigned char Params[0x30];
    long          Index;                           // +0x30
    bool          Enabled;                         // +0x34
    bool          Dirty;                           // +0x35
    unsigned char _pad36[2];
    CLight() : Index(0), Enabled(false), Dirty(true) {}
};
CLight* __fastcall LightCopy(const CLight* first, const CLight* last, CLight* result,
                             const random_access_iterator_tag&, int*);   // 0x0098C2F0 (STLport __copy)
inline random_access_iterator_tag IteratorCategory(const CLight*) { return random_access_iterator_tag(); }
inline CLight* Copy(const CLight* first, const CLight* last, CLight* result)
{
    return LightCopy(first, last, result, IteratorCategory(first), (int*)0);
}
struct CLightVector {                              // STLport vector<CLight>
    CLight* _M_start;
    CLight* _M_finish;
    CLight* _M_end_of_storage;
    CLightVector(unsigned long count);             // 0x0098CCC0
    ~CLightVector() { if (_M_start) free(_M_start); }
};
struct C4DVector {
    float X, Y, Z, W;
    C4DVector() {}
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
};
struct C4DVectorVector {                           // STLport vector<C4DVector>
    C4DVector* _M_start;
    C4DVector* _M_finish;
    C4DVector* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    C4DVector* erase(C4DVector* first, C4DVector* last);                               // 0x0098CF70
    void _M_fill_insert(C4DVector* position, unsigned long count, const C4DVector& value);   // 0x0098D360
    void resize(unsigned long newSize, const C4DVector& value)
    {
        if (newSize < size())
            erase(_M_start + newSize, _M_finish);
        else
            _M_fill_insert(_M_finish, newSize - size(), value);
    }
};
struct CSimpleMemoryPool {
    void FreeAll();                                // 0x00A5C720
    void* Allocate(unsigned long bytes);           // 0x00A5C910
};
#pragma auto_inline(off)
// Same-TU instantiation of the STLport vector<CLight>(n) ctor 0x0098CCC0 (not inlined): with the body visible VC7.1 knows it
// does not keep `this`, so the old-lights start is cached in a register and its frame words are reused.
__declspec(noinline) CLightVector::CLightVector(unsigned long count)
{
    _M_start = 0;
    _M_finish = 0;
    _M_end_of_storage = 0;
    CLight* start = count != 0 ? (CLight*)operator new(count * sizeof(CLight)) : 0;
    _M_start = start;
    _M_finish = start;
    _M_end_of_storage = start + count;
    CLight value;
    CLight* p = start;
    for (unsigned long n = count; n > 0; --n, ++p)
        *p = value;
    _M_finish = p;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
__declspec(noinline) CLight* __fastcall LightCopy(const CLight* first, const CLight* last, CLight* result,
                                                  const random_access_iterator_tag&, int*)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
#pragma auto_inline(on)
template <class T> inline T* PoolNewArray(CSimpleMemoryPool& pool, unsigned long count)
{
    return count != 0 ? ((unsigned __int64)count * sizeof(T) > 0xffffffff ? 0 : (T*)pool.Allocate(count * sizeof(T))) : 0;
}
struct C4DFloat { float V[4]; };
struct CShaderRenderManager {
    unsigned char     _pad000[0x164];
    CLight*           Lights;                      // +0x164
    C4DFloat*         LightEntries;                // +0x168
    unsigned char     _pad16c[4];
    unsigned long     NumLights;                   // +0x170
    unsigned long     NumEntriesPerLight;          // +0x174
    CSimpleMemoryPool Pool;                        // +0x178
    unsigned char     _pad179[0x27];
    C4DVectorVector   LightConstants;              // +0x1a0
    void AllocateLights(unsigned long numLights, unsigned long numEntriesPerLight);
};

void CShaderRenderManager::AllocateLights(unsigned long numLights, unsigned long numEntriesPerLight)
{
    CLightVector oldLights(NumLights);
    if (NumLights > 0)
        Copy(Lights, Lights + NumLights, oldLights._M_start);
    unsigned long oldCount = NumLights;
    if (NumLights < numLights)
        NumLights = numLights;
    if (NumEntriesPerLight < numEntriesPerLight)
        NumEntriesPerLight = numEntriesPerLight;
    Pool.FreeAll();
    Lights = PoolNewArray<CLight>(Pool, NumLights);
    Copy(oldLights._M_start, oldLights._M_finish, Lights);
    if (numLights > oldCount) {
        CLight* last = Lights + numLights;
        CLight value;
        for (CLight* p = Lights + oldCount; p != last; ++p)
            *p = value;
    }
    LightEntries = PoolNewArray<C4DFloat>(Pool, NumEntriesPerLight * NumLights);
    LightConstants.resize((numLights + 3) / 4 * 2, C4DVector(1.0f, 1.0f, 1.0f, 1.0f));
}
// ---- stubs ----
static unsigned char g_arena[0x4000]; static unsigned long g_used; static int g_freeAll, g_allocs; static unsigned long g_lastBytes;
void CSimpleMemoryPool::FreeAll() { ++g_freeAll; g_used = 0; }
void* CSimpleMemoryPool::Allocate(unsigned long bytes) { ++g_allocs; g_lastBytes = bytes; void* p = g_arena + g_used; g_used += bytes; return p; }
static int g_erase, g_fill; static C4DVector* g_eraseFirst; static C4DVector* g_eraseLast; static C4DVector* g_fillPos;
static unsigned long g_fillCount; static C4DVector g_fillValue;
C4DVector* C4DVectorVector::erase(C4DVector* first, C4DVector* last) { ++g_erase; g_eraseFirst = first; g_eraseLast = last; return first; }
void C4DVectorVector::_M_fill_insert(C4DVector* position, unsigned long count, const C4DVector& value)
{ ++g_fill; g_fillPos = position; g_fillCount = count; g_fillValue = value; }
static void Reset() { g_freeAll = g_allocs = g_erase = g_fill = 0; g_lastBytes = 0; g_eraseFirst = g_eraseLast = g_fillPos = 0; g_fillCount = 99; }
static CLight g_old[8];
static C4DVector g_consts[4];
int main()
{
    static CShaderRenderManager mgr; memset(&mgr, 0, sizeof mgr);
    for (int i = 0; i < 8; ++i) { memset(&g_old[i], 0, sizeof(CLight)); g_old[i].Index = 100 + i; g_old[i].Enabled = true; g_old[i].Dirty = false; g_old[i].Params[0] = (unsigned char)i; }
    // grow 2 -> 4 lights, entries stay 3: old lights copied, two default lights appended, constants filled to 2
    Reset(); mgr.Lights = g_old; mgr.NumLights = 2; mgr.NumEntriesPerLight = 3;
    mgr.LightConstants._M_start = mgr.LightConstants._M_finish = mgr.LightConstants._M_end_of_storage = g_consts;
    mgr.AllocateLights(4, 2);
    CHECK(mgr.NumLights == 4 && mgr.NumEntriesPerLight == 3 && g_freeAll == 1 && g_allocs == 2);
    CHECK(mgr.Lights == (CLight*)g_arena);
    CHECK(mgr.Lights[0].Index == 100 && mgr.Lights[1].Index == 101 && mgr.Lights[1].Params[0] == 1 && mgr.Lights[1].Enabled);
    CHECK(mgr.Lights[2].Index == 0 && !mgr.Lights[2].Enabled && mgr.Lights[2].Dirty);
    CHECK(mgr.Lights[3].Index == 0 && !mgr.Lights[3].Enabled && mgr.Lights[3].Dirty);
    CHECK(mgr.LightEntries == (C4DFloat*)(g_arena + 4 * sizeof(CLight)) && g_lastBytes == 3 * 4 * sizeof(C4DFloat));
    CHECK(g_fill == 1 && g_erase == 0 && g_fillPos == g_consts && g_fillCount == 2);
    CHECK(g_fillValue.X == 1.0f && g_fillValue.Y == 1.0f && g_fillValue.Z == 1.0f && g_fillValue.W == 1.0f);
    // shrink request 4 -> 2 with more entries: light count kept, no default fill, constants erased down to 2
    CLight saved[4]; memcpy(saved, mgr.Lights, sizeof saved);
    static CLight src4[4]; memcpy(src4, saved, sizeof src4); src4[3].Index = 777;
    Reset(); mgr.Lights = src4;
    mgr.LightConstants._M_start = g_consts; mgr.LightConstants._M_finish = g_consts + 3;
    mgr.AllocateLights(2, 5);
    CHECK(mgr.NumLights == 4 && mgr.NumEntriesPerLight == 5 && g_freeAll == 1);
    CHECK(mgr.Lights[0].Index == 100 && mgr.Lights[3].Index == 777 && mgr.Lights[2].Dirty);
    CHECK(g_lastBytes == 4 * 5 * sizeof(C4DFloat));
    CHECK(g_erase == 1 && g_fill == 0 && g_eraseFirst == g_consts + 2 && g_eraseLast == g_consts + 3);
    // empty table: nothing allocated (null arrays), constants resized to 0 by an empty fill
    Reset(); mgr.Lights = 0; mgr.NumLights = 0; mgr.NumEntriesPerLight = 0;
    mgr.LightConstants._M_start = mgr.LightConstants._M_finish = g_consts;
    mgr.AllocateLights(0, 0);
    CHECK(mgr.Lights == 0 && mgr.LightEntries == 0 && g_allocs == 0 && g_freeAll == 1);
    CHECK(g_fill == 1 && g_fillCount == 0);
    // entry buffer size overflows 32 bits: the guard returns null without allocating
    Reset(); mgr.Lights = g_old; mgr.NumLights = 1; mgr.NumEntriesPerLight = 0x20000000;
    mgr.LightConstants._M_start = mgr.LightConstants._M_finish = g_consts;
    mgr.AllocateLights(1, 0);
    CHECK(mgr.Lights == (CLight*)g_arena && mgr.Lights[0].Index == 100 && g_allocs == 1 && mgr.LightEntries == 0);
    CHECK(g_fill == 1 && g_fillCount == 2);
    if (fails == 0) printf("0098b420_TEST PASS\n"); else printf("0098b420_TEST FAIL\n");
    return fails;
}