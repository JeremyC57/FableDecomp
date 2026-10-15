// CTCRegionDisplay::`scalar deleting destructor'(unsigned int) -- its vtable, the global-TC removal, ~CurrentAreaNameExtra / ~CurrentAreaName, ~CTCBase, free for flag bit 0
// Leaf: ??_GCTCRegionDisplay
// Retail: Fable.exe 0x0063A230, 59 bytes; CTCRegionDisplay vtable 0x0125A0EC slot 0 (rtti-vtables.tsv). The
// manifest label "CThingTrackNode::`vector_deleting_destructor'" is propagated.
// Callees: CTCBase::RemoveFromGlobalTCs 0x00686DF0 (the destructor body), CWideString::~CWideString
// 0x0099B510 (+0x10 then +0x0c), CTCBase::~CTCBase 0x00686830, operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CTCRegionDisplay (0x28 bytes, same offsets in retail). engine/CTCBase.h models the vptr as a
// data word, which cannot carry the virtual destructor, so CTCBase is restated (as the landed 0x007A62D0).
// Shape: a `__forceinline` user virtual destructor (a plain inline one stays out of line: 30v59) whose body calls RemoveFromGlobalTCs right after the vtable store; members
// die in reverse order before the out-of-line CTCBase destructor; a `new` factory pulls the `??_G` into the TU.
struct CThing;

struct CTCBase {
    CTCBase();
    virtual ~CTCBase();                                  // 0x00686830
    CThing* ParentThing;                                 // +0x04
    bool    ToBeRemoved;                                 // +0x08
    bool    IsInGlobalUpdateTCs;                         // +0x09
    void RemoveFromGlobalTCs();                          // 0x00686DF0
};

struct CWideString {
    void* m_pData;
    CWideString();
    ~CWideString();                                      // 0x0099B510
};

struct CTCRegionDisplay : CTCBase {
    CWideString   CurrentAreaName;                       // +0x0c
    CWideString   CurrentAreaNameExtra;                  // +0x10
    long          FrameNameChanged;                      // +0x14
    long          CurrentAlpha;                          // +0x18
    long          LastAlpha;                             // +0x1c
    float         DisplayTime;                           // +0x20
    bool          TimeOut;                               // +0x24
    bool          InfoDisplay;                           // +0x25
    bool          UpdateWhilePaused;                     // +0x26
    __forceinline virtual ~CTCRegionDisplay() { RemoveFromGlobalTCs(); }
};

CTCRegionDisplay* NewCTCRegionDisplay()
{
    return new CTCRegionDisplay;
}