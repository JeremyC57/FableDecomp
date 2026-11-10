// Behaviour test for 0x00991670 CASoundManager::FlushLoadingQueue_00991670(): processes the sound bank's loading queue
// with no time limit (false, 0.0).
#include <cstdio>

class ResourceBank {
public:
    int calls; bool lastLimit; double lastEnd;
    void ProcessLoadingQueue(bool limitTime, double endTime) { ++calls; lastLimit = limitTime; lastEnd = endTime; }
};
class SoundManager {
public:
    ResourceBank* PSoundBank;
    virtual ~SoundManager() { }
    virtual void FlushLoadingQueue() { PSoundBank->ProcessLoadingQueue(false, 0.0); }
};

int main()
{
    ResourceBank bank; bank.calls = 0; bank.lastLimit = true; bank.lastEnd = 5.0;
    SoundManager m; m.PSoundBank = &bank;
    m.FlushLoadingQueue();
    if (bank.calls != 1 || bank.lastLimit || bank.lastEnd != 0.0) { std::printf("00991670_TEST FAIL\n"); return 1; }
    std::printf("00991670_TEST PASS\n");
    return 0;
}