// CThingCreatureBase::SerialiseGameState(CPersistContext& context) -- serialise the physical thing's state; unless the context is in mode 1 or 3, re-fetch the creature definition into the cached def pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006628E0, length 74 bytes (+ int3 padding).
// Callees: CThingPhysical::SerialiseGameState @ 0x006A5AF0, the thing's definition-manager accessor @ 0x004C7990
// (`[g_main]+0x20`, `this` unused), CDefinitionManager::GetDef<CThingCreatureDef>(long, CDefPointer*) @ 0x00513160.
// Layout (retail, local model): CThing's definition global index (unsigned short) at +0x8c; the creature's
// CDefPointer<const CThingCreatureDef> at +0xe0 (engine/CThingCreatureBase.h lists +0xe0 only as a retail-only long);
// the persist context's mode word at +0x18.
// Shape: the context pointer is kept in edi across the base call; the success path is the `if` body with the shared
// `mov al,1` exit, the two mode compares an `&&` of `!=` tests that skip the definition re-fetch.
struct CThingCreatureDef;

struct CPersistContext {
    unsigned char _pad00[0x18];
    long          m_mode;                       // +0x18
};

struct CDefPointerCreature {                    // CDefPointer<const CThingCreatureDef>
    CThingCreatureDef* m_pObject;
};

struct CDefinitionManager {
    bool GetDef(long index, CDefPointerCreature* out);      // 0x00513160 (GetDef<CThingCreatureDef>)
};

struct CThingPhysical {
    unsigned char  _pad00[0x8c];
    unsigned short m_defGlobalIndex;            // +0x8c
    CDefinitionManager* GetDefinitionManager() const;       // 0x004C7990
    bool SerialiseGameState(CPersistContext& context);      // 0x006A5AF0
};

struct CThingCreatureBase : CThingPhysical {
    unsigned char       _pad8e[0x52];
    CDefPointerCreature m_def;                  // +0xe0
    bool SerialiseGameState(CPersistContext& context);
};

bool CThingCreatureBase::SerialiseGameState(CPersistContext& context)
{
    if (CThingPhysical::SerialiseGameState(context)) {
        if (context.m_mode != 1 && context.m_mode != 3)
            GetDefinitionManager()->GetDef(m_defGlobalIndex, &m_def);
        return true;
    }
    return false;
}