// CTCInventoryExperience::AddSoundAction(NUISystem::CGUIElement* element, CCharString soundName) -- gives a UI element a "play sound" action (type 3003) carrying the sound name as a wide-string parameter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0056F615, length 138 bytes (`ret 8`; `this` unused).
// Frame-pointer size TU: optimize("y",off) + ("s",on) (`leave`, `pop ecx` after operator new).
// Callees: operator new @ 0x00BFEA1A, NUISystem::CAction::CAction @ 0x0042BE50 (manifest "CRumbleDef", propagated),
// CCountedPointer<NUISystem::CAction>(CAction*) @ 0x0042AA29 / its dtor @ 0x00429160 (manifest a propagated _Dest_val),
// CCharString::ToWideString @ 0x0099E480, CActionParamString(const CWideString&) @ 0x0054DAE0, CAction::AddUIActionParam
// @ 0x0053FDB0 (an inlined list push_back on UIActionParams, `this` = the action), element virtual slot 66 (+0x108,
// AddAction(const CCountedPointer<CAction>&)), ~CWideString @ 0x0099B510, ~CCharString @ 0x0099EAE0.
// Shape: the counted pointer's inline operator-> reads the object word ([ebp-0Ch]) for both the type store and the
// AddUIActionParam call; locals destroy in reverse order, then the by-value string argument.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CWideString {
    void* m_pStringData;
    ~CWideString();                                               // 0x0099B510
};
struct CCharString {
    void* m_pStringData;
    ~CCharString();                                               // 0x0099EAE0
    CWideString ToWideString() const;                             // 0x0099E480
};

namespace NUISystem {
struct CActionParam;
struct CActionParamString {
    unsigned char _body[0x0C];
    CActionParamString(const CWideString& text);                  // 0x0054DAE0
};
struct CAction {                                                  // retail 0x10 bytes
    long  ActionType;                                             // +0x00
    void* UIActionParams;                                         // +0x04 (STLport list)
    void* ThingActionParams;                                      // +0x08
    bool  Valid;                                                  // +0x0C
    CAction();                                                    // 0x0042BE50
    void AddUIActionParam(CActionParamString* param);             // 0x0053FDB0
};
}

template<class T> struct CCountedPointer {
    T*    m_pObject;
    void* m_pCount;
    CCountedPointer(T* object);                                   // 0x0042AA29
    ~CCountedPointer();                                           // 0x00429160
    T* operator->() const { return m_pObject; }
};

namespace NUISystem {
struct CGUIElement {
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
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void AddAction(const CCountedPointer<CAction>& action);   // slot 66 (+0x108)
};
}

enum { UI_ACTION_PLAY_SOUND = 0xBBB };

struct CTCInventoryExperience {
    void AddSoundAction(NUISystem::CGUIElement* element, CCharString soundName);
};

void CTCInventoryExperience::AddSoundAction(NUISystem::CGUIElement* element, CCharString soundName)
{
    CCountedPointer<NUISystem::CAction> action(new NUISystem::CAction);
    action->ActionType = UI_ACTION_PLAY_SOUND;
    CWideString wideName = soundName.ToWideString();
    action->AddUIActionParam(new NUISystem::CActionParamString(wideName));
    element->AddAction(action);
}