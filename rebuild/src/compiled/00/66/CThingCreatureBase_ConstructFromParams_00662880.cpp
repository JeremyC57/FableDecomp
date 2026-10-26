// CThingCreatureBase::ConstructFromParams(long defIndex, const C3DVector& position, long, CThingPhysical*, CThingPhysical*, const CCharString&, bool) -- construct the game object from creation parameters, then fetch the creature definition into the cached def pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00662880, length 92 bytes (+ int3 padding).
// Callees: CThingGameObject::ConstructFromParams @ 0x008388D0 (eight arguments: the derived arguments re-ordered, with
// a literal 0 for its fourth), the thing's definition-manager accessor @ 0x004C7990 (`[g_main]+0x20`, `this` unused),
// CDefinitionManager::GetDef<CThingCreatureDef>(long, CDefPointer*) @ 0x00513160.
// Layout (retail, local model): CThing's definition global index (unsigned short) at +0x8c; the creature's
// CDefPointer<const CThingCreatureDef> at +0xe0 (engine/CThingCreatureBase.h lists +0xe0 only as a retail-only long).
// Shape: the success path is the `if` body (`return false` is the cold tail): `GetDefinitionManager()->GetDef(index, &m_def)` with the arguments
// pushed before the accessor runs (a chained call, not a local) and `return true`.
class C3DVector;
class CCharString;
class CThingPhysical;
class CThingCreatureDef;

struct CDefPointerCreature {                    // CDefPointer<const CThingCreatureDef>
    CThingCreatureDef* m_pObject;
};

class CDefinitionManager { public:
    bool GetDef(long index, CDefPointerCreature* out);      // 0x00513160 (GetDef<CThingCreatureDef>)
};

class CThingGameObject { public:
    unsigned char  _pad00[0x8c];
    unsigned short m_defGlobalIndex;            // +0x8c
    CDefinitionManager* GetDefinitionManager() const;       // 0x004C7990
    bool ConstructFromParams(long defIndex, const C3DVector& position, long a3, unsigned long a4,
                             const CCharString& name, CThingPhysical* p1, CThingPhysical* physical, bool flag);   // 0x008388D0
};

class CThingCreatureBase : public CThingGameObject { public:
    unsigned char       _pad8e[0x52];
    CDefPointerCreature m_def;                  // +0xe0
    bool ConstructFromParams(long defIndex, const C3DVector& position, long a3, CThingPhysical* p1,
                             CThingPhysical* physical, const CCharString& name, bool flag);
};

bool CThingCreatureBase::ConstructFromParams(long defIndex, const C3DVector& position, long a3, CThingPhysical* p1,
                                             CThingPhysical* physical, const CCharString& name, bool flag)
{
    if (CThingGameObject::ConstructFromParams(defIndex, position, a3, 0, name, p1, physical, flag)) {
        GetDefinitionManager()->GetDef(m_defGlobalIndex, &m_def);
        return true;
    }
    return false;
}