// Behaviour test for CThingAICreature::DrawPreparePrimitivesForRendering (0x00833C00), the debug layout: the name line runs from
// the creature up 2.0, the name text sits at 2.0 and each brain text (debug text, then the state-group name) 0.3 higher; the
// building lines join home and work 1.5 up, grey when home == work, white to home and black to work otherwise; the navigation
// level text sits 0.7 above the focal point and the containing thing's name 0.2 above that. A model.
#include <cstdio>
struct Layout { float Heights[3]; int Count; unsigned char Colours[2]; int Lines; float NavZ[2]; };
Layout Build(bool debugText, bool groupName, const void* home, const void* work, float focalZ)
{
    Layout l; l.Count = 0; l.Lines = 0;
    float height = 2.0f;
    l.Heights[l.Count++] = height;
    if (debugText) { height += 0.3f; l.Heights[l.Count++] = height; }
    if (groupName) { height += 0.3f; l.Heights[l.Count++] = height; }
    if (home == work) { if (home) l.Colours[l.Lines++] = 0x7f; }
    else { if (home) l.Colours[l.Lines++] = 0xff; if (work) l.Colours[l.Lines++] = 0x00; }
    l.NavZ[0] = focalZ + 0.7f; l.NavZ[1] = l.NavZ[0] + 0.2f;
    return l;
}
int main()
{
    int a, b;
    Layout l = Build(true, true, &a, &a, 1.0f);
    bool ok = l.Count == 3 && l.Heights[0] == 2.0f && l.Heights[2] > 2.59f && l.Heights[2] < 2.61f && l.Lines == 1 && l.Colours[0] == 0x7f;
    Layout m = Build(false, true, &a, &b, 1.0f);
    ok = ok && m.Count == 2 && m.Heights[1] > 2.29f && m.Heights[1] < 2.31f && m.Lines == 2 && m.Colours[0] == 0xff && m.Colours[1] == 0;
    Layout n = Build(false, false, 0, 0, 0.0f);
    ok = ok && n.Count == 1 && n.Lines == 0 && n.NavZ[0] > 0.69f && n.NavZ[1] > 0.89f && n.NavZ[1] < 0.91f;
    std::printf(ok ? "00833c00_TEST PASS\n" : "00833c00_TEST FAIL\n");
    return ok ? 0 : 1;
}
