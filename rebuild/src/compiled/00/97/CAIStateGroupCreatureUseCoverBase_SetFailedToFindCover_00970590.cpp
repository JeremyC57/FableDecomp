// CAIStateGroup_CreatureUseCoverBase::SetFailedToFindCover() -- remember where and when cover search failed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00970590, length 54 bytes (+ int3 padding).
// Callees: CThingPhysical::GetPos @ 0x004C73D0 (reference into the creature's physics
// block, copied memberwise into FailedToFindCoverAt), CAIStateGroupBase::GetPWorld @ 0x008FCFC0
// (PBrain -> World, called for its result object only), CWorld::GetFrameCount @ 0x0049D870
// (a static reading the frame counter global; __fastcall with no arguments).
// Retail layout (the engine header is 4 bytes high here because Ego_r's CoverList vector is
// 16 bytes where retail's is 12): FailedToFindCover +0x34, FailedTime +0x38,
// FailedToFindCoverAt +0x3c. The creature comes from PBrain (+0x4) -> Creature (+0x18).
class C3DVector { public:
    float X, Y, Z;
};

class CThing { public:
    const C3DVector& GetPos() const;         // 0x004C73D0
};
class CThingAICreature : public CThing { public:
};

class CWorld { public:
    static long __fastcall GetFrame(); // 0x0049D870
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* Creature;             // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                       // +0x04
    unsigned char _pad[0xc];                // +0x08..+0x13
    CWorld* GetPWorld();                    // 0x008FCFC0
};

struct CAIStateGroup_CreatureUseCoverBase : CAIStateGroupBase {
    unsigned char _pad2[0x20];              // +0x14..+0x33 (target, flags, cover pointer, cover list)
    bool      FailedToFindCover;            // +0x34
    long      FailedTime;                   // +0x38
    C3DVector FailedToFindCoverAt;          // +0x3c
    void SetFailedToFindCover();
};

void CAIStateGroup_CreatureUseCoverBase::SetFailedToFindCover()
{
    FailedToFindCover = true;
    FailedToFindCoverAt = PBrain->Creature->GetPos();
    FailedTime = GetPWorld()->GetFrame();
}