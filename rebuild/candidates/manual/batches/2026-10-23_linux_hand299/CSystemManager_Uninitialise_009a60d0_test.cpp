#include <cstdio>
// Behaviour model of CSystemManager::Uninitialise: counted-pointer releases, table walk, window teardown, count drop.
static int destroyed = 0;
static void DestroyObject(void*) { ++destroyed; }
struct SCountBlock { long RefCount; void (*Destroy)(void*); void* Object; };
struct CPtr {
    int* Data; SCountBlock* Info;
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) { Info->Destroy(Info->Object); delete Info; }
        }
        Info = 0; Data = 0;
    }
};
static CPtr Make(int* obj, long refs) { CPtr p; p.Data = obj; p.Info = new SCountBlock; p.Info->RefCount = refs; p.Info->Destroy = DestroyObject; p.Info->Object = obj; return p; }
struct Entry { CPtr Object; long Key; long Flags; };
struct Manager {
    bool Valid; CPtr Input, Font, Display; void* Window; bool WindowValid; bool Attached; int windowsDestroyed; int removedCallbacks;
};
static unsigned char initCount = 2;
static void Uninitialise(Manager& m, Entry* entries, unsigned long count)
{
    if (m.Valid == true) {
        m.Input.Release();
        m.Font.Release();
        for (unsigned long i = 0; i != count; ++i) { entries[i].Object.Release(); entries[i].Flags = 0; }
        if (m.Display.Data) ++m.removedCallbacks;
        m.Display.Release();
        if (m.Attached) m.Window = 0;
        else if (m.WindowValid == true) { ++m.windowsDestroyed; m.WindowValid = false; m.Window = 0; }
        --initCount;
        m.Valid = false;
    }
}
int main()
{
    int a = 1, b = 2, c = 3, d = 4;
    Manager m; m.Valid = true; m.Input = Make(&a, 1); m.Font = Make(&b, 2); m.Display = Make(&c, 1);
    m.Window = &a; m.WindowValid = true; m.Attached = false; m.windowsDestroyed = 0; m.removedCallbacks = 0;
    SCountBlock* fontBlock = m.Font.Info;
    Entry e[2]; e[0].Object = Make(&d, 1); e[0].Flags = 5; e[1].Object.Data = 0; e[1].Object.Info = 0; e[1].Flags = 7;
    Uninitialise(m, e, 2);
    if (destroyed != 3 || fontBlock->RefCount != 1 || m.Input.Info || m.Font.Data || e[0].Flags || e[1].Flags
        || m.windowsDestroyed != 1 || m.Window || m.WindowValid || m.removedCallbacks != 1 || initCount != 1 || m.Valid)
    { std::printf("009a60d0_TEST FAIL\n"); return 1; }
    Uninitialise(m, e, 2);
    if (initCount != 1) { std::printf("009a60d0_TEST FAIL\n"); return 1; }
    delete fontBlock;
    std::printf("009a60d0_TEST PASS\n");
    return 0;
}
