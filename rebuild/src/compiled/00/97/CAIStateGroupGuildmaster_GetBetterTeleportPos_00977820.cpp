// CAIStateGroup_Guildmaster::GetBetterTeleportPos(C3DVector&) -- pick the live teleport marker nearest the enemy (closer than the creature is, and further than the minimum), other than the last one used; store its position and remember it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00977820, length 384 bytes (`ret 4`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10
//          (CIntelligentPointer<CThing> vtable 0x01238C6C), CThing::GetPosition @ 0x004C73D0.
// Globals: the minimum marker distance @ 0x0125916C.
// Layout: engine/CAIStateGroup_Guildmaster.h (PEnemy +0x18, PLastTeleportMarker +0x20, TeleportMarkers +0x28: retail
//          vector first +0x28 / last +0x2C) behind CAIStateGroupBase (PBrain +4).
#include "engine/CAIStateGroup_Guildmaster.h"

extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

class C3DVector { public: float X, Y, Z; };
class C2DVector { public:
    float x, y;
    C2DVector(float x_, float y_) : x(x_), y(y_) { }
    float GetLength() const { return (float)sqrt(x * x + y * y); }
};
inline float Distance2D(const C3DVector& a, const C3DVector& b)
{
    return C2DVector(a.X - b.X, a.Y - b.Y).GetLength();
}
class CThing { public:
    unsigned char _pad[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition();             // 0x004C73D0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CThingPointer : CBaseIntelligentPointer {             // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CThingPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CThingPointer& operator=(const CThingPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
__forceinline bool IS_THING_ALIVE(CThing* const& thing)
{
    if (thing != 0) {
        if (!(thing->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CThingPointerVector {                    // vector<CIntelligentPointer<CThing> > (retail: first, last, end of storage)
    CThingPointer* first;
    CThingPointer* last;
    CThingPointer* endOfStorage;
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
extern const float g_MinTeleportMarkerDistance; // 0x0125916C

struct CAIStateGroupGuildmasterMethods : CAIStateGroup_Guildmaster {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThing* GetCreature() const { return Brain()->PCreature; }
    CBaseIntelligentPointer& Enemy() { return *(CBaseIntelligentPointer*)PEnemy; }
    CThingPointer& LastTeleportMarker() { return *(CThingPointer*)PLastTeleportMarker; }
    CThingPointerVector& Markers() { return *(CThingPointerVector*)TeleportMarkers; }
    bool GetBetterTeleportPos(C3DVector& pos);
};

bool CAIStateGroupGuildmasterMethods::GetBetterTeleportPos(C3DVector& pos)
{
    CThing* creature = GetCreature();
    CThing* best = 0;
    double length = Distance2D(creature->GetPosition(), Enemy().GetPItem()->GetPosition());
    float bestDistance = (float)length;
    for (CThingPointer* it = Markers().first; it != Markers().last; ++it) {
        if (IS_THING_ALIVE(*it)) {
            CThing* enemy = Enemy().GetPItem();
            float distance = Distance2D(enemy->GetPosition(), it->GetPItem()->GetPosition());
            if (distance < bestDistance && distance > g_MinTeleportMarkerDistance) {
                bestDistance = distance;
                best = it->GetPItem();
            }
        }
    }
    if (IS_THING_ALIVE(best) && best != LastTeleportMarker().GetPItem()) {
        pos = best->GetPosition();
        LastTeleportMarker() = CThingPointer(best);
        return true;
    }
    return false;
}