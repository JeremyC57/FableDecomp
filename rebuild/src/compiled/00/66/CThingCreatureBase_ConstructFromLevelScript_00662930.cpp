// CThingCreatureBase::ConstructFromLevelScript(const char* script) -- construct the physical thing from its level-script text, then fetch the creature definition into the cached def pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00662930, length 58 bytes (+ int3 padding).
// Callees: CThingPhysical::ConstructFromLevelScript @ 0x00838930, the thing's definition-manager accessor @ 0x004C7990
// (`[g_main]+0x20`, `this` unused), CDefinitionManager::GetDef<CThingCreatureDef>(long, CDefPointer*) @ 0x00513160.
// Layout (retail, local model): CThing's definition global index (unsigned short) at +0x8c; the creature's
// CDefPointer<const CThingCreatureDef> at +0xe0 (engine/CThingCreatureBase.h lists +0xe0 only as a retail-only long).
// Shape: the success path is the `if` body, `GetDefinitionManager()->GetDef(index, &m_def)` with the arguments pushed
// before the accessor runs (a chained call), `return false` the cold tail.
class CThingCreatureDef;

struct CDefPointerCreature {                    // CDefPointer<const CThingCreatureDef>
    CThingCreatureDef* m_pObject;
};

class CDefinitionManager { public:
    bool GetDef(long index, CDefPointerCreature* out);      // 0x00513160 (GetDef<CThingCreatureDef>)
};

class CThingPhysical { public:
    unsigned char  _pad00[0x8c];
    unsigned short m_defGlobalIndex;            // +0x8c
    CDefinitionManager* GetDefinitionManager() const;       // 0x004C7990
    bool ConstructFromLevelScript(const char* script);      // 0x00838930
};

class CThingCreatureBase : public CThingPhysical { public:
    unsigned char       _pad8e[0x52];
    CDefPointerCreature m_def;                  // +0xe0
    bool ConstructFromLevelScript(const char* script);
};

bool CThingCreatureBase::ConstructFromLevelScript(const char* script)
{
    if (CThingPhysical::ConstructFromLevelScript(script)) {
        GetDefinitionManager()->GetDef(m_defGlobalIndex, &m_def);
        return true;
    }
    return false;
}