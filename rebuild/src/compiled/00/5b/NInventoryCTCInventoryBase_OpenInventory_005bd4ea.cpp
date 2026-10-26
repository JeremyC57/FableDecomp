// NInventory::CTCInventoryBase::OpenInventory() -- opens the inventory screen: stamps the render time, flags the first render frame, remembers the display's presentation interval once, marks the inventory open, closes the player GUI, deselects the item, selects the current category (if any) and restarts the item animation on the highlighted item, then rearms the four menu arrow highlights
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD4EA, length 173 bytes. Size TU without frame pointer (optimize("s",on)).
// Callees: GFGetTime @ 0x009E1BC0 (`__fastcall`, a double on the FPU stack stored straight into LastRenderTime), CPlayerGui::Close(bool)
// @ 0x00438400 on g_pPlayerGui (0x013B8790), CTCInventoryBase virtual slot 46 (+0xB8, SelectItem(long)), CCategory::SetAsSelected(bool)
// @ 0x005BC934 (CCategory is 0x2C bytes), CTCInventoryBase::GetHighlightedItem() @ 0x005BCAE3 and ResetInventoryAnim(long) @ 0x005BCD2D;
// the presentation interval is copied from g_pDisplayDevice (0x013B8390) +0x1CC.
// Layout: retail CTCInventoryBase is 0x130 bytes -- the PDB layout minus 4 bytes per preceding std::vector: FirstRenderFrame +0x18,
// Categories +0x20, the four MenuArrowHighlights +0x38..+0x44, LastRenderTime +0x50, InventoryOpen +0x58, OldIntervalDesc +0x5C,
// OldIntervalDescInitialised +0x64, PDef +0x68, SelectedCategoryIndex +0x70, RHSetInvItemDefIndex +0x128.
// Shape: the definition is held alive for the whole body -- an addref with a null test right after the time stamp and the inlined
// release (`dec [edi+4] ; jne ; jmp [vtable+4]`) as the tail. The pointer lives in edi, so the hold is written as the inline AddRef /
// Release pair on a raw pointer local (a CDefPointer local with a destructor is homed in a frame slot and drags in a frame pointer
// that retail has not). The FirstRenderFrame store sits before the addref in source: retail schedules it between the null test and
// its branch. The constant 1 lives in ebx (`xor ebx,ebx ; inc ebx`) and feeds every true/1 store; the 8-byte interval descriptor is
// copied as two dwords; the empty test on Categories is begin() != end(). pack(4) keeps the double from padding the vptr.
#pragma optimize("s", on)
double __fastcall GFGetTime();                   // 0x009E1BC0
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                      // slot 1
    long m_refCount;                             // +0x04
};
class CInventoryDef : public CDefPointeeBase { public: };
template <class T> class CDefPointer { public:          // intrusive counted definition pointer (4 bytes)
    T* Object;
};
template <class T> __forceinline T* AddRef(T* object)
{
    if (object)
        ++object->m_refCount;
    return object;
}
template <class T> __forceinline void Release(T* object)
{
    if (object && --object->m_refCount == 0)
        object->Destroy();
}
struct CPresentationIntervalDesc {               // 8 bytes
    bool PresentImmediate;
    bool PresentImmediateIfSyncMissed;
    long VSyncCount;
};
struct CDisplayDevice {
    unsigned char _pad00[0x1cc];
    CPresentationIntervalDesc PresentationInterval;   // +0x1cc
};
extern CDisplayDevice* g_pDisplayDevice;         // 0x013B8390
class CPlayerGui { public:
    void Close(bool immediately);                // 0x00438400
};
extern CPlayerGui* g_pPlayerGui;                 // 0x013B8790

namespace NInventory {
class CCategory { public:                               // 0x2C bytes
    unsigned char _body[0x2C];
    void SetAsSelected(bool selected);           // 0x005BC934
};
struct CCategoryVector {                         // std::vector<CCategory> (12 bytes)
    CCategory* _M_start;
    CCategory* _M_finish;
    CCategory* _M_end_of_storage;
    CCategory* begin() { return _M_start; }
    CCategory* end() { return _M_finish; }
    CCategory& operator[](unsigned long n) { return *(begin() + n); }
};
#pragma pack(push, 4)
class CTCInventoryBase { public:                        // retail 0x130 bytes
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void SelectItem(long item);          // slot 46 (+0xB8)

    unsigned char   _pad04[0x18 - 0x04];
    bool            FirstRenderFrame;            // +0x18
    long            InventorySelectionState;     // +0x1c
    CCategoryVector Categories;                  // +0x20
    unsigned char   _pad2c[0x38 - 0x2c];
    long            MenuArrowHighlightsCategoryUp;   // +0x38 (ESelectionState)
    long            MenuArrowHighlightsCategoryDn;   // +0x3c
    long            MenuArrowHighlightsItemUp;       // +0x40
    long            MenuArrowHighlightsItemDn;       // +0x44
    long            TriggerStateLeft;            // +0x48
    long            TriggerStateRight;           // +0x4c
    double          LastRenderTime;              // +0x50
    bool            InventoryOpen;               // +0x58
    CPresentationIntervalDesc OldIntervalDesc;   // +0x5c
    bool            OldIntervalDescInitialised;  // +0x64
    CDefPointer<CInventoryDef> PDef;             // +0x68
    long            GlobalDefIndex;              // +0x6c
    long            SelectedCategoryIndex;       // +0x70
    unsigned char   _pad74[0x128 - 0x74];
    long            RHSetInvItemDefIndex;        // +0x128
    long            ParentMapNumber;             // +0x12c

    long GetHighlightedItem();                   // 0x005BCAE3
    void ResetInventoryAnim(long item);          // 0x005BCD2D
    void OpenInventory();
};
#pragma pack(pop)
void CTCInventoryBase::OpenInventory()
{
    LastRenderTime = GFGetTime();
    FirstRenderFrame = true;
    CInventoryDef* inventoryDef = AddRef(PDef.Object);   // keeps the definition alive while the screen comes up
    if (!OldIntervalDescInitialised) {
        OldIntervalDesc = g_pDisplayDevice->PresentationInterval;
        OldIntervalDescInitialised = true;
    }
    InventoryOpen = true;
    g_pPlayerGui->Close(true);
    SelectItem(0);
    if (Categories.begin() != Categories.end())
        Categories[SelectedCategoryIndex].SetAsSelected(true);
    long item = GetHighlightedItem();
    if (item > 0 && RHSetInvItemDefIndex == 0)
        ResetInventoryAnim(item);
    MenuArrowHighlightsCategoryUp = 1;
    MenuArrowHighlightsCategoryDn = 1;
    MenuArrowHighlightsItemUp = 1;
    MenuArrowHighlightsItemDn = 1;
    Release(inventoryDef);
}
}