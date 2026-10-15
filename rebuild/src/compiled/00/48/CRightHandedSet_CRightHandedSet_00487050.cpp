// CRightHandedSet::CRightHandedSet(const C3DVector& up, const C3DVector& forward) -- constructor: copy the up and forward vectors
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00487050, length 46 bytes.
// Leaf: ??0CRightHandedSet
// Retail bytes:  mov eax,ecx ; mov ecx,[esp+4] ; mov edx,[ecx] ; mov [eax],edx ; mov edx,[ecx+4] ; mov [eax+4],edx ; mov ecx,[ecx+8] ;
//                mov [eax+8],ecx ; mov ecx,[esp+8] ; mov edx,[ecx] ; mov [eax+0Ch],edx ; mov edx,[ecx+4] ; mov [eax+10h],edx ;
//                mov ecx,[ecx+8] ; mov [eax+14h],ecx ; ret 8
// PDB layout (struct_layouts_egor.tsv, CRightHandedSet = 24 bytes): +0 C3DVector Up, +0xC C3DVector Forward.
// Shape: both members copy-constructed in the initialiser list through the engine's memberwise C3DVector copy constructor
// (three dword moves each through edx, every store off the `this` copy in eax, as in the landed 0x007AB450); the
// implicit copy constructor or assignment routes each member through its own address register instead (push esi,
// `mov edx,eax` / `lea edx,[eax+0Ch]`).
struct C3DVector {
    float X;                                 // +0x00
    float Y;                                 // +0x04
    float Z;                                 // +0x08
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }   // the engine's memberwise copy constructor
};

struct CRightHandedSet {
    C3DVector Up;                            // +0x00
    C3DVector Forward;                       // +0x0C
    CRightHandedSet(const C3DVector& up, const C3DVector& forward);
};

CRightHandedSet::CRightHandedSet(const C3DVector& up, const C3DVector& forward)
    : Up(up), Forward(forward)
{
}