// Behaviour test for CIsVeryCloseMotionlessCreature::operator() (0x00942270): category, motion and distance tests.
#include <cstdio>
#include <math.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
#ifndef _MSC_VER
#define __int64 long long
#endif
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
    float GetLength() const { return (float)sqrt(X * X + Y * Y); }
};
extern const float g_minLength_0129ba3c = 0.0001f;
struct CTCBase_Retail;
struct STCEntry_Retail {
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the thing's component map at +0x44
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind) const
    {
        STCEntry_Retail* it = m_pFirst;
        while (it != m_pLast && (unsigned long)it->m_kind < kind) ++it;
        return it;
    }
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
struct CTCMotion {                                 // component kind 2
    unsigned char _base[0x0c];
    C3DVector     Position;                        // +0x0c
    C3DVector     LastPosition;                    // +0x18
};
struct CPhysicsBody {
    unsigned char _pad00[0x34];
    float         Radius;                          // +0x34
};
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                    // +0x20
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x10];
    CPhysicsBody*  m_pBody;                        // +0x60
    long m_category; C3DVector m_pos;
    long GetCreatureCategory() const { return m_category; }
    const C3DVector& GetPosition_006a4d00() const { return m_pos; }
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};
__forceinline CTCMotion* GetMotionIfPresent(const CThing* thing)
{
    CTCMotion* tc;                                 // deliberately unset when the component is absent
    if (thing->HasTC(2))
        tc = (CTCMotion*)thing->m_tcs.Find(2);
    return tc;
}
class CIsVeryCloseMotionlessCreature {
public:
    C3DVector MyPos;                               // +0x00
    float     MyRadius;                            // +0x0c
    bool operator()(CThing* thing) const;
};

bool CIsVeryCloseMotionlessCreature::operator()(CThing* thing) const
{
    if (thing->GetCreatureCategory() == 2) {
        CTCMotion* motion = GetMotionIfPresent(thing);
        if (!((motion->Position - motion->LastPosition).GetLengthSquared() > g_minLength_0129ba3c * g_minLength_0129ba3c)) {
            const C3DVector& pos = thing->GetPosition_006a4d00();
            if (C2DVector(pos.X - MyPos.X, pos.Y - MyPos.Y).GetLength() < (thing->m_pBody->Radius + MyRadius) * 1.4f)
                return true;
        }
    }
    return false;
}

int main()
{
    static CThing t; memset(&t, 0, sizeof(t));
    static CTCMotion motion; memset(&motion, 0, sizeof(motion));
    static CPhysicsBody body; body.Radius = 1.0f;
    STCEntry_Retail entries[1] = { { 2, (CTCBase_Retail*)&motion } };
    t.m_tcs.m_pFirst = entries; t.m_tcs.m_pLast = entries + 1; t.m_tcs.m_pEnd = entries + 1;
    t.m_tcMask[0] = 4; t.m_pBody = &body; t.m_category = 2;
    motion.Position = C3DVector(5.0f, 5.0f, 0.0f); motion.LastPosition = motion.Position;
    t.m_pos = C3DVector(3.0f, 4.0f, 9.0f);
    CIsVeryCloseMotionlessCreature p; p.MyPos = C3DVector(0.0f, 0.0f, 0.0f); p.MyRadius = 3.0f;   // (1 + 3) * 1.4 = 5.6 > 5
    int bad = 0;
    if (!p(&t)) bad = 1;
    p.MyRadius = 2.0f;                               // (1 + 2) * 1.4 = 4.2 < 5
    if (p(&t)) bad = 2;
    p.MyRadius = 3.0f; motion.Position.X = 5.01f;    // moving
    if (p(&t)) bad = 3;
    motion.Position.X = 5.0f; t.m_category = 1;
    if (p(&t)) bad = 4;
    if (bad) { std::printf("00942270_TEST FAIL\n"); return 1; }
    std::printf("00942270_TEST PASS\n");
    return 0;
}