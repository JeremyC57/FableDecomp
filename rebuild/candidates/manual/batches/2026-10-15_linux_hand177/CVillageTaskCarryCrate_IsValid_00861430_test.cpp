#include <cstdio>
// Behaviour model of CVillageTaskCarryCrate::IsValid: needs a crate stack; a pickup needs crates, a drop-off fewer than 2.
struct Stack { long crates; long GetNumCrates() const { return crates; } };
struct Task {
    Stack* stack; bool Pickup;
    bool IsValid()
    {
        if (stack == 0) return false;
        long crates = stack->GetNumCrates();
        if ((Pickup && crates > 0) || (!Pickup && crates < 2)) return true;
        return false;
    }
};
int main()
{
    Stack s; Task t; t.stack = &s;
    s.crates = 0; t.Pickup = true;  if (t.IsValid()) { std::printf("00861430_TEST FAIL\n"); return 1; }
    s.crates = 3;                   if (!t.IsValid()) { std::printf("00861430_TEST FAIL\n"); return 1; }
    t.Pickup = false;               if (t.IsValid()) { std::printf("00861430_TEST FAIL\n"); return 1; }
    s.crates = 1;                   if (!t.IsValid()) { std::printf("00861430_TEST FAIL\n"); return 1; }
    t.stack = 0;                    if (t.IsValid()) { std::printf("00861430_TEST FAIL\n"); return 1; }
    std::printf("00861430_TEST PASS\n");
    return 0;
}
