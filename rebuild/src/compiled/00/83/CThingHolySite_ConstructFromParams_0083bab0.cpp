// CThingHolySite::ConstructFromParams(long defIndex, const C3DVector* pos, long a3, bool playerStart) -- base CThingGameObject construction with an empty script name; on success fetches the holy-site def by index and records PlayerStart
// Leaf: ?ConstructFromParams@CThingHolySite
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083BAB0, length 131 bytes (`ret 10h`, + int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("" @ 0x0122D70E), CThingGameObject::ConstructFromParams @ 0x008388D0
// (landed; eight arguments), CCharString::~CCharString @ 0x0099EAE0, CThing::GetDefinitionManager() @ 0x004C7990,
// CDefinitionManager::GetDef<CThingHolySiteDef> @ 0x0083BD10 (manifest label names a propagated CThingPhysicalSwitchDef instantiation).
// Layout: as the landed sibling CThingHolySite::ConstructFromLevelScript 0x0083BBA0: CThing def index word at +0x8c, Def
// (CDefPointer<const CThingHolySiteDef>) at retail +0xd0 (PDB +0xe8), PlayerStart byte at retail +0xd4 (PDB +0xec).
// Shape: the empty name is a named CCharString local in its own block (`lea eax ; push eax`) whose call result goes to a
// bool local (bl) tested after the dtor; base arguments a4 = 1, creators 0, final flag true; `playerStart != 0` gives
// `test al,al ; setne al`.
struct C3DVector;
struct CThingPhysicalArg;
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CThingHolySiteDef {
    unsigned char _pad00[0x7e];
    unsigned char PlayerStart;                         // +0x7e
};
struct CDefPointerHolySiteDef {
    CThingHolySiteDef* m_pDef;
    CThingHolySiteDef* operator->() const { return m_pDef; }
};
struct CDefinitionManager {
    bool GetDef(long globalIndex, CDefPointerHolySiteDef& out);   // 0x0083BD10
};
struct CThingGameObject {
    virtual void v00();
    unsigned char  _pad04[0x88];
    unsigned short m_defIndex;                         // +0x8c
    unsigned char  _pad8e[0x42];
    CDefinitionManager* GetDefinitionManager();        // 0x004C7990
    bool ConstructFromParams(long defIndex, const C3DVector* pos, long a3, unsigned long a4, const CCharString& scriptName,
                             CThingPhysicalArg* creator, CThingPhysicalArg* originalCreator, bool a8);   // 0x008388D0
};
struct CThingHolySite : CThingGameObject {
    CDefPointerHolySiteDef Def;                        // +0xd0
    unsigned char          PlayerStart;                // +0xd4
    bool ConstructFromParams(long defIndex, const C3DVector* pos, long a3, bool playerStart);
};

bool CThingHolySite::ConstructFromParams(long defIndex, const C3DVector* pos, long a3, bool playerStart)
{
    bool ok;
    {
        CCharString name("");
        ok = CThingGameObject::ConstructFromParams(defIndex, pos, a3, 1, name, 0, 0, true);
    }
    if (ok) {
        GetDefinitionManager()->GetDef(m_defIndex, Def);
        PlayerStart = playerStart != 0;
        return true;
    }
    return false;
}