// CPlayerManager::~CPlayerManager() -- destroy the players, then the hero-swap script names and the player list, then the non-copyable base
// Leaf: ??1CPlayerManager@@UAE@XZ
// Retail: Fable.exe @ 0x0044A140, 88 bytes, plain `ret` via a tail `jmp` to ~CBaseClassNonCopyable (0x0099A300); one
// caller 0x0044C033 (the deleting destructor). Catalog label CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip is
// wrong: the vptr stored first is 0x01231CD0, whose COL names .?AVCPlayerManager@@ (: CBaseClassNonCopyable : CBaseClass).
// Body: DestroyPlayers() (0x0044A070, Ego_r.pdb ?DestroyPlayers@CPlayerManager@@AAEXXZ); then the member destructors:
// HeroSwapPlayerScriptNames (+0x20, vector<CCharString>: ~CCharString 0x0099EAE0 over the range, free 0x00BFEA14 on the
// block) and Players (+0x0c, vector<CPlayer*>: free); then the base destructor.
// Layout: retail CPlayerManager -- Component +4, DefinitionManager +8, Players +0x0c, PlayerNeutral +0x18, MainPlayer
// +0x1c, HeroSwapPlayerScriptNames +0x20 (engine/CPlayerManager.h has the Ego_r.pdb offsets with 16-byte vectors:
// MainPlayer +0x20, names +0x24), so the class is modelled locally. STLport vectors over the malloc allocator.
// Shape: /O2 (16-aligned entry; `lea ebx,[ebx]` loop-head pad); the range destroy through a by-value _Destroy(first, last).
#include <stdlib.h>
class CCharString {
public:
    void* PData;
    ~CCharString();                                      // 0x0099EAE0
};
template <class T> inline void _Destroy(T* first, T* last)   // STLport range destroy (by value: the end stays in a register)
{
    for (; first != last; ++first)
        first->~T();
}
template <class T> class CMallocVector {                 // STLport vector<T> over the malloc allocator
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CMallocVector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};
template <class T> class CMallocVector<T*> {             // pointer elements: nothing to destroy
public:
    T** _M_start;
    T** _M_finish;
    T** _M_end_of_storage;
    ~CMallocVector() { if (_M_start) free(_M_start); }
};
class CBaseClassNonCopyable {
public:
    virtual ~CBaseClassNonCopyable();                    // 0x0099A300
};
class CMainGameComponent;
class CGameDefinitionManager;
class CPlayer;
class CPlayerManager : public CBaseClassNonCopyable {
public:
    CMainGameComponent*         Component;               // +0x04
    CGameDefinitionManager*     DefinitionManager;       // +0x08
    CMallocVector<CPlayer*>     Players;                 // +0x0c
    long                        PlayerNeutral;           // +0x18
    long                        MainPlayer;              // +0x1c
    CMallocVector<CCharString>  HeroSwapPlayerScriptNames;   // +0x20
    virtual ~CPlayerManager();
private:
    void DestroyPlayers();                               // 0x0044A070
};
CPlayerManager::~CPlayerManager()
{
    DestroyPlayers();
}