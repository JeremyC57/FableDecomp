// CTCInGameMenu::~CTCInGameMenu() -- in-game (pause) menu destructor: unregister, delete the menu components, clear the dialog pointers, tidy the front end, then the members in reverse and the CTCBase base
// Retail 0x0047E07F, 263 bytes (size-optimised TU: `#pragma optimize("s", on)`; `and dword ptr [m],0`, `pop ecx`
// after free). Only caller: the deleting destructor 0x004861C6 (vtable 0x01236804 is stored first).
// Callees: CTCBase::RemoveFromGlobalTCs 0x00686DF0; the components' virtual deleting destructor (slot 0, `push 1`);
// vector<CComponent*>::erase(first, last) 0x00485AA7 (manifest "CGuiControlListBox::SelectEntryFromData" is
// propagated); CFrontEndManager::GetInstance 0x0041E5F2 (__fastcall) and its virtual slots 6 (+0x18), 29 (+0x74)
// and 37 (+0x94) (names unknown, as in the landed CTCInventoryTrade::DeleteComponents 0x0059C8AE); the global
// front-end node list 0x013B8A98 clear 0x004855A3; member destructors: map<CWideString,bool> 0x00485E45,
// vector<CCharString> 0x00414EA0 (x2), vector<CEnginePrimitiveHandle> 0x00440C40 (x2), vector<CWideString>
// 0x00412130; free 0x00BFEA14 for the POD vectors; CTCBase::~CTCBase 0x00686830 (tail jump).
// Layout: Ego_r.pdb CTCInGameMenu with 12-byte retail STLport vectors (engine/CTCInGameMenu.h uses the PDB's
// 16-byte vectors, so every member past +0x28 sits lower in retail; local model): PDef +0x10, FileNames +0x28,
// FileTimes +0x34, PrimitiveHandles +0x40, MenuPrimitiveHandles +0x4C, Components +0x58, FileRegionNames +0x78,
// FileRegionMiniMapNames +0x84, FilePlayTimes +0x90, the two video-option dialogs +0xB0/+0xB4, Signatures +0xE0.
// Shape: STLport size() as an inline `_M_finish - _M_start` (finish loaded first) and operator[] through begin();
// the components are cleared through an inline `clear() { erase(begin(), end()); }` (end() read through the vector
// pointer in edi; a direct `erase(begin(), end())` re-reads it through `this`); the POD vector dtors are
// `__forceinline` `if (start) free(start)`; the CDefPointer dtor zeroes inside its null test.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

class CThing;
template <class T> struct CPODVector {             // STLport vector of a trivial type (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    __forceinline ~CPODVector()
    {
        if (_M_start)
            free(_M_start);
    }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) { return *(begin() + n); }
    T* erase(T* first, T* last);                   // 0x00485AA7 (vector<CComponent*>)
    void clear() { erase(begin(), end()); }
};
struct CWideStringVector {                         // vector<CWideString>
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    ~CWideStringVector();                          // 0x00412130
};
struct CCharStringVector {                         // vector<CCharString>
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    ~CCharStringVector();                          // 0x00414EA0
};
struct CPrimitiveHandleVector {                    // vector<CEnginePrimitiveHandle>
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    ~CPrimitiveHandleVector();                     // 0x00440C40
};
struct CSignatureMap {                             // map<CWideString,bool>
    void* _M_header; long _M_node_count; long _M_compare; void* _M_pad;
    ~CSignatureMap();                              // 0x00485E45
};
struct CDateAndTime { long Words[4]; };
struct CInventoryDef {
    virtual void v00();
    virtual void Destroy();                        // slot 1 (+0x04)
    long RefCount;                                 // +0x04
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};
struct CDefPointer {                               // CDefPointer<CInventoryDef const>
    CInventoryDef* Object;
    __forceinline ~CDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};

namespace NUISystem {
struct CComponent {
    virtual ~CComponent();
};
struct CFrontEndManager {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void Slot06();                         // +0x18
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void Slot29();                         // +0x74
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void Slot37();                         // +0x94
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
};
}
struct CNodeList {
    void* m_pHead;
    void clear();                                  // 0x004855A3
};
extern CNodeList g_frontEndNodeList;               // 0x013B8A98
struct CYesNo;

class CTCBase {                                    // engine/CTCBase.h offsets
public:
    virtual ~CTCBase();                            // 0x00686830
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
    void RemoveFromGlobalTCs();                    // 0x00686DF0
};

class CTCInGameMenu : public CTCBase {             // vtable 0x01236804
public:
    bool InGameMenuOpen;                           // +0x0c
    CDefPointer PDef;                              // +0x10
    long GlobalDefIndex;                           // +0x14
    long ActiveScreen;                             // +0x18
    long ItemCount;                                // +0x1c
    long ItemHighlightIndex;                       // +0x20
    long ItemTopIndex;                             // +0x24
    CWideStringVector FileNames;                   // +0x28
    CPODVector<CDateAndTime> FileTimes;            // +0x34
    CPrimitiveHandleVector PrimitiveHandles;       // +0x40
    CPrimitiveHandleVector MenuPrimitiveHandles;   // +0x4c
    CPODVector<NUISystem::CComponent*> Components; // +0x58
    long LoadGameIndex;                            // +0x64
    double CurStartPauseTime;                      // +0x68
    double AccumulativePausedTime;                 // +0x70
    CCharStringVector FileRegionNames;             // +0x78
    CCharStringVector FileRegionMiniMapNames;      // +0x84
    CPODVector<long> FilePlayTimes;                // +0x90
    double LastRenderTime;                         // +0xa0
    bool OpenInventory;                            // +0xa8
    CYesNo* m_pOKDialog;                           // +0xac
    CYesNo* PVideoOptionsNotAcceptedDialog;        // +0xb0
    CYesNo* PVideoOptionsChangedDialog;            // +0xb4
    long SaveGameToLoad;                           // +0xb8
    double VideoChangesTimer;                      // +0xc0
    long SaveGameIndex;                            // +0xc8
    long FramesToWaitToSave;                       // +0xcc
    NUISystem::CComponent* PSaveInProgress;        // +0xd0
    double SaveInProgressTime;                     // +0xd8
    CSignatureMap Signatures;                      // +0xe0
    NUISystem::CComponent* PMousePointer;          // +0xf0
    CYesNo* PYesNo;                                // +0xf4
    virtual ~CTCInGameMenu();
};

CTCInGameMenu::~CTCInGameMenu()
{
    RemoveFromGlobalTCs();
    for (unsigned long i = 0; i < Components.size(); ++i)
        delete Components[i];
    Components.clear();
    PVideoOptionsNotAcceptedDialog = 0;
    PVideoOptionsChangedDialog = 0;
    NUISystem::CFrontEndManager::GetInstance()->Slot06();
    NUISystem::CFrontEndManager::GetInstance()->Slot29();
    g_frontEndNodeList.clear();
    NUISystem::CFrontEndManager::GetInstance()->Slot37();
}