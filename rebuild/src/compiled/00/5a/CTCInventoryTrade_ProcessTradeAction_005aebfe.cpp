// CTCInventoryTrade::ProcessTradeAction() -- performs the trade for the selected category when trading is allowed: in the buy category the highlighted item is applied as a tattoo card or a hair card when it is one, otherwise bought; in the sell category the highlighted item is sold
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005AEBFE, length 194 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CTCInventoryBase::GetSelectedCategory() @ 0x005BCACE, CCategory::GetCategoryIdentifier() @ 0x005BD1B3, CTCInventoryTrade
// virtual slot 75 (+0x12C, the trade-allowed query), CTCInventoryBase::GetHighlightedItem() @ 0x005BCAE3, GetTattooCardDef(long)
// @ 0x005D9AA0 and GetHairCardDef(long) @ 0x005D9A10 (`__fastcall`, a counted def pointer by value through the hidden pointer in ecx;
// the catalog labels both GetHairCardDef), CTCInventoryTrade::ApplyTattooCard(long) @ 0x005AD305, ApplyHairCard(const CHairCardDef*)
// @ 0x0059D9C2, BuyHighlightedItem() @ 0x005A3581 and SellHighlightedItem() @ 0x005A3436, the defs' virtual slot 1 on release.
// Shape: the category identifier starts at zero (esi) and is a two-case switch (11 buy, 12 sell: `sub eax,0Bh ; je ; dec eax ; jne`);
// the two card defs are by-value temporaries ([ebp-8], [ebp-4]) released with the manager's long form (`add eax,4 ; dec [eax] ;
// cmp [eax],0`) after their null tests, inner first.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CRefCounted {                                 // the count sits at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                          // slot 1
};
struct CDefBase : CDefVirtual, CRefCounted {
    void ReleaseTemporary()                          // inlined into ~CDefPointer
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:              // a counted def reference returned by value
    T* m_p;
    ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
};
struct CTattooCardDef : CDefBase { };
class CHairCardDef : public CDefBase { public: };
CDefPointer<CTattooCardDef> __fastcall GetTattooCardDef(long itemType);   // 0x005D9AA0
CDefPointer<CHairCardDef> __fastcall GetHairCardDef(long itemType);       // 0x005D9A10
enum EInventoryCategoryIdentifier { INVENTORY_CATEGORY_BUY = 11, INVENTORY_CATEGORY_SELL = 12 };

namespace NInventory {
class CCategory { public:
    long GetCategoryIdentifier();                    // 0x005BD1B3
};
struct CTCInventoryTrade {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74();
    virtual bool IsTradeActionAllowed();             // slot 75 (+0x12c)

    CCategory* GetSelectedCategory();                // 0x005BCACE (CTCInventoryBase)
    long GetHighlightedItem();                       // 0x005BCAE3 (CTCInventoryBase)
    void ApplyTattooCard(long itemType);             // 0x005AD305
    void ApplyHairCard(const CHairCardDef* card);    // 0x0059D9C2
    void BuyHighlightedItem();                       // 0x005A3581
    void SellHighlightedItem();                      // 0x005A3436
    void ProcessTradeAction();
};
void CTCInventoryTrade::ProcessTradeAction()
{
    long category = 0;
    CCategory* selected = GetSelectedCategory();
    if (selected)
        category = selected->GetCategoryIdentifier();
    if (IsTradeActionAllowed()) {
        switch (category) {
        case INVENTORY_CATEGORY_BUY: {
            long item = GetHighlightedItem();
            if (item > 0) {
                CDefPointer<CTattooCardDef> tattoo = GetTattooCardDef(item);
                if (tattoo.m_p) {
                    ApplyTattooCard(item);
                } else {
                    CDefPointer<CHairCardDef> hair = GetHairCardDef(item);
                    if (hair.m_p)
                        ApplyHairCard(hair.m_p);
                    else
                        BuyHighlightedItem();
                }
            }
            break;
        }
        case INVENTORY_CATEGORY_SELL:
            SellHighlightedItem();
            break;
        }
    }
}
}