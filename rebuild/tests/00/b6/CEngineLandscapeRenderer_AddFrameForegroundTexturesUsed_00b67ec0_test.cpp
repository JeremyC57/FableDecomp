#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#endif
extern "C" int __cdecl printf(const char* format, ...);
// CEngineLandscapeRenderer::AddFrameForegroundTexturesUsed(const unsigned short* textures, long count, float priority) -- clamp the
//   priority to >= 0; for each texture index grow the per-texture priority (fill -1.0f) and flag (fill 0) vectors to cover it; the
//   first use this frame (priority < 0) appends the index to the used list and stores the priority, later uses keep the minimum.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B67EC0, 422 bytes (+int3), __fastcall `ret 0xc`; caller CEngineLandscapePatch::LoadTextures 0x00BF4130.
// Callees: memmove 0x00BFEAE6 (STLport erase/__copy_trivial, twice), vector<float>::_M_fill_insert 0x00441020,
//   vector<unsigned char>::_M_fill_insert 0x00485F3A, vector<unsigned long>::_M_insert_overflow 0x00464ACE.
// Layout: local model. engine/CEngineLandscapeRenderer.h (Ego_r.pdb) has MeshBuilder over +0x1c..+0xcf4; the TLC build keeps three
//   STLport vectors at +0x68c (used list), +0x698 (priorities), +0x6a4 (flags) -- names descriptive.
// Earlier tries (texture types, tag ctor forms, push_back by value) are in the pending differ header.
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);             // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* source, unsigned int count);   // 0x00BFEAE6

inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}

struct __true_type { __true_type() {} };

template <class T> struct CVector {                // STLport vector<T> (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) { return *(_M_start + n); }
    T* erase(T* first, T* last)
    {
        T* i = (T*)CopyTrivial(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void _M_fill_insert(T* position, unsigned int n, const T& x);
    template <class Tag> void _M_insert_overflow(T* position, const T& x, const Tag&, unsigned int n, bool atend);   // 0x00464ACE
    void insert(T* position, unsigned int n, const T& x) { _M_fill_insert(position, n, x); }
    void resize(unsigned int newSize, const T& x)
    {
        if (newSize < size())
            erase(begin() + newSize, end());
        else
            insert(end(), newSize - size(), x);
    }
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};

// Same-TU body of vector<unsigned long>::_M_insert_overflow 0x00464ACE (empty tag unnamed, never stored through).
template <class T> template <class Tag>
__declspec(noinline) void CVector<T>::_M_insert_overflow(T* position, const T& x, const Tag&, unsigned int n, bool atend)
{
    unsigned int oldSize = size();
    unsigned int len = oldSize + (oldSize > n ? oldSize : n);
    T* newStart = (T*)malloc(len * sizeof(T));
    T* newFinish = (T*)CopyTrivial(_M_start, position, newStart);
    for (; n != 0; --n)
        *newFinish++ = x;
    if (!atend)
        newFinish = (T*)CopyTrivial(position, _M_finish, newFinish);
    free(_M_start);
    _M_start = newStart;
    _M_finish = newFinish;
    _M_end_of_storage = newStart + len;
}

class CEngineLandscapeRenderer {
public:
    unsigned char                 _pad000[0x68c];
    CVector<unsigned long>        FrameForegroundTexturesUsed;        // +0x68c
    CVector<float>                FrameForegroundTexturePriorities;   // +0x698
    CVector<unsigned char>        FrameForegroundTextureFlags;        // +0x6a4
    void AddFrameForegroundTexturesUsed(const unsigned short* textures, long count, float priority);
};

void CEngineLandscapeRenderer::AddFrameForegroundTexturesUsed(const unsigned short* textures, long count, float priority)
{
    if (priority < 0.0f)
        priority = 0.0f;
    for (long i = 0; i < count; ++i) {
        int texture = textures[i];
        if (texture >= FrameForegroundTexturePriorities.size()) {
            FrameForegroundTexturePriorities.resize(texture + 1, -1.0f);
            FrameForegroundTextureFlags.resize(texture + 1, 0);
        }
        if (FrameForegroundTexturePriorities[texture] < 0.0f) {
            FrameForegroundTexturesUsed.push_back(texture);
            FrameForegroundTexturePriorities[texture] = priority;
        } else if (FrameForegroundTexturePriorities[texture] > priority)
            FrameForegroundTexturePriorities[texture] = priority;
    }
}

// ---- test harness: stand-in for the out-of-line _M_fill_insert (append-only use here) ----
template <class T> void CVector<T>::_M_fill_insert(T* position, unsigned int n, const T& x)
{
    unsigned int oldSize = size();
    unsigned int at = position - _M_start;
    T* p = (T*)malloc((oldSize + n) * sizeof(T));
    for (unsigned int i = 0; i < at; ++i) p[i] = _M_start[i];
    for (unsigned int i = 0; i < n; ++i) p[at + i] = x;
    for (unsigned int i = at; i < oldSize; ++i) p[n + i] = _M_start[i];
    free(_M_start);
    _M_start = p; _M_finish = p + oldSize + n; _M_end_of_storage = _M_finish;
}
static int g_fail = 0;
static void Check(int ok, const char* what) { if (!ok) { ++g_fail; printf("FAIL %s\n", what); } }
int main()
{
    CEngineLandscapeRenderer* r = (CEngineLandscapeRenderer*)malloc(sizeof(CEngineLandscapeRenderer));
    r->FrameForegroundTexturesUsed._M_start = r->FrameForegroundTexturesUsed._M_finish = r->FrameForegroundTexturesUsed._M_end_of_storage = 0;
    r->FrameForegroundTexturePriorities._M_start = r->FrameForegroundTexturePriorities._M_finish = r->FrameForegroundTexturePriorities._M_end_of_storage = 0;
    r->FrameForegroundTextureFlags._M_start = r->FrameForegroundTextureFlags._M_finish = r->FrameForegroundTextureFlags._M_end_of_storage = 0;
    unsigned short a[4] = { 3, 1, 3, 5 };
    r->AddFrameForegroundTexturesUsed(a, 4, 2.0f);
    Check(r->FrameForegroundTexturePriorities.size() == 6, "prio size 6");
    Check(r->FrameForegroundTextureFlags.size() == 6, "flag size 6");
    Check(r->FrameForegroundTexturesUsed.size() == 3, "used 3");
    Check(r->FrameForegroundTexturesUsed[0] == 3 && r->FrameForegroundTexturesUsed[1] == 1 && r->FrameForegroundTexturesUsed[2] == 5, "used order");
    Check(r->FrameForegroundTexturePriorities[0] == -1.0f && r->FrameForegroundTexturePriorities[3] == 2.0f, "prio values");
    unsigned short b[3] = { 1, 0, 2 };
    r->AddFrameForegroundTexturesUsed(b, 3, -4.0f);       // clamped to 0: min for 1, new for 0 and 2
    Check(r->FrameForegroundTexturePriorities[1] == 0.0f, "min kept");
    Check(r->FrameForegroundTexturesUsed.size() == 5, "used 5 (overflow grows)");
    Check(r->FrameForegroundTexturesUsed[3] == 0 && r->FrameForegroundTexturesUsed[4] == 2, "used tail");
    unsigned short c[1] = { 3 };
    r->AddFrameForegroundTexturesUsed(c, 1, 9.0f);        // larger priority: unchanged
    Check(r->FrameForegroundTexturePriorities[3] == 2.0f, "max ignored");
    Check(r->FrameForegroundTexturesUsed.size() == 5, "no re-add");
    Check(r->FrameForegroundTextureFlags[5] == 0, "flag fill 0");
    printf(g_fail ? "00b67ec0_TEST FAIL\n" : "00b67ec0_TEST PASS\n");
    return g_fail;
}