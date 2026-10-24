// CTCTextureDecal::AddDecal(long decal, CCompositeBlendDesc& desc) -- allocate the next decal entry id, append {id, decal, blend desc} to the decal list, mark the component dirty, ask the parent thing for an appearance update and return the id
// Grade: DIFFER(131v131) -- one schedule in the node copy: retail loads both entry words (mov ecx,[esp+10h] ; mov edx,[esp+14h]) before the first store, ours stores the first word between the loads; tried implicit / memberwise user copy ctor / 8-byte POD key member (with and without ctor); QFE compiler same
// Leaf: AddDecal
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070A490, length 131 bytes (+ int3 padding). __fastcall (this in ecx), `ret 8`.
//         Ego_r.pdb: `?AddDecal@CTCTextureDecal@@QAEJJAAVCCompositeBlendDesc@@@Z`.
// Callees: CCompositeBlendDesc::CCompositeBlendDesc(const CCompositeBlendDesc&) @ 0x005BA48A (out of line, twice: into the
//          entry temporary and into the list node), malloc (CRT import thunk) @ 0x00BFEA0E (the 0x20-byte list node),
//          CThing forwarder @ 0x004C7590 (request 4, as the landed DeleteDecal 0x0070A430; the manifest
//          "CRenderTarget::DetachTarget" is propagated).
// Identity: the manifest label "CRenderTarget_EnqueueDecalUpdate" is descriptive; the landed caller CTCWound::CWound::
//           UpdateDecal (0x00757440) calls it as `AddDecal(long, CCompositeBlendDesc&)` and keeps the returned id.
// Layout (retail, as the landed constructor 0x0070A520 / ChangeDecal 0x0070A3E0 / DeleteDecal 0x0070A430): ParentThing +0x04
//         (CTCBase), Decals +0x0C (4-byte STLport list: the sentinel node), CurrentAllocEntryID +0x1C, Dirty +0x20. A list
//         node is {next, prev, CEntry}; CEntry (not in the PDB, 0x18 bytes) = {allocation id, decal, CCompositeBlendDesc}.
// Shape: `long id = CurrentAllocEntryID++;` then `Decals.push_back(CEntry(id, decal, desc))`: the entry temporary is built
//        in the frame (its desc through the out-of-line copy constructor), the STLport push_back -> insert(end(), x) reads
//        the sentinel before the node malloc, the placement copy keeps its null test, and the node is linked before the
//        sentinel with prev read into a local first (the iterator has a user copy constructor -- the GOTCHAS chunk 263
//        list shape). Dirty is set before the forwarder call; the id comes back in ebx.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E (import thunk)
inline void* operator new(unsigned int, void* where) { return where; }

struct CRGBColour { unsigned char B, G, R, A; };
struct CCompositeBlendDesc {                    // Ego_r.pdb, 16 bytes
    unsigned long TextureIndex;                 // +0x00
    long          BlendType;                    // +0x04 ECompositeBlendType
    CRGBColour    DiffuseColour;                // +0x08
    CRGBColour    SpecularColour;               // +0x0C
    CCompositeBlendDesc(const CCompositeBlendDesc& other);   // 0x005BA48A
};

struct CThing {
    void ForwardToGraphicAppearance(long request);   // 0x004C7590
};

struct CTCTextureDecal_CEntry {                 // CTCTextureDecal::CEntry (0x18 bytes)
    long AllocEntryID;                          // +0x00
    long Decal;                                 // +0x04
    CCompositeBlendDesc Desc;                   // +0x08
    CTCTextureDecal_CEntry(long id, long decal, const CCompositeBlendDesc& desc) : AllocEntryID(id), Decal(decal), Desc(desc) {}
};

struct SDecalNode {                             // STLport _List_node<CTCTextureDecal::CEntry>: 0x20 bytes
    SDecalNode* _M_next;                        // +0x00
    SDecalNode* _M_prev;                        // +0x04
    CTCTextureDecal_CEntry _M_data;             // +0x08
};

struct SDecalIterator {                         // STLport _List_iterator
    SDecalNode* _M_node;
    SDecalIterator(SDecalNode* x) : _M_node(x) {}
    SDecalIterator(const SDecalIterator& x) : _M_node(x._M_node) {}
};

struct CRetailList_DecalEntry {                 // STLport list<CEntry>: the sentinel pointer only
    SDecalNode* _M_node;
    SDecalNode* _M_create_node(const CTCTextureDecal_CEntry& x)
    {
        SDecalNode* p = (SDecalNode*)malloc(sizeof(SDecalNode));
        new (&p->_M_data) CTCTextureDecal_CEntry(x);
        return p;
    }
    SDecalIterator end() { return SDecalIterator(_M_node); }
    SDecalIterator insert(SDecalIterator position, const CTCTextureDecal_CEntry& x)
    {
        SDecalNode* tmp = _M_create_node(x);
        SDecalNode* n = position._M_node;
        SDecalNode* prev = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        n->_M_prev = tmp;
        return tmp;
    }
    void push_back(const CTCTextureDecal_CEntry& x) { insert(end(), x); }
};

struct CTCTextureDecal {
    void* __vftable;                            // +0x00
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x4];
    CRetailList_DecalEntry Decals;              // +0x0c
    unsigned char EngineDecals[0xc];            // +0x10 vector<CEngineTextureDecal>
    long CurrentAllocEntryID;                   // +0x1c
    bool Dirty;                                 // +0x20
    long AddDecal(long decal, CCompositeBlendDesc& desc);
};

long CTCTextureDecal::AddDecal(long decal, CCompositeBlendDesc& desc)
{
    long id = CurrentAllocEntryID++;
    Decals.push_back(CTCTextureDecal_CEntry(id, decal, desc));
    Dirty = true;
    ParentThing->ForwardToGraphicAppearance(4);
    return id;
}
