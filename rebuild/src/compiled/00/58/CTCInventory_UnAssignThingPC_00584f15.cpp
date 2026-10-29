// CTCInventory::UnAssignThingPC(unsigned long, NUISystem::CComponent*) -- empties a quick-access slot: hides the container's assignment icon and puts a fresh empty item component in the container
// Leaf: UnAssignThingPC
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00584F15, 206 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 8`.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (x2), the container's CComponent slot 3 (+0x0C, find
//          child by name, CCharString by value), the {unsigned long, list<CComponent*>} action-parameter pair's ctor
//          @ 0x0042BED4 and copy ctor @ 0x0042CD84 (by-value argument), the icon's slots 62 (+0xF8, takes the pair), 14
//          (+0x38, GetColour by hidden pointer) and 13 (+0x34, SetColour), list<CComponent*> dtor @ 0x0042ABCA,
//          NUISystem::CManager::GetInstance() @ 0x0041E5F2, NUISystem::CManager::CreateComponent(const CCharString&,
//          bool) @ 0x0041DB1D, CCountedPointer<NUISystem::CComponent>(CComponent*) @ 0x00429C15 / dtor @ 0x004291DE,
//          ~CCharString @ 0x0099EAE0, the container's slot 60 (+0xF0, takes the counted pointer; name unproven).
// Shape: `QuickAccessItems[slot] = 0` first (`and [eax+ecx*4],0`); the icon gets the pair {1, empty list} by value
// (built in the argument area through the copy ctor) and its colour's alpha cleared through a CRGBColour local in
// the dead `slot` word; the pair dies after the colour calls. The manager is a named local fetched before the
// implicit "PC_EXPRESSION_ITEM_INSIDE_CONTAINER" conversion temporary (re-addressed with `lea`, in the same dead
// word), which dies once the counted pointer holds the new component.
// Layout: retail CTCInventory QuickAccessItems vector<long> at +0x148 (PDB +0x168).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1); // 0x0099EBF0
    CCharString(const CCharString& other);           // (declared: by-value arguments are built in place)
    ~CCharString();                                  // 0x0099EAE0
};

class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour() {}
};

namespace NUISystem { class CComponent; }
struct SComponentListNode;
struct CComponentList {                              // STLport list<NUISystem::CComponent*>: the node pointer only
    SComponentListNode* m_pNode;
    ~CComponentList();                               // 0x0042ABCA
};
struct SActionParamPair {                            // pair<unsigned long, list<NUISystem::CComponent*> >
    unsigned long  first;                            // +0x00
    CComponentList second;                           // +0x04
    SActionParamPair();                              // 0x0042BED4
    SActionParamPair(const SActionParamPair& other); // 0x0042CD84
};

template<class T> class CCountedPointer { public:
    T*    m_pObject;
    void* m_pCount;
    CCountedPointer(T* object);                      // 0x00429C15
    ~CCountedPointer();                              // 0x004291DE
};

namespace NUISystem {
class CComponent { public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CComponent* FindChild(CCharString name); // slot 3 (+0x0C)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void SetColour(const CRGBColour& colour);   // slot 13 (+0x34)
    virtual CRGBColour GetColour() const;            // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void SetContents_slot60(const CCountedPointer<CComponent>& child);   // slot 60 (+0xF0)
    virtual void v61();
    virtual void SetActionParameter_slot62(SActionParamPair parameter);   // slot 62 (+0xF8)
};
class CManager { public:
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
    static CManager* __fastcall GetInstance();                      // 0x0041E5F2 ?GetInstance@CManager@NUISystem@@SIPAV12@XZ
};
}

struct CLongVector {                                 // retail std::vector<long> {first, last, end}
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long& operator[](unsigned long n) { return _M_start[n]; }
};

class CTCInventory { public:
    void*         __vftable;                         // +0x000
    unsigned char _pad_0x004[0x144];
    CLongVector   QuickAccessItems;                  // +0x148 (PDB +0x168)
    void UnAssignThingPC(unsigned long slot, NUISystem::CComponent* container);
};

void CTCInventory::UnAssignThingPC(unsigned long slot, NUISystem::CComponent* container)
{
    QuickAccessItems[slot] = 0;
    NUISystem::CComponent* icon = container->FindChild(CCharString("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_INSIDE_CONTAINER", -1));
    if (icon) {
        SActionParamPair parameter;
        parameter.first = 1;
        icon->SetActionParameter_slot62(parameter);
        CRGBColour colour = icon->GetColour();
        colour.A = 0;
        icon->SetColour(colour);
    }
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CCountedPointer<NUISystem::CComponent> item(manager->CreateComponent("PC_EXPRESSION_ITEM_INSIDE_CONTAINER", false));
    container->SetContents_slot60(item);
}