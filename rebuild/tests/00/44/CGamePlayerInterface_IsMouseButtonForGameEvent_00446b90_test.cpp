// Behaviour test for 0x00446b90 CGamePlayerInterface::IsMouseButtonForGameEvent(EMouseButtonControl, EGameAction): find the action in the sorted
// Controls map (lower bound + key check) and ask the bound input type; false when the action is unbound.
#include <cstdio>
enum EGameAction { GA_0 = 0 };
enum EMouseButtonControl { BTN_0 = 0 };
class CInputTypeBase { public:
    int Button;
    explicit CInputTypeBase(int b) : Button(b) { }
    virtual ~CInputTypeBase() { }
    virtual bool IsInputTypeWithMouseButton(EMouseButtonControl b) const { return Button == (int)b; }
};
struct CCountedPointer_InputType { const CInputTypeBase* m_pObject; void* m_pCount; };
struct SControlEntry { EGameAction first; CCountedPointer_InputType second; };
struct CControlsMap {
    SControlEntry* _M_start; SControlEntry* _M_finish; SControlEntry* _M_end_of_storage;
    const SControlEntry* LowerBound(const EGameAction& key) const
    {
        const SControlEntry* it = _M_start;
        while (it != _M_finish && it->first < key) ++it;
        return it;
    }
    const SControlEntry* end() const { return _M_finish; }
    const SControlEntry* Find(const EGameAction& key) const
    {
        const SControlEntry* it = LowerBound(key);
        const SControlEntry* last = end();
        return (it == last || key < it->first) ? last : it;
    }
};
class CGamePlayerInterface { public:
    CControlsMap Controls;
    bool IsMouseButtonForGameEvent(EMouseButtonControl button, EGameAction action) const
    {
        const SControlEntry* it = Controls.Find(action);
        if (it != Controls.end())
            return it->second.m_pObject->IsInputTypeWithMouseButton(button);
        return false;
    }
};
int main()
{
    CInputTypeBase a(3), b(5);
    SControlEntry entries[2];
    entries[0].first = (EGameAction)2; entries[0].second.m_pObject = &a; entries[0].second.m_pCount = 0;
    entries[1].first = (EGameAction)9; entries[1].second.m_pObject = &b; entries[1].second.m_pCount = 0;
    CGamePlayerInterface gpi;
    gpi.Controls._M_start = entries; gpi.Controls._M_finish = entries + 2; gpi.Controls._M_end_of_storage = entries + 2;
    bool ok = true;
    ok = ok && gpi.IsMouseButtonForGameEvent((EMouseButtonControl)5, (EGameAction)9);
    ok = ok && !gpi.IsMouseButtonForGameEvent((EMouseButtonControl)3, (EGameAction)9);
    ok = ok && gpi.IsMouseButtonForGameEvent((EMouseButtonControl)3, (EGameAction)2);
    ok = ok && !gpi.IsMouseButtonForGameEvent((EMouseButtonControl)3, (EGameAction)4);    // unbound action between keys
    ok = ok && !gpi.IsMouseButtonForGameEvent((EMouseButtonControl)5, (EGameAction)12);   // past the last key
    ok = ok && !gpi.IsMouseButtonForGameEvent((EMouseButtonControl)3, (EGameAction)1);    // before the first key
    if (!ok) { std::printf("00446b90_TEST FAIL\n"); return 1; }
    std::printf("00446b90_TEST PASS\n");
    return 0;
}