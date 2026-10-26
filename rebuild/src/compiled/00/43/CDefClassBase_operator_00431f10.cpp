// CDefClassBase::operator=(const CDefClassBase&) -- the implicit copy assignment: the resource base, the def manager, the global index and the three flag bits
// Leaf: ??4CDefClassBase@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe @ 0x00431F10, 77 bytes, `ret 4`, returns this; called first by CParentDefClassBase::operator=
// (0x00431EDA) and 30+ def assignment operators (0x004536B7 ..). Catalog label CThingBuildingDef::operator= is wrong
// (the 0x004536A9 thunk is CThingBuildingDef's; this is its base's, as the callers' own labels say).
// Body: CResource::operator= (0x00431F60, out of line), PDefManager (+0x1c), GlobalIndex (+0x20), then the bitfield byte
// +0x24 bit by bit (Setup, Template, DefaultValsApplied: `xor / and / xor` merges).
// Layout: Ego_r.pdb CDefClassBase (37 bytes, pack 1) over CDefPointeeBase : CResource (0x1c, real virtual destructor);
// engine/ headers flatten the vptr into bytes, which an implicit operator= would copy, so the classes are local (as 0x00431ED0).
// Shape: /O2; the compiler-generated operator=, emitted out of line by an assignment in a helper. The base copy is
// declared out of line (it is the separate function 0x00431F60, landed as ??4CResource@@); defined inline VC7.1 would
// inline its six dword copies here.
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
    CDefPointeeBase& operator=(const CDefPointeeBase& other);   // 0x00431F60 (the CResource member copy, out of line)
};
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {
public:
    class CSubDefInfo { public:
        long DefIndex;
        long OriginalParentDefIndex;
    };
    CDefinitionManager* PDefManager;                     // +0x1c
    unsigned long       GlobalIndex;                     // +0x20
    unsigned char       Setup : 1;                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
#pragma pack(pop)
void AssignCDefClassBase(CDefClassBase& to, const CDefClassBase& from)               // emission helper, not a retail function
{
    to = from;
}