// CKeepOutOffSmearedOutLine::operator()(CThing* thing) const -- push a thing out of the smeared-out line: the horizontal vector from the closest point on the finite line to the thing, scaled by (radius - distance), becomes a physics impulse, and the thing's TC 169 / TC 168 components get their flag set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FD8D0, length 321 bytes (`ret 4`).
// Callees: GetLineOutFromClosestPointOnFiniteLineToPoint @ 0x008F5B50 (__fastcall: hidden C3DVector result in ecx, the line
//          start in edx, the line end and the point on the stack), CInterfaceMap::LowerBound @ 0x0040F020 (x2), the physics
//          object's virtual slot 81 (+0x144, const C3DVector&); constants 1e-4f @ 0x0129BA3C, 1.0f @ 0x0122DED8, 0.0f @ 0x0122DEDC.
// Naming: the only caller is std::_For_each<CThingCreatureBase**, CKeepOutOffSmearedOutLine> 0x007FDA80 (landed; its label in
//          labels_chunk205_206_corrections); Ego_r.pdb / FableWin.pdb define ??RCKeepOutOffSmearedOutLine@@QBEXPAVCThing@@@Z and the
//          12-byte functor below, while "CApplyImpulseIfCloseToCombatLine" is in no PDB name set (FINDINGS chunk 397). Was landed as
//          "CApplyImpulseIfCloseToCombatLine::operator()".
// Layout: Ego_r.pdb CKeepOutOffSmearedOutLine {C3DVector* StartPos +0, C3DVector* EndPos +4, float Radius +8}; retail
//          CThing: interface bitset +0x20 (TC 0xa9 / 0xa8 live in word +0x34 bits 9 / 8), CVectorMap +0x44, the physics-side
//          object pointer +0x60 whose position is at +0xc; the TC 169 flag byte +0x9f and TC 168 flag byte +0x89 (names
//          unverified).
// Shape: the thing's position is copied into a named local (its slot stays reserved) for the line call; the result's Z is zeroed, the squared
//          horizontal length guarded against 1e-4, then the vector is normalised in place with an explicit reciprocal, scaled by
//          (Radius - length) into the impulse temporary; the two flag setters go through the out-parameter GetInterface(T**,
//          type) template (the landed 0x007504E0 shape), which keeps each component in eax.
// Leaf: ??RCKeepOutOffSmearedOutLine@@QBEXPAVCThing@@@Z
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector& operator*=(float f) { X *= f; Y *= f; Z *= f; return *this; }
};
C3DVector __fastcall GetLineOutFromClosestPointOnFiniteLineToPoint(const C3DVector& start, const C3DVector& end, const C3DVector& point);   // 0x008F5B50

struct CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CPhysicsObject {                         // the thing's physics-side object (+0x60)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080();
    virtual void ApplyImpulse(const C3DVector& impulse);   // slot 81 (+0x144)
    unsigned char _pad04[0x08];
    C3DVector Pos;                              // +0x0c
};
struct CTCFlag9f { unsigned char _pad[0x9f]; bool Flag; };   // TC 169
struct CTCFlag89 { unsigned char _pad[0x89]; bool Flag; };   // TC 168

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x14];
    CPhysicsObject* PPhysics;                   // +0x60
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CKeepOutOffSmearedOutLine {
public:
    C3DVector* StartPos;                        // +0x00
    C3DVector* EndPos;                          // +0x04
    float Radius;                               // +0x08
    void operator()(CThing* thing) const;
};

void CKeepOutOffSmearedOutLine::operator()(CThing* thing) const
{
    CPhysicsObject* physics = thing->PPhysics;
    C3DVector position = physics->Pos;
    C3DVector out = GetLineOutFromClosestPointOnFiniteLineToPoint(*StartPos, *EndPos, position);
    out.Z = 0.0f;
    float lengthSquared = out.X * out.X + out.Y * out.Y;
    if (lengthSquared > 0.0001f) {
        float length = (float)sqrt(lengthSquared);
        out *= 1.0f / length;
        physics->ApplyImpulse(out * (Radius - length));
        CTCFlag9f* flag9f;
        if (thing->GetInterface(&flag9f, 169))
            flag9f->Flag = true;
        CTCFlag89* flag89;
        if (thing->GetInterface(&flag89, 168))
            flag89->Flag = true;
    }
}