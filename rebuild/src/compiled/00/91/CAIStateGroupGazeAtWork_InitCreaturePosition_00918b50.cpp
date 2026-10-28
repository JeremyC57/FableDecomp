// CAIStateGroup_GazeAtWork::InitCreaturePosition() -- place the creature next to a free gaze marker.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00918B50, length 51 bytes.
// Callees: the virtual IsRunnable (slot 11, +0x2c), CAIStateGroup_Gaze::FindFreeGazePos @ 0x00918880,
// CThing::GetPosition @ 0x004C73D0 (manifest "CVertexBufferWin32::DoSizeof" is propagated; it returns the
// C3DVector* pushed to the placer), CThingCreatureBase::AutoPlaceNear(C3DVector*, float) @ 0x00665DF0.
//
// Shape: `if (IsRunnable()) { ... return placer; } return false;` (the nested form: an early `return false`
// lets VC reuse the virtual call's zero `al` instead of emitting retail's shared `xor al,al` tail); the brain's creature (PBrain->PCreature at +0x18, read into
// a local BEFORE GetPosition so it survives the call in esi) is auto-placed within 1.0 of the marker's
// position; the placer's bool is returned straight through (`pop esi ; ret` after the call), the early
// exit is `xor al,al`. The CAIStateGroup_Gaze/GazeAtWork engine headers flatten everything into pads, so
// only CAIStateGroupBase's layout (PBrain at +4) is modelled, with the brain's creature pointer at +0x18.
class C3DVector;

class CThing { public:
    const C3DVector& GetPos() const;    // 0x004C73D0
};
class CThingGameObject : public CThing { public:
};

class CThingAICreature { public:
    bool AutoPlaceNear(C3DVector* pPosition, float radius);   // 0x00665DF0
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};

class CAIStateGroupBase { public:
    virtual void v00();                         // slot 0  (deleting destructor)
    virtual void v01();                         // slot 1  Initialise
    virtual void v02();                         // slot 2  OnActivate
    virtual void v03();                         // slot 3  InitCreaturePosition
    virtual void v04();                         // slot 4  AllowPriorityOverlap
    virtual void v05();                         // slot 5  IsStillValid
    virtual void v06();                         // slot 6  GetDebugText
    virtual void v07();                         // slot 7  IsTimingCritical
    virtual void v08();                         // slot 8  GetStateGroupType
    virtual void v09();                         // slot 9  Cleanup
    virtual void v10();                         // slot 10 OnReplace
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    CAIBrain* PBrain;                           // +0x04
};

struct CAIStateGroup_Gaze : CAIStateGroupBase {
    unsigned char _gaze[0x1c];                  // +0x08 priority/name/flags, GazeMarker, running parameters
    CThingGameObject* FindFreeGazePos();        // 0x00918880
};

struct CAIStateGroup_GazeAtWork : CAIStateGroup_Gaze {
    bool InitCreaturePosition();
};

bool CAIStateGroup_GazeAtWork::InitCreaturePosition()
{
    if (IsRunnable()) {
        CThingGameObject* pGazePos = FindFreeGazePos();
        CThingAICreature* pCreature = PBrain->PCreature;
        return pCreature->AutoPlaceNear((C3DVector*)&pGazePos->GetPos(), 1.0f);
    }
    return false;
}