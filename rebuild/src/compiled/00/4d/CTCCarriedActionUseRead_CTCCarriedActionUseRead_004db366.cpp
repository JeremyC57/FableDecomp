// CTCCarriedActionUseRead::CTCCarriedActionUseRead(CThing*) -- CTCCarriedActionUseBase(owner), own vtable, null readable def, not yet read
// Leaf: ??0CTCCarriedActionUseRead
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DB366, length 32 bytes (a /Os module: unaligned entry,
// `push dword ptr [esp+8]` pass-through, `and dword ptr [..],0` zero store).
// Callee: CTCCarriedActionUseBase::CTCCarriedActionUseBase(CThing*) @ 0x004DAC61 (it calls
// CTCActionUseBase 0x004DA939 and stores vtable 0x0123F894 = CTCCarriedActionUseBase; the manifest's
// "CEnginePrimitiveGroupManager" label is propagated).
// Identity: the manifest label "CTCActionUseBase::CTCActionUseBase" is propagated; the vtable stored is
// 0x01240834 = CTCCarriedActionUseRead (rtti-vtables.tsv).
//
// Layout = Ego_r.pdb CTCCarriedActionUseRead (0x1c bytes): the 0x14-byte base, CDefPointer<
// CCarriedReadableDef> PDef +0x14, AlreadyRead +0x18. Shape: base constructor, vtable, the def pointer's
// inline constructor (`and` under /Os), then AlreadyRead = false as a byte store.
#pragma optimize("s", on)

class CThing;
class CCarriedReadableDef;

class CTCCarriedActionUseBase { public:                            // 0x14 bytes, vtable 0x0123F894
    virtual void v00();
    unsigned char _body[0x10];                              // CTCBase + PActionUseDef + Usable
    CTCCarriedActionUseBase(CThing* owner);                 // 0x004DAC61
};

template<class T>
class CDefPointer { public:
    const T* m_pDef;
    CDefPointer() : m_pDef(0) { }
};

struct CTCCarriedActionUseRead : CTCCarriedActionUseBase {  // vtable 0x01240834
    CDefPointer<CCarriedReadableDef> PDef;                  // +0x14
    bool                             AlreadyRead;           // +0x18
    virtual void v00();
    CTCCarriedActionUseRead(CThing* owner);
};

CTCCarriedActionUseRead::CTCCarriedActionUseRead(CThing* owner)
    : CTCCarriedActionUseBase(owner)
    , AlreadyRead(false)
{
}