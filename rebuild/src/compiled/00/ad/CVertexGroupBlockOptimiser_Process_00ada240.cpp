// CVertexGroupBlockOptimiser::Process(CVertexGroupBlockList& output, bool exhaustive) -- reset the optimiser state for the input list, seed one empty working block, run AddBlocks from block 0 and append the optimal block list to the output
// Leaf: Process
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ADA240, 429 bytes, `ret 8`.
// The catalog row says AddBlocksFromOptimisation; the PDB's public ?Process@CVertexGroupBlockOptimiser@@QAEXAAVCVertexGroupBlockList@@_N@Z
// is the (list&, bool) method that drives AddBlocks and copies OptimalBlockList out.
// Callees: list<CVertexGroupBlock>::clear @ 0x00ADC250 (both lists), memmove @ 0x00BFEAE6 (the inlined byte-array clear),
// the byte-array _M_fill_insert @ 0x004676EF, CalcCurrentBlockSegments @ 0x00AD9B20, malloc @ 0x00BFEA0E (map header
// and list node), CArray<unsigned long> copy constructor @ 0x00535470, map copy constructor @ 0x00A96E40,
// ~CVertexGroupBlock @ 0x00A96DA0, the remaining-group count @ 0x00AD8ED0 (const, no arguments), AddBlocks(unsigned long)
// @ 0x00AD9F10, vector<CVertexGroupBlock>::_M_insert_overflow @ 0x00ADCE60 (the inlined CVertexGroupBlockList::AddBlock).
// Layout (retail, STLport containers 12 bytes, lists 4 bytes): InputList +0x00, OptimalBlockList +0x0c, Blocks +0x10,
// CurrentBlock +0x14, RemainingBlocks +0x18 (byte per block), MaxN +0x44, OptimalSize +0x48, Greedy +0x50,
// Iterations +0x54.
#include <new>
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6
extern "C" void* __cdecl malloc(unsigned int size);            // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                     // 0x00BFEA14

struct __false_type { __false_type() { } };

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
    ~CVertexGroupBlock();                                      // 0x00A96DA0
};


struct CBlockArray {                                           // CArray<CVertexGroupBlock>
    CVertexGroupBlock* _M_start;
    CVertexGroupBlock* _M_finish;
    CVertexGroupBlock* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    void _M_insert_overflow(CVertexGroupBlock* position, const CVertexGroupBlock& x, const __false_type& tag,
                            unsigned long fillLength, bool atEnd);   // 0x00ADCE60
    void push_back(const CVertexGroupBlock& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CVertexGroupBlock(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};

class CVertexGroupBlockList {
public:
    CBlockArray Blocks;
    void AddBlock(const CVertexGroupBlock& block) { Blocks.push_back(block); }
};

struct CBlockNode {
    CBlockNode* _M_next;
    CBlockNode* _M_prev;
    CVertexGroupBlock _M_data;
};
struct CBlockListIterator {
    CBlockNode* _M_node;
    CBlockListIterator(CBlockNode* node) : _M_node(node) { }
    CBlockListIterator(const CBlockListIterator& other) : _M_node(other._M_node) { }
    CVertexGroupBlock& operator*() const { return _M_node->_M_data; }
    CBlockListIterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const CBlockListIterator& other) const { return _M_node != other._M_node; }
};
struct CBlockList {                                            // list<CVertexGroupBlock> (STLport, 4 bytes)
    CBlockNode* _M_node;
    CBlockListIterator begin() const { return CBlockListIterator(_M_node->_M_next); }
    CBlockListIterator end() const { return CBlockListIterator(_M_node); }
    CVertexGroupBlock& front() const { return _M_node->_M_next->_M_data; }
    void clear();                                              // 0x00ADC250
    CBlockNode* _M_create_node(const CVertexGroupBlock& x)
    {
        CBlockNode* p = (CBlockNode*)malloc(sizeof(CBlockNode));
        new (&p->_M_data) CVertexGroupBlock(x);
        return p;
    }
    CBlockListIterator insert(CBlockListIterator position, const CVertexGroupBlock& x)
    {
        CBlockNode* tmp = _M_create_node(x);
        CBlockNode* n = position._M_node;
        CBlockNode* prev = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        n->_M_prev = tmp;
        return CBlockListIterator(tmp);
    }
    void push_back(const CVertexGroupBlock& x) { insert(end(), x); }
};

inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
                           : (char*)memmove(result, first, (const char*)last - (const char*)first) +
                                 ((const char*)last - (const char*)first);
}

struct CByteArray {                                            // the retail RemainingBlocks (one byte per block)
    bool* _M_start;
    bool* _M_finish;
    bool* _M_end_of_storage;
    bool* begin() const { return _M_start; }
    bool* end() const { return _M_finish; }
    void _M_fill_insert(bool* position, unsigned long n, const bool& x);   // 0x004676EF
    void insert(bool* position, unsigned long n, const bool& x) { _M_fill_insert(position, n, x); }
    bool* erase(bool* first, bool* last)
    {
        _M_finish = (bool*)CopyTrivial(last, _M_finish, first);
        return first;
    }
    void clear() { erase(begin(), end()); }
};

class CVertexGroupBlockOptimiser {
public:
    CBlockArray InputList;                                     // +0x00
    CBlockList OptimalBlockList;                               // +0x0c
    CBlockList Blocks;                                         // +0x10
    CVertexGroupBlock* CurrentBlock;                           // +0x14
    CByteArray RemainingBlocks;                                // +0x18
    unsigned char _pad24[0x44 - 0x24];
    unsigned long MaxN;                                        // +0x44
    unsigned long OptimalSize;                                 // +0x48
    unsigned long MaxIterations;                               // +0x4c
    bool Greedy;                                               // +0x50
    unsigned long Iterations;                                  // +0x54

    void CalcCurrentBlockSegments();                           // 0x00AD9B20
    unsigned long CountRemainingGroups() const;                // 0x00AD8ED0
    void AddBlocks(unsigned long block);                       // 0x00AD9F10
    void Process(CVertexGroupBlockList& output, bool exhaustive);
};

void CVertexGroupBlockOptimiser::Process(CVertexGroupBlockList& output, bool exhaustive)
{
    if (InputList.size() == 0)
        return;
    OptimalBlockList.clear();
    RemainingBlocks.clear();
    RemainingBlocks.insert(RemainingBlocks.end(), InputList.size(), true);
    CalcCurrentBlockSegments();
    Blocks.clear();
    {
        CVertexGroupBlock block;
        Blocks.push_back(block);
    }
    CurrentBlock = &Blocks.front();
    unsigned long maxN = MaxN;
    OptimalSize = (CountRemainingGroups() + maxN - 3) / (maxN - 1);
    Greedy = !exhaustive;
    Iterations = 0;
    AddBlocks(0);
    for (CBlockListIterator it = OptimalBlockList.begin(); it != OptimalBlockList.end(); ++it)
        output.AddBlock(*it);
}