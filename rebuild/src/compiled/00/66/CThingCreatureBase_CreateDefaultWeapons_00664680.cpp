// CThingCreatureBase::CreateDefaultWeapons() -- create the creature's default weapons from its def (the initial weapon, or when that is -1 the primary + secondary melee weapons or the ranged weapon), then the initial sheathed weapon: a non-persistent thing object owned by the creature, put into the creature's carry slot that the object's carryable component names
// Leaf: ?CreateDefaultWeapons@CThingCreatureBase
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00664680, length 354 bytes (+14 int3), plain `ret`.
//   Name from Ego_r.pdb `?CreateDefaultWeapons@CThingCreatureBase@@IAEXXZ`.
// Callees: CreateWeaponForThingFromDef(CThing*, long) @ 0x00663990 (`__fastcall`), CCharString(const char*, long)
//   @ 0x0099EBF0 ("" @ 0x0122D70E), CThing::GetPos() const @ 0x004C73D0, CreateThingObject @ 0x00703210 (`__fastcall`:
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
//   pair and keeps it in edi across GetPos); the object's lookup goes through a `const CThing*` (const LowerBound).
class CTCBase;
#include "engine/C3DVector.h"
class CRightHandedSet;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad54[0x3c];
    signed char    PlayerNumber;                        // +0x90
    unsigned char  Dead : 1;                            // +0x91 bit 0
    unsigned char  CreatedByOwnerThingCreation : 1;     // +0x91 bit 1
    unsigned char  _bits91 : 6;
    unsigned char  _bits92 : 4;
    unsigned char  LevelPersistent : 1;                 // +0x92 bit 4
    unsigned char  _bits92b : 3;
    const C3DVector& GetPos() const;               // 0x004C73D0
    void SetAsGameStatePersisted(bool persisted);       // 0x004C7880
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(key);
        CInterfaceMap::value_type* end = map->end();
        return (it == end || type < it->first ? end : it)->second;
    }
    __forceinline const CTCBase* GetTC(long type) const
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound(key);
        const CInterfaceMap::value_type* end = map->end();
        return (it == end || type < it->first ? end : it)->second;
    }
};

class CTCCarryable { public:
    long GetCarrySlotPassive() const;                   // 0x00705D00
};
class CTCCarrying { public:
    void PutThingInCarrySlot(CThing* thing, long slot, bool immediately);   // 0x006A1F80
};

void __fastcall CreateWeaponForThingFromDef(CThing* thing, long def);   // 0x00663990
class CThingObject;
class CThingObject { public:
    static CThingObject* __fastcall Create(long a0, C3DVector const&a1, long a2, CRightHandedSet const*a3, unsigned long a4, CCharString const&a5);   // 0x00703210 (Ego_r.pdb ?Create@CThingObject@@SIPAV1@JABVC3DVector@@JPBVCRightHandedSet@@KABVCCharString@@@Z)
};

class CThingCreatureDef { public:
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

class CThingCreatureBase : public CThing { public:
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
        CThing* item = ((CThing*)CThingObject::Create(GetDef()->InitialSheathedWeapon, GetPos(), PlayerNumber, 0, 0, ""));
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