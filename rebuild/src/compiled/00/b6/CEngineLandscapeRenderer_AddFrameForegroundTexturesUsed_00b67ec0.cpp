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