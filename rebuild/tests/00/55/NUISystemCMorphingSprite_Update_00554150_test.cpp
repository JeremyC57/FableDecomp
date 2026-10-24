#include <cstdio>
#include <list>
static int g_morph = 0, g_base = 0, g_deleted = 0;
struct CHandle { ~CHandle() { ++g_deleted; } };
struct CSprite { virtual ~CSprite() {} virtual void Update(float) { ++g_base; } };
struct CMorphingSprite : CSprite {
    long RenderCount; std::list<CHandle*> Handles; unsigned long SpriteGraphicIndex;
    void UpdateMorph(float) { ++g_morph; }
    virtual void Update(float dt)
    {
        UpdateMorph(dt);
        CSprite::Update(dt);
        if (RenderCount > 0) {
            while (Handles.size() > (unsigned long)(RenderCount - 1)) {
                delete Handles.front();
                Handles.pop_front();
            }
        }
        RenderCount = 0;
        SpriteGraphicIndex = 0;
    }
};
int main()
{
    CMorphingSprite s; int i;
    for (i = 0; i < 5; ++i) s.Handles.push_back(new CHandle);
    s.RenderCount = 3; s.SpriteGraphicIndex = 9;
    s.Update(0.1f);
    if (s.Handles.size() != 2 || g_deleted != 3 || s.RenderCount != 0 || s.SpriteGraphicIndex != 0 || g_morph != 1 || g_base != 1)
        { std::printf("00554150_TEST FAIL\n"); return 1; }
    s.Update(0.1f);
    if (s.Handles.size() != 2 || g_deleted != 3) { std::printf("00554150_TEST FAIL\n"); return 1; }
    while (!s.Handles.empty()) { delete s.Handles.front(); s.Handles.pop_front(); }
    std::printf("00554150_TEST PASS\n");
    return 0;
}