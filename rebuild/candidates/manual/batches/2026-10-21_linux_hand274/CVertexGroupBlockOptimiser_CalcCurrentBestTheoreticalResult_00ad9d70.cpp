// CVertexGroupBlockOptimiser::CalcCurrentBestTheoreticalResult() -- recompute the block segments, then the lower bound on the block count: the existing blocks, the full blocks each segment needs, and the minimum packing of the segment remainders
// Leaf: CalcCurrentBestTheoreticalResult
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD9D70, 404 bytes + int3, plain `ret`, returns unsigned long.
// The catalog row says CountOptimisedBlocks; the PDB's ?CalcCurrentBestTheoreticalResult@CVertexGroupBlockOptimiser@@AAEKXZ
// is the no-argument unsigned long method that calls CalcCurrentBlockSegments and CalcMinimumBlocks.
// Callees: CalcCurrentBlockSegments @ 0x00AD9B20, vector<unsigned long>::reserve @ 0x009B4030, its _M_insert_overflow
// @ 0x00464ACE, malloc @ 0x00BFEA0E (the inlined CArray<bool>(n, true) storage, filled by rep stos), CalcMinimumBlocks(
// const CArray<unsigned long>&, unsigned long&, CArray<bool>&, unsigned long, unsigned long) @ 0x00AD89B0, free @ 0x00BFEA14.
// Layout: as CalcCurrentBlockSegments (retail, 12-byte containers, 4-byte lists): Blocks list +0x10, CurrentBlock +0x14,
// CurrentBlockSegmentGroups +0x24, CurrentBlockSegmentCount +0x3c, MaxGroupIndex +0x40, MaxN +0x44.
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);            // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                     // 0x00BFEA14

struct __false_type { __false_type() { } };
struct __true_type { __true_type() { } };

struct CRbNode {
    unsigned char _M_color;
    CRbNode* _M_parent;
    CRbNode* _M_left;
    CRbNode* _M_right;
};

struct CGroupArray {                                           // CArray<unsigned long>
    unsigned long* _M_start;
    unsigned long* _M_finish;
    unsigned long* _M_end_of_storage;
    CGroupArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    CGroupArray(const CGroupArray& other);                     // 0x00535470
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    ~CGroupArray() { if (_M_start) free(_M_start); }
};

struct CGroupToIndexMap {                                      // map<unsigned long, long> (STLport _Rb_tree)
    CRbNode* _M_header;
    unsigned long _M_node_count;
    unsigned long _M_key_compare;
    CGroupToIndexMap() : _M_header(0)
    {
        _M_header = (CRbNode*)malloc(sizeof(CRbNode) + 8);
        _M_node_count = 0;
        _M_header->_M_color = 0;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
    CGroupToIndexMap(const CGroupToIndexMap& other);           // 0x00A96E40
    ~CGroupToIndexMap()
    {
        clear();
        if (_M_header)
            free(_M_header);
    }
    void _M_erase(CRbNode* x);                                 // 0x004507A1
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
};

class CVertexGroupBlock {
public:
    CGroupArray Groups;                                        // +0x00
    CGroupToIndexMap GroupToIndexMap;                          // +0x0c
};

CVertexGroupBlock* __fastcall CopyBlocks(CVertexGroupBlock* first, CVertexGroupBlock* last, CVertexGroupBlock* result,
                                         const __false_type& tag, int* distance);   // 0x00ADC0D0
void __fastcall DestroyBlocks(CVertexGroupBlock* first, CVertexGroupBlock* last, const __false_type& tag);   // 0x00ADC190

struct CBlockArray {                                           // CArray<CVertexGroupBlock>
    CVertexGroupBlock* _M_start;
    CVertexGroupBlock* _M_finish;
    CVertexGroupBlock* _M_end_of_storage;
    CVertexGroupBlock* begin() const { return _M_start; }
    CVertexGroupBlock* end() const { return _M_finish; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    void reserve(unsigned long n);                             // 0x00ADCB90
    void _M_insert_overflow(CVertexGroupBlock* position, const CVertexGroupBlock& x, const __false_type& tag,
                            unsigned long fillLength, bool atEnd);   // 0x00ADCE60
    CVertexGroupBlock* erase(CVertexGroupBlock* first, CVertexGroupBlock* last)
    {
        CVertexGroupBlock* i = CopyBlocks(last, _M_finish, first, __false_type(), (int*)0);
        DestroyBlocks(i, _M_finish, __false_type());
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
    void push_back(const CVertexGroupBlock& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CVertexGroupBlock(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};

struct CLongArray {                                            // vector<long>
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    long& operator[](unsigned long n) const { return *(_M_start + n); }
    void resize(unsigned long newSize, const long& x);         // 0x004428F0
    void resize(unsigned long newSize) { resize(newSize, long()); }
};

struct CByteArray {                                            // the retail RemainingBlocks (one byte per block)
    bool* _M_start;
    bool* _M_finish;
    bool* _M_end_of_storage;
    bool operator[](unsigned long n) const { return *(_M_start + n); }
};


struct CRemainderArray {                                       // CArray<unsigned long>
    unsigned long* _M_start;
    unsigned long* _M_finish;
    unsigned long* _M_end_of_storage;
    CRemainderArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CRemainderArray() { if (_M_start) free(_M_start); }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    void reserve(unsigned long n);                             // 0x009B4030
    void _M_insert_overflow(unsigned long* position, const unsigned long& x, const __true_type& tag,
                            unsigned long fillLength, bool atEnd);   // 0x00464ACE
    void push_back(const unsigned long& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) unsigned long(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};

struct CBoolArray {                                            // CArray<bool>
    bool* _M_start;
    bool* _M_finish;
    bool* _M_end_of_storage;
    CBoolArray(unsigned long n, const bool& value) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = n != 0 ? (bool*)malloc(n) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
        bool* first = _M_start;
        for (unsigned long count = n; count > 0; --count, ++first)
            *first = value;
        _M_finish = first;
    }
    ~CBoolArray() { if (_M_start) free(_M_start); }
};

struct CBlockNode { CBlockNode* _M_next; CBlockNode* _M_prev; };
struct CBlockList {                                            // list<CVertexGroupBlock> (STLport, 4 bytes)
    CBlockNode* _M_node;
    unsigned long size() const
    {
        unsigned long n = 0;
        for (CBlockNode* it = _M_node->_M_next; it != _M_node; it = it->_M_next)
            ++n;
        return n;
    }
};

class CVertexGroupBlockOptimiser {
public:
    CBlockArray InputList;                                     // +0x00
    void* OptimalBlockList;                                    // +0x0c
    CBlockList Blocks;                                         // +0x10
    CVertexGroupBlock* CurrentBlock;                           // +0x14
    CByteArray RemainingBlocks;                                // +0x18
    CBlockArray CurrentBlockSegmentGroups;                     // +0x24
    CLongArray CurrentBlockSegmentIndices;                     // +0x30
    unsigned long CurrentBlockSegmentCount;                    // +0x3c
    unsigned long MaxGroupIndex;                               // +0x40
    unsigned long MaxN;                                        // +0x44

    void CalcCurrentBlockSegments();                           // 0x00AD9B20
    unsigned long CalcMinimumBlocks(const CRemainderArray& sizes, unsigned long& groupCount, CBoolArray& available,
                                    unsigned long count, unsigned long start);   // 0x00AD89B0
    unsigned long CalcCurrentBestTheoreticalResult();
};

unsigned long CVertexGroupBlockOptimiser::CalcCurrentBestTheoreticalResult()
{
    CalcCurrentBlockSegments();
    unsigned long result = Blocks.size();
    CRemainderArray remainders;
    remainders.reserve(CurrentBlockSegmentCount);
    for (unsigned long i = 0; i < CurrentBlockSegmentCount; ++i) {
        unsigned long groupCount = CurrentBlockSegmentGroups._M_start[i].Groups.size();
        unsigned long fullBlocks = (groupCount - 1) / (MaxN - 1);
        result += fullBlocks;
        unsigned long remainder = groupCount - fullBlocks * (MaxN - 1);
        if (remainder)
            remainders.push_back(remainder);
    }
    unsigned long groupCount = CurrentBlock->Groups.size();
    CBoolArray available(remainders.size(), true);
    return CalcMinimumBlocks(remainders, groupCount, available, remainders.size(), 0) + result;
}
