// CShaderRenderManager::CreatePixelShader(const CArray<unsigned long>& code, const CArray<CCharString>& constNames, const CCharString& name) -- create the D3D pixel shader and wrap it with the PSConstNames indices of its named constants
// Leaf: ?CreatePixelShader@CShaderRenderManager@@AAE?AVCPixelShader@@ABV?$CArray@K@@ABV?$CArray@VCCharString@@@@ABVCCharString@@@Z
// Naming wave 27 (family pxs, DECISIONS_W26 D4): CreatePixelShader and the CPixelShader ctor 0x00A5ECB0 are
//   private (Ego_r.pdb / FableWin.pdb AAE); CPixelShader declares CShaderRenderManager a friend. Was public (QAE).
// Naming wave 26 (family vlong): the vector is STLport 4.6 std::vector<long, allocator<long> >; its grow-insert 0x00440990 is the
//   protected ?_M_insert_overflow@?$vector@JV?$allocator@J@std@@@std@@IAEXPAJABJABU__true_type@2@I_N@Z (size_type unsigned int; the
//   own row's spelling, c689 queue). Was CLongArray::InsertGrow(..., const SScalarTag&, unsigned long, bool). The local is CArray<long>
//   (CArray<T> : std::vector<T>, the landed 0x00B1B380 model), the CPixelShader ctor 0x00A5ECB0's PDB parameter type.
// Retail 0x0098A9A0, 345 bytes, `ret 0x10` (hidden CPixelShader result). Callees: IDirect3DDevice9::CreatePixelShader
// (COM slot 106, +0x1A8), CCharString copy 0x0099EC30 / dtor 0x0099EAE0, the PSConstNames index map's find 0x0042B467,
// the STLport vector<long> growth insert 0x00440990, CPixelShader(IDirect3DPixelShader9*, const CArray<long>&, const
// CCharString&) 0x00A5ECB0 (landed), CRT free 0x00BFEA14 (the import thunk; written free()). Vtables: CPixelShader 0x0129A054,
// CShaderBase 0x0129A030.
// Layout: no engine header; the PDB D3DDevice +0x1E4 / PSConstNames +0x3C8 sit 0x10 / 0x14 higher than retail
// (D3DDevice +0x1D4 as in the landed UpdateAmbient 0x00989700; the name -> index map +0x3C0 as in the landed
// SetPixelShaderConstant 0x0098B390).
// Shape: the loop's CCharString copy is a named local; GetIndex is an inline `it != end ? it->index : -1`; the result is
// a named CPixelShader copied out (no NRVO) and destroyed through the inline release (DecRef / GetRefCount / delete
// this, the landed SetQuestInfoText ReleaseTemporary idiom). The same-TU noinline growth-insert stand-in is what lets
// VC7.1 home push_back's empty tag over the live CCharString in the dead `code` slot (declared-only: frame 0x1C).
#include <new>
#include <stdlib.h>
struct IDirect3DPixelShader9;
struct IDirect3DDevice9;
struct IDirect3DDevice9Vtbl {
    void* _slots[106];
    long (__stdcall *CreatePixelShader)(IDirect3DDevice9* self, const unsigned long* function, IDirect3DPixelShader9** shader);   // slot 106 (+0x1a8)
};
struct IDirect3DDevice9 { IDirect3DDevice9Vtbl* lpVtbl; };

class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};



namespace std {
template <class _Tp> class allocator { public: allocator() { } };
struct __true_type { __true_type() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { // CArray<long>
public:
    _Tp* first; _Tp* last; _Tp* end_of_storage;
    vector() : first(0), last(0), end_of_storage(0) { }
    ~vector() { if (first) free(first); }
protected:
    void _M_insert_overflow(_Tp* where, const _Tp& value, const __true_type& tag, unsigned int count, bool noTail);   // 0x00440990
public:
    void push_back(const _Tp& value)
    {
        if (last != end_of_storage) {
            new (last) _Tp(value);
            ++last;
        }
        else
            _M_insert_overflow(last, value, __true_type(), 1, true);
    }
};
}


#pragma auto_inline(off)
// Same-TU stand-in for the STLport growth insert 0x00440990 (_M_insert_overflow): VC7.1 sees it never reads the tag.
template <class _Tp, class _Alloc>
__declspec(noinline) void std::vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* where, const _Tp& value, const std::__true_type& tag, unsigned int count, bool noTail)
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
        free(first);
    first = start;
    last = finish;
    end_of_storage = start + len;
}
#pragma auto_inline(on)

template<class T> class CArray : public std::vector<T, std::allocator<T> > { public:   // Fable's CArray over the STLport vector
    unsigned long Size() const { return (unsigned long)(this->last - this->first); }
    const T& operator[](unsigned long i) const { return this->first[i]; }
    const T* GetData() const { return Size() > 0 ? this->first : (const T*)this; }
};

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

class CShaderBase { public:                            // vtable 0x0129A030
    virtual ~CShaderBase() { }
};

class CPixelShader : public CShaderBase { public:             // vtable 0x0129A054
    CPixelShaderData* Data;
private:
    CPixelShader(IDirect3DPixelShader9* shader, const CArray<long>& constIndices, const CCharString& name);   // 0x00A5ECB0 (private, AAE)
    friend class CShaderRenderManager;
public:
    CPixelShader(const CPixelShader& other) : Data(other.Data) { if (Data) ++Data->ReferenceCount; }
    virtual ~CPixelShader()
    {
        if (Data)
            Data->ReleaseTemporary();
    }
};

class CShaderRenderManager { public:
    unsigned char     _pad000[0x1d4];
    IDirect3DDevice9* D3DDevice;                // +0x1d4
    unsigned char     _pad1d8[0x1e8];
    CNameIndexMap     PSConstIndexMap;          // +0x3c0 (PSConstNames +0x3b4, its map at +0xc)
private:                                        // private (Ego_r.pdb / FableWin.pdb AAE)
    CPixelShader CreatePixelShader(const CArray<unsigned long>& code, const CArray<CCharString>& constNames, const CCharString& name);
};

CPixelShader CShaderRenderManager::CreatePixelShader(const CArray<unsigned long>& code, const CArray<CCharString>& constNames, const CCharString& name)
{
    IDirect3DPixelShader9* shader = 0;
    D3DDevice->lpVtbl->CreatePixelShader(D3DDevice, code.GetData(), &shader);
    CArray<long> indices;
    for (unsigned long i = 0; i < constNames.Size(); ++i) {
        CCharString constName(constNames[i]);
        long index = PSConstIndexMap.GetIndex(constName);
        if (index != -1)
            indices.push_back(index);
    }
    CPixelShader result(shader, indices, name);
    return result;
}