// CTCGift::IsGiftOfType(EGiftType type) -- fetch the owner's CGiftDef sub-definition and compare its Type; false when the thing has no gift def
// Leaf: IsGiftOfType
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FA280, length 83 bytes.
// Callees: CDefClassBase::GetSubDef<CGiftDef> @ 0x006D3EF0 (it builds CCharString("CGiftDef"); manifest label
// "GetSubDef<CAICreatureWillPowerIndicatorDef>", propagated), the def's virtual Destroy (slot 1) on the last release.
// Label correction: the manifest calls this IsFriendly; it takes the gift type as an argument and is called by three
// one-line wrappers that push 0 (0x008FA2F0), 1 (0x008FA2E0) and 2 (0x008FA300) -- those are the IsFriendly-style
// predicates; this is the shared type test.
// Layout: CTCBase ParentThing +0x04; retail CThing PBaseDef (the CThingBaseDef pointer) +0x70; CGiftDef (Ego_r.pdb, 48 bytes)
// Type (EGiftType) +0x28; the def's reference count +0x04 (CDefPointeeBase).
// Shape: the sub-def pointer is a zero-initialised CDefPointer local in the `push ecx` slot filled by the same-TU GetSubDef stand-in (so
// the slot does not escape); its null-tested destructor releases it on both exits (the success path drops the test after the `def->Type`
// dereference). The thing's def is reached through a getter returning a REFERENCE to the CDefPointer member (`GetPDef()->GetSubDef`),
// which loads ParentThing into eax and the def into ecx (`mov eax,[ecx+4] ; mov ecx,[eax+70h]`) and schedules the type compare
// before the count load; a plain `ParentThing->PBaseDef->` chain reuses ecx for both loads.
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

class CGiftDef : public CDefBase { public:
    unsigned char _pad_0x08[0x20];
    EGiftType     Type;                                 // +0x28
};

struct CDefPointer_CGiftDef {
    CGiftDef* m_pDef;
    CDefPointer_CGiftDef() : m_pDef(0) { }
    ~CDefPointer_CGiftDef()
    {
        if (m_pDef) {
            if (--m_pDef->m_count == 0)
                m_pDef->Destroy();
        }
    }
    CGiftDef* operator->() const { return m_pDef; }
};

class CThingBaseDef { public:
    bool GetSubDef(CDefPointer_CGiftDef* subDef);       // 0x006D3EF0 (CDefClassBase::GetSubDef<CGiftDef>)
};
void* FindDef_CThingBaseDef_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CThingBaseDef::GetSubDef(CDefPointer_CGiftDef* out)
{
    void* def = FindDef_CThingBaseDef_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CThingBaseDefPointer {                          // CDefPointer<const CThingBaseDef> at +0x70
    CThingBaseDef* Object;
    CThingBaseDef* operator->() const { return Object; }
};
class CThing { public:
    unsigned char  _pad_0x00[0x70];
    CThingBaseDefPointer PBaseDef;                      // +0x70 (retail)
    const CThingBaseDefPointer& GetPDef() const { return PBaseDef; }
};

class CTCGift { public:
    unsigned char _pad_0x00[0x04];                      // vptr
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    bool IsGiftOfType(EGiftType type);
};

bool CTCGift::IsGiftOfType(EGiftType type)
{
    CDefPointer_CGiftDef def;
    if (ParentThing->GetPDef()->GetSubDef(&def))
        return def->Type == type;
    return false;
}