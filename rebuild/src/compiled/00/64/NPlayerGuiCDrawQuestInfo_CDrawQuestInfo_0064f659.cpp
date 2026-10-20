#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawQuestInfo::CDrawQuestInfo(CPlayerGui*, std::vector<CDrawBase*>*) -- CDrawBase(gui, list), own vtable, hidden mode with timers and counters cleared, the oracle-symbol list, handle vector, name/text strings and element list built, nine primitive handles reserved and the oracle handles cleared
// Retry 10: RELOCATION_MATCH -- the vector modelled as STLport _Vector_base (m_pEnd an _STLP_alloc_proxy with its own ctor); three plain pointers schedule Name's `lea` before the first store.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064F659, length 129 bytes.
// Callees: NPlayerGui::CDrawBase::CDrawBase @ 0x0064CC65, list<long>::list(const allocator&) @ 0x0042AB58 (OracleSymbols +0x40),
// CWideString::CWideString() @ 0x0099AED0 (Name +0x50, Text +0x54; manifest "CCharString" is the twin body),
// list<CCountedPointer<CQuestInfoElement>>::list(const allocator&) @ 0x00655B33 (Elements +0x58), vector<CEnginePrimitiveHandle>::Resize(long)
// @ 0x00443F90 (+0x1c, 9), vector<CEnginePrimitiveHandle>::erase(iterator, iterator) @ 0x00440D70 (manifest "Vector_EraseRange").
// Vtable 0x0125A688.
// Layout: PDB CDrawQuestInfo (116 bytes), retail 4 lower up to the oracle list and 0x10 lower for Elements (see the landed
// UpdateCounter 0x00644E3C): GuiMode +0x28, FadeInTimer +0x2c, MiniModeTimer +0x30, MiniMode +0x34, FramesUntilMiniMode +0x38,
// MiniGameElementIndex +0x3c, OracleSymbols +0x40, OraclePrimitiveHandles +0x44, Name +0x50, Text +0x54, Elements +0x58.
// Shape: frame-pointer size TU; ebx is the shared zero; the two lists take their default allocator by const reference, a 1-byte
// temporary homed in the dead argument slot ([ebp+0Fh]); the handle vector's inline constructor zeroes through edi, which the body
// reuses for clear() = erase(begin(), end()) after the base vector's Resize.
struct CPlayerGui;
struct CEnginePrimitiveHandle;

struct SAllocator {                                // empty std::allocator stand-in
    SAllocator() { }
};

struct CWideString {
    unsigned short* m_pData;
    CWideString();                                 // 0x0099AED0
};

namespace NPlayerGui {

template <class P> struct CAllocProxy {      // STLport _STLP_alloc_proxy: the end-of-storage pointer with its own ctor
    P p;
    CAllocProxy(P v) : p(v) { }
    operator P() const { return p; }
    CAllocProxy& operator=(P v) { p = v; return *this; }
};
struct CRetailHandleVector {                       // VC6 Dinkumware vector<CEnginePrimitiveHandle>: three words
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CAllocProxy<CEnginePrimitiveHandle*> m_pEnd;
    CRetailHandleVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    CEnginePrimitiveHandle* begin() { return m_pFirst; }
    CEnginePrimitiveHandle* end() { return m_pLast; }
    void Resize(long count);                       // 0x00443F90
    CEnginePrimitiveHandle* erase(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last);   // 0x00440D70
    void clear() { erase(begin(), end()); }
};

struct CRetailLongList {                           // VC6 Dinkumware list<long>: one sentinel pointer
    void* head;
    explicit CRetailLongList(const SAllocator& allocator = SAllocator());   // 0x0042AB58
};

struct CRetailElementList {                        // VC6 Dinkumware list<CCountedPointer<CQuestInfoElement>>: one sentinel pointer
    void* head;
    explicit CRetailElementList(const SAllocator& allocator = SAllocator());   // 0x00655B33
};

struct CDrawBase {
    virtual void v00();
    unsigned char       _body[0x1c - 4];           // +0x04 .. +0x1c
    CRetailHandleVector PrimitiveHandles;          // +0x1c
    CDrawBase(CPlayerGui* gui, void* drawList);    // 0x0064CC65
};

struct CDrawQuestInfo : CDrawBase {                // vtable 0x0125A688
    virtual void v00();
    long                GuiMode;                   // +0x28 (EPlayerGuiMode)
    float               FadeInTimer;               // +0x2c
    float               MiniModeTimer;             // +0x30
    bool                MiniMode;                  // +0x34
    long                FramesUntilMiniMode;       // +0x38
    long                MiniGameElementIndex;      // +0x3c
    CRetailLongList     OracleSymbols;             // +0x40
    CRetailHandleVector OraclePrimitiveHandles;    // +0x44
    CWideString         Name;                      // +0x50
    CWideString         Text;                      // +0x54
    CRetailElementList  Elements;                  // +0x58
    CDrawQuestInfo(CPlayerGui* gui, void* drawList);
};

CDrawQuestInfo::CDrawQuestInfo(CPlayerGui* gui, void* drawList)
    : CDrawBase(gui, drawList), GuiMode(0), FadeInTimer(0.0f), MiniModeTimer(0.0f), MiniMode(false), FramesUntilMiniMode(0),
      MiniGameElementIndex(0), OracleSymbols(), OraclePrimitiveHandles(), Name(), Text(), Elements()
{
    PrimitiveHandles.Resize(9);
    OraclePrimitiveHandles.clear();
}

}