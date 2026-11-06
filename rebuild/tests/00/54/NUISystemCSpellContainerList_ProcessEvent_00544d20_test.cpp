// Behaviour test for NUISystem::CSpellContainerList::ProcessEvent (0x00544D20): input is ignored until InputDelay has passed since
// the last input; rotate left / right wraps the spell index within the selected container's children; list up scrolls down and
// list down scrolls up; assign fires UNASSIGN_SPELL (210) for the slot the spell left (when AssignSpell returns one) then
// ASSIGN_SPELL (209), each with the flattened index (containers - selected - 1) * base spells + slot; unassign fires 210; left /
// back with the exclusive focus accepts every container and fires RESET_ASSIGNED_SPELLS (211). A state model.
#include <cstdio>
enum { LEFT = 2, BACK = 3, ROTATE_LEFT = 4, ROTATE_RIGHT = 5, LIST_UP = 6, LIST_DOWN = 7, ASSIGN = 8, UNASSIGN = 9 };
struct List {
    float Time, LastInputTime, InputDelay;
    unsigned long SpellIndex, SelectedChild, Containers, Children, BaseSpells;
    long MovedFrom; bool Exclusive; int Accepted, Scroll;
    long Actions[8]; unsigned long Params[8]; int NActions;
    void Action(long type, unsigned long param) { Actions[NActions] = type; Params[NActions] = param; ++NActions; }
    unsigned long Flat(unsigned long slot) { return (Containers - SelectedChild - 1) * BaseSpells + slot; }
    void ProcessEvent(int event)
    {
        if (Time - LastInputTime < InputDelay) return;
        switch (event) {
        case LEFT: case BACK:
            if (Exclusive) {
                Exclusive = false; LastInputTime = Time;
                for (unsigned long i = 0; i < Containers; ++i) ++Accepted;
                Action(211, 0);
            }
            break;
        case LIST_UP: --Scroll; LastInputTime = Time; break;
        case LIST_DOWN: ++Scroll; LastInputTime = Time; break;
        case ROTATE_LEFT: SpellIndex = SpellIndex == 0 ? Children - 1 : SpellIndex - 1; LastInputTime = Time; break;
        case ROTATE_RIGHT: SpellIndex = SpellIndex == Children - 1 ? 0 : SpellIndex + 1; LastInputTime = Time; break;
        case ASSIGN:
            if (MovedFrom != -1) Action(210, Flat(MovedFrom));
            Action(209, Flat(SpellIndex)); LastInputTime = Time;
            break;
        case UNASSIGN: Action(210, Flat(SpellIndex)); LastInputTime = Time; break;
        }
    }
};
int main()
{
    List l = { 1.0f, 0.9f, 0.25f, 0, 1, 3, 4, 4, -1, true, 0, 0, { 0 }, { 0 }, 0 };
    l.ProcessEvent(ROTATE_LEFT);
    bool ok = l.SpellIndex == 0;
    l.Time = 2.0f;
    l.ProcessEvent(ROTATE_LEFT);
    ok = ok && l.SpellIndex == 3;
    l.Time = 3.0f; l.ProcessEvent(ROTATE_RIGHT);
    ok = ok && l.SpellIndex == 0;
    l.Time = 4.0f; l.ProcessEvent(ROTATE_RIGHT); l.Time = 5.0f; l.ProcessEvent(ROTATE_RIGHT);
    ok = ok && l.SpellIndex == 2;
    l.Time = 6.0f; l.MovedFrom = 1; l.ProcessEvent(ASSIGN);
    ok = ok && l.NActions == 2 && l.Actions[0] == 210 && l.Params[0] == 5 && l.Actions[1] == 209 && l.Params[1] == 6;
    l.Time = 7.0f; l.ProcessEvent(UNASSIGN);
    ok = ok && l.Actions[2] == 210 && l.Params[2] == 6;
    l.Time = 8.0f; l.ProcessEvent(LIST_UP); l.Time = 9.0f; l.ProcessEvent(LIST_DOWN); l.Time = 10.0f; l.ProcessEvent(LIST_DOWN);
    ok = ok && l.Scroll == 1;
    l.Time = 11.0f; l.ProcessEvent(BACK);
    ok = ok && l.Accepted == 3 && l.Actions[3] == 211 && !l.Exclusive && l.NActions == 4;
    l.Time = 12.0f; l.ProcessEvent(LEFT);
    ok = ok && l.NActions == 4;
    std::printf(ok ? "00544d20_TEST PASS\n" : "00544d20_TEST FAIL\n");
    return ok ? 0 : 1;
}