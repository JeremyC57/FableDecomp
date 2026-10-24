// behaviour test for 0x00B8D3C0 CEngineOcclusionManager::GetPrevFrameFlareTestResultFromRetrivalID
#include <cstdio>

struct HandlePair { unsigned int first; unsigned int second; };

static HandlePair* LowerBound(HandlePair* first, HandlePair* last, unsigned int key)
{
    int count = (int)(last - first);
    while (count > 0) {
        int half = count >> 1;
        HandlePair* middle = first + half;
        if (middle->first < key) { first = middle + 1; count -= half + 1; }
        else count = half;
    }
    return first;
}

struct Manager {
    HandlePair* Start;
    HandlePair* Finish;
    float Results[8];
    float GetPrevFrameFlareTestResult(unsigned int handle) { return Results[handle]; }
    float GetPrevFrameFlareTestResultFromRetrivalID(unsigned int id)
    {
        HandlePair* it = LowerBound(Start, Finish, id);
        if (it != Finish && it->first == id)
            return GetPrevFrameFlareTestResult(it->second);
        return 0.0f;
    }
};

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    HandlePair map[3];
    map[0].first = 10; map[0].second = 2;
    map[1].first = 20; map[1].second = 5;
    map[2].first = 30; map[2].second = 7;
    Manager m; m.Start = map; m.Finish = map + 3;
    for (int i = 0; i < 8; ++i) m.Results[i] = 0.125f * (float)(i + 1);
    if (!Near(m.GetPrevFrameFlareTestResultFromRetrivalID(20), 0.75f)) { std::printf("00b8d3c0_TEST FAIL\n"); return 1; }
    if (!Near(m.GetPrevFrameFlareTestResultFromRetrivalID(30), 1.0f))  { std::printf("00b8d3c0_TEST FAIL\n"); return 1; }
    if (!Near(m.GetPrevFrameFlareTestResultFromRetrivalID(15), 0.0f))  { std::printf("00b8d3c0_TEST FAIL\n"); return 1; }
    if (!Near(m.GetPrevFrameFlareTestResultFromRetrivalID(99), 0.0f))  { std::printf("00b8d3c0_TEST FAIL\n"); return 1; }
    std::printf("00b8d3c0_TEST PASS\n");
    return 0;
}