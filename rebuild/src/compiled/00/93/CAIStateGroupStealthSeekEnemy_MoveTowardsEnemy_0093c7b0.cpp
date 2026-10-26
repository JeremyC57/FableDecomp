// CAIStateGroup_StealthSeekEnemy::MoveTowardsEnemy(const CTCPerceiveThing* perceive) -- while the seek is still runnable, walk to within 1.5 of the last perceived position, re-issuing the move while it reports "in progress" (4); terminate the state group when the seek stops being runnable or the move fails (1)
// Leaf: ?MoveTowardsEnemy@CAIStateGroup_StealthSeekEnemy@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0093C7B0, 118 bytes (128 with padding), __fastcall `ret 4`.
// Identity: Ego_r.pdb ?MoveTowardsEnemy@CAIStateGroup_StealthSeekEnemy@@AAEXPBVCTCPerceiveThing@@@Z (private); the only caller is
//   this class's Update 0x0093C948 (landed, where it is declared descriptively as "MoveOrQuit"). The catalog's
//   "CAIStateGroup_StealthSeekEnemy_MoveOrQuit" is descriptive.
// Callees: IsStillRunnable(const CTCPerceiveThing&) const 0x0093C4E0 (landed), CAIStateGroupBase::AIAction_MoveToPosProximity
//   0x0097E890 (repeat mode 2, the C3DVector BY VALUE, radius 1.5f, &arrived, movement type 3, true, false, true, no leader,
//   true, 0.0f -- the landed MinionCuriosity 0x00933A40 signature), CAIStateGroupBase::Terminate 0x008FCF40.
// Layout: Ego_r.pdb CTCPerceiveThing::LastPerceivedPos at +0x20 (C3DVector).
// Shape: a do/while on the move result with the runnable test at its (aligned) head; the not-runnable exit and the
//   `result == 1` tail share one Terminate block (a `while (IsStillRunnable(..))` loop is rotated and duplicates the test).
//   `bool arrived;` is uninitialised and VC homes it in the dead `perceive` argument slot (`lea ecx,[esp+34h]`). The
//   position goes through a named local copy `C3DVector pos = perceive->LastPerceivedPos;` (user copy constructor): X/Y
//   stay on the FPU stack, Z goes through the local's frame slot, then the by-value argument is built in its slot.
class CThing;

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class CTCPerceiveThing {                        // Ego_r.pdb names
public:
    unsigned char _pad00[0x20];
    C3DVector LastPerceivedPos;                 // +0x20
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];                   // +0x04..+0x13
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float radius, bool* reached, long movementType,
                                     bool a, bool b, bool c, CThing* leader, bool d, float e);   // 0x0097E890
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_StealthSeekEnemy : CAIStateGroupBase {
    bool IsStillRunnable(const CTCPerceiveThing& perceive) const;   // 0x0093C4E0
    void MoveTowardsEnemy(const CTCPerceiveThing* perceive);
};

void CAIStateGroup_StealthSeekEnemy::MoveTowardsEnemy(const CTCPerceiveThing* perceive)
{
    bool arrived;
    long result;
    do {
        if (!IsStillRunnable(*perceive)) {
            Terminate();
            return;
        }
        C3DVector pos = perceive->LastPerceivedPos;
        result = AIAction_MoveToPosProximity(2, pos, 1.5f, &arrived, 3, true, false, true, 0, true, 0.0f);
    } while (result == 4);
    if (result == 1)
        Terminate();
}