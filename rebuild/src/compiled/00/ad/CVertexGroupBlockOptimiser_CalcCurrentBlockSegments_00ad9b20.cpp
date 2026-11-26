// CVertexGroupBlockOptimiser::CalcCurrentBlockSegments() -- reset the per-block segment indices to -1 and the segment group list, then paint one new segment from every remaining block that no segment has claimed yet
// Leaf: ?CalcCurrentBlockSegments@CVertexGroupBlockOptimiser@@AAEXXZ
// NF64 (round 7, q10639): the local vector stand-in replaced by stlport/fable_containers.h std::vector<long>; resize kept out of line
//   (FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE), binding ?resize@?$vector@JV?$allocator@J@std@@@std@@QAEXIABJ@Z at 0x004428F0 (landed self row).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD9B20, 494 bytes + int3, plain `ret`.
// The catalog row says CVertexGroupBlockOptimiser::Optimise; the body is the PDB's private
// ?CalcCurrentBlockSegments@CVertexGroupBlockOptimiser@@AAEXXZ (no arguments, it resets CurrentBlockSegmentIndices /
// CurrentBlockSegmentGroups / CurrentBlockSegmentCount and calls PaintCurrentBlockSegment(unsigned long) per segment).
// Callees: vector<long>::resize(size_type, const long&) @ 0x004428F0, STLport __copy for CVertexGroupBlock @ 0x00ADC0D0 and
// _Destroy range @ 0x00ADC190 (the inlined clear), vector<CVertexGroupBlock>::reserve @ 0x00ADCB90, malloc @ 0x00BFEA0E
// (the empty map's header node), CArray<unsigned long> copy constructor @ 0x00535470, map copy constructor @ 0x00A96E40,
// vector<CVertexGroupBlock>::_M_insert_overflow @ 0x00ADCE60, _Rb_tree::_M_erase @ 0x004507A1, free @ 0x00BFEA14,
// PaintCurrentBlockSegment(unsigned long) @ 0x00AD9380.
// Layout (retail, STLport containers 12 bytes, lists 4 bytes; the PDB view uses 16/12-byte containers): InputList +0x00
// (CArray<CVertexGroupBlock>), OptimalBlockList +0x0c, Blocks +0x10, CurrentBlock +0x14, RemainingBlocks +0x18 (byte per
// block), CurrentBlockSegmentGroups +0x24, CurrentBlockSegmentIndices +0x30, CurrentBlockSegmentCount +0x3c.
// NF258 (2026-11-22, L37): the CGroupArray stand-in (CArray<unsigned long>) is replaced by engine/CArray.h CArray<unsigned long> (Ego_r
//   CVertexGroupBlock::Groups; engine/CVertexGroupBlock.h); the STLport vector copy constructor is declaration-only
//   (FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY), so CVertexGroupBlock's inline copy binds ??0?$vector@KV?$allocator@K@std@@@std@@QAE@ABV01@@Z
//   at 0x00535470 (the landed self row's spelling). The local CVertexGroupBlock model is HEAD debt (port queued).
#include <new>
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY
#include "engine/CArray.h"
extern "C" void* __cdecl malloc(unsigned int size);            // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                     // 0x00BFEA14

struct __false_type { __false_type() { } };

struct CRbNode {
    unsigned char _M_color;
    CRbNode* _M_parent;
    CRbNode* _M_left;
    CRbNode* _M_right;
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
    CArray<unsigned long> Groups;                                          // +0x00
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


struct CByteArray {                                            // the retail RemainingBlocks (one byte per block)
    bool* _M_start;
    bool* _M_finish;
    bool* _M_end_of_storage;
    bool operator[](unsigned long n) const { return *(_M_start + n); }
};

class CVertexGroupBlockOptimiser {
public:
    CBlockArray InputList;                                     // +0x00
    void* OptimalBlockList;                                    // +0x0c
    void* Blocks;                                              // +0x10
    CVertexGroupBlock* CurrentBlock;                           // +0x14
    CByteArray RemainingBlocks;                                // +0x18
    CBlockArray CurrentBlockSegmentGroups;                     // +0x24
    std::vector<long> CurrentBlockSegmentIndices;                     // +0x30
    unsigned long CurrentBlockSegmentCount;                    // +0x3c

    private:
    void PaintCurrentBlockSegment(unsigned long block);
    public:
        // 0x00AD9380
    private:
    void CalcCurrentBlockSegments();
    public:
};

void CVertexGroupBlockOptimiser::CalcCurrentBlockSegments()
{
    CurrentBlockSegmentIndices.resize(InputList.size());
    CurrentBlockSegmentCount = 0;
    CurrentBlockSegmentGroups.clear();
    CurrentBlockSegmentGroups.reserve(InputList.size());
    for (unsigned long i = 0; i < CurrentBlockSegmentIndices.size(); ++i)
        CurrentBlockSegmentIndices[i] = -1;
    for (unsigned long i = 0; i < CurrentBlockSegmentIndices.size(); ++i) {
        if (RemainingBlocks[i] && CurrentBlockSegmentIndices[i] == -1) {
            CurrentBlockSegmentGroups.push_back(CVertexGroupBlock());
            PaintCurrentBlockSegment(i);
            ++CurrentBlockSegmentCount;
        }
    }
}