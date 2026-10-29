// C3DPositionInterpolator::C3DPositionInterpolator(const C3DVector& start, const C3DVector& end) -- CBase(), vtable, copy both end points
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AF570, oracle length 62 bytes.
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0 (landed). Vtable 0x0123981C = C3DPositionInterpolator (rtti-vtables.tsv).
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBaseClass::CBaseClass ; mov eax,[esp+8] ; mov [esi],offset vftable ; mov ecx,[eax] ;
//                mov [esi+4],ecx ; mov edx,[eax+4] ; mov [esi+8],edx ; mov eax,[eax+8] ; mov [esi+0Ch],eax ; mov eax,[esp+0Ch] ;
//                (same three moves into +10h..+18h) ; mov eax,esi ; pop esi ; ret 8
// PDB layout (struct_layouts_egor.tsv, 28 bytes): +4 StartPosition, +0x10 EndPosition (C3DVector = 3 floats).
// Each vector is copied as three dword moves (the engine's memberwise C3DVector copy constructor: a trivial
// struct copy would go through `lea` + register pairs instead, tried: DIFFER).
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }   // the engine's memberwise copy constructor
};

class CBaseClass {
public:
    virtual void v00();
    CBaseClass();                               // 0x0099A2D0
};

class C3DPositionInterpolator : public CBaseClass { public:
    virtual void v00();                    // own vtable (0x0123981C)
    C3DVector StartPosition;               // +0x4
    C3DVector EndPosition;                 // +0x10
    C3DPositionInterpolator(const C3DVector& start, const C3DVector& end);
};

C3DPositionInterpolator::C3DPositionInterpolator(const C3DVector& start, const C3DVector& end)
    : CBaseClass(), StartPosition(start), EndPosition(end)
{
}