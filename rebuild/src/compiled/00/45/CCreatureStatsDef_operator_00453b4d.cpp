// CCreatureStatsDef::operator=(const CCreatureStatsDef& other) -- compiler-generated copy assignment: CSubDefClassBase::operator=, then MoralityForKill, MoralityForPunch, MoralityForWeaponHit, ExperienceWorth, RenownWorth, Speed, Dexterity, StrengthDamageMultiplier memberwise
// Leaf: ??4CCreatureStatsDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00453B4D, 69 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`, returns this.
// Identity (proven): CCreatureStatsDef::operator=(const CCreatureStatsDef&); evidence: its only def caller is CCreatureStatsDef::Copy 0x00453B48 (a `jmp` to it), slot 19 of vtable 0x012321AC (RTTI .?AVCCreatureStatsDef@@), so `*this = *static_cast<const CCreatureStatsDef*>(other)`;
//   RTTI bases CSubDefClassBase <- CDefClassBase; the copied offsets and widths are Ego_r.pdb CCreatureStatsDef's members (retail offset = Ego_r offset); FableWin.pdb name set holds ??4CCreatureStatsDef@@QAEAAV0@ABV0@@Z.
// Label: the manifest's "CThingBuildingDef::operator=" is propagated (family J059: five byte-similar def copy assignments); the Copy thunk's vtable names CCreatureStatsDef.
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10 (landed under that name), reached through the inline implicit CSubDefClassBase::operator=.
// Shape: the implicit operator= copies each scalar member with integer moves (no x87 for the floats); `return *this` in eax. The Copy thunk is defined
//   here only to make the compiler emit the implicit operator= (retail emits it beside Copy, a `jmp` to it).
#pragma optimize("s", on)
class CResourceList;
class CResource {                                        // Ego_r.pdb 0x1C (CIVCountedPointeeBase: vptr, IVRefCount)
public:
    virtual ~CResource();                                // 0x009FC550
    unsigned long  IVRefCount;                           // +0x04
    CResourceList* ResourceList;                         // +0x08
    CResource*     PrevResource;                         // +0x0C
    CResource*     NextResource;                         // +0x10
    unsigned long  ResourceSize;                         // +0x14
    unsigned long  LastUsedFrame;                        // +0x18
};
class CDefPointeeBase : public CResource {               // vtable 0x01230BA0
public:
    virtual ~CDefPointeeBase() { }
};
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {           // vtable 0x01230BCC
public:
    class CSubDefInfo { public:
        long DefIndex;
        long OriginalParentDefIndex;
    };
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10 (compiler-generated in retail, emitted out of line)
    CDefinitionManager* PDefManager;                     // +0x1C
    unsigned long       GlobalIndex;                     // +0x20
    unsigned char       Setup : 1;                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
class CSubDefClassBase : public CDefClassBase {         // vtable 0x01231D54, Ego_r.pdb 0x25 (packed, no members of its own)
};
#pragma pack(pop)
class CCreatureStatsDef : public CSubDefClassBase {                     // vtable 0x012321AC
public:
    virtual void Copy(const CDefClassBase* other);       // vtable 0x012321AC slot 19, 0x00453B48
    long MoralityForKill;                              // +0x28
    long MoralityForPunch;                             // +0x2C
    long MoralityForWeaponHit;                         // +0x30
    long ExperienceWorth;                              // +0x34
    long RenownWorth;                                  // +0x38
    float Speed;                                       // +0x3C
    float Dexterity;                                   // +0x40
    float StrengthDamageMultiplier;                    // +0x44
};
void CCreatureStatsDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CCreatureStatsDef*>(other);
}