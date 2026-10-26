// CTCInGameMenu::CTCInGameMenu(CThing* thing) -- constructor: CTCBase, menu closed, no def, empty item/file/primitive/component lists (five primitive handles reserved), no load or save game selected, the pause and render clocks zeroed, the video-change timer off (-1), no dialogs; then register for the global TC update
// Leaf: ??0CTCInGameMenu
// Retail 0x0047DF8A, 245 bytes (`ret 4`); vtable 0x01236804. Size TU (`push dword ptr [esp+0Ch]` pass-through, `fldz ; fstp`
// double zeroes, `or ecx,-1`): `#pragma optimize("s", on)`, as the landed CTCInGameMenu::ConfirmedVideoOptionsChanged 0x0047D012.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, vector<CEnginePrimitiveHandle>::vector(size_type) @ 0x00485D30 (count 5),
// map<CWideString,bool>::map() @ 0x00486055 (out of line), CTCBase::AddToGlobalTCs @ 0x00686ED0. Double 0x01236700 = -1.0.
// Layout: Ego_r.pdb CTCInGameMenu with retail 12-byte containers (engine/CTCInGameMenu.h carries the PDB offsets, 16-byte
// vectors): InGameMenuOpen +0x0c, PDef +0x10, ActiveScreen +0x18, ItemCount +0x1c, ItemHighlightIndex +0x20, ItemTopIndex
// +0x24, FileNames +0x28, FileTimes +0x34, PrimitiveHandles +0x40, MenuPrimitiveHandles +0x4c, Components +0x58,
// LoadGameIndex +0x64, CurStartPauseTime +0x68, AccumulativePausedTime +0x70, FileRegionNames +0x78, FileRegionMiniMapNames
// +0x84, FilePlayTimes +0x90, LastRenderTime +0xa0, OpenInventory +0xa8, m_pOKDialog +0xac, PVideoOptionsNotAcceptedDialog
// +0xb0, PVideoOptionsChangedDialog +0xb4, SaveGameToLoad +0xb8, VideoChangesTimer +0xc0, SaveGameIndex +0xc8,
// FramesToWaitToSave +0xcc, PSaveInProgress +0xd0, SaveInProgressTime +0xd8, Signatures +0xe0, PMousePointer +0xec, PYesNo +0xf0.
// Shape: the containers are the STLport `_Vector_base` + `_STLP_alloc_proxy` model (it pins the vector(5) argument push after
// the FileTimes zero stores); every scalar is an initialiser in declaration order.
#pragma optimize("s", on)

class CThing;

class CTCBase {
public:
    CTCBase(CThing* thing);                                 // 0x00686800
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
public:
    void AddToGlobalTCs();   // 0x00686ED0
};

template <class T> struct CAllocator { CAllocator() { } };  // STLport allocator<T> (empty)
template <class T> struct CAllocProxy : CAllocator<T> {     // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {                     // STLport _Vector_base<T, allocator<T> >
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
};
template <class T> struct CRetailVector : CVectorBase<T> {  // STLport vector<T> (12 bytes)
    CRetailVector() : CVectorBase<T>(CAllocator<T>()) { }
};
class CEnginePrimitiveHandle;
struct CEnginePrimitiveHandleVector {                       // std::vector<CEnginePrimitiveHandle> (12 bytes)
    void* m_first; void* m_last; void* m_end;
    CEnginePrimitiveHandleVector();
    CEnginePrimitiveHandleVector(unsigned long count);      // 0x00485D30
};
struct CSignatureMap {                                      // std::map<CWideString, bool> (12 bytes)
    void* _M_header; long _M_node_count; long _M_compare;
    CSignatureMap();                                        // 0x00486055
};

class CWideString;
class CCharString;
class CDateAndTime;
class CDefPointeeBase;
namespace NUISystem { class CComponent; class CYesNo; }

class CTCInGameMenu : public CTCBase {                      // vtable 0x01236804
public:
    bool             InGameMenuOpen;                        // +0x0c
    CDefPointeeBase* PDef;                                  // +0x10 CDefPointer<CInventoryDef const>
    long             GlobalDefIndex;                        // +0x14
    long             ActiveScreen;                          // +0x18 EScreenType
    long             ItemCount;                             // +0x1c
    long             ItemHighlightIndex;                    // +0x20
    long             ItemTopIndex;                          // +0x24
    CRetailVector<CWideString>   FileNames;                 // +0x28
    CRetailVector<CDateAndTime>  FileTimes;                 // +0x34
    CEnginePrimitiveHandleVector PrimitiveHandles;          // +0x40
    CRetailVector<CEnginePrimitiveHandle> MenuPrimitiveHandles;   // +0x4c
    CRetailVector<NUISystem::CComponent*> Components;       // +0x58
    long             LoadGameIndex;                         // +0x64
    double           CurStartPauseTime;                     // +0x68
    double           AccumulativePausedTime;                // +0x70
    CRetailVector<CCharString>   FileRegionNames;           // +0x78
    CRetailVector<CCharString>   FileRegionMiniMapNames;    // +0x84
    CRetailVector<long>          FilePlayTimes;             // +0x90
    double           LastRenderTime;                        // +0xa0
    bool             OpenInventory;                         // +0xa8
    NUISystem::CYesNo* m_pOKDialog;                         // +0xac
    NUISystem::CYesNo* PVideoOptionsNotAcceptedDialog;      // +0xb0
    NUISystem::CYesNo* PVideoOptionsChangedDialog;          // +0xb4
    long             SaveGameToLoad;                        // +0xb8
    double           VideoChangesTimer;                     // +0xc0
    long             SaveGameIndex;                         // +0xc8
    long             FramesToWaitToSave;                    // +0xcc
    NUISystem::CComponent* PSaveInProgress;                 // +0xd0
    double           SaveInProgressTime;                    // +0xd8
    CSignatureMap    Signatures;                            // +0xe0
    NUISystem::CComponent* PMousePointer;                   // +0xec
    NUISystem::CYesNo* PYesNo;                              // +0xf0
    CTCInGameMenu(CThing* thing);
    virtual void v00();
};

CTCInGameMenu::CTCInGameMenu(CThing* thing)
    : CTCBase(thing), InGameMenuOpen(false), PDef(0), ActiveScreen(0), ItemCount(0), ItemHighlightIndex(0), ItemTopIndex(0),
      PrimitiveHandles(5), LoadGameIndex(-1), CurStartPauseTime(0.0), AccumulativePausedTime(0.0), LastRenderTime(0.0),
      OpenInventory(false), m_pOKDialog(0), PVideoOptionsNotAcceptedDialog(0), PVideoOptionsChangedDialog(0),
      SaveGameToLoad(-1), VideoChangesTimer(-1.0), SaveGameIndex(-1), FramesToWaitToSave(-1), PSaveInProgress(0),
      PMousePointer(0), PYesNo(0)
{
    AddToGlobalTCs();
}