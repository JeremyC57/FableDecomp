#include <cstdio>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif

struct random_access_iterator_tag { };
static int g_destroys = 0, g_blockDeletes = 0;
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
    ~SCountBlock() { ++g_blockDeletes; }
};
static void __fastcall DestroyObject(void*) { ++g_destroys; }
struct CCountedPointer {
    void*        m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        SCountBlock* count = other.m_pCount;
        void* object = other.m_pObject;
        if (m_pCount != count) {
            if (m_pCount) {
                --m_pCount->m_references;
                if (m_pCount->m_references == 0) {
                    m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                    delete m_pCount;
                }
            }
            m_pObject = object;
            m_pCount = count;
            if (count)
                ++count->m_references;
        }
        return *this;
    }
};
static int g_payload[4];
static SCountBlock* MakeBlock(int i, long refs)
{
    SCountBlock* b = new SCountBlock;
    b->m_references = refs;
    b->m_pfnDestroy = DestroyObject;
    b->m_pObject = &g_payload[i];
    return b;
}
struct SPairLongCountedPointer {
    long            first;
    CCountedPointer second;
    SPairLongCountedPointer() : first(0) { }
};

typedef SPairLongCountedPointer T_;
enum { N = 3 };

static T_* Copy(const T_* first, const T_* last, T_* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

int main()
{
    bool ok = true;
    static T_ src[N];
    static T_ dst[N];
    SCountBlock* blocks[3] = { MakeBlock(0, 1), MakeBlock(1, 1), MakeBlock(2, 1) };
    SCountBlock* old = MakeBlock(3, 2);                       // held by dst[1] and dst[2]
    for (int i = 0; i < N; ++i) {
        T_* s = &src[i];
        T_* d = &dst[i];
        s->first = 1000 + i;
        s->second.m_pObject = &g_payload[i]; s->second.m_pCount = blocks[i];
        d->second.m_pObject = (i == 0) ? &g_payload[0] : &g_payload[3]; d->second.m_pCount = (i == 0) ? blocks[0] : old;   // dst[0] already shares that block
    }
    random_access_iterator_tag tag;
    T_* r = Copy(src, src + N, dst, tag, (int*)0);
    if (r != dst + N) ok = false;
    for (int i = 0; i < N; ++i) {
        const T_* s = &src[i];
        const T_* d = &dst[i];
        if (d->first != 1000 + i) ok = false;
        if (d->second.m_pObject != &g_payload[i] || d->second.m_pCount != blocks[i]) ok = false;
    }
    if (blocks[0]->m_references != 1 || blocks[1]->m_references != 2 || blocks[2]->m_references != 2) ok = false;
    if (g_destroys != 1 || g_blockDeletes != 1) ok = false;    // the shared old block went to zero exactly once
    if (Copy(src, src, dst + 1, tag, (int*)0) != dst + 1) ok = false;
    if (!ok) { std::printf("00828cd0_TEST FAIL\n"); return 1; }
    std::printf("00828cd0_TEST PASS\n");
    return 0;
}