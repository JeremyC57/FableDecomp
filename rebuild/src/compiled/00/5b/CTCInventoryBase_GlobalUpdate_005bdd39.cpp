// CTCInventoryBase::GlobalUpdate(float deltaTime) -- while the inventory is open: refreshes the UI background capture, finishes a category slide that reached 1.0 by selecting the category, then updates the item animation
// Leaf: GlobalUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BDD39, length 83 bytes; size TU; NInventory::CTCInventoryBase vtable 0x01253DBC slot 26.
// Callees: CTBaseSingleton<CUserProfileManager>::Get @ 0x0040D2A0 (`__fastcall`, no arguments),
// CUserProfileManager::CaptureBackBufferForUIBackgroundIfNonExistant @ 0x004075B0, CCategory::SetAsSelected(bool) @ 0x005BC934,
// CTCInventoryBase::UpdateInventoryAnim @ 0x005BD699. Constant 1.0f @ 0x0122DED8.
// Retail CTCInventoryBase: no engine header; PDB layout minus 4 bytes per preceding std::vector: Categories +0x20,
// InventoryOpen +0x58, SelectedCategoryIndex +0x70, Dying +0x88, CategoryMoving +0x89, CategoryMovement +0x8C.
// Shape: `fld 1.0 ; fld member ; fucompp ; test ah,44h ; jp` is `member == 1.0f`; CategoryMoving is addressed
// twice (test + clear), hence `lea ecx,[esi+89h]`. The float argument is unused.
#pragma optimize("s", on)
class CUserProfileManager { public:
    static CUserProfileManager& __fastcall Get();          // 0x0040D2A0 (CTBaseSingleton<CUserProfileManager>::Get)
    void CaptureBackBufferForUIBackgroundIfNonExistant();  // 0x004075B0
};
struct CCategory_Retail {
    unsigned char _body[0x2C];
    void SetAsSelected(bool selected);           // 0x005BC934
};
struct CCategoryVector {
    CCategory_Retail* first;
    CCategory_Retail* last;
    CCategory_Retail* end_of_storage;
    CCategory_Retail& operator[](unsigned long n) { return first[n]; }
};
struct CTCInventoryBase_Retail {
    virtual void v00();
    unsigned char   _pad_0x04[0x1C];
    CCategoryVector Categories;                  // +0x20
    unsigned char   _pad_0x2c[0x2C];
    bool            InventoryOpen;               // +0x58
    unsigned char   _pad_0x59[0x17];
    long            SelectedCategoryIndex;       // +0x70
    unsigned char   _pad_0x74[0x14];
    bool            Dying;                       // +0x88
    bool            CategoryMoving;              // +0x89
    float           CategoryMovement;            // +0x8C
    void UpdateInventoryAnim();                  // 0x005BD699
    virtual void GlobalUpdate(float deltaTime);
};
void CTCInventoryBase_Retail::GlobalUpdate(float deltaTime)
{
    if (InventoryOpen) {
        CUserProfileManager::Get().CaptureBackBufferForUIBackgroundIfNonExistant();
        if (CategoryMoving && CategoryMovement == 1.0f) {
            CategoryMoving = false;
            Categories[SelectedCategoryIndex].SetAsSelected(true);
        }
        UpdateInventoryAnim();
    }
}