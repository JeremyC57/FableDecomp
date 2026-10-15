// CThingAICreature::IsAvailableForInteractionType(long interactionType) -- the creature accepts an interaction type when its def lists it and it is not already engaged (+0x12c is null)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00830320, 41 bytes. No callees. Called from 0x006A747C.
// Header: engine/CThingAICreature.h exists but names nothing at these offsets (all `_pad_*`, PDB offsets that
// sit 0x30 above retail at the tail), so the class is modelled locally like the landed 0x00833BA0.
// Shape: the creature def pointer at +0xe0, its interaction-type mask at def +0xec tested against the argument
// (`test [eax+0ECh],edx`); one `&&` expression gives the full-register 0/1 bool.
struct CCreatureDef {
    unsigned char _pad00[0xec];
    unsigned long m_interactionTypes;                // +0xec
};

struct CThingAICreature {
    unsigned char _pad00[0xe0];
    CCreatureDef* m_pCreatureDef;                    // +0xe0
    unsigned char _pade4[0x12c - 0xe4];
    void*         m_pCurrentInteraction;             // +0x12c
    bool IsAvailableForInteractionType(long interactionType);
};

bool CThingAICreature::IsAvailableForInteractionType(long interactionType)
{
    return (m_pCreatureDef->m_interactionTypes & interactionType) && !m_pCurrentInteraction;
}
