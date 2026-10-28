// CAIStateGroup_GoToPub::WalkToPoint(const C3DVector&, const CRightHandedSet&) -- walk to the pub point while the pub and the bar live, then turn to look along the point's forward direction; on any failure free the bar point
// Copy constructor: the C3DVector model DECLARES its memberwise copy constructor (retail's header class has one); with the
// implicit copy VC7.1 schedules the copies differently (found by the retry-10 copy-constructor sweep).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094A6D0, length 368 bytes (`ret 8`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CAIStateGroupBase::AIAction_MoveToPos @ 0x0097EC00,
//          CThing::GetPosition const @ 0x006A4D00, CAIStateGroupBase::AIAction_LookAtPos @ 0x0097D580,
//          CAIStateGroup_GoToPub::FreeBarPoint @ 0x00949320.
// Globals: none (0.01f look tolerance is an immediate).
// Layout: engine/CAIStateGroupBase.h (PBrain +4) + Ego_r.pdb CAIStateGroup_GoToPub PPub +0x14, PBar +0x1C
//          (CIntelligentPointer); CRightHandedSet Up +0, Forward +0xC.
#include "engine/CAIStateGroupBase.h"

class CThing;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) { X = x; Y = y; Z = z; }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class CRightHandedSet { public:
    C3DVector Up;                               // +0x00
    C3DVector Forward;                          // +0x0c
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CThingPhysical { public:  // retail: CThingPhysical derives from CThing; modelled as its base here (empty insertion, layout unchanged) so CThing-typed callers resolve the call
    const C3DVector& PeekPos() const;       // 0x006A4D00
};
class CThing : public CThingPhysical { public:
    unsigned char _pad[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
};
__forceinline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
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
enum EAIActionReturn { AI_ACTION_FAILED = 0, AI_ACTION_SUCCEEDED = 2, AI_ACTION_RUNNING = 4 };
enum EAIActionRepeat { AI_ACTION_REPEAT_2 = 2 };
enum EMovementType { MOVEMENT_3 = 3 };

struct CAIStateGroupGoToPubMethods : CAIStateGroupBase {
    CIntelligentPointer<CThing> PPub;               // +0x14
    CIntelligentPointer<CThing> PBar;               // +0x1c
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
        C3DVector lookAt = forward + GetCreature()->PeekPos();
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