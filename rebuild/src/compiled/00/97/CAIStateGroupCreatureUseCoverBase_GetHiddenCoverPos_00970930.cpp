// CAIStateGroup_CreatureUseCoverBase::GetHiddenCoverPos(C3DVector&, CThing*) -- with a live enemy, aim behind the cover (away from the enemy, 0.75 of twice the cover radius, at least 1.5) and ask the creature's navigation for a nearby wander position it can reach
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00970930, length 384 bytes (`ret 8`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound const @ 0x004365B0 (interface 7 =
//          CTCCreatureNavigation, bit 7 of +0x20), CThing::GetPosition @ 0x004C73D0, C3DVector::AccurateNormalise @ 0x00A14440
//          (manifest "GetScaled" is wrong: in place, no args), CTCCreatureNavigation::GetNearbyWanderPosition @ 0x006CB6E0,
//          CTCCreatureNavigation::IsAbleToNavigateFromPosToPos @ 0x006CB620.
// Globals: 1.5f @ 0x0125AADC, 0.75f @ 0x01238174, 0.15f @ 0x012786A8.
// Shape: the navigation component comes from the masked-dword HasTC test (byte sign test) + one-ternary GetTC; the offset
//          is spelled per component with the repeated (radius * 0.75f) so the CSE dies on the Z multiply (; an
//          inline operator*(float) keeps its parameter live: fld z / fmul st(1) / fstp st(0)).
// Layout: engine/CAIStateGroup_CreatureUseCoverBase.h (enemy CIntelligentPointer at +0x14); the cover's +0x60 pointee's
//          +0x34 float is its radius (descriptive).
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) { X = x; Y = y; Z = z; }
    C3DVector& AccurateNormalise();                           // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
};

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};
struct CBoundsInfo {
    unsigned char _00[0x34];
    float Radius;                               // +0x34 (descriptive)
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x60 - 0x4c];
    CBoundsInfo* PBounds;                       // +0x60 (descriptive)
    unsigned char _pad2[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition();             // 0x004C73D0
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
class CTCCreatureNavigation { public:                  // interface 7
    bool IsAbleToNavigateFromPosToPos(const C3DVector* from, const C3DVector* to, float tolerance);   // 0x006CB620
    bool GetNearbyWanderPosition(const C3DVector* centre, float radius, C3DVector* result, bool flag); // 0x006CB6E0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupCreatureUseCoverBaseMethods : CAIStateGroup_CreatureUseCoverBase {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThing* GetCreature() const { return Brain()->PCreature; }
    CBaseIntelligentPointer& Enemy() { return *(CBaseIntelligentPointer*)_pad_0x14; }
    bool GetHiddenCoverPos(C3DVector& result, CThing* cover);
};

bool CAIStateGroupCreatureUseCoverBaseMethods::GetHiddenCoverPos(C3DVector& result, CThing* cover)
{
    if (!IS_THING_ALIVE(Enemy()))
        return false;
    CTCCreatureNavigation* navigation;
    CThing* creature = GetCreature();
    if (creature->m_presentBits[0] & 0x80)
        navigation = creature->GetTC<CTCCreatureNavigation>(7);
    const C3DVector& enemyPos = Enemy().GetPItem()->GetPosition();
    const C3DVector& coverPos = cover->GetPosition();
    C3DVector away(coverPos.X - enemyPos.X, coverPos.Y - enemyPos.Y, 0.0f);
    away.AccurateNormalise();
    float radius = cover->PBounds->Radius * 2.0f;
    if (radius < 1.5f)
        radius = 1.5f;
    C3DVector offset(away.X * (radius * 0.75f), away.Y * (radius * 0.75f), away.Z * (radius * 0.75f));
    C3DVector target = cover->GetPosition() + offset;
    if (navigation->GetNearbyWanderPosition(&target, radius * 0.15f, &result, false)
        && navigation->IsAbleToNavigateFromPosToPos(&GetCreature()->GetPosition(), &result, 2.0f))
        return true;
    return false;
}