// C3DMeshMaterial::`scalar deleting destructor'(unsigned int) -- own vtable stored, the shared block at +0x2c released and nulled, the name string at +8 destroyed, CBaseClass::~CBaseClass, flag-tested delete
// Leaf: ??_GC3DMeshMaterial
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABFAA0, length 68 bytes (vtable 0x0129EDC0 slot 0).
// Callees: CCharString::~CCharString @ 0x0099EAE0, CBaseClass::~CBaseClass @ 0x0099A2E0, operator delete @ 0x00BFE9BC.
// The catalog label "CThingHolySite::vector_deleting_destructor" is propagated: the only reference is
// slot 0 of the C3DMeshMaterial vtable 0x0129EDC0 (also the vtable it stores).
// Shape: a USER-declared virtual destructor stores the class's own vtable first (a generated one
// would drop it); members are destroyed in reverse declaration order (+0x2c shared pointer, then
// the +8 CCharString), then the out-of-line base destructor. The destructor is `__forceinline`
// (and the shared pointer's destructor) so the `??_G` inlines them, and the class has an inline constructor plus a `new` factory so the
// vtable (and the `??_G`) are emitted in the TU. The shared block's release lives on the block.
struct SSharedBlock {
    virtual void v00();
    virtual void Destroy();                // slot 1: called when the count reaches zero
    long m_references;                     // +4
    void Release() { if (--m_references == 0) Destroy(); }
};
struct CSharedPointer {
    SSharedBlock* m_pBlock;
    CSharedPointer() : m_pBlock(0) { }
    __forceinline ~CSharedPointer()
    {
        if (m_pBlock) {
            m_pBlock->Release();
            m_pBlock = 0;
        }
    }
};
class CCharString { public:
    void* m_pData;
    CCharString();                         // 0x0099E4B0
    ~CCharString();                        // 0x0099EAE0
};
class CBaseClass { public:
    CBaseClass();
    virtual ~CBaseClass();                 // 0x0099A2E0
};
class C3DMeshMaterial : public CBaseClass { public:
    long           m_flags;                // +0x04
    CCharString    m_name;                 // +0x08
    unsigned char  _pad0c[0x20];           // +0x0c (plain material parameters)
    CSharedPointer m_pTexture;             // +0x2c
    C3DMeshMaterial() { }
    __forceinline virtual ~C3DMeshMaterial() { }
};

C3DMeshMaterial* NewC3DMeshMaterial()
{
    return new C3DMeshMaterial;
}