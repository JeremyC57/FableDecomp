// CThingCreatureBase::GetMeleeWeaponDef() const -- the creature def's PrimaryMeleeWeaponDef (a global def index)
// Leaf: ?GetMeleeWeaponDef@CThingCreatureBase@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006623F0, 13 bytes: mov eax,[ecx+0E0h] ; mov eax,[eax+D8h] ; ret.
// Identity (sweep3, FINDINGS chunk 481; was the propagated CPixelShader::CountReferences): Ego_r.pdb
//   ?GetMeleeWeaponDef@CThingCreatureBase@@QBEJXZ and ?GetRangedWeaponDef@CThingCreatureBase@@QBEJXZ are consecutive
//   0x10-byte functions (0x0053D280 / 0x0053D290) as retail 0x006623F0 / 0x00662400; the fields are Ego_r
//   CThingCreatureDef PrimaryMeleeWeaponDef +0xE0 and RangedWeaponDef +0xE8 (SecondaryMeleeWeaponDef between them), read
//   on PC at +0xD8 / +0xE0 through CThingCreatureBase::PDef (Ego_r +0xFC, PC +0xE0, the def pointer GetPHeroCombatDef
//   0x00663340 reads too). Its twin is 0x00662400 GetRangedWeaponDef. The landed caller CCombatManager::SetupNextSequenceForAttacker 0x006EB390
//   already binds ?GetMeleeWeaponDef@CThingCreatureBase@@QBEJXZ.
// The engine header's CThingCreatureBase is the Ego_r layout (PDef +0xFC), not retail's, so the two fields read are
// modelled locally.
class CThingCreatureDef {
public:
    unsigned char _pad00[0xD8];
    long          PrimaryMeleeWeaponDef;     // +0xD8 (Ego_r +0xE0)
    long          SecondaryMeleeWeaponDef;   // +0xDC
    long          RangedWeaponDef;           // +0xE0 (Ego_r +0xE8)
};

template <class T> class CDefPointer {
public:
    T* m_p;
    T* operator->() const { return m_p; }
};

class CThingCreatureBase {
public:
    unsigned char _pad00[0xE0];
    CDefPointer<const CThingCreatureDef> PDef;   // +0xE0 (Ego_r +0xFC)
    long GetMeleeWeaponDef() const;
};

long CThingCreatureBase::GetMeleeWeaponDef() const
{
    return PDef->PrimaryMeleeWeaponDef;
}
