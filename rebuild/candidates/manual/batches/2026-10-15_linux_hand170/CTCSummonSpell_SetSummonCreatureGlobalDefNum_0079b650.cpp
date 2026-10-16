// CTCSummonSpell::SetSummonCreatureGlobalDefNum(long defNum, const C3DVector& position, const C3DVector& direction) -- record the summoned creature's global def, play the summon-upgrade sound on the caster, kill the current minion and create the new one
// Leaf: SetSummonCreatureGlobalDefNum
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0079B650, length 149 bytes (`ret 0Ch`).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetTC of interface 0x10 = bit 16 of CThing+0x20, the
// sound-player component), CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("HERO_ABILITY_SUMMON_CREATURE_UPGRADE" @ 0x0126C3F0),
// 0x0066BD00 (a CTCSoundPlayer play-sound member taking (const CCharString&, bool, long, long, long); manifest label "CSoundInfo::PlaySound"),
// CCharString::~CCharString @ 0x0099EAE0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill(bool) @ 0x004C9B80 (true),
// CTCSummonSpell::CreateMinion(const C3DVector&, const C3DVector&) @ 0x0079ACC0.
// Globals: CTCSummonSpell::SummonCreatureGlobalDefNum @ 0x0138306C (static member).
// Layout: Ego_r.pdb CTCSummonSpell: Minion (CIntelligentPointer<CThingAICreature>) at the odd offset +0x11 (`#pragma pack(1)`).
// Shape: the component is a named local (`mov esi,[eax+4]`); the CCharString is a named local in its own block, sharing the dead defNum
// argument slot with the lookup key.
class CTCBase;
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    void Kill(bool immediately);                // 0x004C9B80
};
struct CBaseIntelligentPointer {                // CIntelligentPointer<CThingAICreature>: 8 bytes
    void* m_vfptr;
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CTCSoundPlayer {
    void PlaySound(const CCharString& name, bool attached, long a, long b, long c);   // 0x0066BD00
};
#pragma pack(push, 1)
struct CTCSummonSpell {
    void* __vfptr;                              // +0x00 (CTCBase)
    CThing* ParentThing;                        // +0x04
    unsigned char _base08[4];
    void* PDef;                                 // +0x0c CDefPointer<const CSpecialAbilitiesSummonSpellDef>
    bool  JustReleased;                         // +0x10
    CBaseIntelligentPointer Minion;             // +0x11
    static long SummonCreatureGlobalDefNum;     // 0x0138306C
    void CreateMinion(const C3DVector& position, const C3DVector& direction);   // 0x0079ACC0
    void SetSummonCreatureGlobalDefNum(long defNum, const C3DVector& position, const C3DVector& direction);
};
#pragma pack(pop)

void CTCSummonSpell::SetSummonCreatureGlobalDefNum(long defNum, const C3DVector& position, const C3DVector& direction)
{
    SummonCreatureGlobalDefNum = defNum;
    if (ParentThing->HasTC(0x10)) {
        CTCSoundPlayer* sound = (CTCSoundPlayer*)ParentThing->GetTC(0x10);
        {
            CCharString name("HERO_ABILITY_SUMMON_CREATURE_UPGRADE");
            sound->PlaySound(name, true, 0, 0, -1);
        }
    }
    Minion.GetPItem()->Kill(true);
    CreateMinion(position, direction);
}
