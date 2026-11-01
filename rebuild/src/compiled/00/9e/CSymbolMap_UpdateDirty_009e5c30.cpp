// CSymbolMap::UpdateDirty() const -- sort the symbol map if entries were added dirty, then trim the map array's unused capacity
// Leaf: ?UpdateDirty@CSymbolMap@@QBEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E5C30, 39 bytes, thiscall, tail `jmp` to CArray::DeallocateUnusedMemory.
// Identity (proven, label corrected): the catalog's "CTCInventoryQuests::ProcessButtonBReleased" is a propagated label (that class's
//   ProcessButtonBReleased is 0x0061B5F0, vtable 0x012585B4 slot 59). Callers: CDefinitionManager::CreateSymbolsFromPathList 0x009AEF60
//   (twice), CDefinitionManager ctor 0x009B0470, CBankFile::CreateSymbolMap 0x009CC530, CParticleEmitterDatabase::CreateSymbolMap
//   0x00AE85F0 -- each right after a loop of AddSymbolDirty calls on a CSymbolMap. The body is CVectorMap::Sort on SLONGMap (+0x04) plus
//   SLONGMap.Data.DeallocateUnusedMemory(). Of the CSymbolMap names in Ego_r.pdb / FableWin.pdb only UpdateDirty / UpdateDirtyQuick fit
//   a sort, and both PDBs make UpdateDirty the larger (Ego_r 0x40 vs 0x30 slots, FableWin 0x03093780 / 0x030937C0 = 0x40 vs 0x30): this
//   39-byte sort + trim is UpdateDirty, the 33-byte sort-only body at 0x009E5C60 is UpdateDirtyQuick, and FableWin's UpdateDirty,
//   UpdateDirtyQuick order equals the retail order 0x009E5C30, 0x009E5C60. The landed spellings "?Finalise@CSymbolMap@@QAEXXZ" /
//   "?Finalise@CSymbolTable@@QAEXXZ" (callers) and "?UpdateDirty@CSymbolMap@@QBEXXZ" on 0x009E5C60 are wrong (NAME_FIXES.tsv).
// Callees: std::sort<pair<CCharString, CSymbolInfo>*, CCompareString<CSymbolInfo> > 0x009E7E40 (__fastcall: first in ecx, last in edx,
//   the empty comparator by value), CArray<pair<CCharString, CSymbolInfo> >::DeallocateUnusedMemory 0x009E7C70 (landed).
// Layout: Ego_r.pdb CSymbolMap (CBaseClassNonCopyable): SLONGMap (CVectorMap, 0x10: Data CArray 12, Compare byte +0x0C, Dirty +0x0D) at
//   +0x04, NamesUsed (set<CCharString>), CheckForDuplicateSymbols -- the trailing members are not modelled (no size claim). CSymbolInfo:
//   GroupName, EntryName, Val (pair size 0x10). No engine header exists for CSymbolMap.
// Shape: the landed UpdateDirtyQuick 0x009E5C60 model (mutable SLONGMap in a const member) with the Data trim added.
#include "engine/CBaseClassNonCopyable.h"
class CCharStringData;
class CCharString {
public:
    bool FastCompare(const CCharString& other) const;
private:
    CCharStringData* PStringData;
};
namespace std {
template <class _Tp> class allocator { public: };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    _Tp* begin() { return this->_M_start; }
    _Tp* end() { return this->_M_finish; }
};
template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x009E7E40
}
template <class T> class CArray : public std::vector<T> {
public:
    void DeallocateUnusedMemory();                         // 0x009E7C70
};
template <class K, class V> class CKeyPairCompareLess { };
template <class V> class CCompareString {
public:
    bool operator()(const std::pair<CCharString, V>& a, const std::pair<CCharString, V>& b) const { return a.first.FastCompare(b.first); }
};
template <class K, class V, class Cmp = CKeyPairCompareLess<K, V> > class CVectorMap {
public:
    void Sort()
    {
        if (Dirty) {
            std::sort(Data.begin(), Data.end(), Compare);
            Dirty = false;
        }
    }
    CArray<std::pair<K, V> > Data;                         // +0x00
    Cmp Compare;                                           // +0x0C
    bool Dirty;                                            // +0x0D
};
class CSymbolMap : public CBaseClassNonCopyable {
public:
    class CSymbolInfo {
    public:
        CCharString GroupName;
        CCharString EntryName;
        long Val;
    };
    void UpdateDirty() const;
private:
    mutable CVectorMap<CCharString, CSymbolInfo, CCompareString<CSymbolInfo> > SLONGMap;   // +0x04
};

void CSymbolMap::UpdateDirty() const
{
    SLONGMap.Sort();
    SLONGMap.Data.DeallocateUnusedMemory();
}