// CEngineInternalPrimitiveParticleGroup::ClearAllChildPrimitives() -- empty the primitive-handle list, the id-to-iterator table and the free-id list (each cleared, then resized to zero)
// Leaf: ClearAllChildPrimitives
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BBD200, 190 bytes.
// Callees: list<CEnginePrimitiveHandle>::clear 0x00834F90 and ::resize(size_type) 0x00BBDCB0 (landed), the iterator vector's
//   erase(first, last) 0x00BBD7B0 (landed) and insert(position, n, const T&) 0x00BBD950, list<long>::resize(size_type,
//   const long&) 0x00BBDA70, free 0x00BFEA14.
// Layout (retail, 0x14 below the PDB from +0x48 as in the landed 0x00BBC310 / 0x00BBD1A0): Primitives +0x48 (4-byte STLport
//   list), PrimitiveIDToIterator +0x4c (12-byte vector of 8-byte {used byte, list node} iterators), FreePrimitiveIDs +0x58
//   (4-byte list). engine/CEngineInternalPrimitiveParticleGroup.h has the PDB offsets, so `this` is a local model.
// Shape: each container is `clear(); resize(0);`. The vector's in-class erase is inlined at its first use (clear) and called
//   out of line at its second (inside resize: `0 < size() ? erase(begin(), end()) : insert(end(), 0 - size(), T())`, the
//   default iterator temporary's used byte zeroed first); the list<long> clear is inline (free each node, reset the
//   sentinel), its resize out of line with a zero long temporary in the same frame word.
extern "C" void __cdecl free(void* block);             // 0x00BFEA14

struct CHandleList {                                   // list<CEnginePrimitiveHandle> (4 bytes)
    void* _M_node;
    void clear();                                      // 0x00834F90
    void resize(unsigned long count);                  // 0x00BBDCB0
};

struct CPrimitiveIterator {                            // 8 bytes: in-use flag and the list node
    unsigned char m_used;
    void*         m_pNode;
    CPrimitiveIterator() : m_used(0) { }
};

struct CIteratorVector {                               // vector<CPrimitiveIterator> (12 bytes)
    CPrimitiveIterator* _M_start;
    CPrimitiveIterator* _M_finish;
    CPrimitiveIterator* _M_end_of_storage;
    CPrimitiveIterator* begin() { return _M_start; }
    CPrimitiveIterator* end() { return _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
    static CPrimitiveIterator* copy(CPrimitiveIterator* first, CPrimitiveIterator* last, CPrimitiveIterator* result)
    {
        for (long n = last - first; n > 0; --n) {
            *result = *first;
            ++first;
            ++result;
        }
        return result;
    }
    CPrimitiveIterator* erase(CPrimitiveIterator* first, CPrimitiveIterator* last)   // 0x00BBD7B0 when out of line
    {
        CPrimitiveIterator* i = copy(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void insert(CPrimitiveIterator* position, unsigned long n, const CPrimitiveIterator& x);   // 0x00BBD950
    void clear() { erase(begin(), end()); }
    void resize(unsigned long newSize, const CPrimitiveIterator& x = CPrimitiveIterator())
    {
        if (newSize < size())
            erase(begin() + newSize, end());
        else
            insert(end(), newSize - size(), x);
    }
};

struct SLongListNode {
    SLongListNode* _M_next;
    SLongListNode* _M_prev;
    long           _M_data;
};
struct CLongList {                                     // STLport list<long> (4 bytes: the sentinel node)
    SLongListNode* _M_node;
    void clear()
    {
        SLongListNode* cur = _M_node->_M_next;
        while (cur != _M_node) {
            SLongListNode* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    void resize(unsigned long newSize, const long& x = long());   // 0x00BBDA70
};

struct CEngineInternalPrimitiveParticleGroup {
    unsigned char   _pad00[0x48];
    CHandleList     Primitives;                        // +0x48
    CIteratorVector PrimitiveIDToIterator;             // +0x4c
    CLongList       FreePrimitiveIDs;                  // +0x58
    void ClearAllChildPrimitives();
};

void CEngineInternalPrimitiveParticleGroup::ClearAllChildPrimitives()
{
    Primitives.clear();
    Primitives.resize(0);
    PrimitiveIDToIterator.clear();
    PrimitiveIDToIterator.resize(0);
    FreePrimitiveIDs.clear();
    FreePrimitiveIDs.resize(0);
}
