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
// NF48 (round 7): the handle vectors are STLport 4.6.2 std::vector<CEnginePrimitiveHandle> and OracleSymbols std::list<long>, both from
// the shared stlport/fable_containers.h (were a local namespace-std list model, CAllocProxy and the CRetailHandleVector stand-in).
// Opt-ins keep out of line what retail calls: list<long>'s base constructor / destructor (0x0042AB58 / 0x0042AB73), vector<CEnginePrimitiveHandle>
// resize(size_type) 0x00443F90 and erase(iterator, iterator) 0x00440D70.
#define FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CEnginePrimitiveHandle.h"
class CPlayerGui;

struct SAllocator {                                // empty std::allocator stand-in
    SAllocator() { }
};

class CWideString { public:
    unsigned short* m_pData;
    CWideString();                                 // 0x0099AED0
};

namespace NPlayerGui {



struct CRetailElementList {                        // VC6 Dinkumware list<CCountedPointer<CQuestInfoElement>>: one sentinel pointer
    void* head;
    explicit CRetailElementList(const SAllocator& allocator = SAllocator());   // 0x00655B33
};

class CDrawBase { public:
    virtual void v00();
    unsigned char       _body[0x1c - 4];           // +0x04 .. +0x1c
    std::vector<CEnginePrimitiveHandle> PrimitiveHandles;          // +0x1c
    CDrawBase(CPlayerGui* gui, void* drawList);    // 0x0064CC65
};

class CDrawQuestInfo : public CDrawBase { public:                // vtable 0x0125A688
    virtual void v00();
    long                GuiMode;                   // +0x28 (EPlayerGuiMode)
    float               FadeInTimer;               // +0x2c
    float               MiniModeTimer;             // +0x30
    bool                MiniMode;                  // +0x34
    long                FramesUntilMiniMode;       // +0x38
    long                MiniGameElementIndex;      // +0x3c
    std::list<long>     OracleSymbols;             // +0x40
    std::vector<CEnginePrimitiveHandle> OraclePrimitiveHandles;    // +0x44
    CWideString         Name;                      // +0x50
    CWideString         Text;                      // +0x54
    CRetailElementList  Elements;                  // +0x58
    CDrawQuestInfo(CPlayerGui* gui, void* drawList);
};

CDrawQuestInfo::CDrawQuestInfo(CPlayerGui* gui, void* drawList)
    : CDrawBase(gui, drawList), GuiMode(0), FadeInTimer(0.0f), MiniModeTimer(0.0f), MiniMode(false), FramesUntilMiniMode(0),
      MiniGameElementIndex(0), OracleSymbols(), OraclePrimitiveHandles(), Name(), Text(), Elements()
{
    PrimitiveHandles.resize(9);
    OraclePrimitiveHandles.clear();
}

}