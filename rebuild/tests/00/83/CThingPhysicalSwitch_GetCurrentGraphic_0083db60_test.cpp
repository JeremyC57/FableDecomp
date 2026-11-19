// Behaviour test for CThingPhysicalSwitch::GetCurrentGraphic (0x0083DB60): a copy of the row's logic on stand-alone types -- the
// definition (looked up through a counted def pointer that is released before the member is returned) gives the graphic by state.
#include <stdio.h>
struct Graphic { int id; };
struct SwitchDef { long refs; Graphic UntriggeredGraphic, TriggeredGraphic; };
static SwitchDef g_def = { 1, { 10 }, { 20 } };
static int g_lookups;
struct DefPtr { SwitchDef* Object; DefPtr() : Object(0) {} ~DefPtr() { if (Object) --Object->refs; } };
static void GetDef(long, DefPtr* out) { ++g_lookups; out->Object = &g_def; ++g_def.refs; }
struct Switch {
    bool Triggered; long DefGlobalIndex;
    const SwitchDef& PeekDefinition() const { DefPtr def; GetDef(DefGlobalIndex, &def); return *def.Object; }
    const Graphic& GetCurrentGraphic() const
    {
        if (Triggered)
            return PeekDefinition().TriggeredGraphic;
        return PeekDefinition().UntriggeredGraphic;
    }
};
int main()
{
    int fails = 0;
    Switch s = { false, 5 };
    if (s.GetCurrentGraphic().id != 10) ++fails;
    s.Triggered = true;
    if (s.GetCurrentGraphic().id != 20) ++fails;
    if (g_lookups != 2 || g_def.refs != 1) ++fails;   // one lookup per call, every reference released
    printf(fails ? "0083db60_TEST FAIL\n" : "0083db60_TEST PASS\n");
    return fails ? 1 : 0;
}