#include <cstdio>
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

class CInputEvent { public: int Code; };
CInputTypeBase::~CInputTypeBase() {}
void CInputTypeBase::_slot01() const {}
void CInputTypeBase::_slot02() const {}
bool CInputTypeBase::IsEventOfThisType(const CInputEvent&) const { return false; }
static int g_checked;
class CKeyInput : public CInputTypeBase {
public:
    int Code;
    bool IsEventOfThisType(const CInputEvent& e) const { ++g_checked; return e.Code == Code; }
};
const CControlsEntry* CControlsMap::LowerBound(const EGameAction& key) const { const CControlsEntry* p = First; while (p != Last && p->first < key) ++p; return p; }
const CControlsEntry* CControlsMap::UpperBound(const EGameAction& key) const { const CControlsEntry* p = First; while (p != Last && !(key < p->first)) ++p; return p; }
int main()
{
    static unsigned char raw[sizeof(CGamePlayerInterface)];
    CGamePlayerInterface_Methods* gpi = reinterpret_cast<CGamePlayerInterface_Methods*>(raw);
    CKeyInput k1, k2, k3; k1.Code = 10; k2.Code = 20; k3.Code = 30;
    static CControlsEntry e[4];
    e[0].first = (EGameAction)1; e[0].second.Data = &k1;
    e[1].first = (EGameAction)2; e[1].second.Data = &k2;
    e[2].first = (EGameAction)2; e[2].second.Data = &k3;
    e[3].first = (EGameAction)5; e[3].second.Data = &k1;
    CControlsMap* m = reinterpret_cast<CControlsMap*>(gpi->Controls); m->First = e; m->Last = e + 4; m->EndOfStorage = e + 4;
    CInputEvent ev;
    ev.Code = 30; bool ok = gpi->IsEventGameAction((EGameAction)2, ev) && g_checked == 2;
    ev.Code = 10; g_checked = 0; ok = ok && !gpi->IsEventGameAction((EGameAction)2, ev) && g_checked == 2;
    g_checked = 0; ok = ok && gpi->IsEventGameAction((EGameAction)1, ev) && g_checked == 1;
    g_checked = 0; ok = ok && !gpi->IsEventGameAction((EGameAction)3, ev) && g_checked == 0;
    ok = ok && (unsigned char*)m - raw == 0x7a8;
    std::printf(ok ? "00445bd0_TEST PASS\n" : "00445bd0_TEST FAIL\n");
    return ok ? 0 : 1;
}