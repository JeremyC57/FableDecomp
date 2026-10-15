#include <cstdio>
struct Cam { int id; };
struct Type { float fade; float PeekFadeEnd() { return fade; } };
struct Prim {
    Prim* m_pNext; bool active; int calls; bool lastInside; unsigned int lastFrame; Type* lastType; float lastTime;
    bool IsActive() { return active; }
    void RenderUpdate(const Cam&, bool inside, unsigned int frame, Type* t, float time) { ++calls; lastInside = inside; lastFrame = frame; lastType = t; lastTime = time; }
};
struct Coll {
    Type* m_pType; Prim* m_pFirstPrimitive; bool m_bEnabled;
    void RenderUpdate(const Cam& camera, float nearD, float farD, unsigned int frame, float time)
    {
        if (m_bEnabled && m_pType->PeekFadeEnd() > nearD) {
            bool inside = m_pType->PeekFadeEnd() > farD;
            for (Prim* p = m_pFirstPrimitive; p; p = p->m_pNext) if (p->IsActive()) p->RenderUpdate(camera, inside, frame, m_pType, time);
        }
    }
};
int main()
{
    Type t; t.fade = 50.0f; Cam c; c.id = 1;
    Prim b = { 0, false, 0, false, 0, 0, 0 }, a = { &b, true, 0, false, 0, 0, 0 };
    Coll col; col.m_pType = &t; col.m_pFirstPrimitive = &a; col.m_bEnabled = true;
    col.RenderUpdate(c, 10.0f, 40.0f, 7, 2.5f);
    bool fail = a.calls != 1 || !a.lastInside || a.lastFrame != 7 || a.lastType != &t || a.lastTime != 2.5f || b.calls != 0;
    col.RenderUpdate(c, 10.0f, 60.0f, 8, 2.5f); if (a.calls != 2 || a.lastInside) fail = true;
    col.RenderUpdate(c, 50.0f, 60.0f, 9, 2.5f); if (a.calls != 2) fail = true;
    col.m_bEnabled = false; col.RenderUpdate(c, 10.0f, 60.0f, 9, 2.5f); if (a.calls != 2) fail = true;
    if (fail) { std::printf("00bdd860_TEST FAIL\n"); return 1; }
    std::printf("00bdd860_TEST PASS\n");
    return 0;
}
