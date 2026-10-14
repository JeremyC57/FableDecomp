// CAnimSet::CAnimSet() -- default constructor: empty Anims / InternalAlphas vectors, ChangeAlphaBy 0, no animator,
// HadUpdate cleared, and two empty retail std::lists (each a self-linked sentinel node) at +0x54 / +0x58.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00711CF0, length 82 bytes.
// Callees: malloc 0x00BFEA0E (twice, the list sentinel nodes; the two cleanups are merged into one add esp,8).
// Layout: the PDB CAnimSet (0x5c bytes) minus 8 for the two 12-byte vectors: ChangeAlphaBy +0x38, PAnimator +0x3c,
// bit-field byte +0x51 (HadUpdate is bit 3: `and al,0F7h`), then two retail lists (4 bytes each: the sentinel
// pointer only) at +0x54 and +0x58 that the PDB does not show. A list's head is zeroed by its initialiser and
// then set to the fresh sentinel whose next/prev point at itself; the node is the sentinel for an 0x54-byte value.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E

template<class T> struct CRetailVector {
    T* first; T* last; T* end_of_storage;
    CRetailVector() : first(0), last(0), end_of_storage(0) {}
};

struct CTransitionBlendAlpha { float Current; float Last; };

template<class T> struct CRetailList {
    struct node { node* next; node* prev; T value; };
    node* head;
    static node* Buynode()
    {
        node* s = (node*)malloc(sizeof(node));
        s->next = s;
        s->prev = s;
        return s;
    }
    CRetailList() : head(0) { head = Buynode(); }
};

struct SAnimListEntry { unsigned char _body[0x54]; };    // node payload: 0x5c - 8 bytes

struct CAnimateThing;

struct CAnimSet {
    long UpdateMode;                                     // +0
    CRetailVector<short> Anims;                          // +4
    CRetailVector<CTransitionBlendAlpha> InternalAlphas; // +0x10
    float CurrentPercentageComplete;                     // +0x1c
    float LastPercentageComplete;                        // +0x20
    float Speed;                                         // +0x24
    unsigned long FrameInternalAlphasUpdated;            // +0x28
    float CurrentAlpha;                                  // +0x2c
    float LastAlpha;                                     // +0x30
    float LastUpdateChange;                              // +0x34
    float ChangeAlphaBy;                                 // +0x38
    CAnimateThing* PAnimator;                            // +0x3c
    unsigned long Flags;                                 // +0x40
    unsigned long TypeFlags;                             // +0x44
    long AnimDelay;                                      // +0x48
    long Priority;                                       // +0x4c
    unsigned char Type;                                  // +0x50
    unsigned char Looping : 1;                           // +0x51
    unsigned char Finished : 1;
    unsigned char Forwards : 1;
    unsigned char HadUpdate : 1;
    unsigned char OwnedByMainAnim : 1;
    unsigned char _pad[2];
    CRetailList<SAnimListEntry> PendingEntries;          // +0x54
    CRetailList<SAnimListEntry> FinishedEntries;         // +0x58
    CAnimSet();
};

CAnimSet::CAnimSet()
    : ChangeAlphaBy(0.0f), PAnimator(0), HadUpdate(0)
{
}
