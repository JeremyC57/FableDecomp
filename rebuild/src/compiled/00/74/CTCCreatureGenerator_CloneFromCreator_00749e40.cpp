// CTCCreatureGenerator::CloneFromCreator(const CTCCreatureGenerator*) -- copies the generation parameters from the creator's generator, then starts active when self-triggered, with the full trigger count left and not ready to initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00749E40, length 87 bytes (+ int3 padding).
// Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0.
// Layout: engine/CTCCreatureGenerator.h (the Ego_r.pdb layout) sits 0x10 bytes high from GenParams on --
// the retail CArray is 0xc bytes, the STLport list of created creatures 4 and the vector 12 -- so the
// model is local: GenParams (TGenerationParameters, 0x24 bytes) +0x28, Active +0x4c, Initialised +0x4d,
// ReadyToInitialise +0x4e, NumTriggersLeft +0x54. TGenerationParameters = Ego_r.pdb: GenerationRadius +0,
// SelfTriggerRadius +4, SelfTrigger +8, SelfTriggerResetInterval +0xc, TriggerOnActivate +0x10,
// ActiveCreatureLimit +0x14, TotalGenerationLimit +0x18, NumTriggers +0x1c, ScriptNameOfAllGeneratedCreatures +0x20.
// Shape: `GenParams = other->GenParams` is the implicit memberwise assignment inlined (integer moves for
// the floats, the string through its operator=), then the three stores in source order.
class CCharString { public:
    void* m_pStringData;
    const CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};

struct TGenerationParameters {                              // 0x24 bytes
    float       GenerationRadius;                           // +0x00
    float       SelfTriggerRadius;                          // +0x04
    bool        SelfTrigger;                                // +0x08
    long        SelfTriggerResetInterval;                   // +0x0c
    bool        TriggerOnActivate;                          // +0x10
    long        ActiveCreatureLimit;                        // +0x14
    long        TotalGenerationLimit;                       // +0x18
    long        NumTriggers;                                // +0x1c
    CCharString ScriptNameOfAllGeneratedCreatures;          // +0x20
};

class CTCCreatureGenerator { public:
    void*                 __vftable;                        // +0x00
    unsigned char         _pad04[0x24];                     // +0x04 CTCBase, CreatureFamilies, CreatedCreatures, CreatureTriggerers
    TGenerationParameters GenParams;                        // +0x28
    bool                  Active;                           // +0x4c
    bool                  Initialised;                      // +0x4d
    bool                  ReadyToInitialise;                // +0x4e
    bool                  ExternalTrigger;                  // +0x4f
    bool                  WaitForAllCreaturesDead;          // +0x50
    long                  NumTriggersLeft;                  // +0x54
    void CloneFromCreator(const CTCCreatureGenerator* creator);
};

void CTCCreatureGenerator::CloneFromCreator(const CTCCreatureGenerator* creator)
{
    GenParams = creator->GenParams;
    Active = GenParams.SelfTrigger;
    NumTriggersLeft = GenParams.NumTriggers;
    ReadyToInitialise = false;
}