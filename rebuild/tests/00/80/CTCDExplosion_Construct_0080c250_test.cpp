// Test for 0x0080C250 CTCDExplosion::Construct(const C3DVector&): links the source object; the owner thing's physics
// component (+0x60) receives one ConstructFromParams call with a CTCParamsPhysicsBase holding the position and the
// default right-handed set (up (0,0,1), forward (0,1,0)); the params' base constructor and destructor each run once.
#include <stdio.h>
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
static int g_ctor, g_dtor, g_calls;
struct CBaseClassNonCopyable { virtual void v00(); CBaseClassNonCopyable(); ~CBaseClassNonCopyable(); };
CBaseClassNonCopyable::CBaseClassNonCopyable() { ++g_ctor; }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { ++g_dtor; }
void CBaseClassNonCopyable::v00() { }
class CTCParamsBase : public CBaseClassNonCopyable { public: };
class CTCParamsPhysicsBase : public CTCParamsBase { public:
    C3DVector Position;
    CRightHandedSet Rhs;
    virtual void v00();
};
void CTCParamsPhysicsBase::v00() { }
static float g_seen[9];
class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void ConstructFromParams(CTCParamsBase* params);
};
void CTCPhysicsBase::v00() { } void CTCPhysicsBase::v01() { } void CTCPhysicsBase::v02() { }
void CTCPhysicsBase::ConstructFromParams(CTCParamsBase* params)
{
    CTCParamsPhysicsBase* p = (CTCParamsPhysicsBase*)params;
    ++g_calls;
    g_seen[0] = p->Position.X; g_seen[1] = p->Position.Y; g_seen[2] = p->Position.Z;
    g_seen[3] = p->Rhs.Up.X; g_seen[4] = p->Rhs.Up.Y; g_seen[5] = p->Rhs.Up.Z;
    g_seen[6] = p->Rhs.Forward.X; g_seen[7] = p->Rhs.Forward.Y; g_seen[8] = p->Rhs.Forward.Z;
}
class CThing { public: unsigned char _pad00[0x60]; CTCPhysicsBase* PhysicsTC; };
class CTCDExplosion {
    friend int main();
public:
    virtual void v00();
    CThing* Owner;
private:
    void Construct(const C3DVector& position);
};
void CTCDExplosion::v00() { }
int main()
{
    CTCPhysicsBase ph; CThing t; t.PhysicsTC = &ph;
    CTCDExplosion d; d.Owner = &t;
    C3DVector pos(1.0f, 2.0f, 3.0f);
    d.Construct(pos);
    bool ok = g_calls == 1 && g_ctor == 1 && g_dtor == 1
           && g_seen[0] == 1.0f && g_seen[1] == 2.0f && g_seen[2] == 3.0f
           && g_seen[3] == 0.0f && g_seen[4] == 0.0f && g_seen[5] == 1.0f
           && g_seen[6] == 0.0f && g_seen[7] == 1.0f && g_seen[8] == 0.0f;
    if (!ok) { puts("0080c250_TEST FAIL"); return 1; }
    puts("0080c250_TEST PASS");
    return 0;
}
