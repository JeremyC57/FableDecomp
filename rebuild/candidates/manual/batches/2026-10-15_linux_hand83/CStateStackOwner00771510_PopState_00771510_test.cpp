// Behaviour test for 00771510 (CStateStackOwner_00771510::PopState): pops the top definition and re-applies the new top; false on an empty stack.
#include <cstdio>

struct CStateDef { long m_id; };

template <class T> struct CRetailVector {
    T* first;
    T* last;
    T* end_of_storage;
    bool empty() const { return first == last; }
    void pop_back() { --last; }
    T& back() { return *(last - 1); }
};

struct CStateStackOwner {
    CRetailVector<CStateDef*> m_states;
    CStateDef* m_applied;
    int        m_applyCalls;
    void SetupFromDef(CStateDef* def) { m_applied = def; ++m_applyCalls; }
    bool PopState()
    {
        if (!m_states.empty()) {
            m_states.pop_back();
            if (!m_states.empty())
                SetupFromDef(m_states.back());
            return true;
        }
        return false;
    }
};

int main()
{
    CStateDef a = { 1 }, b = { 2 };
    CStateDef* stack[2] = { &a, &b };
    CStateStackOwner o;
    o.m_states.first = stack; o.m_states.last = stack + 2; o.m_states.end_of_storage = stack + 2;
    o.m_applied = 0; o.m_applyCalls = 0;
    bool ok = o.PopState() && o.m_applied == &a && o.m_applyCalls == 1 && o.m_states.last == stack + 1;
    ok = ok && o.PopState() && o.m_applyCalls == 1 && o.m_states.last == stack;
    ok = ok && !o.PopState() && o.m_states.last == stack;
    if (!ok) { std::printf("00771510_TEST FAIL\n"); return 1; }
    std::printf("00771510_TEST PASS\n");
    return 0;
}
