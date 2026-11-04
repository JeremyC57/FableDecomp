// Behaviour test for CQ_AwakeningTheOracleScript::CPretendGuard::Main (0x00D43190), g++ model: register the functor, a frame, then retry scripting a frame at a time, then idle until done.
#include <cstdio>
struct Script { int frames, doneAt, startAt, starts; bool registered, released, scripting;
    bool IsDone() const { return frames >= doneAt; }
    void Main()
    {
        registered = true;
        ++frames; if (IsDone()) return;
        if (scripting) released = true;
        while (!(++starts >= startAt)) { ++frames; if (IsDone()) return; }
        while (!IsDone()) ++frames;
    }
};
int main()
{
    Script a = { 0, 10, 3, 0, false, false, false }; a.Main();
    int ok = a.registered && a.starts == 3 && a.frames == 10;
    Script b = { 0, 1, 3, 0, false, false, false }; b.Main(); ok = ok && b.frames == 1 && b.starts == 0;
    Script c = { 0, 3, 100, 0, false, false, false }; c.Main(); ok = ok && c.frames == 3 && c.starts == 2;
    std::printf(ok ? "00d43190_TEST PASS\n" : "00d43190_TEST FAIL\n");
    return ok ? 0 : 1;
}
