// CRetailList_00883f10::BuyNode(const CDisplacementMonochromeEffectColourEntry&) -- allocate one 0x28-byte list node and copy-construct the screen-effect stack entry at +8 (the links are left for the caller)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00883F10, length 88 bytes.
// Callees: malloc @ 0x00BFEA0E. The chunk label ("CCreatureAction_TeleportOut::Clone") is a propagated STL label: the two vtables stored are
// NScreenEffect::CScreenEffectPriorityStack::CStackEntry (0x01277FF0) and ...::CDisplacementMonochromeEffectColourEntry (0x01277FF8), rtti-vtables.tsv,
// so this is the list's node factory for that entry type (the landed tree twin is 0x009AA9E0). `this` is unused; `ret 4`: the value reference.
// Shape: placement new (the null test on the node's value address), the polymorphic base's implicit copy (its vtable, two dwords and a byte, one
// register each), the derived vtable, then the derived class's 16-byte POD record copied as a block through the two re-based pointers.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E (landed-trusted)
inline void* operator new(unsigned int, void* p) { return p; }   // placement form: the null test on its result is what retail emits

struct CStackEntry {                        // NScreenEffect::CScreenEffectPriorityStack::CStackEntry: vtable 0x01277FF0, 0x10 bytes
    virtual ~CStackEntry() { }
    long m_priority;                        // +0x04
    long m_owner;                           // +0x08
    unsigned char m_isActive;               // +0x0c
};
struct SColourRecord { long r, g, b, a; };  // plain 16-byte record: copied as a block (four moves through re-based pointers at +0x10)
struct CDisplacementMonochromeEffectColourEntry : CStackEntry {   // vtable 0x01277FF8, 0x20 bytes
    SColourRecord m_colour;                 // +0x10
};
struct SNode_00883f10 {                     // 0x28 bytes: next / prev, then the value
    SNode_00883f10* m_pNext;
    SNode_00883f10* m_pPrev;
    CDisplacementMonochromeEffectColourEntry m_value;   // +0x08
};

struct CRetailList_00883f10 {
    SNode_00883f10* BuyNode(const CDisplacementMonochromeEffectColourEntry& value);
};

SNode_00883f10* CRetailList_00883f10::BuyNode(const CDisplacementMonochromeEffectColourEntry& value)
{
    SNode_00883f10* node = (SNode_00883f10*)malloc(sizeof(SNode_00883f10));
    new (&node->m_value) CDisplacementMonochromeEffectColourEntry(value);
    return node;
}