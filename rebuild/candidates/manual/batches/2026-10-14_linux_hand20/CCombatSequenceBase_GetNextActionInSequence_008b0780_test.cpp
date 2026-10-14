#include <cstdio>
struct CThing { unsigned char m_flags91; bool incap; };
static bool __fastcall IsIncapacitated(CThing* t) { return t->incap; }
struct CCombatActionBase { int reinit; void ReinitialiseActionID() { ++reinit; } };
struct CCombatAction { CCombatActionBase* CreatureAction; void* m_pCount; };
struct CSeq {
    virtual bool IsInterruptable() { return interruptable; } virtual bool IsMeleeSequence() { return melee; }
    bool interruptable, melee; long CurrentActionIndex; CCombatAction* first; CCombatAction* last; CThing* Target;
    unsigned long size() const { return (unsigned long)(last - first); }
    CCombatActionBase* GetNextActionInSequence()
    {
        CThing* target = Target;
        if (target && (target->m_flags91 & 1) == 0 && IsIncapacitated(target) && IsMeleeSequence() && IsInterruptable()) return 0;
        long index = CurrentActionIndex;
        if (index >= 0 && (unsigned long)index < size()) { CCombatActionBase* a = first[index].CreatureAction; if (a) a->ReinitialiseActionID(); return a; }
        return 0;
    }
};
int main()
{
    CCombatActionBase a = { 0 }; CCombatAction acts[2] = { { &a, 0 }, { 0, 0 } }; CThing t = { 0, true };
    CSeq s; s.first = acts; s.last = acts + 2; s.Target = &t; s.interruptable = true; s.melee = true; s.CurrentActionIndex = 0;
    if (s.GetNextActionInSequence() != 0) { std::printf("008b0780_TEST FAIL\n"); return 1; }
    s.melee = false;
    if (s.GetNextActionInSequence() != &a || a.reinit != 1) { std::printf("008b0780_TEST FAIL\n"); return 1; }
    s.melee = true; t.m_flags91 = 1;
    if (s.GetNextActionInSequence() != &a || a.reinit != 2) { std::printf("008b0780_TEST FAIL\n"); return 1; }
    s.CurrentActionIndex = 1; if (s.GetNextActionInSequence() != 0) { std::printf("008b0780_TEST FAIL\n"); return 1; }
    s.CurrentActionIndex = 5; if (s.GetNextActionInSequence() != 0) { std::printf("008b0780_TEST FAIL\n"); return 1; }
    s.Target = 0; s.CurrentActionIndex = 0; if (s.GetNextActionInSequence() != &a) { std::printf("008b0780_TEST FAIL\n"); return 1; }
    std::printf("008b0780_TEST PASS\n");
    return 0;
}
