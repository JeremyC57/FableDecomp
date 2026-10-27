// CCreatureAbilityDef::Copy(const CDefClassBase* other) -- the def's virtual copy: assign the other def (of the same class) to this one (the CParentDefClassBase part, then Type)
// Leaf: ?Copy@CCreatureAbilityDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046280E, 25 bytes, thiscall, `ret 4`.
// Identity (proven): slot 19 (Copy) of the CCreatureAbilityDef vtable 0x01234F24 (rtti-vtables); FableWin.pdb ?Copy@CCreatureAbilityDef@@UAEXPBVCDefClassBase@@@Z;
//   the copied member is Ego_r.pdb's CCreatureAbilityDef::Type (ECreatureAbility) at retail +0x3C (PDB - 4).
// Callees: CParentDefClassBase::operator=(const CParentDefClassBase&) 0x00431ED0.
// Shape (family K097, representative 0x0046280E): the landed Copy thunks' `*this = *static_cast<const X*>(other)`, here with the
//   class's implicit operator= inlined (one scalar member): the base assignment out of line, then the member through eax/al.
//   The base is modelled locally as the landed 0x00460256 does: engine/CParentDefClassBase.h has no virtual base chain, and the
//   retail base is a packed 0x39-byte polymorphic class (CDefClassBase 0x25, InstantiationName +0x25, SubDefInfoMap +0x29).
#pragma pack(push, 1)
class CDefClassBase {
public:
    virtual void Copy(const CDefClassBase* other);
    unsigned char _defClassBase[0x21];                   // +0x04 (not touched here)
};
class CParentDefClassBase : public CDefClassBase {
public:
    unsigned char InstantiationName[4];                  // +0x25 CDefString
    unsigned char SubDefInfoMap[0x10];                   // +0x29 CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, ...>
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)
enum ECreatureAbility { };                           // enumerators not needed here
class CCreatureAbilityDef : public CParentDefClassBase {
public:
    virtual void Copy(const CDefClassBase* other);
    ECreatureAbility Type;   // +0x3C
};
void CCreatureAbilityDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CCreatureAbilityDef*>(other);
}