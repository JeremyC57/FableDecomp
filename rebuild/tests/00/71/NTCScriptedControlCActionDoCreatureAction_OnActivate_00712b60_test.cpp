// Behaviour test for 0x00712B60 NTCScriptedControl::CActionDoCreatureAction::OnActivate: adopt a running action, else start ours (standalone copy of the logic)
#include <cstdio>
struct Act { long id; long GetActionID() const { return id; } };
struct Creature { Act* PCurrentAction; bool SetCurrentAction(Act* a) { PCurrentAction = a; return true; } };
struct Do { Creature* C; long ActionID; bool ActionSet; Act* Action;
    void OnActivate() { if (C->PCurrentAction) { ActionID = C->PCurrentAction->GetActionID(); ActionSet = false; return; } C->SetCurrentAction(Action); ActionID = C->PCurrentAction->GetActionID(); ActionSet = true; } };

int main()
{
    Act mine = { 7 }, other = { 3 }; Creature c; c.PCurrentAction = &other; Do d; d.C = &c; d.Action = &mine; d.ActionSet = true;
    d.OnActivate(); if (d.ActionID != 3 || d.ActionSet || c.PCurrentAction != &other) { std::printf("00712b60_TEST FAIL\n"); return 1; }
    c.PCurrentAction = 0; d.OnActivate(); if (d.ActionID != 7 || !d.ActionSet || c.PCurrentAction != &mine) { std::printf("00712b60_TEST FAIL\n"); return 1; }
    std::printf("00712b60_TEST PASS\n");
    return 0;
}