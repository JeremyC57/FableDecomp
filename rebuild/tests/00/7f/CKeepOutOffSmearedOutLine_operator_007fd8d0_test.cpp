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
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector& operator*=(float f) { X *= f; Y *= f; Z *= f; return *this; }
};
C3DVector __fastcall GetLineOutFromClosestPointOnFiniteLineToPoint(const C3DVector& start, const C3DVector& end, const C3DVector& point);   // 0x008F5B50

class CTCBase;
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

// ---- harness ----
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
void CPhysicsObject::v000() { }
void CPhysicsObject::v001() { }
void CPhysicsObject::v002() { }
void CPhysicsObject::v003() { }
void CPhysicsObject::v004() { }
void CPhysicsObject::v005() { }
void CPhysicsObject::v006() { }
void CPhysicsObject::v007() { }
void CPhysicsObject::v008() { }
void CPhysicsObject::v009() { }
void CPhysicsObject::v010() { }
void CPhysicsObject::v011() { }
void CPhysicsObject::v012() { }
void CPhysicsObject::v013() { }
void CPhysicsObject::v014() { }
void CPhysicsObject::v015() { }
void CPhysicsObject::v016() { }
void CPhysicsObject::v017() { }
void CPhysicsObject::v018() { }
void CPhysicsObject::v019() { }
void CPhysicsObject::v020() { }
void CPhysicsObject::v021() { }
void CPhysicsObject::v022() { }
void CPhysicsObject::v023() { }
void CPhysicsObject::v024() { }
void CPhysicsObject::v025() { }
void CPhysicsObject::v026() { }
void CPhysicsObject::v027() { }
void CPhysicsObject::v028() { }
void CPhysicsObject::v029() { }
void CPhysicsObject::v030() { }
void CPhysicsObject::v031() { }
void CPhysicsObject::v032() { }
void CPhysicsObject::v033() { }
void CPhysicsObject::v034() { }
void CPhysicsObject::v035() { }
void CPhysicsObject::v036() { }
void CPhysicsObject::v037() { }
void CPhysicsObject::v038() { }
void CPhysicsObject::v039() { }
void CPhysicsObject::v040() { }
void CPhysicsObject::v041() { }
void CPhysicsObject::v042() { }
void CPhysicsObject::v043() { }
void CPhysicsObject::v044() { }
void CPhysicsObject::v045() { }
void CPhysicsObject::v046() { }
void CPhysicsObject::v047() { }
void CPhysicsObject::v048() { }
void CPhysicsObject::v049() { }
void CPhysicsObject::v050() { }
void CPhysicsObject::v051() { }
void CPhysicsObject::v052() { }
void CPhysicsObject::v053() { }
void CPhysicsObject::v054() { }
void CPhysicsObject::v055() { }
void CPhysicsObject::v056() { }
void CPhysicsObject::v057() { }
void CPhysicsObject::v058() { }
void CPhysicsObject::v059() { }
void CPhysicsObject::v060() { }
void CPhysicsObject::v061() { }
void CPhysicsObject::v062() { }
void CPhysicsObject::v063() { }
void CPhysicsObject::v064() { }
void CPhysicsObject::v065() { }
void CPhysicsObject::v066() { }
void CPhysicsObject::v067() { }
void CPhysicsObject::v068() { }
void CPhysicsObject::v069() { }
void CPhysicsObject::v070() { }
void CPhysicsObject::v071() { }
void CPhysicsObject::v072() { }
void CPhysicsObject::v073() { }
void CPhysicsObject::v074() { }
void CPhysicsObject::v075() { }
void CPhysicsObject::v076() { }
void CPhysicsObject::v077() { }
void CPhysicsObject::v078() { }
void CPhysicsObject::v079() { }
void CPhysicsObject::v080() { }
static int g_impulses = 0;
static C3DVector g_lastImpulse(0.0f, 0.0f, 0.0f);
void CPhysicsObject::ApplyImpulse(const C3DVector& impulse) { ++g_impulses; g_lastImpulse = impulse; }
// the closest-point helper: the vector from the line start to the point (Z 5, zeroed by the caller)
C3DVector __fastcall GetLineOutFromClosestPointOnFiniteLineToPoint(const C3DVector& start, const C3DVector& end, const C3DVector& point)
{
    return C3DVector(point.X - start.X, point.Y - start.Y, 5.0f);
}
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
}
static bool Near(float a, float b) { return a - b < 0.0001f && b - a < 0.0001f; }
static CThing g_thing;
static CPhysicsObject g_physics;
static CTCFlag9f g_flag9f;
static CTCFlag89 g_flag89;
static CInterfaceMap::SEntry g_entries[2];
int main()
{
    g_physics.Pos = C3DVector(3.0f, 4.0f, 7.0f);
    g_thing.PPhysics = &g_physics;
    g_entries[0].type = 168; g_entries[0].component = (CTCBase*)&g_flag89;
    g_entries[1].type = 169; g_entries[1].component = (CTCBase*)&g_flag9f;
    g_thing.m_interfaces.first = g_entries; g_thing.m_interfaces.last = g_entries + 2;
    C3DVector start(0.0f, 0.0f, 0.0f), end(10.0f, 0.0f, 0.0f);
    CKeepOutOffSmearedOutLine keepOut;
    keepOut.StartPos = &start; keepOut.EndPos = &end; keepOut.Radius = 8.0f;
    const CKeepOutOffSmearedOutLine& push = keepOut;
    push(&g_thing);                                   // distance 5: impulse (3,4,0)/5 * (8 - 5); no TCs present
    CHECK(g_impulses == 1 && Near(g_lastImpulse.X, 1.8f) && Near(g_lastImpulse.Y, 2.4f) && g_lastImpulse.Z == 0.0f);
    CHECK(!g_flag9f.Flag && !g_flag89.Flag);
    g_thing.m_presentBits[169 >> 5] = (1UL << (169 & 31));   // TC 169 only
    push(&g_thing);
    CHECK(g_impulses == 2 && g_flag9f.Flag && !g_flag89.Flag);
    g_thing.m_presentBits[168 >> 5] |= (1UL << (168 & 31));
    push(&g_thing);
    CHECK(g_impulses == 3 && g_flag89.Flag);
    g_flag9f.Flag = g_flag89.Flag = false;
    g_physics.Pos = C3DVector(0.001f, 0.001f, 9.0f);  // on the line: no impulse, no flags
    push(&g_thing);
    CHECK(g_impulses == 3 && !g_flag9f.Flag && !g_flag89.Flag);
    if (fails == 0) printf("007fd8d0_TEST PASS\n");
    return fails;
}