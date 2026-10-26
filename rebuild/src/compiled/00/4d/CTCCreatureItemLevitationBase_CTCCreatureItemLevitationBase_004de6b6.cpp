// CTCCreatureItemLevitationBase::CTCCreatureItemLevitationBase(CThing*) -- CTCBase(parent), own vtable, an empty list of levitated items (+0xc) and two cleared words (+0x10, +0x14)
// Leaf: ??0CTCCreatureItemLevitationBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DE6B6, length 47 bytes.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, the STLport list constructor list(const allocator_type&) @ 0x004AB470
// (head word cleared, a 12-byte self-linked sentinel from operator new; manifest "ListNode_AllocateSelfLinked").
// Vtable 0x0124156C = CTCCreatureItemLevitationBase (rtti-vtables.tsv).
// Label correction: the manifest "CTCShotDefinitionProjectile::CTCShotDefinitionProjectile" is propagated.
// Shape: a frame-pointer size TU with pass-through `push dword ptr [ebp+8]` = `#pragma optimize("y", off)` +
// `("s", on)`; the default allocator temporary bound to the list constructor's `const&` lands in the high byte of the
// dead parent-argument slot (`lea eax,[ebp+0Bh]`); the two zero words are initialisers (`and dword ptr [..],0`).
#pragma optimize("y", off)
#pragma optimize("s", on)
class CThing;

class CTCBase { public:
    virtual void v00();
    unsigned char _body[0x8];                // ParentThing +4, flags +8/+9
    CTCBase(CThing* parent);                 // 0x00686800
};

struct SAllocator {
    SAllocator() { }
};

struct CLevitatedItemList {                  // STLport list: one word, the sentinel node
    void* m_pNode;
    CLevitatedItemList(const SAllocator& allocator);   // 0x004AB470
};

class CTCCreatureItemLevitationBase : public CTCBase { public:
    CLevitatedItemList m_items;              // +0x0c
    long               m_count;              // +0x10
    long               m_current;            // +0x14
    virtual void v00();
    CTCCreatureItemLevitationBase(CThing* parent);
};

CTCCreatureItemLevitationBase::CTCCreatureItemLevitationBase(CThing* parent)
    : CTCBase(parent)
    , m_items(SAllocator())
    , m_count(0)
    , m_current(0)
{
}