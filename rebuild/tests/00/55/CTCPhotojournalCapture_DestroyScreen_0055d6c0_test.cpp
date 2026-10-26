// Behaviour test for CTCPhotojournalCapture::DestroyScreen (0x0055D6C0): every non-null component deleted, the list emptied
// through the trivial erase, and event 0x44 (main player, valid, not a replacement) queued once.
#include <cstdio>
#include <cstring>
static int g_deleted = 0;
static void* CopyTrivial(const void* first, const void* last, void* result)
{
    if (first == last)
        return result;
    unsigned int n = (unsigned int)((const char*)last - (const char*)first);
    return (char*)std::memmove(result, first, n) + n;
}
namespace NUISystem {
    class CComponent { public: virtual ~CComponent() { ++g_deleted; } };
}
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(_M_start + n); }
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    T* erase(T* first, T* last)
    {
        T* kept = (T*)CopyTrivial(last, _M_finish, first);
        _M_finish = kept;
        return first;
    }
    void clear() { erase(begin(), end()); }
};
class CGameEvent { public:
    long Type; signed char Player; unsigned char Data[32]; unsigned char EndPos; bool Valid; bool Replacement;
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
};
static int g_events = 0; static long g_type = 0; static int g_player = 0; static int g_valid = 0;
class CPlayerManager { public: long GetMainPlayerNumber() { return 3; } };
class CMainGameComponent { public:
    CPlayerManager* PlayerManager;
    void AddGameEvent(const CGameEvent& event) { ++g_events; g_type = event.Type; g_player = event.Player; g_valid = event.Valid && !event.Replacement && event.EndPos == 0; }
};
static CPlayerManager g_pm; static CMainGameComponent g_mgc = { &g_pm };
CMainGameComponent* g_pMainGameComponent = &g_mgc;
class CTCPhotojournalCapture { public:
    vector<NUISystem::CComponent*> Components;
    void DestroyScreen();
};

void CTCPhotojournalCapture::DestroyScreen()
{
    for (unsigned long i = 0; i < Components.size(); ++i)
        if (Components[i])
            delete Components[i];
    Components.clear();
    CGameEvent event(0x44);
    event.Player = (signed char)g_pMainGameComponent->PlayerManager->GetMainPlayerNumber();
    g_pMainGameComponent->AddGameEvent(event);
}

int main()
{
    int bad = 0;
    NUISystem::CComponent* list[3] = { new NUISystem::CComponent, 0, new NUISystem::CComponent };
    CTCPhotojournalCapture c; c.Components._M_start = list; c.Components._M_finish = list + 3; c.Components._M_end_of_storage = list + 3;
    c.DestroyScreen();
    if (g_deleted != 2 || c.Components._M_finish != list) bad |= 1;
    if (g_events != 1 || g_type != 0x44 || g_player != 3 || !g_valid) bad |= 2;
    c.DestroyScreen();                                  // already empty
    if (g_deleted != 2 || g_events != 2 || c.Components._M_finish != list) bad |= 4;
    if (bad) { std::printf("0055d6c0_TEST FAIL\n"); return 1; }
    std::printf("0055d6c0_TEST PASS\n");
    return 0;
}