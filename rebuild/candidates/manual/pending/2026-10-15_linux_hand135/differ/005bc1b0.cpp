// CEnginePrimitive2DMorphedAnimatingMesh::CEnginePrimitive2DMorphedAnimatingMesh(...) -- the 2D animating-mesh primitive descriptor plus copies of the morph-target vector and the morph-channel map; primitive type 0x26
// Grade: DIFFER(ours v retail) 102v102 -- only the in-slot CRGBColour copy schedule: retail loads the B/G dword into edx at the very top (`mov edx,[ebp+34h]` before the pushes) and stores +2,+3 (from `mov cx,[ebp+36h]`) then +1,+0 from dh/dl; ours stores +2,+3 then loads the dword into ecx late (between the pushes) and stores +1,+0 from ch/cl. Tried: out-of-line copy ctor (93v102), __forceinline memberwise B,G,R,A init list (store order +0,+1,+2,+3), body R,A,G,B, inline Set(a, r, g, b) taking all four first.
// Leaf: ??0CEnginePrimitive2DMorphedAnimatingMesh
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC1B0, length 102 bytes; size-optimised, frame-pointer TU.
// Callees: the base descriptor constructor @ 0x00550A70 (manifest "CEnginePrimitiveAnimation"; it stores type 0x25 and
// copies its arguments), the morph-target vector copy constructor @ 0x005BBDF1 (on +0x80, manifest "CExpressionSet::Add"),
// the morph-channel map copy constructor @ 0x00514400 (on +0x8C, manifest "std_map_insert_CCharString").
// Shape: twelve stack arguments (`ret 30h`); arguments 4 and 5 are const references for the two member copies, the rest go to
// the base in order (1, 2, 3, 6..11) with the trailing CRGBColour passed BY VALUE: it is built in its reserved argument slot
// (`push ecx ; mov eax,esp`) by the colour's memberwise byte copy constructor. The derived body then overwrites the
// descriptor's type word at +0 with 0x26 (no vtable: these are plain records). `push dword ptr [ebp+N]` pass-throughs with a
// frame pointer and no spills = `#pragma optimize("y", off)` + `optimize("s", on)` (GOTCHAS 24b).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CRGBColour {
    unsigned char B, G, R, A;
    __forceinline void Set(unsigned char a, unsigned char r, unsigned char g, unsigned char b)
    {
        R = r;
        A = a;
        G = g;
        B = b;
    }
    __forceinline CRGBColour(const CRGBColour& other)
    {
        Set(other.A, other.R, other.G, other.B);
    }
};

struct CMorphTargetVector {                        // a retail 12-byte vector
    void* first;
    void* last;
    void* end;
    CMorphTargetVector(const CMorphTargetVector& other);   // 0x005BBDF1
};

struct CMorphChannelMap {                          // a retail 8-byte map {header, count}
    void*         header;
    unsigned long count;
    CMorphChannelMap(const CMorphChannelMap& other);       // 0x00514400
};

struct CEnginePrimitive2DAnimatingMeshDesc {       // 0x80 bytes, type word first
    long          m_type;                          // +0x00
    unsigned char _body[0x7c];
    CEnginePrimitive2DAnimatingMeshDesc(void* a1, void* a2, void* a3, void* a6, void* a7, void* a8, void* a9, void* a10,
                                        void* a11, CRGBColour colour);   // 0x00550A70
};

struct CEnginePrimitive2DMorphedAnimatingMesh : CEnginePrimitive2DAnimatingMeshDesc {
    CMorphTargetVector m_morphTargets;             // +0x80
    CMorphChannelMap   m_morphChannels;            // +0x8C
    CEnginePrimitive2DMorphedAnimatingMesh(void* a1, void* a2, void* a3, const CMorphTargetVector& morphTargets,
                                           const CMorphChannelMap& morphChannels, void* a6, void* a7, void* a8, void* a9,
                                           void* a10, void* a11, CRGBColour colour);
};

CEnginePrimitive2DMorphedAnimatingMesh::CEnginePrimitive2DMorphedAnimatingMesh(
    void* a1, void* a2, void* a3, const CMorphTargetVector& morphTargets, const CMorphChannelMap& morphChannels, void* a6,
    void* a7, void* a8, void* a9, void* a10, void* a11, CRGBColour colour)
    : CEnginePrimitive2DAnimatingMeshDesc(a1, a2, a3, a6, a7, a8, a9, a10, a11, colour),
      m_morphTargets(morphTargets), m_morphChannels(morphChannels)
{
    m_type = 0x26;
}
