// CRetailMap_00851cc0::Add(long key, long value) -- build the (key, value) pair on the stack and insert it into the tree (unique-key insert @ 0x00463E2E, result discarded)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00851CC0, length 38 bytes.
// Leaf: Add
// Callees: the VC6-era Dinkumware `_Tree::insert(const value_type&)` @ 0x00463E2E (landed as
// CRetailTree_longkey_ins00463eb4::insert: signed long key at node +0x10, pair<iterator,bool> returned through the hidden pointer).
// The chunk label "CTCLook::IsTurningBody" is propagated: the body has no direct callers and no CTCLook member; `this` is
// passed straight through to the tree (the map object starts with the tree's head), so the class is named by address.
// Shape: the 8-byte value_type is a stack local at [esp] (the two arguments copied through eax/edx), the pair<iterator,bool>
// result a second frame local at [esp+8]; `add esp,10h` releases the frame because insert pops its own two arguments.
struct CRetailMap_00851cc0 {
    struct SValue { long first; long second; };
    struct SNode;
    struct SIterator { SNode* m_pNode; };
    struct SInsertResult { SIterator first; bool second; };
    SNode*        m_pHead;                         // +0x00
    unsigned long m_size;                          // +0x04
    SInsertResult insert(const SValue& value);     // 0x00463E2E
    void Add(long key, long value);
};

void CRetailMap_00851cc0::Add(long key, long value)
{
    SValue entry;
    entry.first = key;
    entry.second = value;
    insert(entry);
}
