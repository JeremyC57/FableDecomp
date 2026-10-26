// QuadricOptimiserInternals::COptimisedPrimitive::~COptimisedPrimitive() -- destroy the primitive's containers in reverse member order
// Leaf: ??1COptimisedPrimitive
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD84D0, 243 bytes (+ int3 padding), plain `ret`.
// Identity: NOT CWound::Kill (propagated label). Its only caller is the scalar deleting destructor 0x00AD8770 (no
//   vtable store: COptimisedPrimitive is not polymorphic), and the members it destroys line up with Ego_r.pdb
//   QuadricOptimiserInternals::COptimisedPrimitive once the PDB's 16-byte CArrays are retail's 12-byte STLport vectors:
//   MaterialIndex +0x00, UniqueVertices +0x04 (CUniqueVertex 0x1C, dtor 0x00AD4A70 -- landed as "CTwoLongSets"),
//   Vertices +0x10 (COptimisedVertex 0xB8, dtor 0x00AD7400, landed), Faces +0x1C, Edges +0x28,
//   VertexPairContractions +0x34 (multimap<float, CCountedPointer<CVertexPairContraction>>, 12 bytes),
//   SegmentCount +0x40, SegmentSizes +0x44, SubChunksToKeep +0x50 (CArray<CCountedPointer<CChunkedFileChunk>>),
//   Parameters +0x5C (trivially destroyed).
// Callees: _Destroy_range(first, last, tag&) 0x00AA9140 (`__fastcall`, counted-pointer release; the empty tag is the
//   `push ecx` frame slot, `lea eax,[esp+7]`), the multimap's recursive node erase 0x00AD3FB0 (thiscall on the tree),
//   ~COptimisedVertex 0x00AD7400, ~CUniqueVertex 0x00AD4A70, CRT free 0x00BFEA14 (every vector/header release).
// Shape: vectors of trivially destroyed elements only free their storage; vectors of class elements run an inline
//   `_Destroy(first, last)` loop (by-value range, `last` loaded first) before the free; the tree destructor is the
//   STLport clear() (erase(root), leftmost = header, root = 0, rightmost = header, count = 0) then free(header).
// Layout: local model (no engine/ header for the quadric optimiser); element bodies are opaque.
extern "C" void free(void* block);                              // 0x00BFEA14

namespace QuadricOptimiserInternals {

class CUniqueVertex { public:
    unsigned char _body[0x1c];
    ~CUniqueVertex();                                           // 0x00AD4A70
};
class COptimisedVertex { public:
    unsigned char _body[0xb8];
    ~COptimisedVertex();                                        // 0x00AD7400
};
class COptimisedFace { public: unsigned char _body[0x48]; };
class COptimisedEdge { public: long Vertex[2]; long Face[2]; };
struct CCountedPointerAny { void* m_pObject; void* m_pCount; }; // CCountedPointer<CChunkedFileChunk>

struct SNonscalarPtrIteratorTag { SNonscalarPtrIteratorTag() { } };
void __fastcall _Destroy_range(CCountedPointerAny* first, CCountedPointerAny* last, const SNonscalarPtrIteratorTag& tag);   // 0x00AA9140

template<class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}

template<class T> struct CPlainArray {                          // retail CArray of trivially destroyed T
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    ~CPlainArray() { if (_M_start != 0) free(_M_start); }
};
template<class T> struct CObjectArray {                         // retail CArray of T with a destructor
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    ~CObjectArray() { _Destroy(_M_start, _M_finish); if (_M_start != 0) free(_M_start); }
};
struct CCountedArray {                                          // retail CArray<CCountedPointer<..>>
    CCountedPointerAny* _M_start; CCountedPointerAny* _M_finish; CCountedPointerAny* _M_end_of_storage;
    ~CCountedArray()
    {
        _Destroy_range(_M_start, _M_finish, SNonscalarPtrIteratorTag());
        if (_M_start != 0) free(_M_start);
    }
};

struct CTreeNode { long _M_color; CTreeNode* _M_parent; CTreeNode* _M_left; CTreeNode* _M_right; };
struct CContractionMultimap {                                   // STLport _Rb_tree: header, count, empty compare
    CTreeNode* _M_header;
    unsigned long _M_node_count;
    long _M_key_compare;
    void _M_erase(CTreeNode* x);                                // 0x00AD3FB0
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
    ~CContractionMultimap() { clear(); if (_M_header != 0) free(_M_header); }
};

class COptimisationParameters { public: unsigned char _body[0x28]; };

class COptimisedPrimitive { public:
    long MaterialIndex;                                         // +0x00
    CObjectArray<CUniqueVertex> UniqueVertices;                 // +0x04
    CObjectArray<COptimisedVertex> Vertices;                    // +0x10
    CPlainArray<COptimisedFace> Faces;                          // +0x1c
    CPlainArray<COptimisedEdge> Edges;                          // +0x28
    CContractionMultimap VertexPairContractions;                // +0x34
    long SegmentCount;                                          // +0x40
    CPlainArray<float> SegmentSizes;                            // +0x44
    CCountedArray SubChunksToKeep;                              // +0x50
    COptimisationParameters Parameters;                         // +0x5c
    ~COptimisedPrimitive();
};

COptimisedPrimitive::~COptimisedPrimitive()
{
}

}