#ifndef FABLE_ENGINE_CPREALLOCVECTORMAP_H
#define FABLE_ENGINE_CPREALLOCVECTORMAP_H
// CPreAllocVectorMap<K,V,CompareType> (chunk 868, row 0x00A2F740): a sorted pair array over pre-allocated storage. Layout (Ego_r.pdb
// through the landed C3DMeshStats rows 0x00A2E900 / 0x00A2E760): CompareType Compare +0x0, pair<K,V>* Elements +0x4, unsigned long
// Size +0x8, bool Dirty +0xC; sizeof 0x10. FableWin.pdb members (the <unsigned long, C3DHelperPoint> instance): ??0 (default), Count,
// Find, GetAsCArray, GetByteSize, Initialise(pair*, unsigned long), LowerBound / UpperBound (out of line in retail: 0x00A316B0 /
// 0x00A31700 helper points, 0x00A31790 / 0x00A317E0 dummy objects, all landed), RelocateData(void*, void*), SetDirty(bool), Sort(),
// begin / end (const and non-const), size. Retail 0x00A2F740 expands Sort as `std::sort(begin(), end(), Compare)` (0x00A35150 /
// 0x00A351B0 instantiations) followed by the Dirty clear, and the pair copies into Elements as std::copy (0x00A31150 / 0x00A311B0).
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/CVectorMap.h"
template<class K, class V, class CompareType = CKeyPairCompareLess<K, V> >
class CPreAllocVectorMap {
public:
    typedef std::pair<K, V> value_type;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
    CPreAllocVectorMap() : Elements(0), Size(0), Dirty(false) { }
    void Initialise(value_type* elements, unsigned long size) { Elements = elements; Size = size; Dirty = true; }
    iterator begin() { return Elements; }
    const_iterator begin() const { return Elements; }
    iterator end() { return Elements + Size; }
    const_iterator end() const { return Elements + Size; }
    unsigned long size() const { return Size; }
    unsigned long GetByteSize() const { return Size * sizeof(value_type); }
    value_type* GetAsCArray() { return Elements; }
    void SetDirty(bool dirty) { Dirty = dirty; }
    void Sort() { std::sort(Elements, Elements + Size, Compare); Dirty = false; }
    const_iterator LowerBound(const K& key) const;
    const_iterator UpperBound(const K& key) const;
    long Count(const K& key) const { return (long)(UpperBound(key) - LowerBound(key)); }
    const_iterator Find(const K& key) const { const_iterator i = LowerBound(key); return i == end() || key < i->first ? end() : i; }
    void RelocateData(void* oldBase, void* newBase) { Elements = (value_type*)((char*)Elements + ((char*)newBase - (char*)oldBase)); }
private:
    CompareType Compare;                                                 // +0x00
    value_type* Elements;                                                // +0x04
    unsigned long Size;                                                  // +0x08
    bool Dirty;                                                          // +0x0C
};
#endif // FABLE_ENGINE_CPREALLOCVECTORMAP_H
