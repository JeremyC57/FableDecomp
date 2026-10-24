#include <cstdio>
#include <cstdlib>

// Model of 0x00409090 CUserProfileManager::GetMaxAntialiasing: fetch the supported levels; with no config cap
// (-1) return the last index, otherwise the index of the last level not above the cap (levels ascend), 0 if none.
struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CLongVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CLongVector() { if (_M_start) std::free(_M_start); }
    long* begin() { return _M_start; }
    long* end() { return _M_finish; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
};
static const long g_supported[] = { 0, 2, 4, 8 };
struct CDisplayManager {
    void GetSupportedAntialiasingLevels(CLongVector& levels)
    {
        levels._M_start = (long*)std::malloc(sizeof(g_supported));
        for (int i = 0; i < 4; ++i)
            levels._M_start[i] = g_supported[i];
        levels._M_finish = levels._M_end_of_storage = levels._M_start + 4;
    }
};
struct CSystemManager {
    CDisplayManager* PDisplayManager;
};
static CDisplayManager g_display;
static CSystemManager g_system = { &g_display };
static CSystemManager* GFGetSystemManager() { return &g_system; }
static long g_ConfigLevel = -1;
struct CUserProfileManager {
    unsigned long GetMaxAntialiasing() const
    {
        CLongVector levels;
        CSystemManager* system = GFGetSystemManager();
        system->PDisplayManager->GetSupportedAntialiasingLevels(levels);
        if (g_ConfigLevel != -1) {
            long index = -1;
            for (long* it = levels.begin(); it != levels.end(); ++it) {
                if (*it > g_ConfigLevel)
                    break;
                ++index;
            }
            if (index != -1)
                return index;
            return 0;
        }
        return levels.size() - 1;
    }
};

int main()
{
    CUserProfileManager manager;
    const long caps[] = { -1, 4, 5, 0, -2, 100, 1 };
    const unsigned long expected[] = { 3, 2, 2, 0, 0, 3, 0 };
    for (int i = 0; i < 7; ++i) {
        g_ConfigLevel = caps[i];
        if (manager.GetMaxAntialiasing() != expected[i]) {
            std::printf("00409090_TEST FAIL\n");
            return 1;
        }
    }
    std::printf("00409090_TEST PASS\n");
    return 0;
}