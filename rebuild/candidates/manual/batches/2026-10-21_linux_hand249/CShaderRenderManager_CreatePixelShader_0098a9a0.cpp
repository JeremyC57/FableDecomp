// CShaderRenderManager::CreatePixelShader(const CArray<unsigned long>& code, const CArray<CCharString>& constNames, const CCharString& name) -- create the D3D pixel shader and wrap it with the PSConstNames indices of its named constants
// Leaf: CreatePixelShader
// Retail 0x0098A9A0, 345 bytes, `ret 0x10` (hidden CPixelShader result). Callees: IDirect3DDevice9::CreatePixelShader
// (COM slot 106, +0x1A8), CCharString copy 0x0099EC30 / dtor 0x0099EAE0, the PSConstNames index map's find 0x0042B467,
// the STLport vector<long> growth insert 0x00440990, CPixelShader(IDirect3DPixelShader9*, const CArray<long>&, const
// CCharString&) 0x00A5ECB0 (landed), operator delete 0x00BFEA14 (manifest "free"). Vtables: CPixelShader 0x0129A054,
// CShaderBase 0x0129A030.
// Layout: no engine header; the PDB D3DDevice +0x1E4 / PSConstNames +0x3C8 sit 0x10 / 0x14 higher than retail
// (D3DDevice +0x1D4 as in the landed UpdateAmbient 0x00989700; the name -> index map +0x3C0 as in the landed
// SetPixelShaderConstant 0x0098B390).
// Shape: the loop's CCharString copy is a named local; GetIndex is an inline `it != end ? it->index : -1`; the result is
// a named CPixelShader copied out (no NRVO) and destroyed through the inline release (DecRef / GetRefCount / delete
// this, the landed SetQuestInfoText ReleaseTemporary idiom). The same-TU noinline growth-insert stand-in is what lets
// VC7.1 home push_back's empty tag over the live CCharString in the dead `code` slot (declared-only: frame 0x1C).
#include <new>
struct IDirect3DPixelShader9;
struct IDirect3DDevice9;
struct IDirect3DDevice9Vtbl {
    void* _slots[106];
    long (__stdcall *CreatePixelShader)(IDirect3DDevice9* self, const unsigned long* function, IDirect3DPixelShader9** shader);   // slot 106 (+0x1a8)
};
struct IDirect3DDevice9 { IDirect3DDevice9Vtbl* lpVtbl; };

struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};

template<class T> struct CArray {
    T* first; T* last; T* end_of_storage;
    unsigned long Size() const { return (unsigned long)(last - first); }
    const T& operator[](unsigned long i) const { return first[i]; }
    const T* GetData() const { return Size() > 0 ? first : (const T*)this; }
};

struct SScalarTag { SScalarTag() { } };

struct CLongArray {                             // CArray<long>
    long* first; long* last; long* end_of_storage;
    CLongArray() : first(0), last(0), end_of_storage(0) { }
    ~CLongArray() { if (first) operator delete(first); }
    void InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00440990
    void push_back(const long& value)
    {
        if (last != end_of_storage) {
            new (last) long(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};

#pragma auto_inline(off)
// Same-TU stand-in for the STLport growth insert 0x00440990 (_M_insert_overflow): VC7.1 sees it never reads the tag.
__declspec(noinline) void CLongArray::InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail)
{
    unsigned long size = last - first;
    unsigned long len = size + (size > count ? size : count);
    long* start = (long*)operator new(len * sizeof(long));
    long* finish = start;
    for (long* p = first; p != where; ++p, ++finish)
        *finish = *p;
    for (unsigned long n = 0; n < count; ++n, ++finish)
        *finish = value;
    if (!noTail)
        for (long* p = where; p != last; ++p, ++finish)
            *finish = *p;
    if (first)
        operator delete(first);
    first = start;
    last = finish;
    end_of_storage = start + len;
}
#pragma auto_inline(on)

struct CNameIndexNode {                          // STLport _Rb_tree_node<pair<const CCharString, long>>
    long            color;
    CNameIndexNode* parent;
    CNameIndexNode* left;
    CNameIndexNode* right;
    CCharString     key;                         // +0x10
    long            index;                       // +0x14
};

struct CNameIndexMap {                           // map<CCharString, long>
    CNameIndexNode* header;                      // +0x00 end()
    unsigned long   count;
    unsigned long   compare;
    CNameIndexNode* find(const CCharString& name);   // 0x0042B467
    long GetIndex(const CCharString& name)
    {
        CNameIndexNode* it = find(name);
        if (it != header)
            return it->index;
        return -1;
    }
};

struct CPixelShaderData {
    virtual ~CPixelShaderData();
    IDirect3DPixelShader9* PixelShader;
    unsigned long          ReferenceCount;      // +0x08
    void DecRef() { --ReferenceCount; }
    unsigned long GetRefCount() const { return ReferenceCount; }
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            delete this;
    }
};

struct CShaderBase {                            // vtable 0x0129A030
    virtual ~CShaderBase() { }
};

struct CPixelShader : CShaderBase {             // vtable 0x0129A054
    CPixelShaderData* Data;
    CPixelShader(IDirect3DPixelShader9* shader, const CLongArray& constIndices, const CCharString& name);   // 0x00A5ECB0
    CPixelShader(const CPixelShader& other) : Data(other.Data) { if (Data) ++Data->ReferenceCount; }
    virtual ~CPixelShader()
    {
        if (Data)
            Data->ReleaseTemporary();
    }
};

struct CShaderRenderManager {
    unsigned char     _pad000[0x1d4];
    IDirect3DDevice9* D3DDevice;                // +0x1d4
    unsigned char     _pad1d8[0x1e8];
    CNameIndexMap     PSConstIndexMap;          // +0x3c0 (PSConstNames +0x3b4, its map at +0xc)
    CPixelShader CreatePixelShader(const CArray<unsigned long>& code, const CArray<CCharString>& constNames, const CCharString& name);
};

CPixelShader CShaderRenderManager::CreatePixelShader(const CArray<unsigned long>& code, const CArray<CCharString>& constNames, const CCharString& name)
{
    IDirect3DPixelShader9* shader = 0;
    D3DDevice->lpVtbl->CreatePixelShader(D3DDevice, code.GetData(), &shader);
    CLongArray indices;
    for (unsigned long i = 0; i < constNames.Size(); ++i) {
        CCharString constName(constNames[i]);
        long index = PSConstIndexMap.GetIndex(constName);
        if (index != -1)
            indices.push_back(index);
    }
    CPixelShader result(shader, indices, name);
    return result;
}
