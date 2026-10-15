// CThingCreatureBase::NewCreatureInteraction(ECreatureInteractionType type) -- start a new creature interaction of the given type with this creature as its first participant: allocate it and hand it to the creature's counted interaction pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00662970, length 64 bytes.
// Callees: operator new @ 0x00BFEA1A (8 bytes), CCreatureInteraction::CCreatureInteraction(ECreatureInteractionType,
// CThingCreatureBase&) @ 0x00863200 (Ego_r.pdb `??0CCreatureInteraction@@QAE@W4ECreatureInteractionType@@AAVCThingCreatureBase@@@Z`;
// builds the 8-byte {participant list head, interaction type} record),
// CCountedPointer<CCreatureInteraction>::Reset(CCreatureInteraction*) @ 0x00669440 (manifest label propagated from
// another instantiation of the same body).
// Layout (retail, local model): the CCountedPointer<CCreatureInteraction> at +0x12c {object, count block +0x130}
// (engine/CThingCreatureBase.h has PCreatureInteraction at the PDB offset +0x150).
// Shape: `Reset(new T(a, b))` -- the null test of operator new's result tail-duplicates the out-of-line Reset call
// into both arms (`xor eax,eax ; push eax` for the failed allocation).
struct CThingCreatureBase;
enum ECreatureInteractionType { CREATURE_INTERACTION_NONE };

struct CCreatureInteraction {
    void* m_participants;                       // +0 list head
    ECreatureInteractionType m_type;            // +4
    CCreatureInteraction(ECreatureInteractionType type, CThingCreatureBase& first);   // 0x00863200
};

struct SCountBlock {
    long m_references;                          // +0
    void (__fastcall* m_pfnDestroy)(void*);     // +4
    void* m_pObject;                            // +8
};

struct CInteractionPointer {                    // CCountedPointer<CCreatureInteraction>
    CCreatureInteraction* m_pObject;            // +0
    SCountBlock*          m_pCount;             // +4
    void Reset(CCreatureInteraction* object);   // 0x00669440
};

struct CThingCreatureBase {
    unsigned char       _pad00[0x12c];
    CInteractionPointer m_creatureInteraction;  // +0x12c
    void NewCreatureInteraction(ECreatureInteractionType type);
};

void CThingCreatureBase::NewCreatureInteraction(ECreatureInteractionType type)
{
    m_creatureInteraction.Reset(new CCreatureInteraction(type, *this));
}
