// CTCCreatureGenerator::Persist(CPersistContext* context, bool flag) -- persist the generator's families and generation parameters, and with the flag set its live state
// Leaf: Persist
// Retail 0x0074B900, 416 bytes (`ret 8`), CTCCreatureGenerator vtable slot 1 (the TC persist hook).
// Callees: CPersistContext::Transfer of the CArray<CDefIndex> families @ 0x0074DE50, Transfer<float> @ 0x00410620,
// Transfer<bool> @ 0x004045C0, Transfer<long> @ 0x00410BE0, Transfer<CCharString> @ 0x004109A0 (manifest labels
// are propagated), CCharString() @ 0x0099E4B0, CCharString(const CCharString&) @ 0x0099EC30, ~CCharString @ 0x0099EAE0.
// Layout: Ego_r.pdb puts GenParams (TGenerationParameters) at +0x38 and Active at +0x5c; retail is 0x10 lower
// (CreatedCreatures list and CreatureTriggerers vector are smaller in retail): GenerationRadius +0x28 ..
// ScriptNameOfAllGeneratedCreatures +0x48, Active +0x4c, ReadyToInitialise +0x4e, NumTriggersLeft +0x54,
// Destroyed +0x89 (as the Transfer member addresses show).
// Shape: every scalar default is a temporary bound to the const reference and homed in the dead context-argument
// slot; the script-name default is a block-scoped local copied from an inline EmptyName() that returns a named
// local (no NRVO in VC7.1: ctor in the dead argument slot, copy into the frame word, dtor before the Transfer); with the flag set,
// Active defaults to SelfTrigger and NumTriggersLeft to NumTriggers (the members are passed as the defaults).
class CCharString { public:
    void* PStringData;
    CCharString();                                  // 0x0099E4B0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
};

inline CCharString EmptyName() { CCharString name; return name; }

struct CDefIndexArray { unsigned char _pad[0x10]; };

class CPersistContext { public:
    template <class T> void Transfer(const char* name, T& value);   // CPersistContext::Transfer<T> (member template, PDB)
    //   instances here: CDefIndexArray 0x0074DE50
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // CPersistContext::Transfer<T> (member template, PDB)
    //   instances here: float 0x00410620; bool 0x004045C0; long 0x00410BE0; CCharString 0x004109A0
};

struct TGenerationParameters {
    float       GenerationRadius;                   // +0x28
    float       SelfTriggerRadius;                  // +0x2c
    bool        SelfTrigger;                        // +0x30
    long        SelfTriggerResetInterval;           // +0x34
    bool        TriggerOnActivate;                  // +0x38
    long        ActiveCreatureLimit;                // +0x3c
    long        TotalGenerationLimit;               // +0x40
    long        NumTriggers;                        // +0x44
    CCharString ScriptNameOfAllGeneratedCreatures;  // +0x48
};

class CTCCreatureGenerator { public:
    unsigned char         _pad04[0x08];             // +0x04 CTCBase
    CDefIndexArray        CreatureFamilies;         // +0x0c
    unsigned char         _pad1c[0x0c];             // +0x1c CreatedCreatures (retail size)
    TGenerationParameters GenParams;                // +0x28
    bool                  Active;                   // +0x4c
    bool                  Initialised;              // +0x4d
    bool                  ReadyToInitialise;        // +0x4e
    bool                  ExternalTrigger;          // +0x4f
    bool                  WaitForAllCreaturesDead;  // +0x50
    long                  NumTriggersLeft;          // +0x54
    unsigned char         _pad58[0x89 - 0x58];
    bool                  Destroyed;                // +0x89
    virtual void v00();
    virtual void Persist(CPersistContext* context, bool flag);
};

void CTCCreatureGenerator::Persist(CPersistContext* context, bool flag)
{
    context->Transfer("CreatureFamilies", CreatureFamilies);
    context->Transfer("GenerationRadius", GenParams.GenerationRadius, 0.0f);
    context->Transfer("SelfTriggerRadius", GenParams.SelfTriggerRadius, 0.0f);
    context->Transfer("SelfTrigger", GenParams.SelfTrigger, false);
    context->Transfer("SelfTriggerResetInterval", GenParams.SelfTriggerResetInterval, 0L);
    context->Transfer("TriggerOnActivate", GenParams.TriggerOnActivate, false);
    context->Transfer("ActiveCreatureLimit", GenParams.ActiveCreatureLimit, -1L);
    context->Transfer("TotalGenerationLimit", GenParams.TotalGenerationLimit, -1L);
    context->Transfer("NumTriggers", GenParams.NumTriggers, -1L);
    {
        CCharString defaultName = EmptyName();
        context->Transfer("ScriptNameOfAllGeneratedCreatures", GenParams.ScriptNameOfAllGeneratedCreatures, defaultName);
    }
    if (flag) {
        context->Transfer("Active", Active, GenParams.SelfTrigger);
        context->Transfer("ReadyToInitialise", ReadyToInitialise, true);
        context->Transfer("NumTriggersLeft", NumTriggersLeft, GenParams.NumTriggers);
        context->Transfer("Destroyed", Destroyed, false);
    }
}