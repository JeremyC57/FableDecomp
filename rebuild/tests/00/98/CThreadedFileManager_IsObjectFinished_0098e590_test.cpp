// Behaviour test for 0x0098E590 CThreadedFileManager::IsObjectFinished(unsigned long) const: under the lock, false only
// for a known object whose LoadingStatus is not 4; unknown objects count as finished. The lock is always released.
#include <cstdio>

struct FileObject { unsigned long UID; int LoadingStatus; };
class Manager {
public:
    FileObject objects[3]; int count; mutable int lockDepth; mutable int locks;
    void Enter() const { ++lockDepth; ++locks; }
    void Leave() const { --lockDepth; }
    const FileObject* Find(unsigned long uid) const { for (int i = 0; i < count; ++i) if (objects[i].UID == uid) return &objects[i]; return 0; }
    bool IsObjectFinished(unsigned long uid) const
    {
        Enter();
        const FileObject* it = Find(uid);
        bool finished;
        if (it != 0 && it->LoadingStatus != 4) finished = false;
        else finished = true;
        Leave();
        return finished;
    }
};

int main()
{
    Manager m; m.count = 2; m.lockDepth = 0; m.locks = 0;
    m.objects[0].UID = 7; m.objects[0].LoadingStatus = 4;
    m.objects[1].UID = 9; m.objects[1].LoadingStatus = 2;
    if (!m.IsObjectFinished(7) || m.IsObjectFinished(9) || !m.IsObjectFinished(123)) { std::printf("0098e590_TEST FAIL\n"); return 1; }
    if (m.lockDepth != 0 || m.locks != 3) { std::printf("0098e590_TEST FAIL\n"); return 1; }
    std::printf("0098e590_TEST PASS\n");
    return 0;
}