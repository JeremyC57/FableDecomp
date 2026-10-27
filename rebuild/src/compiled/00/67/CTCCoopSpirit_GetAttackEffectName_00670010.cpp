// CTCCoopSpirit::GetAttackEffectName() const -- the text of the def's AttackTargetParticle from the global def-string table
// Leaf: ?GetAttackEffectName@CTCCoopSpirit@@QBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00670010, 56 bytes, thiscall, `ret 4` (hidden CCharString result).
// Identity (proven): Ego_r.pdb ?GetAttackEffectName@CTCCoopSpirit@@QBE?AVCCharString@@XZ; CTCCoopSpirit::PCoopSpiritDef is the CDefPointer<const CCoopSpiritDef> at +0x28 and CCoopSpiritDef::AttackTargetParticle the CDefString at +0x34 (both Ego_r.pdb, both read here); it sits among the CTCCoopSpirit methods (0x0066FF20 SwapToHero, 0x00670050 ctor) and its caller is CCreatureAction_SpiritAttack::CreateAttackEffect 0x008548B0. The manifest's "CTCActionUseBase::GetActionUseAnimationGroupName" is propagated across
//   byte-identical bodies.
// Callees: CDefStringTable::GetString(long) const 0x009D49B0 on g_DefStringTable 0x013CA828 (Ego_r.pdb GDefStringTable; the
//   landed TUs' spelling), CCharString::CCharString(const CCharString&) 0x0099EC30, CCharString::~CCharString() 0x0099EAE0.
// Shape (family M186, representative 0x00670010): `CCharString name = g_DefStringTable.GetString(PCoopSpiritDef->AttackTargetParticle.TablePos);
//   return name;` -- the table's result lands in the `push ecx` local, is copied into the hidden result and destroyed (no
//   NRVO in VC7.1). Speed TU.
class CCharString {
public:
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
    void* PStringData;                                    // +0x00
};
class CDefString {
public:
    long TablePos;                                        // +0x00
};
class CDefStringTable {
public:
    CCharString GetString(long index) const;              // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                  // 0x013CA828 (Ego_r.pdb GDefStringTable)
template <class T> class CDefPointer {
public:
    T* operator->() const { return Data; }
    T* Data;                                              // +0x00
};
class CCoopSpiritDef {                                             // Ego_r.pdb
public:
    char Base[0x34];
    CDefString AttackTargetParticle;                                     // +0x34
};
class CTCCoopSpirit {
public:
    CCharString GetAttackEffectName() const;
    void* VTable;                                         // +0x00 (CTCBase)
    void* Base[2];                                        // +0x04
    long NoMovementTimer;
    long Score;
    long OffscreenCounter;
    float MovementVector[3];
    void* Master;
    CDefPointer<const CCoopSpiritDef> PCoopSpiritDef;              // +0x28
};
CCharString CTCCoopSpirit::GetAttackEffectName() const
{
    CCharString name = g_DefStringTable.GetString(PCoopSpiritDef->AttackTargetParticle.TablePos);
    return name;
}