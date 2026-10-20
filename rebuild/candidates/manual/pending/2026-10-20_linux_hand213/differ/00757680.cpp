// CTCWound::~CTCWound() -- when the parent still has its texture-decal component, every remaining wound's four decal entries (wound/scar texture and bump) are deleted from it and the wound list is emptied from the back; then the list and the CTCBase base are destroyed
// Grade: DIFFER(ours v retail) 216v217 -- register swap only: retail keeps `this` in ebp and the shared zero in ebx, ours `this` in ebx and the zero in ebp (one byte: the vtable store through ebp needs a disp8)
// Leaf: ??1CTCWound
// Retail: Fable.exe (TLC, VC7.1) @ 0x00757680, 217 bytes (+7 int3). __fastcall, `ret`. CTCWound vtable 0x012698B4 stored first.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020; CTCTextureDecal::DeleteDecal(long) 0x0070A430;
//          free 0x00BFEA14 (the list node); list<CTCWound::CWound>::~list 0x00758B00 (manifest "CGuiControlTreePane::
//          SortTreeRecursively" is propagated); CTCBase::~CTCBase 0x00686830 (manifest "~CChunkedFileChunk" is propagated).
// Shape: interface 0xE9 (CTCTextureDecal, bitset word 7 bit 9 -> `test ch,2` on +0x3C) fetched through the out-parameter
//        form into edi (esi pushed only on that path); `while (!Wounds.empty())` re-reads the sentinel each pass, the back
//        wound is reached as `Head->Prev->Value` (the value pointer formed after the first field load), each non-zero decal id
//        is deleted and zeroed through one shared zero register (ebx), the node is unlinked (next, prev) and freed. The list
//        destructor and the base destructor run out of line on `this` kept in ebp.
// Layout: CTCWound (PDB, 0x1C with a 12-byte list): ParentThing +0x04, LastFrameChanged +0x0C, Wounds +0x10 (retail STLport
//         list = one sentinel pointer); CWound (0x44): WoundTextureDecalEntryID +0x2c, WoundBumpDecalEntryID +0x30,
//         ScarTextureDecalEntryID +0x34, ScarBumpDecalEntryID +0x38; retail CThing bitset +0x20, interface map +0x44.
extern "C" void __cdecl free(void* block);       // 0x00BFEA14
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020 (the other TU copy is 0x004365B0)
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCTextureDecal {
    void DeleteDecal(long entryID);             // 0x0070A430
};
struct CWound {
    unsigned char _pad_0x00[0x2c];              // Desc (CEntry), Duration
    long WoundTextureDecalEntryID;              // +0x2c
    long WoundBumpDecalEntryID;                 // +0x30
    long ScarTextureDecalEntryID;               // +0x34
    long ScarBumpDecalEntryID;                  // +0x38
    long MorphIndex;                            // +0x3c
    bool Dead;                                  // +0x40
    bool HadUpdate;                             // +0x41
};
struct CWoundList {                             // STLport list<CTCWound::CWound>: the sentinel node pointer only
    struct Node { Node* Next; Node* Prev; CWound Value; };
    Node* Head;
    bool empty() const { return Head->Next == Head; }
    CWound& back() const { return Head->Prev->Value; }
    void pop_back()
    {
        Node* pos = Head->Prev;
        Node* next = pos->Next;
        Node* prev = pos->Prev;
        prev->Next = next;
        next->Prev = prev;
        free(pos);
    }
    ~CWoundList();                              // 0x00758B00 (erases the remaining nodes, frees the sentinel)
};
struct CTCBase {                                // 0xc bytes
    virtual ~CTCBase();                         // 0x00686830
    CThing* ParentThing;                        // +0x04
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
};
struct CTCWound : CTCBase {                     // vtable 0x012698B4
    unsigned long LastFrameChanged;             // +0x0c
    CWoundList    Wounds;                       // +0x10
    virtual ~CTCWound();
};
CTCWound::~CTCWound()
{
    CTCTextureDecal* decal;
    if (ParentThing->GetInterface(&decal, 0xe9)) {
        while (!Wounds.empty()) {
            CWound& wound = Wounds.back();
            if (wound.WoundTextureDecalEntryID) {
                decal->DeleteDecal(wound.WoundTextureDecalEntryID);
                wound.WoundTextureDecalEntryID = 0;
            }
            if (wound.WoundBumpDecalEntryID) {
                decal->DeleteDecal(wound.WoundBumpDecalEntryID);
                wound.WoundBumpDecalEntryID = 0;
            }
            if (wound.ScarTextureDecalEntryID) {
                decal->DeleteDecal(wound.ScarTextureDecalEntryID);
                wound.ScarTextureDecalEntryID = 0;
            }
            if (wound.ScarBumpDecalEntryID) {
                decal->DeleteDecal(wound.ScarBumpDecalEntryID);
                wound.ScarBumpDecalEntryID = 0;
            }
            Wounds.pop_back();
        }
    }
}
