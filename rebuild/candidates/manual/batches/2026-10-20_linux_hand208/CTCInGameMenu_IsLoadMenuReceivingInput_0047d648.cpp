// CTCInGameMenu::IsLoadMenuReceivingInput() const -- true when any menu component has a "UI_MENU_ENTRY_LOAD_CONFIRM" child in input state 5 or 3
// Leaf: IsLoadMenuReceivingInput
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047D648, 170 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`). Label correction: the catalog calls this row
// "IsQuitMenuReceivingInput" (that is 0x0047D537, landed); Ego_r.pdb lists ?IsLoadMenuReceivingInput@CTCInGameMenu@@QBE_NXZ
// and this body looks up the load-confirm entry. Twin of the landed 0x0047D6F2 (same loop over "COMPONENT_SAVE").
// Callees: STLport list<CComponent*> ctor @ 0x0042ABAF (allocator by const reference) / dtor @ 0x0042ABCA,
//          CCharString::CCharString(const char*, long) @ 0x0099EBF0 (the by-value name, built in its slot), the
//          component's virtual slot 5 (+0x14, find children by name into the list), the child's slot 87 (+0x15C).
// Shape: the index lives in the frame, the list at [ebp-8], the allocator temporary at [ebp-1]; each child's state
// is read twice (`== 5 || == 3`); the early `return true` destroys the list before the shared epilogue.
// Layout: retail CTCInGameMenu Components vector +0x58 (PDB +0x68; the engine header carries PDB offsets).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CCharString {
public:
    void* m_pData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    CCharString(const CCharString& other);
};

namespace NUISystem { class CComponent; }
struct SAllocator { SAllocator() { } };
struct SComponentListNode {
    SComponentListNode*    m_pNext;
    SComponentListNode*    m_pPrev;
    NUISystem::CComponent* m_data;
};
struct CComponentList {                                   // STLport list<NUISystem::CComponent*>: the node pointer only
    SComponentListNode* m_pNode;
    CComponentList(const SAllocator& allocator = SAllocator());   // 0x0042ABAF
    ~CComponentList();                                    // 0x0042ABCA
};

namespace NUISystem {
class CComponent {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void FindChildrenByName(CCharString name, CComponentList& found);   // slot 5 (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79();
    virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84();
    virtual void v85(); virtual void v86();
    virtual long GetState();                              // slot 87 (+0x15c)
};
}

struct CComponentVector {                                 // retail 12-byte STLport vector
    NUISystem::CComponent** m_pFirst;
    NUISystem::CComponent** m_pLast;
    NUISystem::CComponent** m_pEnd;
    NUISystem::CComponent** begin() const { return m_pFirst; }
    unsigned long size() const { return m_pLast - m_pFirst; }
    NUISystem::CComponent* operator[](unsigned long n) const { return *(begin() + n); }
};

class CTCInGameMenu {
public:
    unsigned char    _pad00[0x58];
    CComponentVector Components;                          // +0x58
    bool IsLoadMenuReceivingInput() const;
};

bool CTCInGameMenu::IsLoadMenuReceivingInput() const
{
    for (unsigned long i = 0; i < Components.size(); ++i) {
        CComponentList found;
        Components[i]->FindChildrenByName(CCharString("UI_MENU_ENTRY_LOAD_CONFIRM", -1), found);
        for (SComponentListNode* node = found.m_pNode->m_pNext; node != found.m_pNode; node = node->m_pNext) {
            NUISystem::CComponent* child = node->m_data;
            if (child && (child->GetState() == 5 || child->GetState() == 3))
                return true;
        }
    }
    return false;
}
