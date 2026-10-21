// CAIStateGroup_KidWanderNearKids::Update() -- repeatedly: find the nearest other child, walk to a wander position within 3m of it, idle, and wait a tick; terminate when no child is found or the walk fails
// Grade: DIFFER(ours v retail) -- named filters + bind before the loop give retail's hoisted bind stores and entry nav load, but the IsNot filter gets a dead zero init and the frame is 4 bytes larger (kid out pointer does not share the nav home)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00914D80, length 338 bytes.
// Callees: the inlined STLport lower_bound of the interface map (7 = CTCCreatureNavigation, bit 7 of +0x20 as a
//          bitset::test byte), CThing::GetPosition const @ 0x006A4D00 / non-const @ 0x004C73D0,
//          CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000, CThingSearchTools::GetPBestThing<CFilter_Bind2<
//          CThingFilter_IsNot, CThingFilter_IsChild>, CThingCompare_Nearest, CThingAICreature> @ 0x00914B20,
//          CTCCreatureNavigation::GetNearbyWanderPosition @ 0x006CB6E0, CAIStateGroupBase::AIAction_MoveToPos @ 0x0097EC00
//          (C3DVector by value), AIAction_MinionIdle @ 0x0097FA00, AIAction_DoNothing @ 0x0097E200, Terminate @ 0x008FCF40.
// Shape: the navigation out pointer is function-scope and only written on a hit, so its never-written home is loaded
//          into esi at entry; `IsNot(creature) && IsChild()` binds two filter temporaries by reference (VC7.1 hoists the
//          bind's two address stores out of the loop); the nearest comparator copies the creature position.
// Layout: engine/CAIStateGroup_KidWanderNearKids.h (PBrain is the first dword of the flattened base at +0x04).
#include "engine/CAIStateGroup_KidWanderNearKids.h"

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)        // STLport lower_bound over the 8-byte entries (inlined in this TU)
    {
        SEntry* it = first;
        int len = int(last - first);
        while (len > 0) {
            int half = len / 2;
            SEntry* middle = it + half;
            if (middle->type < *type) {
                it = middle + 1;
                len = len - half - 1;
            } else
                len = half;
        }
        return it;
    }
};

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& v) { X = v.X; Y = v.Y; Z = v.Z; }
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPosition();             // 0x004C73D0
    const C3DVector& GetPosition() const;       // 0x006A4D00
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
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return has;
    }
};
struct CThingAICreature : CThing { };

struct CTCCreatureNavigation {                  // interface 7
    bool GetNearbyWanderPosition(const C3DVector& centre, float radius, C3DVector* result, bool onMap);   // 0x006CB6E0
};

struct CThingFilter_IsNot {
    const CThing* Excluded;
    CThingFilter_IsNot(const CThing* thing) : Excluded(thing) { }
};
struct CThingFilter_IsChild {
    CThingFilter_IsChild() { }
};
template <class A, class B> struct CFilter_Bind2 {     // both filters, held by reference
    const A& First;
    const B& Second;
    CFilter_Bind2(const A& a, const B& b) : First(a), Second(b) { }
};
template <class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
struct CThingCompare_Nearest {
    C3DVector m_pos;
    CThingCompare_Nearest(const C3DVector& pos) : m_pos(pos) { }
};
struct CThingSearchTools {
    bool GetPBestThing(const CFilter_Bind2<CThingFilter_IsNot, CThingFilter_IsChild>& filter,
                       const CThingCompare_Nearest& compare, CThingAICreature** out);   // 0x00914B20
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupKidWanderNearKidsMethods : CAIStateGroup_KidWanderNearKids {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_base_0x04; }
    CThing* GetCreature() const { return Brain()->PCreature; }
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long movement, bool a, bool b,
        CThing* avoid, bool c, float speed);    // 0x0097EC00
    long AIAction_MinionIdle();                 // 0x0097FA00
    long AIAction_DoNothing();                  // 0x0097E200
    void Terminate();                           // 0x008FCF40
    void Update();
};

void CAIStateGroupKidWanderNearKidsMethods::Update()
{
    CTCCreatureNavigation* navigation;
    CThingFilter_IsChild isChild;
    CThingFilter_IsNot notMe(0);
    CFilter_Bind2<CThingFilter_IsNot, CThingFilter_IsChild> filter(notMe, isChild);
    do {
        GetCreature()->GetInterfaceT(&navigation, 7);
        CThingAICreature* kid;
        notMe = CThingFilter_IsNot(GetCreature());
        if (!PeekThingSearchTools()->GetPBestThing(filter,
                CThingCompare_Nearest(((const CThing*)GetCreature())->GetPosition()), &kid)) {
            Terminate();
            return;
        }
        C3DVector target;
        if (navigation->GetNearbyWanderPosition(kid->GetPosition(), 3.0f, &target, false)) {
            bool reached = false;
            if (AIAction_MoveToPos(1, target, &reached, 3, true, true, 0, true, 0.0f) != 2) {
                Terminate();
                return;
            }
        }
        if (AIAction_MinionIdle() != 2)
            return;
    } while (AIAction_DoNothing() == 4);
}
