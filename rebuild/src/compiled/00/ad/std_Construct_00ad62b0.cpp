// std::_Construct(QuadricOptimiserInternals::CUniqueVertex*, const QuadricOptimiserInternals::CUniqueVertex&) -- placement copy of an optimiser unique vertex: exists flag and two index lists (out-of-line set copies)
// Retail: Fable.exe @ 0x00AD62B0, 41 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CIndexList copy ctor @ 0x00AD3A10 (x2).
// Leaf: _Construct
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N00ad62b0 {
struct CIndexList {                              // set<unsigned long> body, 12 bytes in retail
    void*         m_header;
    unsigned long m_nodeCount;
    unsigned long m_compare;
    CIndexList(const CIndexList& other);         // 0x00AD3A10
};
struct CUniqueVertex {                           // QuadricOptimiserInternals::CUniqueVertex (retail sizeof 0x1C)
    bool       Exists;                           // +0x00
    CIndexList VertexIndices;                    // +0x04
    CIndexList FaceList;                         // +0x10
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CUniqueVertex* p, const CUniqueVertex& value)
{
    if (p != 0)
        new (p) CUniqueVertex(value);
}
}