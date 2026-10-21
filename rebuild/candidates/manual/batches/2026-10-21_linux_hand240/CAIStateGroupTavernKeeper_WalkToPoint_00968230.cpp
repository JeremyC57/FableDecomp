// CAIStateGroup_TavernKeeper::WalkToPoint(const C3DVector&, const CRightHandedSet&) -- while the pub and bar live: walk to the point (retrying while in progress), then turn to look along the point's forward axis; true when both finished
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00968230, length 368 bytes (`ret 8`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PBar +0x1c, PPub +0x24),
//          CAIStateGroupBase::AIAction_MoveToPos @ 0x0097EC00 / AIAction_LookAtPos @ 0x0097D580 (C3DVector by value),
//          CThing::GetPosition const @ 0x006A4D00.
// Shape: the constant 1 lives in ebx (bool pushes and the dead-bit tests); the look target is `forward + position` with
//          the forward copied through C3DVector's user copy ctor and the sum built by a three-float ctor (Z first).
// Layout: engine/CAIStateGroup_TavernKeeper.h (PBar +0x1c, PPub +0x24); the
//          CRightHandedSet's forward axis at +0x0c (descriptive).
#include "engine/CAIStateGroup_TavernKeeper.h"

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) { X = x; Y = y; Z = z; }
    C3DVector(const C3DVector& v) { X = v.X; Y = v.Y; Z = v.Z; }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
};
struct CRightHandedSet {
    C3DVector Up;                               // +0x00 (descriptive)
    C3DVector Forward;                          // +0x0c (descriptive)
};
struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x006A4D00
};
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;                            // +0x04
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
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupTavernKeeperMethods : CAIStateGroup_TavernKeeper {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThing* GetCreature() const { return Brain()->PCreature; }
    CBaseIntelligentPointer& Bar() { return *(CBaseIntelligentPointer*)PBar; }
    CBaseIntelligentPointer& Pub() { return *(CBaseIntelligentPointer*)PPub; }
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long movement, bool a, bool b,
        CThing* avoid, bool c, float speed);    // 0x0097EC00
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float speed);   // 0x0097D580
    bool WalkToPoint(const C3DVector& point, const CRightHandedSet& orientation);
};

bool CAIStateGroupTavernKeeperMethods::WalkToPoint(const C3DVector& point, const CRightHandedSet& orientation)
{
    long result;
    do {
        if (!IS_THING_ALIVE(Pub()) || !IS_THING_ALIVE(Bar()))
            return false;
        bool reached;
        result = AIAction_MoveToPos(2, point, &reached, 3, true, true, 0, true, 0.0f);
    } while (result == 4);
    if (result != 2)
        return false;
    C3DVector forward = orientation.Forward;
    C3DVector look = forward + GetCreature()->GetPosition();
    do {
        if (!IS_THING_ALIVE(Pub()) || !IS_THING_ALIVE(Bar()))
            return false;
        result = AIAction_LookAtPos(2, look, 0x11, 0.01f);
    } while (result == 4);
    return result == 2 ? true : false;
}
