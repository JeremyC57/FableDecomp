// Behaviour test for 0x00713D50 NTCScriptedControl::CActionFollowCreature::SetAsSneaking(bool): the row's logic over stub classes (C++03).
#include <stdio.h>
#include <stdlib.h>
#include <string>
static std::string trace;
static void check(bool v, int line) { if (!v) { printf("00713d50_TEST FAIL line %d\n", line); exit(1); } }
enum ETCInterfaceType { TCI_ENTITY_MODE_MANAGER = 49, TCI_STEALTH = 54 };
class CTCStealth { public:
    bool Sneaking;
    bool IsSneaking() const { return Sneaking; }
    void SetSneaking(bool sneaking) { Sneaking = sneaking; }
};
class CTCCreatureModeManager { public:
    bool AddMode(unsigned long mode) { check(mode == 7, __LINE__); trace += 'A'; return true; }
    void RemoveMode(unsigned long mode) { check(mode == 7, __LINE__); trace += 'M'; }
};
class CThing { public:
    bool has_stealth; CTCStealth stealth; CTCCreatureModeManager* modes;
    bool HasTC(ETCInterfaceType type) const { check(type == TCI_STEALTH, __LINE__); return has_stealth; }
    void AddTC(const char* name, bool activate, void* params) { check(std::string(name) == "CTCStealth" && !activate && !params, __LINE__); trace += 'C'; has_stealth = true; stealth.Sneaking = false; }
    void RemoveTC(ETCInterfaceType type) { check(type == TCI_STEALTH, __LINE__); trace += 'X'; has_stealth = false; }
    bool GetTC(CTCStealth** out) { trace += 'S'; if (!has_stealth) return false; *out = &stealth; return true; }
    bool GetTC(CTCCreatureModeManager** out) { trace += 'G'; if (!modes) return false; *out = modes; return true; }
};
class CActionFollowCreature { public:
    CThing thing; bool Sneaking; bool AddedStealthTC;
    CThing& GetParentThing() { return thing; }
    void SetAsSneaking(bool sneaking);
};

void CActionFollowCreature::SetAsSneaking(bool sneaking)
{
    if (Sneaking == sneaking)
        return;
    if (sneaking) {
        if (!GetParentThing().HasTC(TCI_STEALTH)) {
            GetParentThing().AddTC("CTCStealth", false, 0);
            AddedStealthTC = true;
        }
        CTCStealth* stealth;
        GetParentThing().GetTC(&stealth);
        if (!stealth->IsSneaking()) {
            stealth->SetSneaking(true);
            CTCCreatureModeManager* modes;
            if (GetParentThing().GetTC(&modes))
                modes->AddMode(7);
        }
        Sneaking = true;
    } else {
        CTCStealth* stealth;
        GetParentThing().GetTC(&stealth);
        stealth->SetSneaking(false);
        CTCCreatureModeManager* modes;
        if (GetParentThing().GetTC(&modes))
            modes->RemoveMode(7);
        if (AddedStealthTC) {
            GetParentThing().RemoveTC(TCI_STEALTH);
            AddedStealthTC = false;
        }
        Sneaking = false;
    }
}

int main()
{
    for (int bits = 0; bits != 64; ++bits) {
        CTCCreatureModeManager manager;
        CActionFollowCreature action;
        action.Sneaking = (bits & 1) != 0;
        bool request = (bits & 2) != 0;
        action.thing.has_stealth = (bits & 4) != 0 || !request;   // switching off needs the component (retail reads it unchecked)
        action.thing.stealth.Sneaking = (bits & 8) != 0;
        action.thing.modes = (bits & 16) ? &manager : 0;
        action.AddedStealthTC = (bits & 32) != 0;
        bool had = action.thing.has_stealth, was = action.thing.stealth.Sneaking, added = action.AddedStealthTC;
        trace = "";
        action.SetAsSneaking(request);
        std::string expected;
        if (action.Sneaking == request && (bits & 1) == (request ? 1 : 0)) {
            if (((bits & 1) != 0) == request) { check(trace.empty(), __LINE__); continue; }
        }
        if (request) {
            bool fresh = !had;
            if (fresh) expected += 'C';
            expected += 'S';
            if (fresh || !was) { expected += 'G'; if (action.thing.modes) expected += 'A'; }
            check(action.thing.stealth.Sneaking, __LINE__);
            check(action.AddedStealthTC == (fresh ? true : added), __LINE__);
        } else {
            expected += "SG";
            if (action.thing.modes) expected += 'M';
            if (added) expected += 'X';
            check(!action.thing.stealth.Sneaking, __LINE__);
            check(!action.AddedStealthTC, __LINE__);
            check(action.thing.has_stealth == !added, __LINE__);
        }
        check(action.Sneaking == request, __LINE__);
        check(trace == expected, __LINE__);
    }
    puts("00713d50_TEST PASS");
    return 0;
}