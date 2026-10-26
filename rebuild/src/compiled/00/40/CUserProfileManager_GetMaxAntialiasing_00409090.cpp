// CUserProfileManager::GetMaxAntialiasing() const -- highest selectable antialiasing index: the last supported level, or (under the config-detection cap) the index of the last level not above the cap, 0 when none qualifies
// Leaf: ?GetMaxAntialiasing@CUserProfileManager@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00409090, 174 bytes (+2 int3 pad). Callees: GFGetSystemManager
// 0x009A4EC0, the display manager's supported-levels query (vector<long>& out) 0x009C05C0, free
// 0x00BFEA14 (the inline vector destructor, expanded on every return path); the cap global 0x01375080.
//
// Retail bytes:  sub esp,0xc ; push esi ; push edi ; <levels = {0,0,0} immediates> ; call GFGetSystemManager ;
//   lea ecx,[levels] ; push ecx ; mov ecx,[eax+0x60] ; call GetSupportedAntialiasingLevels ;
//   mov edi,[g_cap] ; cmp edi,-1 ; je A ; mov edx,[start] ; mov ecx,[finish] ; or esi,-1 ;
//   cmp edx,ecx ; mov eax,edx ; je N ; L: cmp [eax],edi ; jg B ; add eax,4 ; inc esi ; cmp eax,ecx ;
//   jne L ; B: cmp esi,-1 ; je N ; <~levels> ; return esi ; N: <~levels> ; return 0 ;
//   A: esi = (finish - start) / 4 - 1 ; <~levels> ; return esi
// Identity: FableWin.pdb `?GetMaxAntialiasing@CUserProfileManager@@QBEKXZ` (const, `this` unused, no
// arguments): it sits right before the landed CUserProfileManager::ClampAntialiasing 0x00409140 and
// shares its model (the 12-byte STLport vector<long> with init-list zeros and a `free` destructor,
// CSystemManager::PDisplayManager +0x60). The manifest name Engine_FindPrimitiveInArray is
// descriptive. The cap 0x01375080 is the config-detection level GFConfigDetection 0x004017B0 writes
// (-1 = no cap). Shape: the capped path is the `if (cap != -1)` then-block (retail falls through
// into it and jumps away for the uncapped size - 1), the count loop the plain `for (it = begin; it
// != end; ++it) { if (*it > cap) break; ++index; }` from index -1, then `if (index != -1) return
// index; return 0;`. The system manager is a named local, as in ClampAntialiasing (the chained call
// gives `add eax,0x60 ; mov ecx,[eax]` instead of `mov ecx,[eax+0x60]`).
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct CLongVector {                               // STLport vector<long> (12 bytes in retail)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CLongVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CLongVector() { if (_M_start) free(_M_start); }
    long* begin() { return _M_start; }
    long* end() { return _M_finish; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
};

class CDisplayManager { public:
    void GetSupportedAntialiasingLevels(CLongVector& levels);   // 0x009C05C0
};

template <class T> class CCountedPointer { public:
    T*    Data;
    void* Info;
    T* operator->() const { return Data; }
};

class CSystemManager { public:
    unsigned char    _pad00[0x60];
    CCountedPointer<CDisplayManager> PDisplayManager;   // +0x60
};

CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0

extern long g_ConfigLevel;                         // 0x01375080 (GFConfigDetection result, -1 = no cap)

class CUserProfileManager { public:
    unsigned long GetMaxAntialiasing() const;
};

unsigned long CUserProfileManager::GetMaxAntialiasing() const
{
    CLongVector levels;
    CSystemManager* system = GFGetSystemManager();
    system->PDisplayManager->GetSupportedAntialiasingLevels(levels);
    if (g_ConfigLevel != -1)
    {
        long index = -1;
        for (long* it = levels.begin(); it != levels.end(); ++it)
        {
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