// CTeleporterDef::operator=(const CTeleporterDef& other) -- the compiler-generated copy assignment of a CDefClassBase-derived def: the base part through CDefClassBase::operator=, then Radius, ActiveEffect, ActiveByProximity, returning this
// Leaf: ??4CTeleporterDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E131A, 39 bytes, thiscall, `ret 4`, returns this.
// Identity: CTeleporterDef -- proven; evidence: its only caller is the 5-byte `jmp` 0x004E1315 in vtable slot 19 (Copy) of vtable
//   0x0123D834 (RTTI .?AVCTeleporterDef@@), i.e. `CTeleporterDef::Copy(const CDefClassBase* other) { *this = *(const CTeleporterDef*)other; }`
//   (PDB name set: ?Copy@CTeleporterDef@@UAEXPBVCDefClassBase@@@Z, ??4CTeleporterDef@@QAEAAV0@ABV0@@Z); members per Ego_r.pdb CTeleporterDef (PDB +40/+44/+48).
// Label: the manifest's "CThingBuildingDef::operator=" is propagated across byte-identical bodies.
// Callees: CDefClassBase::operator= @ 0x00431F10. Its body looks compiler-generated (landed 0x00431ED0 / 0x00431F10), but every
//   def assignment in retail (30+) calls it out of line, while VC7.1 inlines an implicit CParentDefClassBase::operator=
//   into the derived one (69 bytes, graded), so the bases' assignments are declared here and defined in their own rows.
// Layout: the landed CParentDefClassBase::operator= 0x00431ED0 model -- CDefClassBase and CParentDefClassBase under pack(1)
//   (37 and 0x39 bytes), the derived def at natural alignment, so its first member lands at 0x28.
// Model: local -- no engine header for this def.
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

class CTeleporterDef : public CDefClassBase {
public:
    float              Radius;                     // +0x28
    long               ActiveEffect;
    bool               ActiveByProximity;
    virtual void Copy(const CDefClassBase* other);       // 0x004E1315
};
void CTeleporterDef::Copy(const CDefClassBase* other)
{
    *this = *(const CTeleporterDef*)other;
}