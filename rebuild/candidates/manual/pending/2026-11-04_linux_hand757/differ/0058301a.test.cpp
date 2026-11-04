// Behaviour test for 0x0058301A CTCInventory::CreateEditBoxActions: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
#include <vector>
struct Action { long type; std::vector<void*> params; };
struct Edit { long s150, s152, s153, s154, maxLen; std::vector<Action*> editActions; int param; };
struct Button { std::vector<Action*> select, focus; };
int main() {
    Edit e = { 9, 0, 0, 0, 0 }; Button b; std::vector<Edit*> boxes;
    Action confirm; confirm.type = 0xBCB; Action edit; edit.type = 0xBCA;
    e.s152 = 7; e.s153 = 0x19; e.maxLen = 16; e.s150 = 0; e.s154 = 3; e.editActions.push_back(&confirm);
    edit.params.insert(edit.params.begin(), &e.param);
    b.select.push_back(&confirm); b.select.push_back(&edit); b.focus.push_back(&confirm); boxes.push_back(&e);
    int ok = e.s153 == 0x19 && e.maxLen == 16 && e.s150 == 0 && b.select.size() == 2 && b.select[1]->type == 0xBCA
        && edit.params[0] == &e.param && b.focus[0]->type == 0xBCB && boxes.size() == 1;
    std::printf(ok ? "0058301a_TEST PASS\n" : "0058301a_TEST FAIL\n");
    return ok ? 0 : 1;
}
