// CAIStateGroup_OpinionReactionRoot::FollowUpdate(float distance, bool run) -- once the reacted-to thing is farther than `distance`, turn to face it and walk (or run) to a wander position within 0.9 x distance of it; true while following is fine
// Grade: DIFFER(ours v retail) -- regalloc only: the navigation lookup loads the brain into eax (retail ecx then eax/edx), and run lands in dl/cl with the wander X loaded late (retail bl/eax, X loaded early)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009852C0, length 352 bytes (`ret 8`); PDB signature
//          ?FollowUpdate@CAIStateGroup_OpinionReactionRoot@@AAE_NM_N@Z.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (ThingReactingTo +0x6b), CThing::GetPosition @ 0x004C73D0,
//          TurnToFace @ 0x0095EDF0, the inlined GetInterface 7 (CTCCreatureNavigation, bit 7 of +0x20 as a bitset::test
//          byte; the miss path reads the dead `run` argument slot that VC7.1 made the out pointer's home),
//          CTCCreatureNavigation::GetNearbyWanderPosition @ 0x006CB6E0, CAIStateGroupBase::AIAction_MoveToPosProximity
//          @ 0x0097E890 (C3DVector by value).
// Globals: 0.9f @ 0x01253AF0.
// Shape: `target - creature` through a three-float ctor (Z first), its length through the sqrt intrinsic; the movement
//          type is `run ? 5 : 3` (setne + lea); the result is `MoveToPosProximity(...) == 2`.
// Layout: engine/CAIStateGroup_OpinionReactionRoot.h (ThingReactingTo +0x6b); PBrain is the first dword at +0x04.
#include "engine/CAIStateGroup_OpinionReactionRoot.h"
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) { X = x; Y = y; Z = z; }
    C3DVector(const C3DVector& v) { X = v.X; Y = v.Y; Z = v.Z; }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetMagnitudeSquared() const { return X * X + Y * Y + Z * Z; }
    float GetMagnitude() const { return (float)sqrt(GetMagnitudeSquared()); }
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPosition();             // 0x004C73D0
    bool TestTC(long interfaceType) const
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
        return has;
    }
};
struct CTCCreatureNavigation {                  // interface 7
    bool GetNearbyWanderPosition(const C3DVector& centre, float radius, C3DVector* result, bool onMap);   // 0x006CB6E0
};
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupOpinionReactionRootMethods : CAIStateGroup_OpinionReactionRoot {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThing* GetCreature() const { return Brain()->PCreature; }
    CBaseIntelligentPointer& Target() { return *(CBaseIntelligentPointer*)ThingReactingTo; }
    bool TurnToFace();                          // 0x0095EDF0
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float distance, bool* reached, long movement,
        bool a, bool b, bool c, CThing* avoid, bool d, float speed);        // 0x0097E890
    bool FollowUpdate(float distance, bool run);
};

bool CAIStateGroupOpinionReactionRootMethods::FollowUpdate(float distance, bool run)
{
    CThing* creature = GetCreature();
    CThing* target = Target().GetPItem();
    const C3DVector& from = creature->GetPosition();
    float lengthSquared;
    {
        C3DVector delta = target->GetPosition() - from;
        lengthSquared = delta.GetMagnitudeSquared();
    }
    if ((float)sqrt(lengthSquared) > distance) {
        if (!TurnToFace())
            return false;
        CTCCreatureNavigation* navigation;
        GetCreature()->GetInterfaceT(&navigation, 7);
        C3DVector wander;
        if (navigation->GetNearbyWanderPosition(Target().GetPItem()->GetPosition(), distance * 0.9f, &wander, false)) {
            bool reached;
            return AIAction_MoveToPosProximity(1, wander, 0.5f, &reached, run ? 5 : 3, true, false, true, 0, true, 0.0f) == 2;
        }
    }
    return true;
}
