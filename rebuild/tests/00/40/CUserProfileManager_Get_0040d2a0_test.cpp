#include <stdio.h>
// Behaviour model: CUserProfileManager::Get creates the singleton once through its private static counted pointer PClass
// (Reset with a new manager) and returns the same object afterwards.
static int g_resets = 0;
static int g_constructed = 0;
template <class T> class CCountedPointer {
public:
    T* Data;
    void* Info;
    bool IsNull() const { return Data == 0; }
    T& operator*() const { return *Data; }
    void Reset(T* data) { ++g_resets; Data = data; Info = data ? (void*)data : 0; }
};
class CUserProfileManager {
public:
    CUserProfileManager() : Marker(0x140) { ++g_constructed; }
    static CUserProfileManager& Get();
    int Marker;
private:
    static CCountedPointer<CUserProfileManager> PClass;
};
CCountedPointer<CUserProfileManager> CUserProfileManager::PClass = { 0, 0 };
CUserProfileManager& CUserProfileManager::Get()
{
    if (PClass.IsNull())
        PClass.Reset(new CUserProfileManager);
    return *PClass;
}
int main()
{
    CUserProfileManager& a = CUserProfileManager::Get();
    CUserProfileManager& b = CUserProfileManager::Get();
    if (&a != &b || g_resets != 1 || g_constructed != 1 || a.Marker != 0x140) { printf("0040d2a0_TEST FAIL\n"); return 1; }
    printf("0040d2a0_TEST PASS\n");
    return 0;
}
