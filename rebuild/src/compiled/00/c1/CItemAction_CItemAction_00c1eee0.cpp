// CItemAction::CItemAction(CItemDispatcher* dispatcher, CString name, bool named) -- binds the action to the dispatcher's item for the name; when there is none, a new item is created from the name's hash (a named item that also keeps the name when `named`, else a hashed one) and attached to the dispatcher
// Flags: /EHsc
// Leaf: ??0CItemAction
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1EEE0, 217 bytes (`ret 0Ch`). Built under /EHsc. Descriptive name (no PDB covers
//   this library): the manifest's "CAnimationSet_LoadAndUnsheatheWeapons" is propagated. Sibling of the keyed
//   constructor 0x00C1EE60; callers 0x00C2E74A / 0x00C2E82E (event-trigger drivers).
// Callees: CNamedSet::Find @ 0x00C101D0 (chunk 431: (name, collision counter = 0)) on the dispatcher, scalar operator
//   new @ 0x00BFEA1A, CString::GetHash @ 0x00C0DBB0, the named item's constructor @ 0x00C20230 (name, hash; 0x14 bytes,
//   landed CIntelligentPointer_00c20230) or the hashed item's @ 0x00C1F640 (hash; 0x10 bytes, landed
//   CVtable_012b3474), the new item's Attach slot 6 (+0x18), ~CString @ 0x00C0BA40 for the by-value name.
// Shape: the by-value CString parameter puts the frame in EH state 0 from entry (the callee destroys it at the end,
//   state -1); each `new` raises the state to 1 / 2 around its constructor and the shared attach drops it back to 0.
//   The new pointer is homed in the dead `named` slot.
struct CString {
    char* m_pchData;                        // +0x00
    ~CString();                             // 0x00C0BA40
    unsigned long GetHash() const;          // 0x00C0DBB0
};

struct CItemDispatcher;

struct CItem {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05();
    virtual void Attach(CItemDispatcher* dispatcher);   // slot 6 (+0x18)
};

struct CNamedItem : CItem {                 // 0x14 bytes, vtable 0x012B3600 (factory type 0x10103)
    unsigned long m_hash;                   // +4
    unsigned char m_member[8];              // +8
    CString m_name;                         // +0x10
    CNamedItem(const CString& name, unsigned long hash);   // 0x00C20230
};

struct CHashedItem : CItem {                // 0x10 bytes, vtable 0x012B3474 (factory type 0x10102)
    unsigned long m_hash;                   // +4
    unsigned char m_member[8];              // +8
    CHashedItem(unsigned long hash);        // 0x00C1F640
};

struct CItemDispatcher {
    CItem* Find(const CString& name, long* pCollisions);   // 0x00C101D0
};

struct CItemAction {                        // 8-byte {dispatcher, item}
    CItemDispatcher* m_pDispatcher;         // +0
    CItem* m_pItem;                         // +4
    CItemAction(CItemDispatcher* dispatcher, CString name, bool named);
};

CItemAction::CItemAction(CItemDispatcher* dispatcher, CString name, bool named)
    : m_pDispatcher(dispatcher), m_pItem(dispatcher->Find(name, 0))
{
    if (!m_pItem) {
        if (named)
            m_pItem = new CNamedItem(name, name.GetHash());
        else
            m_pItem = new CHashedItem(name.GetHash());
        m_pItem->Attach(m_pDispatcher);
    }
}