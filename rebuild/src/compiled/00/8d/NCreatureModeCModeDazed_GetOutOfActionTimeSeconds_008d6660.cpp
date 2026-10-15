// NCreatureMode::CModeDazed::GetOutOfActionTimeSeconds() -- how long a dazed creature stays out of action: the creature rolls it from its def
// Leaf: GetOutOfActionTimeSeconds
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D6660, length 9 bytes (piece of catalogue row 008d6620, ledger chunk 909).
// Vtable 0x01267CC4 slot 14. Callee: CThingCreatureBase::GetDazedTimeSeconds @ 0x006620A0.
// Shape: `call ; ret` rather than a tail jump because the float result goes through a named local (GOTCHAS 24k).
struct CThingCreatureBase {                        // the creature this mode runs on (ParentThing at +0x04 of every mode)
    long  GetOutOfActionRecoveryFrames();          // 0x00662020: def +0xfc through the creature def at +0xe0 (label "Getter_FieldE0_FC")
    float GetStunnedTimeSeconds();                 // 0x00662040: random seconds between def +0x70 and +0x74 (label "GetDazedTimeSeconds", propagated)
    float GetDazedTimeSeconds();                   // 0x006620A0: random seconds between def +0x7c and +0x80
};
struct CModeBase {                                 // CTCCreatureModeManager::CModeBase (0x68 bytes, vtable 0x012673E8)
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void FrameUpdate();                    // slot 3 (+0x0c): 0x0073AC50 counts FramesBeforeRemovingSelf down
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
    CThingCreatureBase* ParentThing;               // +0x04
    unsigned char       _body08[0x60];             // +0x08 .. +0x67 mode state
};
namespace NCreatureMode {
#pragma pack(push, 1)
struct CModeOutOfActionBase : CModeBase {          // vtable 0x0126743C; byte-packed (the attacker pointer sits at +0x6d)
    virtual long  GetOutOfActionRecoveryFrames();  // slot 13 (+0x34): base 0x0073DA20, overridden per mode
    virtual float GetOutOfActionTimeSeconds() = 0; // slot 14 (+0x38): how long the creature stays out of action
    virtual void  Recover() = 0;                   // slot 15 (+0x3c): the mode's way back into action
    long m_frames;                                 // +0x68 frames left before the creature may recover
    bool m_canRecover;                             // +0x6c
    unsigned char _attacker[8];                    // +0x6d CIntelligentPointer<CThing> to whoever put us here
    void FrameUpdate();                            // 0x008D6620
};
#pragma pack(pop)

struct CModeDazed : CModeOutOfActionBase {              // vtable 0x01267CC4
    virtual float GetOutOfActionTimeSeconds();
};
float CModeDazed::GetOutOfActionTimeSeconds()
{
    float seconds = ParentThing->GetDazedTimeSeconds();
    return seconds;
}
}   // namespace NCreatureMode