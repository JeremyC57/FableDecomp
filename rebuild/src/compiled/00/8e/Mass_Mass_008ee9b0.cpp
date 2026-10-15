// Mass::Mass() -- default constructor: position and velocity copied from zero C2DVector temporaries, own vtable, angular position and velocity zeroed (inverse masses left unset)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EE9B0, length 79 bytes.
// Leaf: ??0Mass
// Retail bytes:  sub esp,8 ; mov eax,ecx ; mov dword ptr [esp],0 ; mov ecx,[esp] ; mov dword ptr [esp+4],0 ; mov edx,[esp+4] ;
//                mov [eax+4],ecx ; mov dword ptr [esp],0 ; mov ecx,[esp] ; mov [eax+8],edx ; mov dword ptr [esp+4],0 ;
//                mov edx,[esp+4] ; mov [eax+0Ch],ecx ; xor ecx,ecx ; mov dword ptr [eax],offset Mass::`vftable' ;
//                mov [eax+10h],edx ; mov [eax+18h],ecx ; mov [eax+1Ch],ecx ; add esp,8 ; ret
// Manifest label "CResource::CResource" is propagated and wrong: vtable 0x01288230 is Mass (rtti-vtables.tsv; the landed
// Mass scalar deleting destructor 0x008EE910 sits just before). PDB layout (struct_layouts_egor.tsv, Mass = 36 bytes):
// +4 C2DVector Pos, +0xC C2DVector Vel, +0x14 float IMass, +0x18 float APos, +0x1C float AVel, +0x20 float AIMass.
// Shape: each C2DVector member is ASSIGNED in the body from an 8-byte frame temporary built by the (x, y) constructor
// (both temporaries share the frame slot; the implicit assignment moves the words through ecx/edx -- an init-list
// `Pos(C2DVector(0, 0))` is folded to plain zero stores). Body assignments are scheduled around the vtable store, and the
// two float zeros share the zero register.
struct C2DVector {
    float X;                                 // +0x00
    float Y;                                 // +0x04
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};

struct Mass {                                // vtable 0x01288230, 0x24 bytes
    C2DVector Pos;                           // +0x04
    C2DVector Vel;                           // +0x0C
    float IMass;                             // +0x14 (not initialised here)
    float APos;                              // +0x18
    float AVel;                              // +0x1C
    float AIMass;                            // +0x20 (not initialised here)
    Mass();
    virtual ~Mass() { }
};

Mass::Mass()
{
    Pos = C2DVector(0.0f, 0.0f);
    Vel = C2DVector(0.0f, 0.0f);
    APos = 0.0f;
    AVel = 0.0f;
}