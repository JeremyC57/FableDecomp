// Behaviour test for NUISystem::CKeyRedefiner::Redefine @ 0x00557D20 (copy of the model with stub profile/parent/sound).
#include <cstdio>
struct CActionInputControl { long GameAction, ControllerType, KeyboardKey, XboxButton, MouseButton; float DX, DY; };
static int g_redefines, g_lastIndex, g_sounds, g_notified; static long g_actionsSeen[4];
struct CUserProfileManager {
    void RedefineControlPassive(CActionInputControl* c, CActionInputControl* prev, long subType)
    { if (c->GameAction == prev->GameAction) g_actionsSeen[g_redefines++ & 3] = c->GameAction; g_lastIndex = (int)subType; }
};
static CUserProfileManager g_profile;
struct CGameState { void PlaySound(long, long, long) { ++g_sounds; } };
static CGameState* g_pGameState;
struct ActionNode { ActionNode* _M_next; ActionNode* _M_prev; long _M_data; };
struct CKeyRedefiner;
struct CComponent { long type; CComponent* parent; CComponent* GetParent() { return parent; } long GetType() { return type; } };
struct CRedefinerList : CComponent { void NotifyOfRedefinition(const CActionInputControl&, const CKeyRedefiner*) { ++g_notified; } };
struct CKeyRedefiner : CComponent {
    ActionNode* Actions; long SubType; CActionInputControl Definition;
    void Redefine(const CActionInputControl& control, bool notify)
    {
        CActionInputControl& current = Definition;
        if (Definition.ControllerType == control.ControllerType) {
            switch (Definition.ControllerType) {
            case 1: if (current.XboxButton != control.XboxButton) break; return;
            case 2: if (current.KeyboardKey != control.KeyboardKey) break; return;
            case 3: if (current.MouseButton != control.MouseButton) break; return;
            default: return;
            }
        }
        CActionInputControl previous = current;
        long index = 0;
        current = control;
        if (notify && GetParent() && GetParent()->GetType() == 0x28) { CRedefinerList* list = static_cast<CRedefinerList*>(GetParent()); list->NotifyOfRedefinition(control, this); }
        if (SubType >= 1 && SubType <= 9) index = SubType - 1;
        for (ActionNode* node = Actions->_M_next; node != Actions; node = node->_M_next) {
            previous.GameAction = current.GameAction = node->_M_data;
            g_profile.RedefineControlPassive(&current, &previous, index);
        }
        if (g_pGameState) { g_pGameState->PlaySound(0x10, 0, 0); g_pGameState->PlaySound(0x80, 0, 0); g_pGameState->PlaySound(0x40, 0, 0); }
        current.GameAction = Actions->_M_next->_M_data;
    }
};
int main()
{
    int bad = 0; CGameState gs; g_pGameState = &gs;
    ActionNode head, a, b; head._M_next = &a; a._M_next = &b; b._M_next = &head; a._M_data = 11; b._M_data = 12;
    CRedefinerList list; list.type = 0x28; list.parent = 0;
    CKeyRedefiner r; r.type = 1; r.parent = &list; r.Actions = &head; r.SubType = 3;
    CActionInputControl d = { 0, 2, 'A', 0, 0, 0, 0 }; r.Definition = d;
    CActionInputControl same = { 0, 2, 'A', 5, 0, 0, 0 };
    r.Redefine(same, true); if (g_redefines || g_sounds || g_notified) bad++;              // same key: nothing
    CActionInputControl other = { 0, 2, 'B', 0, 0, 0, 0 };
    r.Redefine(other, true);
    if (g_redefines != 2 || g_actionsSeen[0] != 11 || g_actionsSeen[1] != 12 || g_lastIndex != 2 || g_sounds != 3 || g_notified != 1) bad++;
    if (r.Definition.KeyboardKey != 'B' || r.Definition.GameAction != 11) bad++;
    CActionInputControl pad = { 0, 4, 0, 0, 0, 0, 0 }; r.Definition.ControllerType = 4; r.Redefine(pad, false); if (g_redefines != 2) bad++;   // other type equal: unchanged
    CActionInputControl mouse = { 0, 3, 0, 0, 1, 0, 0 }; r.SubType = 0; r.Redefine(mouse, false);
    if (g_redefines != 4 || g_lastIndex != 0 || g_notified != 1 || r.Definition.ControllerType != 3) bad++;
    if (bad) { std::printf("00557d20_TEST FAIL\n"); return 1; }
    std::printf("00557d20_TEST PASS\n");
    return 0;
}
