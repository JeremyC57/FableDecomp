// CTCInventory::ConstructStyleCardsScreenPC(long, long, long, CComponent*) -- PC style-cards screen: hands this inventory and its owning thing to the front-end manager, then builds the style-cards list component
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0058A729, length 58 bytes.
// Callees: NUISystem::CFrontEndManager::GetInstance @ 0x0041E5F2 (called twice, once per virtual call),
//          front-end manager virtual slots 21 (+0x54) and 26 (+0x68),
//          CTCInventory::GetStyleCardsListUIComponent(long, long, long, CComponent*) @ 0x00589402.
// Pragma: optimize("s", on) -- the four arguments are forwarded with pass-through `push [esp+14h]`.
// Layout: only the CTCBase owner pointer at +0x04 is read (engine/CTCInventory.h pads it; the
// shipped class is narrower than the PDB layout, so a local view names the one member).
struct CThing;
struct CComponent;
struct CTCInventory;

// NUISystem::CFrontEndManager as this body sees it: two virtual setters (slots 21 and 26).
struct CFrontEndManager_Calls {
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
    virtual void SetActiveInventory(CTCInventory* inventory);   // slot 21 (+0x54)
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void SetActiveThing(CThing* thing);                 // slot 26 (+0x68)
    static CFrontEndManager_Calls* __fastcall GetInstance();    // 0x0041E5F2
};

struct CTCInventory {
    void*   __vftable;                            // +0x00
    CThing* PThing;                               // +0x04 CTCBase owner

    CComponent* GetStyleCardsListUIComponent(long a, long b, long c, CComponent* parent);   // 0x00589402
    CComponent* ConstructStyleCardsScreenPC(long a, long b, long c, CComponent* parent);
};

CComponent* CTCInventory::ConstructStyleCardsScreenPC(long a, long b, long c, CComponent* parent)
{
    CFrontEndManager_Calls::GetInstance()->SetActiveInventory(this);
    CFrontEndManager_Calls::GetInstance()->SetActiveThing(PThing);
    return GetStyleCardsListUIComponent(a, b, c, parent);
}