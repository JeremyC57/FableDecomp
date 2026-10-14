#include <cstdio>
struct CCombatActionBase { virtual bool IsStrikeAction() { return strike; } bool strike; };
struct CCombatAction { CCombatActionBase* CreatureAction; void* m_pCount; };
struct CSeq {
    long CurrentActionIndex; CCombatAction* first; CCombatAction* last;
    unsigned long size() const { return (unsigned long)(last - first); }
    bool IsNextActionInSequenceStrikeAction() { long index = CurrentActionIndex; return index >= 0 && (unsigned long)index < size() && first[index].CreatureAction->IsStrikeAction(); }
};
int main()
{
    CCombatActionBase a, b; a.strike = false; b.strike = true; CCombatAction acts[2] = { { &a, 0 }, { &b, 0 } };
    CSeq s; s.first = acts; s.last = acts + 2;
    s.CurrentActionIndex = 1; if (!s.IsNextActionInSequenceStrikeAction()) { std::printf("008b0750_TEST FAIL\n"); return 1; }
    s.CurrentActionIndex = 0; if (s.IsNextActionInSequenceStrikeAction()) { std::printf("008b0750_TEST FAIL\n"); return 1; }
    s.CurrentActionIndex = 2; if (s.IsNextActionInSequenceStrikeAction()) { std::printf("008b0750_TEST FAIL\n"); return 1; }
    s.CurrentActionIndex = -1; if (s.IsNextActionInSequenceStrikeAction()) { std::printf("008b0750_TEST FAIL\n"); return 1; }
    std::printf("008b0750_TEST PASS\n");
    return 0;
}