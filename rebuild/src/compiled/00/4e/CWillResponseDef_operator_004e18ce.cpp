// CWillResponseDef::operator=(const CWillResponseDef& other) -- compiler-generated copy assignment: CSubDefClassBase::operator=, then IsForcePushable, IsLifeDrainable, IsLightningable, IsNotBloodyWillable, IsAffectedBySlowTime, IsAbleToBeLifted, IsEpicPowerExplodable, IsEpicSpellable memberwise
// Leaf: ??4CWillResponseDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E18CE, 69 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`, returns this.
// Identity (proven): CWillResponseDef::operator=(const CWillResponseDef&); evidence: its only def caller is CWillResponseDef::Copy 0x004E18C9 (a `jmp` to it), slot 19 of vtable 0x0123E324 (RTTI .?AVCWillResponseDef@@), so `*this = *static_cast<const CWillResponseDef*>(other)`;
//   RTTI bases CSubDefClassBase <- CDefClassBase; the copied offsets and widths are Ego_r.pdb CWillResponseDef's members (retail offset = Ego_r offset); FableWin.pdb name set holds ??4CWillResponseDef@@QAEAAV0@ABV0@@Z.
// Label: the manifest's "CThingBuildingDef::operator=_8bytes" is propagated (family J059: five byte-similar def copy assignments); the Copy thunk's vtable names CWillResponseDef.
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
class CWillResponseDef : public CSubDefClassBase {                     // vtable 0x0123E324
public:
    virtual void Copy(const CDefClassBase* other);       // vtable 0x0123E324 slot 19, 0x004E18C9
    bool IsForcePushable;                              // +0x25
    bool IsLifeDrainable;                              // +0x26
    bool IsLightningable;                              // +0x27
    bool IsNotBloodyWillable;                          // +0x28
    bool IsAffectedBySlowTime;                         // +0x29
    bool IsAbleToBeLifted;                             // +0x2A
    bool IsEpicPowerExplodable;                        // +0x2B
    bool IsEpicSpellable;                              // +0x2C
};
void CWillResponseDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CWillResponseDef*>(other);
}