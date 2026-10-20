// Grade: DIFFER(292v280) -- register allocation of the two loops (retail: ebp walks Blocks for the inner loop and holds the kept finish pointer, edi the index; zero result words through edi/ebp); first form kept
// CVertexGroupBlockList::RemoveAllSubsets() -- drop every block whose groups are a proper subset of another block's: keep the rest in order in a new list and assign it back
// Leaf: RemoveAllSubsets
extern "C" void __cdecl free(void* block);        // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
struct CULongVector {                              // vector<unsigned long> (12 bytes)
    unsigned long* m_first;
    unsigned long* m_last;
    unsigned long* m_end;
    CULongVector(const CULongVector& other);       // 0x00535470
};
struct CGroupToIndexMap {                          // map<unsigned long, long> (12 bytes)
    void*         Header;
    unsigned long NodeCount;
    unsigned long Compare;
    CGroupToIndexMap(const CGroupToIndexMap& other);   // 0x00A96E40
};
struct CVertexGroupBlock {                         // retail 0x18 bytes
    CULongVector     Groups;                       // +0x00
    CGroupToIndexMap GroupToIndexMap;              // +0x0c
    CVertexGroupBlock(const CVertexGroupBlock& other) : Groups(other.Groups), GroupToIndexMap(other.GroupToIndexMap) { }
    bool IsProperSubsetOf(const CVertexGroupBlock& other) const;   // 0x00AD8CA0
};
void __fastcall DestroyBlocks(CVertexGroupBlock* first, CVertexGroupBlock* last, const __false_type& tag);   // 0x00ADC190 (_Destroy range)
template <class T> struct vector {                 // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector()
    {
        DestroyBlocks(_M_start, _M_finish, __false_type());
        if (_M_start)
            free(_M_start);
    }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) { return *(_M_start + n); }
    vector& operator=(const vector& other);        // 0x00ADCA30
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned long fill_len, bool atend);   // 0x00ADCE60
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
struct CVertexGroupBlockList {
    vector<CVertexGroupBlock> Blocks;              // +0x00 (CArray<CVertexGroupBlock>)
    void RemoveAllSubsets();
};
void CVertexGroupBlockList::RemoveAllSubsets()
{
    vector<CVertexGroupBlock> kept;
    for (unsigned long i = 0; i < Blocks.size(); ++i) {
        const CVertexGroupBlock& block = Blocks[i];
        unsigned long count = Blocks.size();
        for (unsigned long j = 0; j < count; ++j) {
            if (block.IsProperSubsetOf(Blocks[j]))
                goto next;
        }
        kept.push_back(block);
    next:;
    }
    Blocks = kept;
}
