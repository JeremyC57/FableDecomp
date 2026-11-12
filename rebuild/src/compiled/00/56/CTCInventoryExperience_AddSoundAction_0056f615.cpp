// CTCInventoryExperience::AddSoundAction(NUISystem::CComponent* element, CCharString soundName) -- gives a UI element a "play sound" action (type 3003) carrying the sound name as a wide-string parameter
// Leaf: ?AddSoundAction@CTCInventoryExperience@@AAEXPAVCComponent@NUISystem@@VCCharString@@@Z
// Binder respelling (agentFAM13, FAM12 notes): the scope-end release of the local action pointer calls 0x00429160, which is
//   ?ReleaseData@?$CCountedPointer@VCAction@NUISystem@@@@QAEXXZ (FableWin.pdb; the destructor is the 5-byte jmp COMDAT 0x00429B72):
//   per-TU opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY (the inline destructor expands to the out-of-line ReleaseData).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0056F615, length 138 bytes (`ret 8`; `this` unused). Identity: Ego_r.pdb / FableWin.pdb
//   ?AddSoundAction@CTCInventoryExperience@@AAEXPAVCComponent@NUISystem@@VCCharString@@@Z (private; landing 2026-11-03f respelled the
//   public CGUIElement stand-in and the AddUIActionParam(CActionParamString*) callee spelling).
// Frame-pointer size TU: optimize("y",off) + ("s",on) (`leave`, `pop ecx` after operator new).
// Callees: operator new @ 0x00BFEA1A, NUISystem::CAction::CAction @ 0x0042BE50 (manifest "CRumbleDef", propagated),
// CCountedPointer<NUISystem::CAction>(CAction*) @ 0x0042AA29 / its dtor @ 0x00429160 (manifest a propagated _Dest_val),
// CCharString::ToWideString @ 0x0099E480, CActionParamString(const CWideString&) @ 0x0054DAE0, CAction::AddParam(CActionParam*)
// @ 0x0053FDB0 (Ego_r / FableWin ?AddParam@CAction@NUISystem@@QAEXPAVCActionParam@2@@Z) (an inlined list push_back on UIActionParams, `this` = the action), element virtual slot 66 (+0x108,
// AddAction(const CCountedPointer<CAction>&)), ~CWideString @ 0x0099B510, ~CCharString @ 0x0099EAE0.
// Shape: the counted pointer's inline operator-> reads the object word ([ebp-0Ch]) for both the type store and the
// AddParam call; locals destroy in reverse order, then the by-value string argument.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CWideString { public:
    void* m_pStringData;
    ~CWideString();                                               // 0x0099B510
};
class CCharString { public:
    void* m_pStringData;
    ~CCharString();                                               // 0x0099EAE0
    CWideString ToWideString() const;                             // 0x0099E480
};

namespace NUISystem {
class CActionParam { public:                                         // +0x00 vptr, +0x04 type
    virtual ~CActionParam();
    long ActionParamType;
};
class CActionParamString : public CActionParam { public:             // sizeof 0x0C (operator new(0xC))
    unsigned char _body[0x04];
    CActionParamString(const CWideString& text);                  // 0x0054DAE0
};
class CAction { public:                                                  // retail 0x10 bytes
    long  ActionType;                                             // +0x00
    void* UIActionParams;                                         // +0x04 (STLport list)
    void* ThingActionParams;                                      // +0x08
    bool  Valid;                                                  // +0x0C
    CAction();                                                    // 0x0042BE50
    void AddParam(CActionParam* param);                           // 0x0053FDB0
};
}

#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"

namespace NUISystem {
class CComponent { public:
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

class CTCInventoryExperience {
    void AddSoundAction(NUISystem::CComponent* element, CCharString soundName);   // private (AAE)
};

void CTCInventoryExperience::AddSoundAction(NUISystem::CComponent* element, CCharString soundName)
{
    CCountedPointer<NUISystem::CAction> action(new NUISystem::CAction);
    action->ActionType = UI_ACTION_PLAY_SOUND;
    CWideString wideName = soundName.ToWideString();
    action->AddParam(new NUISystem::CActionParamString(wideName));
    element->AddAction(action);
}