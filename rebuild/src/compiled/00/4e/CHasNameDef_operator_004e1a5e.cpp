// CHasNameDef::operator=(const CHasNameDef& other) -- the compiler-generated copy assignment of a CDefClassBase-derived def: the base part through CDefClassBase::operator=, then DefaultNameTag, Home, Occupation, returning this
// Leaf: ??4CHasNameDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E1A5E, 39 bytes, thiscall, `ret 4`, returns this.
// Identity: CHasNameDef -- proven; evidence: its only caller is the 5-byte `jmp` 0x004E1A59 in vtable slot 19 (Copy) of vtable
//   0x0123E67C (RTTI .?AVCHasNameDef@@), i.e. `CHasNameDef::Copy(const CDefClassBase* other) { *this = *(const CHasNameDef*)other; }`
//   (PDB name set: ?Copy@CHasNameDef@@UAEXPBVCDefClassBase@@@Z, ??4CHasNameDef@@QAEAAV0@ABV0@@Z); members per Ego_r.pdb CHasNameDef (PDB +40/+44/+48).
// Label: the manifest's "CThingBuildingDef::operator=_3fields" is propagated across byte-identical bodies.
// Callees: CDefClassBase::operator= @ 0x00431F10. Its body looks compiler-generated (landed 0x00431ED0 / 0x00431F10), but every
//   def assignment in retail (30+) calls it out of line, while VC7.1 inlines an implicit CParentDefClassBase::operator=
//   into the derived one (69 bytes, graded), so the bases' assignments are declared here and defined in their own rows.
// Layout: the landed CParentDefClassBase::operator= 0x00431ED0 model -- CDefClassBase and CParentDefClassBase under pack(1)
//   (37 and 0x39 bytes), the derived def at natural alignment, so its first member lands at 0x28.
// Model: local -- engine/CHasNameDef.h exists but is data-only (pack(1), vptr and base flattened into bytes), which an implicit operator= would copy word for word instead of calling the base's operator=.
// Shape: a size-optimised def TU (retail packs these functions back to back at odd addresses, no 16-byte alignment):
//   `#pragma optimize("s", on)` gives retail's one-register member copies (/O2 rotates eax/ecx/edx, graded).
#pragma optimize("s", on)
class CResourceList;
class CResource {
public:
    unsigned long  IVRefCount;                           // +0x04
    CResourceList* ResourceList;                         // +0x08
    CResource*     PrevResource;                         // +0x0c
    CResource*     NextResource;                         // +0x10
    unsigned long  ResourceSize;                         // +0x14
    unsigned long  LastUsedFrame;                        // +0x18
    virtual ~CResource();                                // 0x009FC550
};
class CDefPointeeBase : public CResource {
public:
    virtual ~CDefPointeeBase() {}
};
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {
public:
    class CSubDefInfo { public:
        long DefIndex;
        long OriginalParentDefIndex;
    };
    virtual void Copy(const CDefClassBase* other);       // vtable slot 19
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10 (see Callees)
    CDefinitionManager* PDefManager;                     // +0x1c
    unsigned long       GlobalIndex;                     // +0x20
    unsigned char       Setup : 1;                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
#pragma pack(pop)

class CHasNameDef : public CDefClassBase {
public:
    unsigned long      DefaultNameTag;                     // +0x28
    unsigned long      Home;
    unsigned long      Occupation;
    virtual void Copy(const CDefClassBase* other);       // 0x004E1A59
};
void CHasNameDef::Copy(const CDefClassBase* other)
{
    *this = *(const CHasNameDef*)other;
}