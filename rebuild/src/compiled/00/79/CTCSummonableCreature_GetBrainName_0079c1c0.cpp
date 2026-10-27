// CTCSummonableCreature::GetBrainName() const -- the text of the def's SummonedBrain from the global def-string table
// Leaf: ?GetBrainName@CTCSummonableCreature@@QBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079C1C0, 56 bytes, thiscall, `ret 4` (hidden CCharString result).
// Identity (proven): Ego_r.pdb ?GetBrainName@CTCSummonableCreature@@QBE?AVCCharString@@XZ; CTCSummonableCreature::PDef is the CDefPointer<const CSummonableCreatureDef> at +0xC and CSummonableCreatureDef::SummonedBrain the CDefString at +0x2C (both Ego_r.pdb); it sits before the CTCSummonableCreature ctor 0x0079C240 and its caller is CTCSummonSpell::CreateMinion 0x0079ACC0. The manifest's "CTCActionUseBase::GetActionUseAnimationGroupName" is propagated across
//   byte-identical bodies.
// Callees: CDefStringTable::GetString(long) const 0x009D49B0 on g_DefStringTable 0x013CA828 (Ego_r.pdb GDefStringTable; the
//   landed TUs' spelling), CCharString::CCharString(const CCharString&) 0x0099EC30, CCharString::~CCharString() 0x0099EAE0.
// Shape (family M186, representative 0x00670010): `CCharString name = g_DefStringTable.GetString(PDef->SummonedBrain.TablePos);
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
class CSummonableCreatureDef {                                             // Ego_r.pdb
public:
    char Base[0x2C];
    CDefString SummonedBrain;                                     // +0x2C
};
class CTCSummonableCreature {
public:
    CCharString GetBrainName() const;
    void* VTable;                                         // +0x00 (CTCBase)
    void* Base[2];                                        // +0x04
    CDefPointer<const CSummonableCreatureDef> PDef;              // +0xC
};
CCharString CTCSummonableCreature::GetBrainName() const
{
    CCharString name = g_DefStringTable.GetString(PDef->SummonedBrain.TablePos);
    return name;
}