// CVertexGroupBlockList::FilterBySubset(const CVertexGroupBlockList& supersets) -- keep, in superset order, each block that is a subset of some block of `supersets` (each block taken once) and replace the list with them
// Leaf: FilterBySubset
// Grade: DIFFER(415v416) -- retail keeps the filtered array finish pointer in ebx across both loops (inner index homed in the dead argument slot); VC here leaves finish in memory and gives ebp to the index (5 variants tried)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD9980, 414 bytes + padding, `ret 4`.
// The catalog row names the class CVertexGroupBlock; `this` is the block LIST (an array of 0x18-byte CVertexGroupBlock
// elements walked with the /24 size idiom), so the method is CVertexGroupBlockList's. The TLC name is the catalog's.
// Callees: CVertexGroupBlock::IsSubsetOf(const CVertexGroupBlock&) const @ 0x00AD8C30, CArray<bool>(size_type, const bool&,
// const allocator&) @ 0x00451583, CArray<unsigned long> copy constructor @ 0x00535470 and the map<unsigned long,long>
// copy constructor @ 0x00A96E40 (the inlined CVertexGroupBlock copy in push_back), vector<CVertexGroupBlock>
// _M_insert_overflow @ 0x00ADCE60, vector<CVertexGroupBlock>::operator= @ 0x00ADCA30, the STLport _Destroy range
// @ 0x00ADC190 (fastcall), free @ 0x00BFEA14.
// Layout: CVertexGroupBlockList = { CArray<CVertexGroupBlock> Blocks } (retail STLport vector, 12 bytes);
// CVertexGroupBlock = { CArray<unsigned long> Groups +0, map<unsigned long,long> GroupToIndexMap +0xc } = 0x18.
#include <new>
extern "C" void __cdecl free(void* block);                     // 0x00BFEA14

struct __false_type { __false_type() { } };
struct CBoolAllocator { CBoolAllocator() { } };

struct CGroupArray {                                           // CArray<unsigned long>
    unsigned long* _M_start;
    unsigned long* _M_finish;
    unsigned long* _M_end_of_storage;
    CGroupArray(const CGroupArray& other);                     // 0x00535470
};

struct CGroupToIndexMap {                                      // map<unsigned long, long> (STLport, 12 bytes)
    void* _M_header;
    unsigned long _M_node_count;
    unsigned long _M_key_compare;
    CGroupToIndexMap(const CGroupToIndexMap& other);           // 0x00A96E40
};

class CVertexGroupBlock {
public:
    CGroupArray Groups;                                        // +0x00
    CGroupToIndexMap GroupToIndexMap;                          // +0x0c
    bool IsSubsetOf(const CVertexGroupBlock& other) const;     // 0x00AD8C30
};

struct CBoolArray {                                            // CArray<bool>
    bool* _M_start;
    bool* _M_finish;
    bool* _M_end_of_storage;
    CBoolArray(unsigned long n, const bool& value, const CBoolAllocator& allocator = CBoolAllocator());   // 0x00451583
    ~CBoolArray() { if (_M_start) free(_M_start); }
    bool& operator[](unsigned long n) const { return *(_M_start + n); }
};

void __fastcall DestroyBlocks(CVertexGroupBlock* first, CVertexGroupBlock* last, const __false_type& tag);   // 0x00ADC190

struct CBlockArray {                                           // CArray<CVertexGroupBlock>
    CVertexGroupBlock* _M_start;
    CVertexGroupBlock* _M_finish;
    CVertexGroupBlock* _M_end_of_storage;
    CBlockArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CBlockArray()
    {
        DestroyBlocks(_M_start, _M_finish, __false_type());
        if (_M_start)
            free(_M_start);
    }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    CVertexGroupBlock& operator[](unsigned long n) const { return *(_M_start + n); }
    CBlockArray& operator=(const CBlockArray& other);         // 0x00ADCA30
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
    void FilterBySubset(const CVertexGroupBlockList& supersets);
};

void CVertexGroupBlockList::FilterBySubset(const CVertexGroupBlockList& supersets)
{
    CBlockArray filtered;
    unsigned long count = Blocks.size();
    CBoolArray available(count, true);
    for (unsigned long j = 0; j < supersets.Blocks.size(); ++j) {
        for (unsigned long i = 0; i < count; ++i) {
            if (available[i]) {
                const CVertexGroupBlock& block = Blocks[i];
                if (block.IsSubsetOf(supersets.Blocks[j])) {
                    filtered.push_back(block);
                    available[i] = false;
                }
            }
        }
    }
    Blocks = filtered;
}
