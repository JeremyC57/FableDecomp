// CTCGuard::CTCGuard(CThing* thing) -- construct the guard component: CTCBase, a null guard def, an empty pending-crime deque, and the bribe / crime frame counters
// Leaf: ??0CTCGuard
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B2DB0, length 98 bytes (+14 int3 padding), `ret 4`.
// Label correction: the catalog row says "CTCHeroRangedAttacks::CTCHeroRangedAttacks", but the vtable store is CTCGuard's
// (0x0126D80C, rtti-vtables.tsv) and the members match Ego_r.pdb CTCGuard (0x44; 0x54 in retail with the 0x28-byte STLport deque).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, STLport _Deque_base<CCrimeWaitingForBribe>::_M_initialize_map(unsigned)
// @ 0x007B4420 (manifest "CharacterBuffer_InitializeDeque").
// Layout: PDef +0x0c, BribePool +0x10, PendingCrimes +0x14 (start/finish iterators, map, map size), FramePendingCrimesAdded +0x3c,
// FrameLastBribeAdded +0x40, LastCrimeSeenSeverity +0x44, FrameLastCrimeSeen +0x48, FrameLastReceivedApology +0x4c,
// WaitingForRegionKickConfirmation +0x50.
// Shape: the deque is the STLport _Deque_base with an _STLP_alloc_proxy map member (a plain pointer schedules the
// `push 0` after the last zero store); PDef and the deque are the initialiser list (one shared zero register); the counters are body assignments
// (-0x0CCCCCCC = LONG_MIN / 10 for the two "long ago" frames, -0x40000000 = LONG_MIN / 2 for the bribe frame).
struct CCrimeWaitingForBribe { long Crime; long Frame; };
template <class T> struct CDequeIterator {      // STLport _Deque_iterator
    T*  _M_cur;
    T*  _M_first;
    T*  _M_last;
    T** _M_node;
    CDequeIterator() : _M_cur(0), _M_first(0), _M_last(0), _M_node(0) {}
};
template <class T> struct CMapAllocProxy {      // STLport _STLP_alloc_proxy<T**, T*, allocator<T*> >
    T** _M_data;
    CMapAllocProxy(T** p) : _M_data(p) {}
};
template <class T> struct CRetailDeque {        // STLport deque (_Deque_base, 0x28 bytes)
    CDequeIterator<T> _M_start;
    CDequeIterator<T> _M_finish;
    CMapAllocProxy<T> _M_map;
    unsigned int      _M_map_size;
    CRetailDeque() : _M_map(0), _M_map_size(0) { _M_initialize_map(0); }
    void _M_initialize_map(unsigned int numElements);   // 0x007B4420
};
class CThing;
struct CDefPointeeBase;
template <class T> struct CDefPointer {
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) {}
};
struct CGuardDef;
class CTCBase {
public:
    CTCBase(CThing* thing);                     // 0x00686800
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CTCGuard : public CTCBase {               // vtable 0x0126D80C
public:
    CDefPointer<const CGuardDef>        PDef;                       // +0x0c
    long                                BribePool;                  // +0x10
    CRetailDeque<CCrimeWaitingForBribe> PendingCrimes;              // +0x14
    long                                FramePendingCrimesAdded;    // +0x3c
    long                                FrameLastBribeAdded;        // +0x40
    long                                LastCrimeSeenSeverity;      // +0x44
    long                                FrameLastCrimeSeen;         // +0x48
    long                                FrameLastReceivedApology;   // +0x4c
    bool                                WaitingForRegionKickConfirmation;   // +0x50
    CTCGuard(CThing* thing);
    virtual ~CTCGuard();
};

CTCGuard::CTCGuard(CThing* thing)
    : CTCBase(thing)
{
    FrameLastCrimeSeen = -0x0CCCCCCC;
    FrameLastReceivedApology = -0x0CCCCCCC;
    BribePool = 0;
    FramePendingCrimesAdded = 0;
    LastCrimeSeenSeverity = 0;
    WaitingForRegionKickConfirmation = false;
    FrameLastBribeAdded = -0x40000000;
}
