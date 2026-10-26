// NHeroInformationScreens::CManager::RemoveAllPlayerModes() -- take every registered screen's player mode off the main player
// Retail: Fable.exe @ 0x0066E985, 100 bytes, `ret`; frame-pointer size TU (`push ebp ; mov ebp,esp`, `leave`).
// The main player comes from g_pMainGameComponent (0x013B86A0) -> PPlayerManager (+0x1C) -> GetMainPlayer (0x00449970);
// a local vector<const CBase*> (zeroed in [ebp-10h]) is filled by GetAllRegisteredScreens (0x0066E941, landed); for
// each screen the mode from its virtual slot 1 (+4) is removed with CPlayer::RemoveMode (0x00634190). The vector's
// destructor frees the buffer through the allocator (`if (p) free(p)`, CRT free 0x00BFEA14).
// Layout: retail NHeroInformationScreens::CManager as the landed 0x0066EA85 / 0x0066E941 (12-byte STLport vectors).
// Callers 0x004A48E3, 0x0062B448.
// Leaf: RemoveAllPlayerModes
// Shape: retail keeps the local vector's _M_start in ebx across the loop calls into the destructor (`test ebx,ebx ; push ebx`),
// which VC7.1 does only when the filler GetAllRegisteredScreens is a same-TU body that never stores the vector's address
// (declared only, &screens escapes and the destructor re-reads [ebp-10h], 104v100).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14 (import thunk)
class CWorld;
enum EPlayerMode { PLAYER_MODE_NONE };
class CPlayer {
public:
    void RemoveMode(EPlayerMode mode);                   // 0x00634190
};
class CPlayerManager {
public:
    CPlayer* GetMainPlayer();                            // 0x00449970
};
class CMainGameComponent { public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PPlayerManager;                      // +0x1c scoped_ptr<CPlayerManager>
};
extern CMainGameComponent* g_pMainGameComponent;         // 0x013B86A0
namespace NHeroInformationScreens {
class CBase {
public:
    virtual ~CBase();
    virtual EPlayerMode GetPlayerMode() const;           // slot 1 (+0x04)
};
template <class T> class CVector {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    CVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CVector() { if (_M_start) free(_M_start); }
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
class CManager {
public:
    CWorld*               World;                         // +0x00
    CVector<const CBase*> Screens;                       // +0x04
    CVector<const CBase*> InventoryScreens;              // +0x10
    CVector<long>         ScrollBarIndices;              // +0x1c
    void GetAllRegisteredScreens(CVector<const CBase*>& screens) const;   // 0x0066E941
    void RemoveAllPlayerModes();
};
// Same-TU stand-in for GetAllRegisteredScreens 0x0066E941 (body is not the retail one): it never stores the vector's
// address, so the local vector stays unaliased across the loop's calls and its _M_start stays in ebx for the destructor.
const CBase* const* RegisteredScreens_0066E941(const CManager* manager, unsigned int* count);
#pragma auto_inline(off)
__declspec(noinline) void CManager::GetAllRegisteredScreens(CVector<const CBase*>& screens) const
{
    unsigned int count;
    const CBase* const* source = RegisteredScreens_0066E941(this, &count);
    screens._M_finish = screens._M_start + count;
    screens._M_end_of_storage = (const CBase**)source;
}
#pragma auto_inline(on)
void CManager::RemoveAllPlayerModes()
{
    CPlayer* player = g_pMainGameComponent->PPlayerManager->GetMainPlayer();
    unsigned int i = 0;
    CVector<const CBase*> screens;
    GetAllRegisteredScreens(screens);
    const unsigned int count = screens.size();
    const CBase** first = screens._M_start;
    for (; i < count; ++i)
        player->RemoveMode(first[i]->GetPlayerMode());
}
}
