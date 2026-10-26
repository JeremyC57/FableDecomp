// Behaviour test for 0075cd30: src/0075cd30.cpp copied verbatim, the position getter and TransformPosition stubbed so the sum and the
// forwarded arguments are observable. CTCCameraPointScripted::GetStartPos(const C3DVector*, const CRightHandedSet*) const
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCCameraPointScripted::GetStartPos(const C3DVector* origin, const CRightHandedSet* axes) const -- the scripted camera's start position: (parent position + StartPos) transformed into the given frame
// Leaf: GetStartPos
// Retail: Fable.exe (TLC, VC7.1) function @ 0x000075CD30, length 81 bytes (`ret 0Ch`: hidden result + two pointers).
// Callees: CThing::GetPosition() const @ 0x004C73D0 (const C3DVector&), CTCCameraPointDefinitionBase::TransformPosition(const
//   C3DVector&, const C3DVector*, const CRightHandedSet*) const @ 0x0075C420 (landed; returns through this function's own
//   hidden result pointer).
// Label: Ego_r.pdb names the adjacent pair ?GetStartPos / ?GetEndPos@CTCCameraPointScripted@@QBE?AVC3DVector@@PBV2@PBVCRightHandedSet@@@Z
//   (two pointer parameters); 0x0075CD30 reads +0x40..+0x48 = StartPos and 0x0075CD90 reads +0x4C..+0x54 = EndPos (PDB
//   CTCCameraPointScripted StartPos +0x40, EndPos +0x4C, LookPos +0x58; CTCCameraPointDefinitionBase is 0x40 bytes), so the
//   catalog's "GetEndPos" label on 0x0075CD30 is GetStartPos.
// Layout: CTCCameraPointDefinitionBase as the landed TransformPosition 0x0075C420 model (packed, 0x3D bytes used, size 0x40).
// Shape: the sum is a 12-byte frame temporary built by an inline free operator+ (three-float constructor); the parent thing
//   is reached through an inline GetParentThing() accessor -- with the plain member VC7.1 loads the position's X first
//   (`fld [eax] ; fadd [esi+N]`), through the accessor the member first as retail.
class C3DVector { public:
    float X;
    float Y;
    float Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

inline C3DVector operator+(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
}

class CRightHandedSet;

class CThing { public:
    const C3DVector& GetPosition() const;           // 0x004C73D0
};

#pragma pack(push, 1)
class CTCCameraPointDefinitionBase {
public:
    virtual void v00();
    CThing*       ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad08[0x3d - 0x08];              // +0x08 CTCBase flags .. Activated (+0x3c)
    C3DVector TransformPosition(const C3DVector& position, const C3DVector* origin, const CRightHandedSet* axes) const;   // 0x0075C420
};
class CTCCameraPointScripted : public CTCCameraPointDefinitionBase {
public:
    unsigned char _pad3d[3];                        // +0x3d (base padded to 0x40)
    C3DVector     StartPos;                         // +0x40
    C3DVector     EndPos;                           // +0x4c
    C3DVector     LookPos;                          // +0x58
    CThing* GetParentThing() const { return ParentThing; }
    C3DVector GetStartPos(const C3DVector* origin, const CRightHandedSet* axes) const;
};
#pragma pack(pop)

C3DVector CTCCameraPointScripted::GetStartPos(const C3DVector* origin, const CRightHandedSet* axes) const
{
    return TransformPosition(GetParentThing()->GetPosition() + StartPos, origin, axes);
}

// ---- stubs ----
static C3DVector g_pos(1.0f, 2.0f, 3.0f); static const CThing* g_posOf = 0;
const C3DVector& CThing::GetPosition() const { g_posOf = this; return g_pos; }
void CTCCameraPointDefinitionBase::v00() {}
static C3DVector g_seen; static const C3DVector* g_origin = 0; static const CRightHandedSet* g_axes = 0; static const void* g_self = 0;
C3DVector CTCCameraPointDefinitionBase::TransformPosition(const C3DVector& position, const C3DVector* origin, const CRightHandedSet* axes) const
{ g_seen = position; g_origin = origin; g_axes = axes; g_self = this; return C3DVector(position.X * 2, position.Y * 2, position.Z * 2); }
int main()
{
    static CThing thing;
    static unsigned char raw[sizeof(CTCCameraPointScripted)];
    CTCCameraPointScripted* cp = (CTCCameraPointScripted*)raw;
    cp->ParentThing = &thing;
    cp->StartPos = C3DVector(10.0f, 20.0f, 30.0f); cp->EndPos = C3DVector(100.0f, 200.0f, 300.0f);
    C3DVector origin(5.0f, 5.0f, 5.0f); const CRightHandedSet* axes = (const CRightHandedSet*)0x1234;
    C3DVector r = cp->GetStartPos(&origin, axes);
    const C3DVector& m = cp->StartPos;
    CHECK(g_posOf == &thing); CHECK(g_self == cp); CHECK(g_origin == &origin && g_axes == axes);
    CHECK(g_seen.X == 1.0f + m.X && g_seen.Y == 2.0f + m.Y && g_seen.Z == 3.0f + m.Z);
    CHECK(r.X == 2 * g_seen.X && r.Y == 2 * g_seen.Y && r.Z == 2 * g_seen.Z);
    r = cp->GetStartPos(0, 0);
    CHECK(g_origin == 0 && g_axes == 0);
    if (fails == 0) printf("0075cd30_TEST PASS\n"); else printf("0075cd30_TEST FAIL\n");
    return fails;
}