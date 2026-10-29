// NInventory::CTCInventoryStats::ConstructScreen(NUISystem::CComponent* topLevel) -- creates the UI_STATS_LIST screen, touches its three entries, and wires the experience entry's select / back actions (type 3: go to the top-level menu screen in state 0 / 1); returns the screen
// Retail: Fable.exe (TLC, VC7.1) @ 0x005CD074, 562 bytes, `ret 4`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CManager::GetInstance @ 0x0041E5F2 (called once with the result dropped, then again for CreateComponent
// @ 0x0041DB1D), CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0, the component's slot 3 (+0x0C, child by name; CCharString by value
// built in its argument slot), operator new @ 0x00BFEA1A, NUISystem::CAction::CAction @ 0x0042BE50, CCountedPointer<CAction>
// (CAction*) @ 0x0042AA29 and its dtor @ 0x00429160, the param list push_back(const T&) @ 0x0042B646, the inline
// CActionParamState(long) ctor (vtable 0x012366EC, type 2), the component's slots 66 (+0x108) and 68 (+0x110).
// Shape: a component converts to its CActionParam subobject at +0x18 with the null-checked `neg ; sbb ; and` adjust. The
// push_back gets a same-TU noinline stand-in (it only reads the value), which lets VC7.1 put both param temporaries in the
// dead topLevel argument slot ([ebp+8]) as retail does; with a declaration only they take the frame word at [ebp-8].
// The first `if` arm calls AddSelectedAction on the experience entry, not on the personality entry (retail behaviour).
// Layout: CAction is the landed 0x10-byte model (AddSoundAction 0x0056F615); slot names 66/68 are descriptive.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
namespace NUISystem {
class CActionParam { public:
    virtual void v00();
    long Type;                                          // +0x04
    CActionParam(long type) : Type(type) {}
};
struct CActionParamState : CActionParam {               // vtable 0x012366EC
    long State;                                         // +0x08
    CActionParamState(long state) : CActionParam(2), State(state) {}
};
struct CActionParamList {                               // STLport list<CActionParam*>
    void* _M_node;
    void push_back(CActionParam* const& value);         // 0x0042B646
};
class CAction { public:                                        // retail 0x10 bytes
    long             ActionType;                        // +0x00
    CActionParamList UIActionParams;                    // +0x04
    void*            ThingActionParams;                 // +0x08
    bool             Valid;                             // +0x0C
    CAction();                                          // 0x0042BE50
};
}
void InsertParamNode(void* node, NUISystem::CActionParam* value);   // stand-in callee of the push_back body below
#pragma auto_inline(off)
__declspec(noinline) void NUISystem::CActionParamList::push_back(NUISystem::CActionParam* const& value)
{
    InsertParamNode(_M_node, value);
}
#pragma auto_inline(on)
template<class T> class CCountedPointer { public:
    T*    m_pObject;
    void* m_pCount;
    CCountedPointer(T* object);                         // 0x0042AA29
    ~CCountedPointer();                                 // 0x00429160
    T* operator->() const { return m_pObject; }
};
namespace NUISystem {
struct CComponentBase {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual class CComponent* GetChild(CCharString name);   // slot 3 (+0x0C)
    virtual void v04(); virtual void v05();
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
    virtual void AddSelectedAction(const CCountedPointer<CAction>& action);   // slot 66 (+0x108)
    virtual void v67();
    virtual void AddBackAction(const CCountedPointer<CAction>& action);       // slot 68 (+0x110)
    unsigned char _pad04[0x18 - 0x04];
};
class CComponent : public CComponentBase, public CActionParam { public:      // the action-parameter subobject at +0x18
};
class CManager { public:
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
    static CManager* __fastcall GetInstance();    // 0x0041E5F2
};
}

namespace NInventory {
struct CTCInventoryStats {
    NUISystem::CComponent* ConstructScreen(NUISystem::CComponent* topLevel);
};
NUISystem::CComponent* CTCInventoryStats::ConstructScreen(NUISystem::CComponent* topLevel)
{
    NUISystem::CManager::GetInstance();
    NUISystem::CComponent* screen;
    {
        CCharString name("UI_STATS_LIST");
        screen = NUISystem::CManager::GetInstance()->CreateComponent(name, false);
    }
    screen->GetChild(CCharString("UI_MENU_ENTRY_EXPERIENCE"));
    screen->GetChild(CCharString("UI_MENU_ENTRY_PERSONALITY"));
    screen->GetChild(CCharString("UI_STATS_EXP_SCREEN"));
    CCountedPointer<NUISystem::CAction> action(new NUISystem::CAction);
    action->ActionType = 3;
    action->UIActionParams.push_back(topLevel->GetChild(CCharString("UI_TOP_LEVEL_MENU_SCREEN")));
    action->UIActionParams.push_back(new NUISystem::CActionParamState(0));
    NUISystem::CComponent* top = topLevel->GetChild(CCharString("UI_TOP_LEVEL_MENU_SCREEN"));
    if (top) {
        CCountedPointer<NUISystem::CAction> back(new NUISystem::CAction);
        back->ActionType = 3;
        back->UIActionParams.push_back(top);
        back->UIActionParams.push_back(new NUISystem::CActionParamState(1));
        NUISystem::CComponent* personality = screen->GetChild(CCharString("UI_MENU_ENTRY_PERSONALITY"));
        NUISystem::CComponent* experience = screen->GetChild(CCharString("UI_MENU_ENTRY_EXPERIENCE"));
        if (personality) {
            personality->AddBackAction(back);
            experience->AddSelectedAction(action);
        }
        if (experience) {
            experience->AddBackAction(back);
            experience->AddSelectedAction(action);
        }
    }
    return screen;
}
}