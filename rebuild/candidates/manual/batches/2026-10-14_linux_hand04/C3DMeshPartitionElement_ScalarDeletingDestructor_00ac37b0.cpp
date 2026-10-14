// C3DMeshPartitionElement::`scalar deleting destructor'(unsigned int) -- delete both sub-elements (recursively through this same helper), zero the triangle range, free on bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC37B0, oracle length 67 bytes.
// Callees: itself (0x00AC37B0, the `delete` of a non-polymorphic class with a non-trivial destructor routes
//          through its scalar deleting destructor), operator delete @ 0x00BFE9BC (landed-trusted).
//
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+20h] ; test ecx,ecx ; je A ; push 1 ; call ??_G ; A: mov ecx,[esi+24h] ;
//                test ecx,ecx ; je B ; push 1 ; call ??_G ; B: test byte ptr [esp+8],1 ; mov dword ptr [esi+18h],0 ;
//                mov dword ptr [esi+1Ch],0 ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// PDB layout (struct_layouts_egor.tsv, C3DMeshPartitionElement = 56 bytes, no vtable): +0 BoundingBox,
// +0x18 StartTriangle, +0x1C TriangleCount, +0x20 SubElements[2], +0x28 BoundingSphere. The inline
// destructor deletes the two children (each `delete` is a null test + `push 1 ; call` of this very
// helper) and resets the triangle range; the compiler sinks the two zero stores past the flag test.
struct C3DVector { float X, Y, Z; };
struct C3DBoundingBox { C3DVector Min, Max; };
struct C3DBoundingSphere { C3DVector Center; float Radius; };

struct C3DMeshPartitionElement {
    C3DBoundingBox           BoundingBox;      // +0x00
    long                     StartTriangle;    // +0x18
    long                     TriangleCount;    // +0x1C
    C3DMeshPartitionElement* SubElements[2];   // +0x20
    C3DBoundingSphere        BoundingSphere;   // +0x28

    ~C3DMeshPartitionElement()
    {
        delete SubElements[0];
        delete SubElements[1];
        StartTriangle = 0;
        TriangleCount = 0;
    }
};

// The deleting destructor is emitted when something in the translation unit deletes the object.
void DestroyOne(C3DMeshPartitionElement* p)
{
    delete p;
}
