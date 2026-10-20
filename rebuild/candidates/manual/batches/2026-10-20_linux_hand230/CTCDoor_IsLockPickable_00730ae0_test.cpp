// Behaviour test for CTCDoor::IsLockPickable (0x00730AE0).
#include <cstdio>

struct Village { bool forbids; };
struct Owner { bool isVillager; Village* village; };
struct Door {
    bool hasDef, defPickable, CurrentOpen, Opening, Closing;
    bool hasC2, c2Active, c2Thing;
    bool hasOwnedTC; Owner* owner;
    bool IsLockPickable() const
    {
        if (!hasDef || !defPickable) return false;
        if (!Opening && !Closing && CurrentOpen) return false;
        if (hasC2 && c2Active && c2Thing) return false;
        if (hasOwnedTC && owner && owner->isVillager) {
            Village* v = owner->village;
            if (v && v->forbids) return false;
        }
        return true;
    }
};

int main()
{
    Village v = { true };
    Owner o = { true, &v };
    Door d = { true, true, false, false, false, false, false, false, false, 0 };
    if (!d.IsLockPickable()) { std::printf("00730ae0_TEST FAIL\n"); return 1; }
    d.CurrentOpen = true; if (d.IsLockPickable()) { std::printf("00730ae0_TEST FAIL\n"); return 1; }
    d.Opening = true; if (!d.IsLockPickable()) { std::printf("00730ae0_TEST FAIL\n"); return 1; }
    d.defPickable = false; if (d.IsLockPickable()) { std::printf("00730ae0_TEST FAIL\n"); return 1; }
    d.defPickable = true; d.hasC2 = true; d.c2Active = true; d.c2Thing = true;
    if (d.IsLockPickable()) { std::printf("00730ae0_TEST FAIL\n"); return 1; }
    d.c2Thing = false; d.hasOwnedTC = true; d.owner = &o;
    if (d.IsLockPickable()) { std::printf("00730ae0_TEST FAIL\n"); return 1; }
    v.forbids = false; if (!d.IsLockPickable()) { std::printf("00730ae0_TEST FAIL\n"); return 1; }
    std::printf("00730ae0_TEST PASS\n");
    return 0;
}
