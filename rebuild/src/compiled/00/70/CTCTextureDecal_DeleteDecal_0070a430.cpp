// CTCTextureDecal::DeleteDecal(long id) -- erases every decal entry with the given allocation id; if any went, marks the component dirty and forwards appearance request 4 to the parent thing
// Retail: Fable.exe @ 0x0070A430, 81 bytes (`ret 4`).
// Callees: free 0x00BFEA14 (the STLport list node), the CThing forwarder 0x004C7590 (`if
//          (PTCGraphicAppearance) ...(request)`, as the landed CTCLight::RequestAppearanceUpdate 0x00640D00;
//          the manifest "CRenderTarget::DetachTarget" is propagated).
// Layout as ChangeDecal 0x0070A3E0 / the landed constructor 0x0070A520: Decals +0x0C (STLport list,
// sentinel pointer), Dirty +0x20, the CTCBase parent thing +0x04; node = {next, prev, CEntry} with
// the entry's allocation id at node+0x08.
// Shape: `it = erase(it)` with the STLport erase inlined (next/prev unlinked, the node freed, the
//        successor returned) and a `found` byte flag (`xor dl,dl` / `mov dl,1`); the end is re-read
//        from the member each pass.
#include <stdlib.h>

struct CDecalEntry {                    // 0x18 bytes
    long m_allocEntryID;                // +0x00 (node +0x08)
    unsigned char _rest[0x14];
};

struct SDecalNode {                     // STLport _List_node<CTCTextureDecal::CEntry>: 0x20 bytes
    SDecalNode* Next;                   // +0x00
    SDecalNode* Prev;                   // +0x04
    CDecalEntry Entry;                  // +0x08
};

struct CRetailList_DecalEntry {         // STLport list<CEntry>: the sentinel pointer only
    SDecalNode* Node;
    SDecalNode* Begin() { return Node->Next; }
    SDecalNode* End() { return Node; }
    SDecalNode* Erase(SDecalNode* pos)
    {
        SDecalNode* next = pos->Next;
        SDecalNode* prev = pos->Prev;
        prev->Next = next;
        next->Prev = prev;
        free(pos);
        return next;
    }
};

struct CThing {
    void ForwardToGraphicAppearance(long request);  // 0x004C7590
};

struct CTCTextureDecal {
    void* __vftable;                    // +0x00
    CThing* m_pParentThing;             // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x4];
    CRetailList_DecalEntry Decals;      // +0x0c
    unsigned char EngineDecals[0xc];    // +0x10 vector<CEngineTextureDecal>
    long CurrentAllocEntryID;           // +0x1c
    bool Dirty;                         // +0x20
    void DeleteDecal(long id);
};

void CTCTextureDecal::DeleteDecal(long id)
{
    bool found = false;
    for (SDecalNode* it = Decals.Begin(); it != Decals.End(); ) {
        if (it->Entry.m_allocEntryID == id) {
            it = Decals.Erase(it);
            found = true;
        } else {
            it = it->Next;
        }
    }
    if (found) {
        Dirty = true;
        m_pParentThing->ForwardToGraphicAppearance(4);
    }
}