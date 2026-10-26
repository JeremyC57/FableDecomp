// CEnginePrimitiveTypeInfoDerived_00ca4f40::CEnginePrimitiveTypeInfoDerived_00ca4f40(COwner*, long, long) -- forward to the type-info base, keep the owner, install four vtables
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA4F40, length 75 bytes (+ int3 padding).
// Callees: CEnginePrimitiveTypeInfo::CEnginePrimitiveTypeInfo(long, COwner*, void*, long, long)
// @ 0x00CAC260 with (0, owner, &owner->+0x7c, b, c).
// Vtables 0x012BF408 (+0), 0x012BF3C0 (+0xc), 0x012BF3AC (+0x10), 0x012BF388 (+0x98): a no-RTTI
// module (no entries in rtti-vtables.tsv), so the classes carry descriptive names; the manifest's
// "CAIStateGroup_WhisperPotionCombat" label is propagated. The base is a four-way multiple
// inheritance object (primary 0xc bytes, a 4-byte interface, a 0x88-byte one, a 0x40-byte one).
// Shape: the owner store at +0xd8 happens BEFORE the vtable stores, so it belongs to an inlined
// intermediate constructor whose own vtable stores are dead (overwritten by the most-derived
// class, which overrides one virtual in each base). Arguments are pushed right to left.
struct COwnerBlock { unsigned char _pad[0x7c]; unsigned char m_block[4]; };

struct IPrimitiveType0 { virtual void f0(); long m_a; long m_b; };            // +0x00, 0xc bytes
struct IPrimitiveType1 { virtual void f1(); };                                  // +0x0c
struct IPrimitiveType2 { virtual void f2(); unsigned char _pad[0x84]; };       // +0x10
struct IPrimitiveType3 { virtual void f3(); unsigned char _pad[0x3c]; };       // +0x98

class CEnginePrimitiveTypeInfo : public IPrimitiveType0, public IPrimitiveType1, public IPrimitiveType2, public IPrimitiveType3 { public:
    CEnginePrimitiveTypeInfo(long zero, COwnerBlock* owner, void* block, long b, long c);   // 0x00CAC260
};

struct CEnginePrimitiveTypeInfoOwned_00ca4f40 : CEnginePrimitiveTypeInfo {
    COwnerBlock* m_pOwner;                      // +0xd8
    CEnginePrimitiveTypeInfoOwned_00ca4f40(COwnerBlock* owner, long b, long c)
        : CEnginePrimitiveTypeInfo(0, owner, owner->m_block, b, c), m_pOwner(owner) {}
};

struct CEnginePrimitiveTypeInfoDerived_00ca4f40 : CEnginePrimitiveTypeInfoOwned_00ca4f40 {
    virtual void f0();
    virtual void f1();
    virtual void f2();
    virtual void f3();
    CEnginePrimitiveTypeInfoDerived_00ca4f40(COwnerBlock* owner, long b, long c);
};

CEnginePrimitiveTypeInfoDerived_00ca4f40::CEnginePrimitiveTypeInfoDerived_00ca4f40(COwnerBlock* owner, long b, long c)
    : CEnginePrimitiveTypeInfoOwned_00ca4f40(owner, b, c)
{
}