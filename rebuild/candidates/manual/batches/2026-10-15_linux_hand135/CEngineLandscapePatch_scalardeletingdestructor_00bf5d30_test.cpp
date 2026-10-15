// Behaviour test for CEngineLandscapePatch::`scalar deleting destructor' @ 0x00BF5D30 (hand model of the inlined body).
#include <cstdio>

static int g_cleared = 0, g_destroyed = 0, g_blockFreed = 0; static void* g_freedPatch = 0;
struct SCountBlock { long m_count; void (*m_pfnDestroy)(void*); void* m_pObject; };
static void DestroyObject(void*) { ++g_destroyed; }
struct CCountedResourcePointer {
    void* m_pObject; SCountBlock* m_pBlock;
    void Release()
    {
        if (m_pBlock) {
            --m_pBlock->m_count;
            if (m_pBlock->m_count == 0) { m_pBlock->m_pfnDestroy(m_pBlock->m_pObject); ++g_blockFreed; }
        }
        m_pObject = 0; m_pBlock = 0;
    }
};
enum { VT_PATCH = 0x12a8200, VT_MOVABLE = 0x129b860 };
struct CEngineLandscapePatch {
    unsigned int vt; unsigned char pad[0x3c]; CCountedResourcePointer m_resource;
    void Clear() { ++g_cleared; }
};
static CEngineLandscapePatch* ScalarDeletingDestructor(CEngineLandscapePatch* self, unsigned int flags)
{
    self->vt = VT_PATCH; self->Clear(); self->m_resource.Release(); self->vt = VT_MOVABLE;
    if (flags & 1) g_freedPatch = self;
    return self;
}

int main()
{
    int obj = 0; SCountBlock block = { 2, DestroyObject, &obj };
    CEngineLandscapePatch a; a.m_resource.m_pObject = &obj; a.m_resource.m_pBlock = &block;
    CEngineLandscapePatch b; b.m_resource.m_pObject = &obj; b.m_resource.m_pBlock = &block;
    CEngineLandscapePatch* r = ScalarDeletingDestructor(&a, 0);
    int ok = r == &a && g_cleared == 1 && block.m_count == 1 && g_destroyed == 0 && a.m_resource.m_pBlock == 0
          && a.m_resource.m_pObject == 0 && a.vt == VT_MOVABLE && g_freedPatch == 0;
    r = ScalarDeletingDestructor(&b, 1);
    ok = ok && r == &b && g_cleared == 2 && block.m_count == 0 && g_destroyed == 1 && g_blockFreed == 1 && g_freedPatch == &b;
    CEngineLandscapePatch c; c.m_resource.m_pObject = 0; c.m_resource.m_pBlock = 0;
    ScalarDeletingDestructor(&c, 1);
    ok = ok && g_cleared == 3 && g_destroyed == 1 && g_freedPatch == &c;
    if (!ok) { std::printf("00bf5d30_TEST FAIL\n"); return 1; }
    std::printf("00bf5d30_TEST PASS\n");
    return 0;
}
