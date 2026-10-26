#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawQuestInfo::AddTick(EGameAction action, bool ticked, float scale) -- look up the input assigned to the game action (primary, else secondary binding) and its display name, create a tick element for it, append it through AddElement and return its new ID (-1 when unbound or unnamed)
// Leaf: AddTick
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064FB47, length 218 bytes (+ padding), `ret 0Ch`.
// Callees: CTBaseSingleton<CUserProfileManager>::Get @ 0x0040D2A0, CUserProfileManager::GetAssignedInputForAction(
// EGameAction, bool, CActionInputControl*) @ 0x00408C90, CWideString::CWideString() @ 0x0099AED0 / ~CWideString
// @ 0x0099B510, GetActionName(const CActionInputControl&, CWideString&, bool) @ 0x00446720 (`__fastcall`),
// operator new @ 0x00BFEA1A (0x28 bytes), CQuestInfoElementTick::CQuestInfoElementTick(const CWideString&, bool, float)
// @ 0x00656C77, CCountedPointer<CQuestInfoElementTick>::CCountedPointer(T*) @ 0x00657176, the converting copy
// CCountedPointer<CQuestInfoElement>(const CCountedPointer<CQuestInfoElementTick>&) @ 0x00654C4A,
// CDrawQuestInfo::AddElement @ 0x0064A3ED, ~CCountedPointer<CQuestInfoElementTick> @ 0x00654C1E.
// Layout: CActionInputControl (PDB, 0x1c bytes): GameAction, ControllerType, KeyboardKey, XboxButton, MouseButton, then a
// C2DVector ControlDirection, all zeroed by its inline constructor (the floats with fldz: a size TU).
// Shape: a frame-pointer size TU as the landed AddTimer/AddBar; the two binding lookups form one `||` condition; the
// action name is a named local placed by VC in the dead `action` argument slot; the element ID is read through the
// counted local before its destructor; both failure paths share `or eax,-1`.
class C2DVector { public:
    float X, Y;
};
enum EGameAction { GAME_ACTION_NONE = 0 };
class CActionInputControl { public:
    long GameAction;                               // +0x00
    long ControllerType;                           // +0x04
    long KeyboardKey;                              // +0x08
    long XboxButton;                               // +0x0c
    long MouseButton;                              // +0x10
    C2DVector ControlDirection;                    // +0x14
    CActionInputControl() : GameAction(0), ControllerType(0), KeyboardKey(0), XboxButton(0), MouseButton(0)
    {
        ControlDirection.X = 0.0f;
        ControlDirection.Y = 0.0f;
    }
};
class CWideString { public:
    void* PStringData;
    CWideString();                                 // 0x0099AED0
    ~CWideString();                                // 0x0099B510
};
class CUserProfileManager { public:
    bool GetAssignedInputForAction(EGameAction action, bool primary, CActionInputControl* control);   // 0x00408C90
};
template <class T> struct CTBaseSingleton {
    static T& __fastcall Get();                    // 0x0040D2A0
};
bool __fastcall GetActionName(const CActionInputControl& control, CWideString& name, bool shortName);   // 0x00446720

namespace NPlayerGui {
template <class T> struct CCountedPointer {
    T*    m_pObject;
    void* m_pBlock;
    explicit CCountedPointer(T* object);
    template <class U> CCountedPointer(const CCountedPointer<U>& other);
    ~CCountedPointer();
    T* operator->() const { return m_pObject; }
};
struct CQuestInfoElement {                         // retail 0x1c bytes
    virtual void v00();
    long  ID;                                      // +0x04
    long  ScriptID;                                // +0x08
    float Scale;                                   // +0x0c
    void* PrimitiveHandles[3];                     // +0x10
};
struct CQuestInfoElementTick : CQuestInfoElement { // retail 0x28 bytes
    unsigned char _tick[0x28 - 0x1c];
    CQuestInfoElementTick(const CWideString& name, bool ticked, float scale);   // 0x00656C77
};
class CDrawQuestInfo { public:
    void AddElement(CCountedPointer<CQuestInfoElement> element);       // 0x0064A3ED
    long AddTick(EGameAction action, bool ticked, float scale);
};

long CDrawQuestInfo::AddTick(EGameAction action, bool ticked, float scale)
{
    CActionInputControl control;
    if (CTBaseSingleton<CUserProfileManager>::Get().GetAssignedInputForAction(action, true, &control)
        || CTBaseSingleton<CUserProfileManager>::Get().GetAssignedInputForAction(action, false, &control)) {
        CWideString name;
        if (GetActionName(control, name, true)) {
            CCountedPointer<CQuestInfoElementTick> element(new CQuestInfoElementTick(name, ticked, scale));
            AddElement(element);
            return element->ID;
        }
    }
    return -1;
}
}