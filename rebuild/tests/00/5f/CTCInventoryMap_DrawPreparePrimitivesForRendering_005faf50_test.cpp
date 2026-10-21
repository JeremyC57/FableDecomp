// Behaviour test for CTCInventoryMap::DrawPreparePrimitivesForRendering (0x005FAF50): a pending quest drop queues event 9
// and draws nothing; otherwise components are updated each frame, rendered after the first, and the backdrop alpha is the
// accumulated fade time over the fade length, clamped to 1.
#include <cstdio>

struct Map {
    long questToBeDropped; bool open; bool infoActive; bool firstFrame; float accum;
    int events; int updates; int renders; float lastAlpha;
    void Draw(float deltaTime, float fadeTime, int components)
    {
        if (questToBeDropped > 0) {
            ++events;
        } else if (open && !infoActive) {
            for (int i = 0; i < components; ++i) {
                ++updates;
                if (firstFrame) firstFrame = false; else ++renders;
            }
            accum += deltaTime;
            float alpha = accum / fadeTime;
            if (alpha > 1.0f) alpha = 1.0f;
            lastAlpha = alpha;
        }
    }
};
int main()
{
    Map m; m.questToBeDropped = 1; m.open = true; m.infoActive = false; m.firstFrame = true; m.accum = 0.0f;
    m.events = 0; m.updates = 0; m.renders = 0; m.lastAlpha = -1.0f;
    m.Draw(0.5f, 2.0f, 3);
    if (m.events != 1 || m.updates != 0) { std::printf("005faf50_TEST FAIL\n"); return 1; }
    m.questToBeDropped = 0;
    m.Draw(0.5f, 2.0f, 3);
    if (m.updates != 3 || m.renders != 2 || m.lastAlpha != 0.25f) { std::printf("005faf50_TEST FAIL\n"); return 1; }
    m.Draw(4.0f, 2.0f, 3);
    if (m.lastAlpha != 1.0f) { std::printf("005faf50_TEST FAIL\n"); return 1; }
    std::printf("005faf50_TEST PASS\n");
    return 0;
}