// CAIStateGroupBehaviourBase::IsTargetSeenWithinCloseRange() -- true when the perceived target is being watched directly, in line of sight, and is within 5 units on the ground plane
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009576D0, length 123 bytes. No callers and no vtable slot (it follows
// OnActivate @ 0x009576C0 in the same catalogue row, split via the skipped-row ledger, chunk 906); the manifest label
// "Piece_009576d0" is the ledger placeholder and the name here is descriptive.
// Callees: CTCPerceiveThing::GetDirectAwareness @ 0x00744430 (reads the bool at +0x30; manifest label
// "CTCInventoryBase::GetCanItemBeQuickAssigned" is propagated), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 on
// PTargetThing (+0x14), CThing::GetPosition @ 0x004C73D0 (returns a pointer into the thing's transform; manifest
// label "CVertexBufferWin32::DoSizeof" is propagated). Constants: 0.0f @ 0x0122DEDC, 25.0f @ 0x0123003C.
// Shape: `this` in esi; the three early-out tests share one `xor eax,eax` exit and the success path is `mov eax,1`
// (an int-width result); the brain's creature is loaded into edi before the target's GetPItem/GetPosition pair,
// whose result is kept in esi while the creature's position returns in eax; dx and dy are formed as
// `fld [creature] ; fsub [target]`, squared and summed from dx (the sum is accumulated one statement at a time: a single expression squares dy first),
// then compared with 25.0f (`test ah,5 ; jp`).
// Model: local. CAIStateGroupBase: PBrain +0x04 (CAIBrain: Creature +0x18). CTCPerceiveThing: AwarenessTimer +0x18,
// LineOfSight +0x2d, DirectAwareness +0x30.
struct C3DVector { float X, Y, Z; };

struct CThing {
    const C3DVector* GetPosition() const;          // 0x004C73D0
};

struct CAIBrain {
    unsigned char _pad00[0x18];
    CThing*       Creature;                        // +0x18 the creature this brain drives
};

struct CTCPerceiveThing {
    unsigned char _pad00[0x18];
    float         AwarenessTimer;                  // +0x18 non-zero while the thing is being perceived
    unsigned char _pad1c[0x11];
    bool          LineOfSight;                     // +0x2d
    unsigned char _pad2e[2];
    bool          DirectAwareness;                 // +0x30
    bool GetDirectAwareness();                     // 0x00744430
};

struct CBaseIntelligentPointer {
    unsigned char _pad[8];
    CThing* GetPItem();                            // 0x00A01B50
};

struct CAIStateGroupBehaviourBase {
    void*                   __vftable;             // +0x00
    CAIBrain*               PBrain;                // +0x04
    unsigned char           _pad08[0x0c];          // +0x08 ScriptAIPriority, Name, flags
    CBaseIntelligentPointer PTargetThing;          // +0x14 CIntelligentPointer<const CThing>
    CTCPerceiveThing*       PTCPerception;         // +0x1c
    int IsTargetSeenWithinCloseRange();
};

int CAIStateGroupBehaviourBase::IsTargetSeenWithinCloseRange()
{
    if (PTCPerception->AwarenessTimer != 0.0f && PTCPerception->GetDirectAwareness() && PTCPerception->LineOfSight) {
        CThing* creature = PBrain->Creature;
        const C3DVector* targetPos = PTargetThing.GetPItem()->GetPosition();
        const C3DVector* creaturePos = creature->GetPosition();
        float dx = creaturePos->X - targetPos->X;
        float dy = creaturePos->Y - targetPos->Y;
        float distSq = dx * dx;
        distSq += dy * dy;
        if (distSq < 25.0f)
            return 1;
    }
    return 0;
}
