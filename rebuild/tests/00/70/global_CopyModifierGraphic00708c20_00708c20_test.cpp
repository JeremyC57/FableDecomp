#include <cstdio>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif

struct random_access_iterator_tag { };
static int g_deletes = 0;
struct CCountedBase {
    unsigned long RefCount;
    CCountedBase() : RefCount(0) { }
    virtual void Destroy(int) { }
    virtual void Delete() { ++g_deletes; }
    void Release()
    {
        if (--RefCount == 0)
            Delete();
    }
};
struct CIntrusivePointer {
    CCountedBase* Object;
    CIntrusivePointer() : Object(0) { }
    CIntrusivePointer& operator=(const CIntrusivePointer& other)
    {
        CCountedBase* old = Object;
        if (old != other.Object) {
            if (old) {
                old->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};
static CCountedBase g_objects[4];
struct SEntry {
    CIntrusivePointer Pointer;
    long  Word1;
    float Word2;
    SEntry() : Word1(0), Word2(0) { }
};

typedef SEntry T_;
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
    for (int i = 0; i < N; ++i) {
        T_* s = &src[i];
        T_* d = &dst[i];
        s->Pointer.Object = (i == 2) ? 0 : &g_objects[i]; g_objects[i].RefCount = 1; s->Word1 = 10 + i; s->Word2 = 0.5f * i;
        d->Pointer.Object = (i == 0) ? &g_objects[0] : &g_objects[3]; d->Word1 = -1; d->Word2 = -1.0f;
    }
    g_objects[3].RefCount = 2;
    random_access_iterator_tag tag;
    T_* r = Copy(src, src + N, dst, tag, (int*)0);
    if (r != dst + N) ok = false;
    for (int i = 0; i < N; ++i) {
        const T_* s = &src[i];
        const T_* d = &dst[i];
        if (d->Pointer.Object != s->Pointer.Object || d->Word1 != 10 + i || d->Word2 != 0.5f * i) ok = false;
    }
    if (g_objects[1].RefCount != 2) ok = false;      // adopted by dst[1]
    if (g_objects[0].RefCount != 1) ok = false;      // same object: untouched
    if (g_objects[3].RefCount != 0 || g_deletes != 1) ok = false;   // released by dst[1] and dst[2]
    if (Copy(src, src, dst + 1, tag, (int*)0) != dst + 1) ok = false;
    if (!ok) { std::printf("00708c20_TEST FAIL\n"); return 1; }
    std::printf("00708c20_TEST PASS\n");
    return 0;
}