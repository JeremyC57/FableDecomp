// Grade: DIFFER(422v422) -- frame slots only: the empty __true_type tag of push_back's _M_insert_overflow takes local +0x13 (retail aliases it onto the `priority` parameter slot, [esp+0x38] at the push) so the resize fill byte moves to +0x12; every instruction otherwise identical
// CEngineLandscapeRenderer::AddFrameForegroundTexturesUsed(const unsigned short* textures, long count, float priority) -- clamp the
//   priority to >= 0; for each texture index grow the per-texture priority (fill -1.0f) and flag (fill 0) vectors to cover it; the
//   first use this frame (priority < 0) appends the index to the used list and stores the priority, later uses keep the minimum.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B67EC0, 422 bytes (+int3), __thiscall `ret 0xc`; caller CEngineLandscapePatch::LoadTextures 0x00BF4130.
// Callees: memmove 0x00BFEAE6 (STLport erase/__copy_trivial, twice), vector<float>::_M_fill_insert 0x00441020,
//   vector<unsigned char>::_M_fill_insert 0x00485F3A, vector<unsigned long>::_M_insert_overflow 0x00464ACE.
// Layout: local model. engine/CEngineLandscapeRenderer.h (Ego_r.pdb) has MeshBuilder over +0x1c..+0xcf4; the TLC build keeps three
//   STLport vectors at +0x68c (used list), +0x698 (priorities), +0x6a4 (flags) -- names descriptive.
// Tried: texture as unsigned short (tag lands on the parameter slot like retail but the ushort->ulong push_back temporary costs a
//   `mov edx,ebx` copy, 429v422), int/unsigned/long texture (no copy, but the value-initialised tag gets a slot + zero store),
//   tag with a user ctor / uninitialised named tag / tag-returning helper / wrapper member (slot stays local), push_back by value,
//   _Construct helper, vector<int>/<long>/<bool> element types.
#include <new>
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
    void _M_insert_overflow(T* position, const T& x, const __true_type& tag, unsigned int n, bool atend);
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
