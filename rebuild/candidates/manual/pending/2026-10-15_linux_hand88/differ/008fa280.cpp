// CTCGift::IsGiftOfType(EGiftType type) -- fetch the owner's CGiftDef sub-definition and compare its Type; false when the thing has no gift def
// Grade: DIFFER(104v83) -- the lane loads the sub-def pointer into eax (retail ecx) after GetSubDef, so each inlined
//        release becomes `mov ecx,eax ; add eax,4 ; dec ; test ; mov [eax]` (retail `dec [ecx+4]` / load-dec-store through ecx) and ebx is
//        pushed at entry (retail pushes it only on the success path). Tried: count in the class, local-copy release, the landed
//        two-base CDefBase model, reversed polarity.
// Leaf: IsGiftOfType
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FA280, length 83 bytes.
// Callees: CDefClassBase::GetSubDef<CGiftDef> @ 0x006D3EF0 (it builds CCharString("CGiftDef"); manifest label
// "GetSubDef<CAICreatureWillPowerIndicatorDef>", propagated), the def's virtual Destroy (slot 1) on the last release.
// Label correction: the manifest calls this IsFriendly; it takes the gift type as an argument and is called by three
// one-line wrappers that push 0 (0x008FA2F0), 1 (0x008FA2E0) and 2 (0x008FA300) -- those are the IsFriendly-style
// predicates; this is the shared type test.
// Layout: CTCBase ParentThing +0x04; retail CThing PBaseDef (the CThingBaseDef pointer) +0x70; CGiftDef (Ego_r.pdb, 48 bytes)
// Type (EGiftType) +0x28; the def's reference count +0x04 (CDefPointeeBase).
// Shape: the sub-def pointer is a zero-initialised local in the `push ecx` slot filled by GetSubDef; its destructor
// releases it on both exits (the success path drops the null test after the `def->Type` dereference). The count must sit
// in a second, non-polymorphic base of the def (the landed GetSubDef family's CDefBase : CDefVirtual, CRefCounted) for
// `if (--m_count == 0)` to fold into `dec [ecx+4]`; a count declared in the polymorphic class itself gives the
// `add eax,4 ; dec ; test ; mov [eax]` accessor shape.
enum EGiftType { GIFT_TYPE_0, GIFT_TYPE_1, GIFT_TYPE_2 };

struct CRefCounted {                                   // second base: the count sits at +4 behind the vptr
    long m_count;
};

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                             // slot 1
};

struct CDefBase : CDefVirtual, CRefCounted {            // CDefPointeeBase: vptr + reference count
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};

struct CGiftDef : CDefBase {
    unsigned char _pad_0x08[0x20];
    EGiftType     Type;                                 // +0x28
};

struct CDefPointer_CGiftDef {
    CGiftDef* m_pDef;
    CDefPointer_CGiftDef() : m_pDef(0) { }
    ~CDefPointer_CGiftDef()
    {
        if (m_pDef)
            m_pDef->Release();
    }
    CGiftDef* operator->() const { return m_pDef; }
};

struct CThingBaseDef {
    bool GetSubDef(CDefPointer_CGiftDef* subDef);       // 0x006D3EF0 (CDefClassBase::GetSubDef<CGiftDef>)
};

struct CThing {
    unsigned char  _pad_0x00[0x70];
    CThingBaseDef* PBaseDef;                            // +0x70 (retail)
};

struct CTCGift {
    unsigned char _pad_0x00[0x04];                      // vptr
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    bool IsGiftOfType(EGiftType type);
};

bool CTCGift::IsGiftOfType(EGiftType type)
{
    CDefPointer_CGiftDef def;
    if (ParentThing->PBaseDef->GetSubDef(&def))
        return def->Type == type;
    return false;
}
