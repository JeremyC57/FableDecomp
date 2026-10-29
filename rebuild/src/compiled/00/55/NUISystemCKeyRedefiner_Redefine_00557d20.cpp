// NUISystem::CKeyRedefiner::Redefine(const CActionInputControl& control, bool notify) -- unless the new control is the same key/button for the same controller type, make it this redefiner's definition, optionally tell the parent redefiner list (type 0x28) about it, hand every action of this redefiner to the user profile as a passive redefinition (old control, sub-type index), play the three confirmation sounds and leave the definition on the first action
// Leaf: Redefine
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00557D20, length 390 bytes, `ret 8`.
// Callees: the component's virtual slots 52 (parent) and 65 (type), CRedefinerList::NotifyOfRedefinition(const
//   CActionInputControl&, const CKeyRedefiner*) @ 0x00556980 (landed name), CTBaseSingleton<CUserProfileManager>::Get
//   @ 0x0040D2A0 (`__fastcall`), CUserProfileManager::RedefineControlPassive(CActionInputControl*, CActionInputControl*,
//   long) @ 0x00407CD0, the game state global @ 0x013B8394 virtual slot 46 (sound, 0, 0).
// Layout: Ego_r.pdb NUISystem::CKeyRedefiner with the retail 4-byte list sentinel: Actions (list<EGameAction> node) +0x19C,
//   SubType +0x1A0, Definition (CActionInputControl, 28 bytes: GameAction, ControllerType, KeyboardKey, XboxButton,
//   MouseButton, ControlDirection) +0x1A4. No engine header for the class; modelled locally.
// Shape: the definition is reached through a `CActionInputControl& current` (its address hoisted into ebx); the
//   "unchanged" test is a switch on the controller type whose cases are `if (differs) break; return;` (separate return
//   epilogues); C2DVector's user copy constructor makes the copy-out memberwise while the assignment stays a `rep movsd`;
//   `long index = 0` sits between them so it lives in a frame word; the parent list is named before the notify call;
//   in the action walk `previous.GameAction = current.GameAction = action` stores the member first.
namespace NUISystem {
struct C2DVector {
    float X, Y;
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
};
struct CActionInputControl {
    long      GameAction;                          // +0x00
    long      ControllerType;                      // +0x04
    long      KeyboardKey;                         // +0x08
    long      XboxButton;                          // +0x0c
    long      MouseButton;                         // +0x10
    C2DVector ControlDirection;                    // +0x14
};
struct CUserProfileManager {
    void RedefineControlPassive(CActionInputControl* control, CActionInputControl* previous, long subType);   // 0x00407CD0
};
} class CUserProfileManager : public NUISystem::CUserProfileManager { public: static CUserProfileManager& __fastcall Get(); }; namespace NUISystem {   // 0x0040D2A0 ?Get@CUserProfileManager@@SIAAV1@XZ
struct CGameState {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045();
    virtual void PlaySound(long sound, long a, long b);                  // slot 46 (+0xb8)
};
extern CGameState* g_pGameState;                   // 0x013B8394
struct ActionNode {
    ActionNode* _M_next;
    ActionNode* _M_prev;
    long        _M_data;
};
class CKeyRedefiner;
class CComponent { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual CComponent* GetParent();                                     // slot 52 (+0xd0)
    virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060();
    virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual long GetType();                                              // slot 65 (+0x104)
};
class CRedefinerList : public CComponent { public:
    void NotifyOfRedefinition(const CActionInputControl& control, const CKeyRedefiner* source);   // 0x00556980
};
class CKeyRedefiner : public CComponent {
public:
    unsigned char       _pad004[0x19c - 0x04];
    ActionNode*         Actions;                   // +0x19c
    long                SubType;                   // +0x1a0
    CActionInputControl Definition;                // +0x1a4
    void Redefine(const CActionInputControl& control, bool notify);
};

void CKeyRedefiner::Redefine(const CActionInputControl& control, bool notify)
{
    CActionInputControl& current = Definition;
    if (Definition.ControllerType == control.ControllerType) {
        switch (Definition.ControllerType) {
        case 1:
            if (current.XboxButton != control.XboxButton)
                break;
            return;
        case 2:
            if (current.KeyboardKey != control.KeyboardKey)
                break;
            return;
        case 3:
            if (current.MouseButton != control.MouseButton)
                break;
            return;
        default:
            return;
        }
    }
    CActionInputControl previous = current;
    long index = 0;
    current = control;
    if (notify && GetParent() && GetParent()->GetType() == 0x28) {
        CRedefinerList* list = static_cast<CRedefinerList*>(GetParent());
        list->NotifyOfRedefinition(control, this);
    }
    if (SubType >= 1 && SubType <= 9)
        index = SubType - 1;
    for (ActionNode* node = Actions->_M_next; node != Actions; node = node->_M_next) {
        previous.GameAction = current.GameAction = node->_M_data;
        ::CUserProfileManager::Get().RedefineControlPassive(&current, &previous, index);
    }
    if (g_pGameState) {
        g_pGameState->PlaySound(0x10, 0, 0);
        g_pGameState->PlaySound(0x80, 0, 0);
        g_pGameState->PlaySound(0x40, 0, 0);
    }
    current.GameAction = Actions->_M_next->_M_data;
}
}