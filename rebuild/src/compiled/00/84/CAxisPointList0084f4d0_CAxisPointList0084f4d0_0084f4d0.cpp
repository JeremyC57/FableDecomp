// CAxisPointList_0084f4d0::CAxisPointList_0084f4d0(const C3DVector& origin, const C3DVector& axis) -- copies two vectors to +0 / +0xc and builds the empty point vector (+0x18) and the trailing word (+0x24)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084F4D0, length 60 bytes.
// Leaf: ??0CAxisPointList_0084f4d0
// Retail bytes: mov eax,ecx ; mov ecx,[esp+4] ; (three dword moves through edx/ecx to +0..+8) ; mov ecx,[esp+8] ; (three to +0xc..+0x14) ;
//               xor ecx,ecx ; mov [eax+18h],ecx ; mov [eax+1Ch],ecx ; mov [eax+20h],ecx ; mov [eax+24h],ecx ; ret 8
// The manifest label "CRightHandedSet::CRightHandedSet" is propagated: CRightHandedSet is 0x18 bytes (Up, Forward)
// while this constructor initialises 0x28 bytes. Its one caller (0x0084E590, a melee-opponent tracker) builds the
// object on the stack from a C3DVector argument and the literal (0, 0, 1), appends 16-byte {C3DVector, float}
// records through 0x0084E390 (whose `add ecx,18h` + `[ecx+4]`/`[ecx+8]` compare is the retail three-pointer
// vector at +0x18) and frees the vector's first pointer at the end, so the layout is two vectors, a retail
// std::vector (first/last/end) and one more zeroed word. The zeros share one register: integer/pointer members.
// C3DVector carries the memberwise user copy constructor: a POD copy goes through a re-based pointer (lea edx) and esi.
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}   // memberwise: one register per dword
};
struct SAxisPoint_0084f4d0 {               // 16-byte element of the point vector
    C3DVector Position;
    float     Weight;
};
struct CAxisPointList_0084f4d0 {
    C3DVector            Origin;           // +0x00
    C3DVector            Axis;             // +0x0c
    SAxisPoint_0084f4d0* m_pFirst;         // +0x18 retail vector: first
    SAxisPoint_0084f4d0* m_pLast;          // +0x1c                last
    SAxisPoint_0084f4d0* m_pEnd;           // +0x20                end of storage
    long                 m_count;          // +0x24
    CAxisPointList_0084f4d0(const C3DVector& origin, const C3DVector& axis);
};

CAxisPointList_0084f4d0::CAxisPointList_0084f4d0(const C3DVector& origin, const C3DVector& axis)
    : Origin(origin), Axis(axis), m_pFirst(0), m_pLast(0), m_pEnd(0), m_count(0)
{
}