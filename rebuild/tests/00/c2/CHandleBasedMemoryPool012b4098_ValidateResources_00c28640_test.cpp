// Behaviour test for CHandleBasedMemoryPool::ValidateResources (0x00C28640): every handle slot that holds a taken ('T')
// area hands its resource to ValidateResource; empty slots and free areas are skipped.
#include <cstdio>
#include <vector>

struct CHeader { void* Resource; long Handle; long Size; short Padding; unsigned char Status; unsigned char Locked; };
struct Pool {
    std::vector<CHeader*> Handles;
    void* seen[8]; int n;
    void ValidateResource(void* r) { seen[n++] = r; }
    void ValidateResources()
    {
        for (long index = 0; index < (long)Handles.size(); ++index) {
            if (Handles[index] && Handles[index]->Status == 'T')
                ValidateResource(Handles[index]->Resource);
        }
    }
};

int main()
{
    CHeader a = { (void*)0x10, 0, 0, 0, 'T', 0 }, b = { (void*)0x20, 2, 0, 0, 'N', 0 }, c = { (void*)0x30, 3, 0, 0, 'T', 0 };
    Pool p; p.n = 0;
    p.ValidateResources();
    bool ok = p.n == 0;
    p.Handles.push_back(&a); p.Handles.push_back(0); p.Handles.push_back(&b); p.Handles.push_back(&c);
    p.ValidateResources();
    ok = ok && p.n == 2 && p.seen[0] == (void*)0x10 && p.seen[1] == (void*)0x30;
    if (!ok) { std::printf("00c28640_TEST FAIL\n"); return 1; }
    std::printf("00c28640_TEST PASS\n");
    return 0;
}