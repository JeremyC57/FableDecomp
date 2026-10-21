// CGamePlayerInterface::IsEventGameAction(EGameAction action, const CInputEvent& event) const -- does any input bound to the action recognise the event
// Leaf: ?IsEventGameAction@CGamePlayerInterface_Methods@@QBE_NW4EGameAction@@ABVCInputEvent@@@Z
// Retail: Fable.exe @ 0x00445BD0, 84 bytes, `ret 8`; reached by a tail jmp from 0x00687CF3; Ego_r.pdb
// ?IsEventGameAction@CGamePlayerInterface@@QBE_NW4EGameAction@@ABVCInputEvent@@@Z.
// Body: over the Controls multimap range for the action -- end = Controls.UpperBound(action) (0x00447CE0), then from
// Controls.LowerBound(action) (0x00447C80): if entry.second->IsEventOfThisType(event) (virtual slot 3, +0xc) return true;
// return false.
// Layout: engine/CGamePlayerInterface.h Controls +0x7a8 (CVectorMap<EGameAction, CCountedPointer<const CInputTypeBase> >,
// opaque there); entries are 12 bytes {key, Data, Info}.
// Shape: /O2 (16-aligned entry); the upper bound fetched first as a named end, then the loop from the lower bound.
#include "engine/CGamePlayerInterface.h"
enum EGameAction { GAME_ACTION_NONE };
class CInputEvent;
class CInputTypeBase {
public:
    virtual ~CInputTypeBase();
    virtual void _slot01() const;
    virtual void _slot02() const;
    virtual bool IsEventOfThisType(const CInputEvent& event) const;   // slot 3 (+0xc)
};
template <class T> class CCountedPointer {
public:
    T*    Data;                                          // +0x00
    void* Info;                                          // +0x04
    T* operator->() const { return Data; }
};
struct CControlsEntry {                                  // std::pair<EGameAction, CCountedPointer<const CInputTypeBase> >
    EGameAction                             first;
    CCountedPointer<const CInputTypeBase>   second;
};
class CControlsMap {                                     // CVectorMap<EGameAction, CCountedPointer<const CInputTypeBase>, ...>
public:
    CControlsEntry* First;
    CControlsEntry* Last;
    CControlsEntry* EndOfStorage;
    const CControlsEntry* LowerBound(const EGameAction& key) const;   // 0x00447C80
    const CControlsEntry* UpperBound(const EGameAction& key) const;   // 0x00447CE0
};
struct CGamePlayerInterface_Methods : CGamePlayerInterface {
    const CControlsMap& GetControls() const { return *reinterpret_cast<const CControlsMap*>(Controls); }
    bool IsEventGameAction(EGameAction action, const CInputEvent& event) const;
};
bool CGamePlayerInterface_Methods::IsEventGameAction(EGameAction action, const CInputEvent& event) const
{
    const CControlsEntry* end = GetControls().UpperBound(action);
    for (const CControlsEntry* it = GetControls().LowerBound(action); it != end; ++it)
    {
        if (it->second->IsEventOfThisType(event))
            return true;
    }
    return false;
}