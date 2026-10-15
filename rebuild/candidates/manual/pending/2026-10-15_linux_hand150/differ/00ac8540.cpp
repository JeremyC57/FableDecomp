// COptimisedPrimitive::PaintSegments() -- split the faces into connected segments: every existing face not yet in a segment seeds a new segment number and is flood-filled from a fresh work list
// Leaf: PaintSegments
// Grade: DIFFER(133v133) -- scheduling only: retail pops the malloc argument (`add esp,4`) right after the call, ours sinks it below the head-node stores and the SegmentCount load; tried a direct malloc, an inline static allocate helper, a member-assigned head, the STLport alloc-proxy allocate(1) (worse)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC8540, length 133 bytes (padded to 144).
// Callees: malloc @ 0x00BFEA0E (the work list's head node), COptimisedPrimitive::PaintSegment(unsigned long face, long segment,
//          list<unsigned long>& work) @ 0x00AC8170 (manifest "PaintSegments_AddSegmentLink"), list<unsigned long>::~list
//          @ 0x0042AB73 (manifest "CActiveFile::OnReadFinished" is propagated: it clears the nodes and frees the head).
// Layout: retail COptimisedPrimitive: Faces (12-byte vector of 0x48-byte COptimisedFace) +0x1c, SegmentCount +0x40
// (PDB +0x54 with 16-byte CArrays and multimap). COptimisedFace: Exists +0x3c, Segment +0x44.
// Shape: the face count is hoisted into a signed local before the loop (`jl`); the work list is a block-scoped STLport
// list (one frame word: the malloc'd self-linked head) built by an inline constructor and destroyed out of line.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E

struct C3DVector { float X, Y, Z; };
struct COptimisedFace {                     // QuadricOptimiserInternals::COptimisedFace (0x48 bytes)
    unsigned long VertexIndices[3];         // +0x00
    unsigned long UniqueVertexIndices[3];   // +0x0c
    long          Edges[3];                 // +0x18
    C3DVector     UTangent;                 // +0x24
    C3DVector     VTangent;                 // +0x30
    bool          Exists;                   // +0x3c
    unsigned long OutputIndex;              // +0x40
    long          Segment;                  // +0x44
};
template <class T> struct CRetailVector {   // retail std::vector (12 bytes)
    T* m_first;
    T* m_last;
    T* m_end;
    unsigned long size() const { return m_last - m_first; }
    T& operator[](unsigned long n) { return m_first[n]; }
};
struct SIndexNode {
    SIndexNode*   m_next;
    SIndexNode*   m_prev;
    unsigned long m_data;
};
struct CIndexWorkList {                     // STLport list<unsigned long> (4 bytes: the head node)
    SIndexNode* m_node;
    CIndexWorkList()
    {
        SIndexNode* head = (SIndexNode*)malloc(sizeof(SIndexNode));
        head->m_next = head;
        head->m_prev = head;
        m_node = head;
    }
    ~CIndexWorkList();                      // 0x0042AB73
};

class COptimisedPrimitive {
public:
    long                          MaterialIndex;        // +0x00
    unsigned char                 UniqueVertices[0xc];  // +0x04
    unsigned char                 Vertices[0xc];        // +0x10
    CRetailVector<COptimisedFace> Faces;                // +0x1c
    unsigned char                 Edges[0xc];           // +0x28
    unsigned char                 VertexPairContractions[0xc];  // +0x34
    long                          SegmentCount;         // +0x40
    void PaintSegment(unsigned long face, long segment, CIndexWorkList& work);   // 0x00AC8170
    void PaintSegments();
};

void COptimisedPrimitive::PaintSegments()
{
    long faceCount = Faces.size();
    SegmentCount = 0;
    for (long i = 0; i < faceCount; ++i)
    {
        COptimisedFace& face = Faces[i];
        if (face.Exists && face.Segment == -1)
        {
            CIndexWorkList work;
            PaintSegment(i, SegmentCount, work);
            SegmentCount++;
        }
    }
}
