// CTextureMorphs::AddMorph(unsigned long textureIndex) -- append an empty texture morph record (the texture index, morph 0, no blends) to the vector and return it
// Leaf: ?AddMorph@
// Grade: DIFFER(128v138) -- the in-place arm: retail stores MorphID from the zero register (edi) and has no temporary
//        destructor there (it knows the blends pointer is still 0 after the out-of-line vector copy), with its own
//        `++finish; return finish - 1` tail; ours reloads MorphID from the frame and merges both arms into one
//        destructor + back() tail. Tried: unnamed temporary (128), const named local (130), the vector copy constructor
//        visible but not inlined (128), the PDB (long, long) constructor (128).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CA8C0, length 138 bytes (+ int3 padding), thiscall `ret 4`. Single
// caller: CTextureMorphs::AddBlend 0x006CA950 (landed; its model declares `CEngineTextureMorph* AddMorph(unsigned long
// textureIndex); // 0x006CA8C0`). `this` is the STLport vector<CEngineTextureMorph> itself; the class and method
// names are the landed caller's descriptive ones. The manifest label "Map_InsertEntry" is descriptive.
// Callees: vector<CCompositeBlendDesc>'s copy constructor @ 0x005BB346 (out of line), vector<CEngineTextureMorph>::
// _M_insert_overflow(pos, x, const __false_type&, 1, true) @ 0x006CB390, free @ 0x00BFEA14 (the temporary's blends).
// Layout: CEngineTextureMorph (Ego_r.pdb: OriginalTextureIndex +0, MorphID +4, vector<CCompositeBlendDesc> Blends +8;
// 20 bytes in retail); STLport vector {_M_start, _M_finish, _M_end_of_storage}.
// Shape: `push_back(CEngineTextureMorph(textureIndex)); return &back();` -- the temporary is built in the frame
// ({index, 0, 0, 0, 0}); STLport push_back inlines: the in-place arm stores the two known dwords straight from
// registers and copies the (empty) blends through the out-of-line vector copy, the overflow arm passes the temporary
// and a fresh tag homed over the dead argument slot. VC7.1 keeps the temporary's destructor (the null-tested free)
// only on the overflow arm: on the in-place arm it knows the blends pointer is still the 0 it stored.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

struct CRGBColour {                                   // D3DCOLOR byte order
    unsigned char B, G, R, A;
};

struct CCompositeBlendDesc {                          // Ego_r.pdb, 16 bytes
    unsigned long TextureIndex;                       // +0x00
    long          BlendType;                          // +0x04 ECompositeBlendType
    CRGBColour    DiffuseColour;                      // +0x08
    CRGBColour    SpecularColour;                     // +0x0C
};

namespace std {
struct __false_type { __false_type() { } };

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class T> struct allocator {
    void deallocate(T* p, unsigned int) { if (p) free(p); }
};

template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    struct : A { T* _M_data; } _M_end_of_storage;
    typedef unsigned int size_type;
    vector() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    vector(const vector& x);                          // 0x005BB346 for vector<CCompositeBlendDesc>
    ~vector() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    T& back() { return *(_M_finish - 1); }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, size_type fill_len,
                            bool atend = false);      // 0x006CB390 for vector<CEngineTextureMorph>
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            _Construct(_M_finish, x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);
    }
};
}

struct CEngineTextureMorph {                          // Ego_r.pdb; 20 bytes in retail
    long                                  OriginalTextureIndex;   // +0x00
    long                                  MorphID;                // +0x04
    std::vector<CCompositeBlendDesc>      Blends;                 // +0x08
    CEngineTextureMorph(long originalTextureIndex, long morphID)   // PDB ??0CEngineTextureMorph@@QAE@JJ@Z
        : OriginalTextureIndex(originalTextureIndex), MorphID(morphID) { }
};

class CTextureMorphs : public std::vector<CEngineTextureMorph> {
public:
    CEngineTextureMorph* AddMorph(unsigned long textureIndex);
};

CEngineTextureMorph* CTextureMorphs::AddMorph(unsigned long textureIndex)
{
    push_back(CEngineTextureMorph(textureIndex, 0));
    return &back();
}
