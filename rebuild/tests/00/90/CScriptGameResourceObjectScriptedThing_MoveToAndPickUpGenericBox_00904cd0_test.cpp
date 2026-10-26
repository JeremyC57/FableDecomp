#include <cstdio>
// behaviour test: MoveToAndPickUpGenericBox queues move-to (movement 3/5/1), turn-to-face and pick-up for a live box
class C3DVector { public: float X, Y, Z; };
struct CThing_Retail { bool m_dead; C3DVector pos; const C3DVector& GetPosition() const { return pos; } };
class CScriptThing { public: CThing_Retail* t; CThing_Retail* GetPThing() const { return t; } };
static int g_kinds[16], g_n = 0; static long g_movement = -1; static bool g_run = false; static float g_speed = 0;
struct CActionBase { int kind; virtual ~CActionBase() {} };
struct CActionMoveTo : CActionBase { CActionMoveTo(CThing_Retail*, long m, float p, bool a, bool run, bool, bool, bool, bool z)
    { kind = 1; g_movement = m; g_run = run; (void)p; (void)a; (void)z; } };
struct CActionTurnToFacePosition : CActionBase { CActionTurnToFacePosition(const C3DVector&, float s) { kind = 2; g_speed = s; } };
struct CActionPickUpGenericBox : CActionBase { CActionPickUpGenericBox(CThing_Retail*) { kind = 3; } };
class CTCScriptedControl { public: void AddAction(CActionBase* a) { g_kinds[g_n++] = a->kind; delete a; } };
static CTCScriptedControl g_control;
struct CThingWithControl : CThing_Retail { bool GetInterface(CTCScriptedControl** out, long) { *out = &g_control; return true; } };
struct CIntelligentPointer_Retail { CThingWithControl* item; CThingWithControl* GetPItem() { return item; } };
struct CScriptGameResourceObjectScriptedThing_Methods {
    CIntelligentPointer_Retail PThing;
    void MoveToAndPickUpGenericBox(const CScriptThing& box, long approach, bool run);
};
void CScriptGameResourceObjectScriptedThing_Methods::MoveToAndPickUpGenericBox(const CScriptThing& box, long approach, bool run)
{
    CIntelligentPointer_Retail* thing = &PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            CThing_Retail* boxThing = box.GetPThing();
            if (boxThing && !boxThing->m_dead) {
                CTCScriptedControl* control;
                thing->GetPItem()->GetInterface(&control, 0x1f);
                long movement;
                switch (approach) {
                case 0: movement = 3; break;
                case 1: movement = 5; break;
                default: movement = 1; break;
                }
                control->AddAction(new CActionMoveTo(boxThing, movement, 1.5f, true, run, true, true, true, false));
                control->AddAction(new CActionTurnToFacePosition(boxThing->GetPosition(), 0.013888889f));
                control->AddAction(new CActionPickUpGenericBox(boxThing));
            }
        }
    }
}
int main()
{
    CThingWithControl me; me.m_dead = false; CThing_Retail boxT; boxT.m_dead = false;
    CScriptGameResourceObjectScriptedThing_Methods s; s.PThing.item = &me; CScriptThing box; box.t = &boxT;
    s.MoveToAndPickUpGenericBox(box, 1, true);
    bool ok = g_n == 3 && g_kinds[0] == 1 && g_kinds[1] == 2 && g_kinds[2] == 3 && g_movement == 5 && g_run && g_speed > 0.0138f && g_speed < 0.0139f;
    s.MoveToAndPickUpGenericBox(box, 0, false); ok = ok && g_n == 6 && g_movement == 3 && !g_run;
    s.MoveToAndPickUpGenericBox(box, 7, false); ok = ok && g_n == 9 && g_movement == 1;
    boxT.m_dead = true; s.MoveToAndPickUpGenericBox(box, 0, false); ok = ok && g_n == 9;
    boxT.m_dead = false; me.m_dead = true; s.MoveToAndPickUpGenericBox(box, 0, false); ok = ok && g_n == 9;
    box.t = 0; me.m_dead = false; s.MoveToAndPickUpGenericBox(box, 0, false); ok = ok && g_n == 9;
    if (!ok) { std::printf("00904cd0_TEST FAIL\n"); return 1; }
    std::printf("00904cd0_TEST PASS\n");
    return 0;
}