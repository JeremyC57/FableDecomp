#include <cstdio>
#include <cstring>
#if !defined(_MSC_VER)
#define __int64 long long
#define __fastcall
#define __cdecl
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
// CThingCreatureBase::CreateDefaultWeapons() -- create the creature's default weapons from its def (the initial weapon, or when that is -1 the primary + secondary melee weapons or the ranged weapon), then the initial sheathed weapon: a non-persistent thing object owned by the creature, put into the creature's carry slot that the object's carryable component names
// Leaf: ?CreateDefaultWeapons@CThingCreatureBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00664680, length 354 bytes (+14 int3), plain `ret`.
//   Name from Ego_r.pdb `?CreateDefaultWeapons@CThingCreatureBase@@IAEXXZ`.
// Callees: CreateWeaponForThingFromDef(CThing*, long) @ 0x00663990 (`__fastcall`), CCharString(const char*, long)
//   @ 0x0099EBF0 ("" @ 0x0122D70E), CThing::GetPosition() const @ 0x004C73D0, CreateThingObject @ 0x00703210 (`__fastcall`:
//   def, position; player, orientation, flags, script name), ~CCharString @ 0x0099EAE0, CThing::SetAsGameStatePersisted
//   @ 0x004C7880, the const CVectorMap LowerBound copy @ 0x004365B0 (the object's lookup) and the non-const @ 0x0040F020
//   (the creature's), the carryable's slot query @ 0x00705D00 (manifest "CTCTavernGame::GetBanter" is propagated; a
//   `long ... const` getter of CTCCarryable, GetCarrySlotPassive by the PDB name list -- unproven),
//   CTCCarrying::PutThingInCarrySlot(CThing*, long, bool) @ 0x006A1F80.
// Layout (retail): CThing bitset +0x20 (0x46 CTCCarrying = bit 6 of +0x28, 0x47 CTCCarryable = bit 7), map +0x44,
//   PlayerNumber +0x90, Dead / CreatedByOwnerThingCreation bits 0/1 of +0x91, LevelPersistent bit 4 of +0x92 (PDB +0xa0..
//   +0xa2, CThing is 0x10 lower); CThingCreatureBase::PDef +0xe0 (PDB +0xfc); CThingCreatureDef InitialWeaponDef +0xd4,
//   Primary/SecondaryMeleeWeaponDef +0xd8/+0xdc, RangedWeaponDef +0xe0, InitialSheathedWeapon +0xe4,
//   UseMeleeWeaponAsDefault bit 4 of +0x148 (PDB 8 higher: def base 4, the NavigatorTypes vector 4).
// Shape: every def read goes through an inline GetDef() pointer accessor (retail copies the def into eax for the melee
//   pair and keeps it in edi across GetPosition); the object's lookup goes through a `const CThing*` (const LowerBound).
struct CTCBase;
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x90 - 0x4c];
    signed char    PlayerNumber;                        // +0x90
    unsigned char  Dead : 1;                            // +0x91 bit 0
    unsigned char  CreatedByOwnerThingCreation : 1;     // +0x91 bit 1
    unsigned char  _bits91 : 6;
    unsigned char  _bits92 : 4;
    unsigned char  LevelPersistent : 1;                 // +0x92 bit 4
    unsigned char  _bits92b : 3;
    const C3DVector& GetPosition() const;               // 0x004C73D0
    void SetAsGameStatePersisted(bool persisted);       // 0x004C7880
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
    __forceinline const CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        const CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};

struct CTCCarryable {
    long GetCarrySlotPassive() const;                   // 0x00705D00
};
struct CTCCarrying {
    void PutThingInCarrySlot(CThing* thing, long slot, bool immediately);   // 0x006A1F80
};

void __fastcall CreateWeaponForThingFromDef(CThing* thing, long def);   // 0x00663990
CThing* __fastcall CreateThingObject(long defIndex, const C3DVector& position, long player, CRightHandedSet* orientation,
    unsigned long flags, const CCharString& scriptName);   // 0x00703210

struct CThingCreatureDef {
    unsigned char _pad00[0xd4];
    long          InitialWeaponDef;                     // +0xd4
    long          PrimaryMeleeWeaponDef;                // +0xd8
    long          SecondaryMeleeWeaponDef;              // +0xdc
    long          RangedWeaponDef;                      // +0xe0
    long          InitialSheathedWeapon;                // +0xe4
    unsigned char _pade8[0x148 - 0xe8];
    unsigned char _bits148 : 4;
    unsigned char UseMeleeWeaponAsDefault : 1;          // +0x148 bit 4
};

struct CThingCreatureBase : CThing {
    unsigned char _pad94[0xe0 - 0x94];
    const CThingCreatureDef* PDef;                      // +0xe0 CDefPointer<const CThingCreatureDef>::Object
    const CThingCreatureDef* GetDef() const { return PDef; }
    void CreateDefaultWeapons();
};

void CThingCreatureBase::CreateDefaultWeapons()
{
    long weapon = 0;
    long secondWeapon = 0;
    if (GetDef()->InitialWeaponDef == -1) {
        if (GetDef()->UseMeleeWeaponAsDefault) {
            weapon = GetDef()->PrimaryMeleeWeaponDef;
            secondWeapon = GetDef()->SecondaryMeleeWeaponDef;
        } else {
            weapon = GetDef()->RangedWeaponDef;
        }
    } else if (GetDef()->InitialWeaponDef) {
        weapon = GetDef()->InitialWeaponDef;
    }
    CreateWeaponForThingFromDef(this, weapon);
    CreateWeaponForThingFromDef(this, secondWeapon);
    if (GetDef()->InitialSheathedWeapon > 0) {
        CThing* item = CreateThingObject(GetDef()->InitialSheathedWeapon, GetPosition(), PlayerNumber, 0, 0, "");
        if (item && !item->Dead) {
            item->SetAsGameStatePersisted(false);
            item->LevelPersistent = false;
            item->CreatedByOwnerThingCreation = true;
            if (item->HasTC(0x47)) {
                const CThing* constItem = item;
                long slot = ((const CTCCarryable*)constItem->GetTC(0x47))->GetCarrySlotPassive();
                if (slot > 0 && HasTC(0x46))
                    ((CTCCarrying*)GetTC(0x46))->PutThingInCarrySlot(item, slot, false);
            }
        }
    }
}


CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; }
CCharString::~CCharString() {}
static C3DVector g_pos = { 1.0f, 2.0f, 3.0f };
const C3DVector& CThing::GetPosition() const { return g_pos; }
static int g_persistCalls = 0;
void CThing::SetAsGameStatePersisted(bool persisted) { if (!persisted) ++g_persistCalls; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
static int g_constLookups = 0;
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{ ++g_constLookups; SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
static long g_slot = 2;
long CTCCarryable::GetCarrySlotPassive() const { return g_slot; }
static CThing* g_carried = 0; static long g_carriedSlot = 0; static int g_puts = 0;
void CTCCarrying::PutThingInCarrySlot(CThing* thing, long slot, bool immediately) { if (!immediately) { ++g_puts; g_carried = thing; g_carriedSlot = slot; } }
static long g_weapons[8]; static int g_nWeapons = 0; static CThing* g_weaponOwner = 0;
void __fastcall CreateWeaponForThingFromDef(CThing* thing, long def) { g_weaponOwner = thing; g_weapons[g_nWeapons++] = def; }
static CThing g_item; static bool g_createFails = false; static long g_createdDef = 0, g_createdPlayer = 0; static const C3DVector* g_createdAt = 0;
CThing* __fastcall CreateThingObject(long defIndex, const C3DVector& position, long player, CRightHandedSet* orientation,
    unsigned long flags, const CCharString& scriptName)
{ g_createdDef = defIndex; g_createdAt = &position; g_createdPlayer = player;
  if (g_createFails || orientation || flags || std::strcmp((const char*)scriptName.PStringData, "")) return 0; return &g_item; }
#define CHECK(c) if (!(c)) { std::printf("00664680_TEST FAIL line %d\n", __LINE__); return 1; }

int main()
{
    static CThingCreatureDef def; static CThingCreatureBase c; c.PDef = &def; c.PlayerNumber = -1;
    static CTCCarrying carrying; static CTCCarryable carryable; static CInterfaceMap::SEntry ce[2], ie[2];
    ce[0].type = 0x46; ce[0].component = (CTCBase*)&carrying; ce[1].type = 0x7fffffff; ce[1].component = 0;
    ie[0].type = 0x47; ie[0].component = (CTCBase*)&carryable; ie[1].type = 0x7fffffff; ie[1].component = 0;
    c.m_interfaces.first = ce; c.m_interfaces.last = ce + 1; c.m_presentBits[2] = 0x40;
    g_item.m_interfaces.first = ie; g_item.m_interfaces.last = ie + 1; g_item.m_presentBits[2] = 0x80; g_item.LevelPersistent = 1;
    def.InitialWeaponDef = 11; def.PrimaryMeleeWeaponDef = 12; def.SecondaryMeleeWeaponDef = 13; def.RangedWeaponDef = 14;
    def.InitialSheathedWeapon = 0;
    c.CreateDefaultWeapons();
    CHECK(g_nWeapons == 2 && g_weapons[0] == 11 && g_weapons[1] == 0 && g_weaponOwner == &c && g_createdDef == 0);
    def.InitialWeaponDef = -1; def.UseMeleeWeaponAsDefault = 1; g_nWeapons = 0;
    c.CreateDefaultWeapons();
    CHECK(g_nWeapons == 2 && g_weapons[0] == 12 && g_weapons[1] == 13);
    def.UseMeleeWeaponAsDefault = 0; g_nWeapons = 0; def.InitialSheathedWeapon = 20;
    c.CreateDefaultWeapons();
    CHECK(g_nWeapons == 2 && g_weapons[0] == 14 && g_weapons[1] == 0);
    CHECK(g_createdDef == 20 && g_createdAt == &g_pos && g_createdPlayer == -1 && g_persistCalls == 1);
    CHECK(!g_item.LevelPersistent && g_item.CreatedByOwnerThingCreation && g_puts == 1 && g_carried == &g_item && g_carriedSlot == 2);
    CHECK(g_constLookups == 1);
    def.InitialWeaponDef = 0; g_nWeapons = 0; g_slot = 0;
    c.CreateDefaultWeapons();
    CHECK(g_weapons[0] == 0 && g_weapons[1] == 0 && g_persistCalls == 2 && g_puts == 1);
    g_slot = 3; g_item.Dead = 1;
    c.CreateDefaultWeapons();
    CHECK(g_persistCalls == 2 && g_puts == 1);
    g_item.Dead = 0; c.m_presentBits[2] = 0;
    c.CreateDefaultWeapons();
    CHECK(g_persistCalls == 3 && g_puts == 1);
    g_createFails = true;
    c.CreateDefaultWeapons();
    CHECK(g_persistCalls == 3);
    std::printf("00664680_TEST PASS\n");
    return 0;
}
