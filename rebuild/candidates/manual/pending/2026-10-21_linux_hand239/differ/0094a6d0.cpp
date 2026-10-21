// CAIStateGroup_GoToPub::WalkToPoint(const C3DVector&, const CRightHandedSet&) -- walk to the pub point while the pub and the bar live, then turn to look along the point's forward direction; on any failure free the bar point
// Grade: DIFFER(368v361) -- regalloc: retail keeps constant 1 in ebx (push ebx for the bool args, test bl for the dead bit, mov al,bl for true) and the look-at vector in memory; VC7.1 here enregisters the look-at vector (ebp/edi/ebx) and uses immediates
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094A6D0, length 368 bytes (`ret 8`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CAIStateGroupBase::AIAction_MoveToPos @ 0x0097EC00,
//          CThing::GetPosition const @ 0x006A4D00, CAIStateGroupBase::AIAction_LookAtPos @ 0x0097D580,
//          CAIStateGroup_GoToPub::FreeBarPoint @ 0x00949320.
// Globals: none (0.01f look tolerance is an immediate).
// Layout: engine/CAIStateGroupBase.h (PBrain +4) + Ego_r.pdb CAIStateGroup_GoToPub PPub +0x14, PBar +0x1C
//          (CIntelligentPointer); CRightHandedSet Up +0, Forward +0xC.
#include "engine/CAIStateGroupBase.h"

struct CThing;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) { X = x; Y = y; Z = z; }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
};
struct CRightHandedSet {
    C3DVector Up;                               // +0x00
    C3DVector Forward;                          // +0x0c
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x006A4D00
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
enum EAIActionReturn { AI_ACTION_FAILED = 0, AI_ACTION_SUCCEEDED = 2, AI_ACTION_RUNNING = 4 };
enum EAIActionRepeat { AI_ACTION_REPEAT_2 = 2 };
enum EMovementType { MOVEMENT_3 = 3 };

struct CAIStateGroupGoToPubMethods : CAIStateGroupBase {
    CBaseIntelligentPointer PPub;               // +0x14
    CBaseIntelligentPointer PBar;               // +0x1c
    CThing* GetCreature() const { return PBrain->PCreature; }
    EAIActionReturn AIAction_MoveToPos(EAIActionRepeat repeat, C3DVector pos, bool* reached, EMovementType movement,
        bool a, bool b, CThing* avoid, bool c, float speed);   // 0x0097EC00
    EAIActionReturn AIAction_LookAtPos(EAIActionRepeat repeat, C3DVector pos, long lookFlags, float tolerance);   // 0x0097D580
    void FreeBarPoint();                        // 0x00949320
    bool WalkToPoint(const C3DVector& pos, const CRightHandedSet& rhSet);
};

bool CAIStateGroupGoToPubMethods::WalkToPoint(const C3DVector& pos, const CRightHandedSet& rhSet)
{
    EAIActionReturn result;
    do {
        if (!IS_THING_ALIVE(PPub) || !IS_THING_ALIVE(PBar))
            goto failed;
        bool reached;
        result = AIAction_MoveToPos(AI_ACTION_REPEAT_2, pos, &reached, MOVEMENT_3, true, true, 0, true, 0.0f);
    } while (result == AI_ACTION_RUNNING);
    if (result == AI_ACTION_SUCCEEDED) {
        C3DVector forward = rhSet.Forward;
        C3DVector lookAt = forward + GetCreature()->GetPosition();
        do {
            if (!IS_THING_ALIVE(PPub) || !IS_THING_ALIVE(PBar))
                goto failed;
            result = AIAction_LookAtPos(AI_ACTION_REPEAT_2, lookAt, 0x11, 0.01f);
        } while (result == AI_ACTION_RUNNING);
        if (result == AI_ACTION_SUCCEEDED)
            return true;
    }
failed:
    FreeBarPoint();
    return false;
}
