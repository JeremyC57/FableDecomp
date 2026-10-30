// Grade: DIFFER(321v321) (retry wave 2) -- unchanged residue: the address-taken CEditBox* store to [ebp+10h] lands before the vtable load (retail after mov ecx,esi). Now on the shared engine/CCountedPointer.h (header debt paid, bytes unchanged). Tried: cast-first box, split declaration, late declaration, box-only calls. Next: model the v00.. filler slots from the NUISystem::CComponent vtable.
// CTCInventory::CreateEditBoxActions(CComponent* parent, const CCharString& buttonName, const CCharString& editBoxName, long a, long b, long c) -- configure the named edit box, give it a "confirm" action (type 3019), give the named button the confirm action and an edit action (type 3018) that carries the edit box, and remember the edit box
// Leaf: CreateEditBoxActions
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0058301A, 321 bytes, `ret 18h`. Size TU with a frame pointer:
//   `#pragma optimize("y", off)` + `("s", on)` (`leave`, `pop ecx` after operator new).
// Callees: CCharString(const CCharString&) @ 0x0099EC30 (FindChild's by-value argument), the component's virtual slots
//   3 FindChild (+0x0C), 152 / 153 / 143 / 150 / 154 (+0x260 / +0x264 / +0x23C / +0x258 / +0x268, edit-box setup),
//   148 (+0x250, AddAction on the edit box), 71 (+0x11C) and 68 (+0x110) on the button; operator new @ 0x00BFEA1A,
//   NUISystem::CAction::CAction @ 0x0042BE50, CCountedPointer<CAction>(CAction*) @ 0x0042AA29 / dtor @ 0x00429160,
//   the action's parameter list push_front(const T&) @ 0x0042B646, list<CEditBox*>::push_back @ 0x005939EE.
// Layout: retail CTCInventory EditBoxes list at +0x144 (PDB +0x15C; the retail 12-byte vectors before it); CAction as
//   the landed 0x0056F615 (UIActionParams +0x04); the edit box's action-parameter base at +0x18.
#include "engine/CCountedPointer.h"
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
};

namespace NUISystem {
struct CActionParam {
    void* __vftable;
};
template <class T> struct CParamList {
    void* _M_node;
    void push_front(const T& x);                     // 0x0042B646
};
struct CAction {                                     // retail 0x10 bytes
    long  ActionType;                                // +0x00
    CParamList<CActionParam*> UIActionParams;        // +0x04
    void* ThingActionParams;                         // +0x08
    bool  Valid;                                     // +0x0C
    CAction();                                       // 0x0042BE50
};
}

namespace NUISystem {
struct CComponent {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CComponent* FindChild(CCharString name);  // slot 3 (+0x0C)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38();
    virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48();
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58();
    virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67();
    virtual void AddFocusAction(const CCountedPointer<CAction>& action);      // slot 68 (+0x110)
    virtual void v69(); virtual void v70();
    virtual void AddSelectAction(const CCountedPointer<CAction>& action);     // slot 71 (+0x11C)
    virtual void v72(); virtual void v73();
    virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78();
    virtual void v79(); virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83();
    virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88();
    virtual void v89(); virtual void v90(); virtual void v91(); virtual void v92(); virtual void v93();
    virtual void v94(); virtual void v95(); virtual void v96(); virtual void v97(); virtual void v98();
    virtual void v99(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
    virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108();
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113();
    virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118();
    virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123();
    virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128();
    virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133();
    virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138();
    virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142();
    virtual void SetMaxLength(long length);           // slot 143 (+0x23C)
    virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147();
    virtual void AddEditAction(const CCountedPointer<CAction>& action);       // slot 148 (+0x250)
    virtual void v149();
    virtual void SetSlot150(long value);              // slot 150 (+0x258)
    virtual void v151();
    virtual void SetSlot152(long value);              // slot 152 (+0x260)
    virtual void SetSlot153(long value);              // slot 153 (+0x264)
    virtual void SetSlot154(long value);              // slot 154 (+0x268)
    unsigned char _pad04[0x14];
    CActionParam  EditParam;                          // +0x18
};
}
using NUISystem::CAction;
using NUISystem::CActionParam;
using NUISystem::CComponent;

template <class T> struct CRetailList {
    void* _M_node;
    void push_back(const T& x);                      // 0x005939EE
};

struct CEditBox : CComponent { };

struct CTCInventory {
    unsigned char _pad000[0x144];
    CRetailList<CEditBox*> EditBoxes;                 // +0x144

    void CreateEditBoxActions(CComponent* parent, const CCharString& buttonName, const CCharString& editBoxName,
                              long maxLength, long value152, long value154);
};

void CTCInventory::CreateEditBoxActions(CComponent* parent, const CCharString& buttonName, const CCharString& editBoxName,
                                        long maxLength, long value152, long value154)
{
    CComponent* editBox = parent->FindChild(editBoxName);
    CEditBox* box = static_cast<CEditBox*>(editBox);
    editBox->SetSlot152(value152);
    editBox->SetSlot153(0x19);
    editBox->SetMaxLength(maxLength);
    editBox->SetSlot150(0);
    editBox->SetSlot154(value154);
    CCountedPointer<CAction> confirm(new CAction);
    confirm->ActionType = 0xBCB;
    editBox->AddEditAction(confirm);
    CComponent* button = parent->FindChild(buttonName);
    CCountedPointer<CAction> edit(new CAction);
    edit->ActionType = 0xBCA;
    edit->UIActionParams.push_front(&editBox->EditParam);
    button->AddSelectAction(confirm);
    button->AddSelectAction(edit);
    button->AddFocusAction(confirm);
    EditBoxes.push_back(box);
}
