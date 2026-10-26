// CChestDef::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CChestDef inlined (the member's destructor, then the base chain down to the first user-written destructor), and operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCChestDef@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D8072, 44 bytes, thiscall, `ret 4`; reached only through vtable slot 0.
// Identity (proven): CChestDef::`scalar deleting destructor'; evidence: vtable 0x0123ACDC slot 0 (RTTI .?AVCChestDef@@ : CSubDefClassBase : CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase)
// Label: the manifest's "CChestDef::__vecDelDtor" names the right class; it is the scalar (not vector) deleting destructor.
// Callees: CCharString::~CCharString 0x0099EAE0 (OpenAnimationForCreature, +0x30), CResource::~CResource 0x009FC550 (after the inline user-written ~CDefPointeeBase stores 0x01230BA0), operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CChestDef members at their PDB offsets; retail size 0x3C from the def's size slot.
// Shape: no vtable store for CChestDef itself -- VC7.1 resets the vptr only in user-written destructors (agent952's finding, 0x006DF8C0); the stored
//   vtable 0x01230BA0 belongs to the first user-written (inline) base destructor. The deleting destructor is emitted with the vtable, i.e. in a TU
//   that constructs the class: MakeOne is that construction (a modelling device); the qualified explicit destructor calls make VC7.1 define the
//   implicit destructors before it generates the deleting destructor, so the chain inlines.
#pragma optimize("s", on)
class CResourceList;
class CResource {                                        // Ego_r.pdb 0x1C (CIVCountedPointeeBase: vptr, IVRefCount)
public:
    CResource();                                         // 0x009FBEC0
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
    virtual ~CDefPointeeBase() { }                       // user-written, inline: stores 0x01230BA0, then ~CResource
};
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {           // vtable 0x01230BCC (implicit destructor)
public:
    CDefinitionManager* PDefManager;                     // +0x1C
    unsigned long       GlobalIndex;                     // +0x20
    unsigned char       Setup : 1;                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
class CSubDefClassBase : public CDefClassBase {         // vtable 0x01231D54, Ego_r.pdb 0x25 (packed, no members, implicit destructor)
public:
    CSubDefClassBase();                                  // 0x0044C0C0
};
class CEngineGraphic {                                   // Ego_r.pdb 14 bytes, packed
public:
    CEngineGraphic() : BankIndex(0), AnimStep(0.0f), RenderSizeX(0.0f), AdditiveAlpha(false), Type(0) { }
    unsigned long BankIndex;                             // +0x00
    float         AnimStep;                              // +0x04
    float         RenderSizeX;                           // +0x08
    bool          AdditiveAlpha;                         // +0x0C
    unsigned char Type;                                  // +0x0D (EEngineGraphicType)
};
#pragma pack(pop)
class CCharString {                                      // 4 bytes: the shared string data pointer
public:
    CCharString();
    ~CCharString();                                      // 0x0099EAE0
    void* PStringData;
};
class CChestDef : public CSubDefClassBase {              // vtable 0x0123ACDC, retail 0x3C (size slot 20)
public:
    bool        PersistOnOpening;                        // +0x25
    long        OpenParticleEffect;                      // +0x28
    bool        DisplayMessageOnEmpty;                   // +0x2C
    CCharString OpenAnimationForCreature;                // +0x30
    long        OpenerObject;                            // +0x34
    long        OpenersRequired;                         // +0x38
};
void* MakeOne()
{
    return new CChestDef;
}
void KillDefBase(CDefClassBase* p) { p->CDefClassBase::~CDefClassBase(); }
void KillSubDef(CSubDefClassBase* p) { p->CSubDefClassBase::~CSubDefClassBase(); }
void KillOne(CChestDef* p) { p->~CChestDef(); }