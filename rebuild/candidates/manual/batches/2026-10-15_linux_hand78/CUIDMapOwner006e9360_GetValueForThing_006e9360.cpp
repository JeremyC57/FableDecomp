// CUIDMapOwner_006e9360::GetValueForThing(const CKeyedThing* thing) -- copy the thing's 64-bit id (+0x18/+0x1c) into a local key, look it up in the owner's STLport map at +0x10 (the out-of-line node find) and return the address of the found node's mapped value (+0x18); the caller relies on the id being present
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E9360, length 41 bytes.
// Leaf: GetValueForThing
// Callees: the map's node find @ 0x006F0250 (`__thiscall` on the tree, key by reference, the node pointer in eax:
// a root walk comparing the high word then the low word unsigned, then the end()/`key < node` rejection).
// Shape: an 8-byte frame for the key copy (low word stored before the push, high word after `add ecx,10h`),
// `lea edx,[esp] ; push edx`, then `add eax,18h` (node value = colour, parent, left, right, 8-byte key, value),
// `ret 4`. The manifest's "CSurface::GetHeight" label is propagated over this body and its byte-identical twin
// 0x006E9390; neither touches a surface (the owner class is unidentified, so it is named by address).
struct SEntryValue {
    long m_data;                                   // +0x00 (opaque mapped value)
};

struct SMapNode {                                  // STLport _Rb_tree_node<pair<const unsigned __int64, SEntryValue> >
    long             m_colour;                     // +0x00
    SMapNode*        m_pParent;                    // +0x04
    SMapNode*        m_pLeft;                      // +0x08
    SMapNode*        m_pRight;                     // +0x0c
    unsigned __int64 m_key;                        // +0x10
    SEntryValue      m_value;                      // +0x18
};

struct CUIDMap {                                   // the map's header pointer
    SMapNode* m_pHeader;                           // +0x00
    SMapNode* FindNode(const unsigned __int64& key) const;   // 0x006F0250
};

struct CKeyedThing {
    unsigned char    _pad00[0x18];
    unsigned __int64 m_uid;                        // +0x18
};

struct CUIDMapOwner_006e9360 {
    unsigned char _pad00[0x10];
    CUIDMap       m_values;                        // +0x10
    SEntryValue* GetValueForThing(const CKeyedThing* thing);
};

SEntryValue* CUIDMapOwner_006e9360::GetValueForThing(const CKeyedThing* thing)
{
    unsigned __int64 key = thing->m_uid;
    return &m_values.FindNode(key)->m_value;
}
