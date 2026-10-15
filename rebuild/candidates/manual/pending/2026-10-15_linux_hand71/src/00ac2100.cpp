// C3DMeshPartitionElement::~C3DMeshPartitionElement() -- destroy and free both sub-elements (recursively), then zero the triangle range
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC2100, length 63 bytes.
// Leaf: ??1C3DMeshPartitionElement
// Retail bytes:  push ebx ; push esi ; mov esi,ecx ; push edi ; mov edi,[esi+20h] ; xor ebx,ebx ; cmp edi,ebx ; je A ; mov ecx,edi ;
//                call ~C3DMeshPartitionElement (self) ; push edi ; call operator delete ; add esp,4 ; A: mov edi,[esi+24h] ;
//                cmp edi,ebx ; je B ; mov ecx,edi ; call (self) ; push edi ; call operator delete ; add esp,4 ; B: pop edi ;
//                mov [esi+18h],ebx ; mov [esi+1Ch],ebx ; pop esi ; pop ebx ; ret
// Manifest label "CRenderTarget::Uninitialise" is propagated and wrong: the body sits just before
// C3DMeshPartitionElement::BuildSubElement (0x00AC2160) and recurses through the PDB's SubElements[2] at +0x20/+0x24
// exactly like the landed C3DMeshPartitionElement scalar deleting destructor 0x00AC37B0 (same model), which is this
// destructor inlined. Here the destructor is out of line, so each child `delete` is the destructor called on the
// child (self call) followed by operator delete @ 0x00BFE9BC, under the null test.
void __cdecl operator delete(void* block);          // 0x00BFE9BC

struct C3DVector { float X, Y, Z; };
struct C3DBoundingBox { C3DVector Min, Max; };
struct C3DBoundingSphere { C3DVector Center; float Radius; };

struct C3DMeshPartitionElement {
    C3DBoundingBox           BoundingBox;      // +0x00
    long                     StartTriangle;    // +0x18
    long                     TriangleCount;    // +0x1C
    C3DMeshPartitionElement* SubElements[2];   // +0x20
    C3DBoundingSphere        BoundingSphere;   // +0x28

    ~C3DMeshPartitionElement();
};

C3DMeshPartitionElement::~C3DMeshPartitionElement()
{
    delete SubElements[0];
    delete SubElements[1];
    StartTriangle = 0;
    TriangleCount = 0;
}
