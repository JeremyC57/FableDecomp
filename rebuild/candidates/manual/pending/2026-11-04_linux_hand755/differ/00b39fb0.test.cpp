// Behaviour test for CEnginePrimitiveAttachmentManager::UpdatePrimitives (0x00B39FB0), g++ model.
#include <cstdio>
#include <list>
struct Output { float Radius; float MaxDraw; bool Enable; };
struct Attached { unsigned long RefCount; long ParentType; bool SphereOk; float SphereRadius; Output* Out; int Updates; };
static float g_lastRadius;
static bool IsVisible(float radius, float maxDistance) { g_lastRadius = radius; return radius < maxDistance; }
static void UpdatePrimitives(std::list<Attached*>& prims)
{
    for (std::list<Attached*>::iterator it = prims.begin(); it != prims.end(); ) {
        std::list<Attached*>::iterator next = it; ++next;
        Attached* a = *it;
        if (a && a->RefCount > 1) {
            bool updated = false;
            if (a->ParentType == 2 && a->SphereOk) {
                float radius = a->SphereRadius + a->Out->Radius * 2.0f;
                if (IsVisible(radius, a->Out->MaxDraw)) { ++a->Updates; updated = true; }
            }
            a->Out->Enable = updated;
        } else {
            prims.erase(it);
        }
        it = next;
    }
}
int main()
{
    Output o1 = { 1.0f, 10.0f, true }, o2 = { 1.0f, 3.0f, true }, o3 = { 1.0f, 10.0f, true };
    Attached a1 = { 2, 2, true, 2.0f, &o1, 0 }, a2 = { 2, 2, true, 2.0f, &o2, 0 }, a3 = { 1, 2, true, 2.0f, &o3, 0 }, a4 = { 3, 1, true, 2.0f, &o3, 0 };
    std::list<Attached*> prims;
    prims.push_back(&a1); prims.push_back(&a2); prims.push_back(&a3); prims.push_back(&a4);
    UpdatePrimitives(prims);
    int ok = 1;
    ok &= a1.Updates == 1 && o1.Enable && g_lastRadius == 4.0f;
    ok &= a2.Updates == 0 && !o2.Enable;
    ok &= prims.size() == 3 && a4.Updates == 0 && !o3.Enable;
    std::printf(ok ? "00b39fb0_TEST PASS\n" : "00b39fb0_TEST FAIL\n");
    return ok ? 0 : 1;
}
