// CTCInventoryClothing::OpenInventory() -- sets up the mannequin and the selected clothes, opens the base inventory, then notifies the front-end manager (virtual slot 22) with this screen
// Leaf: OpenInventory@CTCInventoryClothing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B4E7C, length 44 bytes; size TU. Virtual slot 30 of the CTCInventoryClothing vtable 0x0124356C.
// Callees: CTCInventoryClothing::InitialiseMannequin @ 0x005B37F7, CTCInventoryClothing::SetUpSelectedClothes @ 0x005B4795,
// CTCInventoryBase::OpenInventory @ 0x005BD4EA (explicit base call), the front-end manager accessor @ 0x0041E5F2 (`__fastcall`,
// no arguments; manifest "GetInstance" -> CFrontEndManager*), and that object's virtual slot 22 (+0x58) taking `this`
// (slot name unknown; "OnInventoryOpened" is descriptive).
// Model: engine/CTCInventoryClothing.h (PDB) has UseMannequin at +0x180; the retail flag is +0x15C, so a local view is used.
#pragma optimize("s", on)
class CTCInventoryClothing;
namespace NUISystem { class CManager { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void OnInventoryOpened(CTCInventoryClothing* inventory);   // slot 22 (+0x58)
    static CManager* __fastcall GetInstance();           // 0x0041E5F2
}; }
namespace NInventory {
class CTCInventoryBase { public:                                 // vptr +0
    virtual void OpenInventory();                                // 0x005BD4EA Ego_r.pdb ?OpenInventory@CTCInventoryBase@NInventory@@UAEXXZ
};
}
class CTCInventoryClothing : public NInventory::CTCInventoryBase { public:
    unsigned char _pad_0x004[0x15C - 0x4];
    bool          UseMannequin;                                  // +0x15C
    virtual void OpenInventory();                                // Ego_r.pdb ?OpenInventory@CTCInventoryClothing@@UAEXXZ
private:
    void InitialiseMannequin();                                  // 0x005B37F7 Ego_r.pdb ?InitialiseMannequin@CTCInventoryClothing@@AAEXXZ
    void SetUpSelectedClothes();                                 // 0x005B4795 Ego_r.pdb ?SetUpSelectedClothes@CTCInventoryClothing@@AAEXXZ
};
void CTCInventoryClothing::OpenInventory()
{
    InitialiseMannequin();
    UseMannequin = true;
    SetUpSelectedClothes();
    NInventory::CTCInventoryBase::OpenInventory();
    NUISystem::CManager::GetInstance()->OnInventoryOpened(this);
}