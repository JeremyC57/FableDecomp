// CTCDrainLife::SetAICast(long spellLevel, long defIndex) -- store the spell level and, for a valid def index, load the drain-life definition into PDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C3040, length 35 bytes.
// Callees: CGameDefinitionManager::Get() @ 0x0044C6B0 (static __fastcall singleton accessor, as the landed
// 0x00928550 / 0x008C2CB0 model it); CDefinitionManager::GetDef<CSpecialAbilitiesDrainLifeDef>(long, CDefPointer&)
// @ 0x007C3A60 (manifest label "GetDef<CThingPhysicalSwitchDef>" names a byte-identical instantiation).
// PDB CTCDrainLife (0x34 bytes): PDef (CDefPointer<const CSpecialAbilitiesDrainLifeDef>) +0x0c, SpellLevel +0x10.
// Shape: `ret 8`; the GetDef arguments (&PDef formed in place with `add ecx,0Ch`, then the index) are pushed
// BEFORE the singleton call that yields the object -- the plain chained `Get().GetDef(index, PDef)`; the
// bool result is dropped and the call is not a sibling jump (different argument count).
struct CSpecialAbilitiesDrainLifeDef;

template<class T> struct CDefPointer {
    T* Object;
};

struct CDefinitionManager_Calls {
    template<class T> bool GetDef(long globalIndex, CDefPointer<T>& out);   // 0x007C3A60 for T = CSpecialAbilitiesDrainLifeDef
};

struct CGameDefinitionManager : CDefinitionManager_Calls {
    static CGameDefinitionManager& __fastcall Get();        // 0x0044C6B0
};

struct CTCDrainLife {
    virtual void v00();
    unsigned char _base[0x8];                               // CTCBase: ParentThing +4, flags +8/+9
    CDefPointer<const CSpecialAbilitiesDrainLifeDef> PDef;  // +0x0c
    long          SpellLevel;                               // +0x10
    void SetAICast(long spellLevel, long defIndex);
};

void CTCDrainLife::SetAICast(long spellLevel, long defIndex)
{
    SpellLevel = spellLevel;
    if (defIndex > 0)
        CGameDefinitionManager::Get().GetDef(defIndex, PDef);
}
