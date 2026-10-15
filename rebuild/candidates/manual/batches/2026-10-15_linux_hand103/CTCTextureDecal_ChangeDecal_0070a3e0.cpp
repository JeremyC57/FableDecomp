// CTCTextureDecal::ChangeDecal(long id, const SDecalData& data) -- overwrites the 16-byte data of every decal entry with the given allocation id and marks the component dirty
// Retail: Fable.exe @ 0x0070A3E0, 69 bytes (`ret 8`), no callees.
// Layout (retail, as the landed constructor 0x0070A520): Decals +0x0C is a 4-byte STLport list (the
// sentinel node pointer), EngineDecals +0x10 a 12-byte vector, CurrentAllocEntryID +0x1C, Dirty +0x20.
// A list node is {next, prev, CEntry}: the entry's allocation id at node+0x08 and a 16-byte data
// record at node+0x10 (CEntry is not in the PDB; its other word is not touched here).
// Shape: a plain `for` over the list (begin = sentinel->next, end re-read from the member each
//        pass because the stores might alias it); the data record is a POD assigned whole, which VC7.1
//        copies dword by dword through one register with `lea esi,[node+10h]` / `mov edi,data`.
struct SDecalData {                     // 16 bytes
    long m_words[4];
};

struct CDecalEntry {                    // 0x18 bytes
    long m_allocEntryID;                // +0x00 (node +0x08)
    long m_unknown;                     // +0x04
    SDecalData m_data;                  // +0x08 (node +0x10)
};

struct SDecalNode {                     // STLport _List_node<CTCTextureDecal::CEntry>: 0x20 bytes
    SDecalNode* Next;                   // +0x00
    SDecalNode* Prev;                   // +0x04
    CDecalEntry Entry;                  // +0x08
};

struct CTCTextureDecal {
    void* __vftable;                    // +0x00
    unsigned char _pad_0x04[0x8];       // +0x04 CTCBase
    SDecalNode* Decals;                 // +0x0c STLport list<CEntry> (sentinel)
    unsigned char EngineDecals[0xc];    // +0x10 vector<CEngineTextureDecal>
    long CurrentAllocEntryID;           // +0x1c
    bool Dirty;                         // +0x20
    void ChangeDecal(long id, const SDecalData& data);
};

void CTCTextureDecal::ChangeDecal(long id, const SDecalData& data)
{
    for (SDecalNode* node = Decals->Next; node != Decals; node = node->Next) {
        if (node->Entry.m_allocEntryID == id) {
            node->Entry.m_data = data;
            Dirty = true;
        }
    }
}
