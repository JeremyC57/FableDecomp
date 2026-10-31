// CEnginePrimitiveRenderer::ProcessPrimitiveStats(const std::map<unsigned long, unsigned long>& stats, EEnginePrimitiveStatIndex statIndex, unsigned long& total, CArray<CCharString>& names) const -- turn a collected primitive-stats map into display lines: zero the total and empty the name list, then for every (key, size) entry add the size to the total and, for stat index 0 / 1 (meshes: the key is the C3DMeshInfo), push the mesh's bank symbol, for stat index 2 (textures: the key is a main graphic bank entry) push "<first source filename> -> <size as bytes/KB/MB, 2 decimals>"
// Grade: DIFFER(396v396) -- frame packing only: every instruction, register and branch matches; retail homes the texture line in the dead `total` argument slot, the mesh symbol in the dead `names` slot and the empty __false_type tag in the `stats` slot (frame 8), where VC7.1 here keeps line / symbol / both string temporaries in a 0x10 frame and puts the tag in `total`'s slot, so every esp offset differs. Tried (4 grades): unnamed GetEntrySymbol temporary (385v396, push_back then reuses the returned pointer instead of re-leaing the slot), named symbol local (396v396, this file), trivial __false_type without a user ctor (401v396, worse), Ego_r CArray::push_back wrapper over vector::push_back (396v396, identical bytes; kept as PDB-backed).
// Leaf: ?ProcessPrimitiveStats@CEnginePrimitiveRenderer@@QBEXABV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@W4EEnginePrimitiveStatIndex@@AAKAAV?$CArray@VCCharString@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B33390, 396 bytes, thiscall `ret 10h` (`this` unused: a const method that reads only
//   its arguments and the resource-manager global).
// Identity (proven, two sources): (1) Ego_r.pdb and FableWin.pdb both carry
//   ?ProcessPrimitiveStats@CEnginePrimitiveRenderer@@QBEXABV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@W4EEnginePrimitiveStatIndex@@AAKAAV?$CArray@VCCharString@@@@@Z
//   right after CollectPrimitiveStatsForArea, and CEngine has the same pair as virtuals; the body is exactly that signature
//   (four stack arguments = `ret 10h`; map<K,K> walk; enum compared signed 0 / 1 / 2; unsigned long& accumulated;
//   CArray<CCharString> cleared and appended). (2) The CEngine forwarder at 0x00B23CA0 is `mov ecx,[0x01436E80] ; jmp
//   0x00B33390` right after the forwarder 0x00B23C90 to CollectPrimitiveStatsForArea 0x00B321B0, the PDB's virtual
//   order; 0x01436E80 is the primitive-renderer global; the keys match the producer CEngineInternalPrimitiveMeshBase::
//   CollectStats 0x00BEC680 (stat 0 / 1 key = the C3DMeshInfo address, stat 2 key = a graphic bank entry, value = a data
//   size). Manifest label "ProcessMorphStats" is wrong (class and method).
// Callees: CArray<CCharString> (STLport vector) erase(first, last) 0x0043336A and _M_insert_overflow 0x00433530 (bound
//   spellings from 0x00521EB0 / 0x004B2850), CCharString copy ctor 0x0099EC30, ~CCharString 0x0099EAE0, operator+=
//   0x0099F0A0, operator+(const char*, const CCharString&) 0x0099F690, GFByteSizeToCharString 0x0099FA90 (Ego_r name),
//   CBankFile::GetEntrySymbol 0x009CC1D0. Global g_pResourceManager 0x01436E98 (landed spelling, CEngine_SetVideoOptions
//   0x00B24690); literal " -> " 0x012A133C.
// Layout: CMeshDataBankEntry {vptr, Bank +4, Index +8} is C3DMeshInfo's RTTI base at 0; CMeshDataBank / CGraphicDataBank
//   derive from CBankFileAsync -> CBankFile at offset 0 (RTTI); CBankFile::UpdateData (retail +0x2C, CArray of
//   CBankFileEntryUpdateData*), whose Filenames (CSmallVector<CCharString,8>, Start first) sits at +0x0C (Ego_r).
// Shape: the temporaries of the texture line live in the two frame words, the line in the dead `total` slot, the mesh
//   symbol in the dead `names` slot; both arms share the push_back tail and one string destructor call.
#include <stddef.h>
#define CEnginePrimitiveRenderer CEnginePrimitiveRendererDonor
#include "engine/CEnginePrimitiveRenderer.h"
#undef CEnginePrimitiveRenderer
#define CBankFile CBankFileDonor
#include "engine/CBankFile.h"
#undef CBankFile
#include "engine/CMeshDataBankEntry.h"
#include "engine/CEngineResourceManager.h"

inline void* __cdecl operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    CCharString(const CCharString& other);                       // 0x0099EC30
    ~CCharString();                                              // 0x0099EAE0
    const CCharString& operator+=(const CCharString& other);     // 0x0099F0A0
    void* PStringData;
};
CCharString __fastcall operator+(const char* left, const CCharString& right);      // 0x0099F690
CCharString __fastcall GFByteSizeToCharString(unsigned long size, long decimals);  // 0x0099FA90

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp> struct less { bool operator()(const _Tp& x, const _Tp& y) const { return x < y; } };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Value _M_data; };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    _Tp* begin() { return this->_M_start; }
    _Tp* end() { return this->_M_finish; }
    _Tp* erase(_Tp* __first, _Tp* __last);                       // 0x0043336A
    void clear() { erase(begin(), end()); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);               // 0x00433530
};
struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    const _Value& operator*() const { return static_cast<_Rb_tree_node<_Value>*>(_M_node)->_M_value_field; }
    const _Value* operator->() const { return &**this; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
    _Rb_tree_iterator& operator++()
    {
        _Rb_tree_node_base* __x = _M_node;
        if (__x->_M_right != 0) {
            __x = __x->_M_right;
            while (__x->_M_left != 0)
                __x = __x->_M_left;
        }
        else {
            _Rb_tree_node_base* __y = __x->_M_parent;
            while (__x == __y->_M_right) {
                __x = __y;
                __y = __y->_M_parent;
            }
            if (__x->_M_right != __y)
                __x = __y;
        }
        _M_node = __x;
        return *this;
    }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_iterator<value_type> const_iterator;
    _STLP_alloc_proxy<_Rb_tree_node_base*, _Alloc> _M_header;   // STLport _Rb_tree: header, node count, compare
    unsigned int _M_node_count;
    const_iterator begin() const { const_iterator it; it._M_node = _M_header._M_data->_M_left; return it; }
    const_iterator end() const { const_iterator it; it._M_node = _M_header._M_data; return it; }
};
}

template <class T> class CArray : public std::vector<T> {
public:
    void push_back(const T& value) { std::vector<T>::push_back(value); }   // Ego_r ?push_back@?$CArray@...@@QAEXABV..@@Z
};

enum EEnginePrimitiveStatIndex { };

class CBankFileEntryUpdateData {
public:
    unsigned long StateBlockCRC;                                 // +0x00
    unsigned long InfoSize;                                      // +0x04
    char* InfoBuffer;                                            // +0x08
    CCharString* Filenames_Start;                                // +0x0C CSmallVector<CCharString,8>::Start
};
class CBankFile : public CBankFileDonor {
public:
    CCharString GetEntrySymbol(unsigned long index) const;       // 0x009CC1D0
    CBankFileEntryUpdateData* GetUpdateData(unsigned long index) const
    {
        return (*reinterpret_cast<CBankFileEntryUpdateData** const*>(UpdateData))[index];   // CArray begin, +0x2C
    }
};
class CBankFileAsync : public CBankFile { };
class CMeshDataBank : public CBankFileAsync { };
class CGraphicDataBank : public CBankFileAsync { };

extern CEngineResourceManager* g_pResourceManager;              // 0x01436E98

class CEnginePrimitiveRenderer : public CEnginePrimitiveRendererDonor {
public:
    void ProcessPrimitiveStats(const std::map<unsigned long, unsigned long>& stats, EEnginePrimitiveStatIndex statIndex,
                               unsigned long& total, CArray<CCharString>& names) const;
};

void CEnginePrimitiveRenderer::ProcessPrimitiveStats(const std::map<unsigned long, unsigned long>& stats,
                                                     EEnginePrimitiveStatIndex statIndex, unsigned long& total,
                                                     CArray<CCharString>& names) const
{
    total = 0;
    names.clear();
    for (std::map<unsigned long, unsigned long>::const_iterator it = stats.begin(); it != stats.end(); ++it) {
        const unsigned long key = it->first;
        const unsigned long size = it->second;
        switch (statIndex) {
        case 0:
        case 1: {
            const CMeshDataBankEntry* mesh = reinterpret_cast<const CMeshDataBankEntry*>(key);
            CCharString symbol(mesh->Bank->GetEntrySymbol(mesh->Index));
            names.push_back(symbol);
            break;
        }
        case 2: {
            CCharString line(g_pResourceManager->GraphicBanks[0]->GetUpdateData(key)->Filenames_Start[0]);
            line += " -> " + GFByteSizeToCharString(size, 2);
            names.push_back(line);
            break;
        }
        }
        total += size;
    }
}
