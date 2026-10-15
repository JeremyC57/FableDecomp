// CDefClassBase::CDefClassBase() -- CResource base, the three status bits cleared, own vtable, no definition manager yet
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042D9E0, length 29 bytes.
// Leaf: ??0CDefClassBase
// Callees: CResource::CResource @ 0x009FBEC0.
// Identity: the stored vtable 0x01230BCC is CDefClassBase's (rtti-vtables.tsv, slots 10.. = the landed
// CDefClassBase::GetInstantiationName 0x0042DA10 etc.); the manifest label "CPSCBase::CPSCBase" is
// propagated. Layout from Ego_r.pdb (engine/CDefClassBase.h, packed): PDefManager +0x1c, GlobalIndex
// +0x20 (left uninitialised), the Setup/Template/DefaultValsApplied bits in the byte at +0x24.
// The header models the vptr as a data member, so the polymorphic chain is modelled locally:
// CResource (out-of-line constructor) <- CDefPointeeBase (inline empty constructor, its vtable store is
// dead) <- CDefClassBase. Shape: the bit clears fold into one `and byte ptr [esi+24h],0F8h` placed
// before the vtable store; the pointer initialiser is an immediate zero store after it.
struct CDefinitionManager;

struct CResource {
    virtual void v00();
    unsigned char _resource[0x18];          // +0x04 resource list links, size, last used frame
    CResource();                            // 0x009FBEC0
};

struct CDefPointeeBase : CResource {        // vtable 0x01230BA0
    virtual void v00();
    CDefPointeeBase() { }
};

#pragma pack(push, 1)                       // def classes are byte packed (GlobalIndex bits at +0x24, a dword at +0x25 next)
struct CDefClassBase : CDefPointeeBase {    // vtable 0x01230BCC
    CDefinitionManager* PDefManager;        // +0x1c
    unsigned long       GlobalIndex;        // +0x20
    unsigned char       Setup : 1;          // +0x24 bit 0
    unsigned char       Template : 1;       // +0x24 bit 1
    unsigned char       DefaultValsApplied : 1;     // +0x24 bit 2
    virtual void v00();
    CDefClassBase();
};
#pragma pack(pop)

CDefClassBase::CDefClassBase()
    : PDefManager(0)
{
    Setup = 0;
    Template = 0;
    DefaultValsApplied = 0;
}