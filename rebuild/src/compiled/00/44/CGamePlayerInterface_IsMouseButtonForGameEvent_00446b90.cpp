// CGamePlayerInterface::IsMouseButtonForGameEvent(EMouseButtonControl button, EGameAction action) const -- true when the input type bound to the game action is that mouse button
// Leaf: ?IsMouseButtonForGameEvent@CGamePlayerInterface_Methods@@
// Retail: Fable.exe @ 0x00446B90, 72 bytes (+ int3 padding), `ret 8`; speed TU (no frame, no pragma).
// Name: the Ego_r.pdb name set holds ?IsMouseButtonForGameEvent@CGamePlayerInterface@@QBE_NW4EMouseButtonControl@@W4EGameAction@@@Z
// next to ?IsButtonForGameEvent@... (the twin at 0x00446B40); the donor addresses differ, the shape fits: the CInputTypeBase
// virtual called is slot 7 (+0x1c) = IsInputTypeWithMouseButton (CInputTypeXboxPadButtonEvent's entry is 0x00445650, landed), and the
// callers CDrawAbilityButtons::UpdateWeaponAbilities (0x0064B1AC) and CDrawTargetInfo::Draw (0x0064B8B3) call the pad twin and this one back to back (three sites in each).
// Callees: CVectorMap<EGameAction, CCountedPointer<const CInputTypeBase> >::LowerBound @ 0x00447C80 (thiscall, key by address;
// the catalog label names the identical CVectorMap<long, CIntelligentPointer<CThing> > body), virtual slot 7.
// Layout: engine/CGamePlayerInterface.h places Controls at +0x7a8 (the retail offset agrees; the header keeps it opaque, so the
// map is modelled locally as the retail STLport vector {start, finish, end_of_storage} plus Compare/Dirty). A map entry is
// pair<EGameAction, CCountedPointer<const CInputTypeBase> > = 12 bytes, the input type object at +4.
// Shape: the map's inline Find (lower bound, then end() when it hit end or key < entry key -- the signed `jge`), compared
// against the re-read end(); hit -> the virtual call's bool is returned; miss -> `xor al,al`.
#include "engine/CGamePlayerInterface.h"

enum EGameAction { GAME_ACTION_FORCE_DWORD = 0x7fffffff };
enum EXboxControllerButton { XBOX_CONTROLLER_BUTTON_FORCE_DWORD = 0x7fffffff };
enum EMouseButtonControl { MOUSE_BUTTON_CONTROL_FORCE_DWORD = 0x7fffffff };

struct CInputTypeBase {
    virtual ~CInputTypeBase();                                                      // slot 0
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual bool IsInputTypeWithButton(EXboxControllerButton button) const;          // slot 6 (+0x18)
    virtual bool IsInputTypeWithMouseButton(EMouseButtonControl button) const;       // slot 7 (+0x1c)
};

struct SCountBlock;
struct CCountedPointer_InputType {              // CCountedPointer<const CInputTypeBase>
    const CInputTypeBase* m_pObject;            // +0
    SCountBlock*          m_pCount;             // +4
};

struct SControlEntry {                          // pair<EGameAction, CCountedPointer<const CInputTypeBase> >
    EGameAction               first;            // +0x00
    CCountedPointer_InputType second;           // +0x04
};

struct CControlsMap {                           // CVectorMap<EGameAction, CCountedPointer<const CInputTypeBase>, CKeyPairCompareLess<...> >
    SControlEntry* _M_start;                    // +0x00
    SControlEntry* _M_finish;                   // +0x04
    SControlEntry* _M_end_of_storage;           // +0x08
    bool           Compare;                     // +0x0c CKeyPairCompareLess (empty)
    bool           Dirty;                       // +0x0d

    const SControlEntry* LowerBound(const EGameAction& key) const;                  // 0x00447C80
    const SControlEntry* end() const { return _M_finish; }
    const SControlEntry* Find(const EGameAction& key) const
    {
        const SControlEntry* it = LowerBound(key);
        const SControlEntry* last = end();
        return (it == last || key < it->first) ? last : it;
    }
};

struct CGamePlayerInterface_Methods : CGamePlayerInterface {
    const CControlsMap& GetControls() const { return *reinterpret_cast<const CControlsMap*>(Controls); }
    bool IsMouseButtonForGameEvent(EMouseButtonControl button, EGameAction action) const;
};

bool CGamePlayerInterface_Methods::IsMouseButtonForGameEvent(EMouseButtonControl button, EGameAction action) const
{
    const SControlEntry* it = GetControls().Find(action);
    if (it != GetControls().end())
        return it->second.m_pObject->IsInputTypeWithMouseButton(button);
    return false;
}