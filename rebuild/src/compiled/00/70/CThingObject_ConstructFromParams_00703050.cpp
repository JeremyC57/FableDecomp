// CThingObject::ConstructFromParams(long defIndex, const C3DVector* pos, long a3, unsigned long a4, const CCharString& scriptName) -- base CThingGameObject construction; on success fetches the object def by index and starts at full health
// Leaf: ?ConstructFromParams@CThingObject
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00703050, length 103 bytes (`ret 14h`, + int3 padding).
// Callees: CThingGameObject::ConstructFromParams @ 0x008388D0 (landed; eight arguments), CThing::GetDefinitionManager() @ 0x004C7990,
// CDefinitionManager::GetDef<CThingObjectDef> @ 0x0042B040 (manifest label names a propagated CSpecialAbilitiesDrunkennessDef instantiation).
// Layout (retail, local model): CThing def index word at +0x8c; CThingPhysical MaxHealth +0xb0 / Health +0xb4 (PDB +0xc8/+0xcc,
// retail CThing 24 bytes smaller); CThingObject PDef (CDefPointer<const CThingObjectDef>) at retail +0xd0 (PDB +0xe8); the def's
// starting health float at +0x50.
// Shape: `MaxHealth = Health = PDef->Health;` gives `fld ; fst [b4] ; fstp [b0]`; the PDef address is kept in edi across
// the GetDef call and re-read for the def pointer.
class C3DVector;
struct CThingPhysicalArg;
class CCharString { public:
    void* m_pData;
};
class CThingObjectDef { public:
    unsigned char _pad00[0x50];
    float         Health;                              // +0x50
};
struct CDefPointerObjectDef {
    CThingObjectDef* m_pDef;
    CThingObjectDef* operator->() const { return m_pDef; }
};
class CDefinitionManager { public:
    bool GetDef(long globalIndex, CDefPointerObjectDef& out);   // 0x0042B040
};
class CThingGameObject { public:
    virtual void v00();
    unsigned char  _pad04[0x88];
    unsigned short m_defIndex;                         // +0x8c
    unsigned char  _pad8e[0xb0 - 0x8e];
    float          MaxHealth;                          // +0xb0
    float          Health;                             // +0xb4
    unsigned char  _padb8[0xd0 - 0xb8];
    CDefinitionManager* GetDefinitionManager();        // 0x004C7990
    bool ConstructFromParams(long defIndex, const C3DVector* pos, long a3, unsigned long a4, const CCharString& scriptName,
                             CThingPhysicalArg* creator, CThingPhysicalArg* originalCreator, bool a8);   // 0x008388D0
};
class CThingObject : public CThingGameObject { public:
    CDefPointerObjectDef PDef;                         // +0xd0
    bool ConstructFromParams(long defIndex, const C3DVector* pos, long a3, unsigned long a4, const CCharString& scriptName);
};

bool CThingObject::ConstructFromParams(long defIndex, const C3DVector* pos, long a3, unsigned long a4, const CCharString& scriptName)
{
    if (CThingGameObject::ConstructFromParams(defIndex, pos, a3, a4, scriptName, 0, 0, true)) {
        GetDefinitionManager()->GetDef(m_defIndex, PDef);
        MaxHealth = Health = PDef->Health;
        return true;
    }
    return false;
}