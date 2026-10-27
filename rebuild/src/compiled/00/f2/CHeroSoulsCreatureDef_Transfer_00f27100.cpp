// CHeroSoulsCreatureDef::Transfer(CPersistContext& context) -- persist the Hero Souls creature def: CreatureType and Location (default-valued strings), NumCreatures (default 0)
// Leaf: ?Transfer@CHeroSoulsCreatureDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F27100, 114 bytes, thiscall, `ret 4`.
// Identity (proven): slot 18 (Transfer) of the CHeroSoulsCreatureDef vtable 0x012FB500, stored by the landed ctors
//   CHeroSoulsCreatureDef::CHeroSoulsCreatureDef 0x00F27060 / 0x00F27650; the persisted names "CreatureType" / "Location" /
//   "NumCreatures" are exactly its PDB members (Ego_r.pdb CreatureType +0x28, Location +0x2c, NumCreatures +0x30).
//   ?Transfer@CHeroSoulsCreatureDef@@UAEXAAVCPersistContext@@@Z is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00f27100" is a gap-scan placeholder.
// Callees: CPersistContext::GetDefaultVal<CCharString> 0x0040E1E0 (protected static __fastcall, hidden result in ecx),
//   CPersistContext::Transfer<CCharString> 0x004109A0, ~CCharString 0x0099EAE0, CPersistContext::Transfer<long> 0x00410BE0.
// Layout: CDefClassBase is 0x28 bytes; CreatureType +0x28, Location +0x2c, NumCreatures +0x30 (no container: Ego_r.pdb offsets).
// Shape: the two strings go through the two-argument Transfer overload (`??$Transfer@VCCharString@@@CPersistContext@@QAEXPBDAAVCCharString@@@Z`,
//   inline: Transfer(name, value, GetDefaultVal<T>())), so each default is a GetDefaultVal temporary destroyed after its call;
//   the second temporary and the long default reuse the dead context-argument slot.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
};

class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // CCharString 0x004109A0, long 0x00410BE0
    template <class T> void Transfer(const char* name, T& value) { Transfer(name, value, GetDefaultVal<T>()); }
protected:
    template <class T> static T __fastcall GetDefaultVal();                               // CCharString 0x0040E1E0
};

class CDefClassBase {
public:
    virtual ~CDefClassBase();
    virtual void Transfer(CPersistContext& context);
    unsigned char _base_0x04[0x24];
};

class CHeroSoulsCreatureDef : public CDefClassBase {     // vtable 0x012FB500
public:
    CCharString CreatureType;                             // +0x28
    CCharString Location;                                 // +0x2c
    long        NumCreatures;                             // +0x30
    virtual void Transfer(CPersistContext& context);
};

void CHeroSoulsCreatureDef::Transfer(CPersistContext& context)
{
    context.Transfer("CreatureType", CreatureType);
    context.Transfer("Location", Location);
    context.Transfer("NumCreatures", NumCreatures, 0L);
}