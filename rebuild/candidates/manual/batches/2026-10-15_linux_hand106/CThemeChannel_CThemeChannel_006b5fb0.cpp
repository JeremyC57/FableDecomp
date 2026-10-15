// CThemeChannel::CThemeChannel() -- default constructor: Colours and Scalers are left uninitialised; the three CIDrawEnvironmentTextureSets each get their two empty CArrays (Textures, Blends: three null pointers each)
// Leaf: ??0CThemeChannel
// Retail: Fable.exe @ 0x006B5FB0, 90 bytes (+ int3 padding). No callees.
// Layout: PDB CThemeChannel: Colours CRGBFloatColour[19] +0, Scalers float[30] +0x130, TextureSets
// CIDrawEnvironmentTextureSet[3] +0x1a8; the retail texture set is 0x18 bytes (two 12-byte STLport-style CArrays,
// PDB 0x20), so the class is modelled locally with the PDB names.
// Shape: the array of three texture sets is constructed by a fully unrolled inline loop: the first element through
// `this`, the second and third through an element pointer advanced by 0x18 (`lea edx,[eax+1A8h]`, `add edx,18h`), all
// eighteen words zeroed from one register. The second element's Textures stores land BEFORE the pointer advance
// (`[edx+18h]`) only with the real STLport vector structure (the wave-25 `_Vector_base` + `_STLP_alloc_proxy` model);
// a plain three-pointer CArray constructor advances first.
// The retail CArray<T> is an STLport vector: _Vector_base(const allocator&) nulls _M_start / _M_finish and builds the
// end-of-storage pointer inside an _STLP_alloc_proxy over the (empty) allocator from an allocator_type() temporary.
template <class T> struct allocator {
    allocator() { }
    allocator(const allocator&) { }
};

template <class Value, class T, class Alloc> struct _STLP_alloc_proxy : Alloc {
    Value _M_data;
    _STLP_alloc_proxy(const Alloc& a, Value p) : Alloc(a), _M_data(p) { }
};

template <class T> struct _Vector_base {
    typedef allocator<T> allocator_type;
    T* _M_start;                                   // +0x00
    T* _M_finish;                                  // +0x04
    _STLP_alloc_proxy<T*, T, allocator_type> _M_end_of_storage;   // +0x08
    _Vector_base(const allocator_type& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
};

template <class T> struct vector : _Vector_base<T> {
    typedef allocator<T> allocator_type;
    explicit vector(const allocator_type& a = allocator_type()) : _Vector_base<T>(a) { }
};

template <class T> struct CArray : vector<T> {     // retail 12 bytes
    CArray() { }
};

struct CRGBFloatColour {
    float R, G, B, A;
};

struct CIDrawEnvironmentTextureSet {
    CArray<long>  Textures;                        // +0x00
    CArray<float> Blends;                          // +0x0c
};

struct CThemeChannel {
    CRGBFloatColour             Colours[19];       // +0x000
    float                       Scalers[30];       // +0x130
    CIDrawEnvironmentTextureSet TextureSets[3];    // +0x1a8
    CThemeChannel();
};

CThemeChannel::CThemeChannel()
{
}
