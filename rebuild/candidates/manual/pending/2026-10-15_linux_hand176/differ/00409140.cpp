// CUserProfileManager::ClampAntialiasing(unsigned long* level, long max_level) -- validates an antialiasing setting against the display manager's supported list: an out-of-range index resets to 0, and with a cap (not -1) the index steps down until the supported level no longer exceeds the cap
// Leaf: ClampAntialiasing
// Grade: DIFFER(128v126) -- the step-down loop: retail keeps it unrotated (`cmp [eax+ecx*4],edx ; jle ; dec ecx ; jmp`, vector start in eax), ours rotates it into a pointer-strength-reduced do-while with ebx (`mov ebx,[ecx+eax*4-4] ; dec ; cmp ; jg`) and swaps eax/ecx (tried: while, for(;;)+break, store inside the loop, named system/display locals)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00409140, length 126 bytes (padded to 128), `ret 8` (`this` unused).
// Callees: GFGetSystemManager @ 0x009A4EC0 (__fastcall), the display manager's supported-levels query
// (vector<long>& out) @ 0x009C05C0 (catalog label "CGuiControlTreePane_RebuildTreeFromArray" is propagated; the
// real name is not known), free @ 0x00BFEA14 (the inline vector dtor).
// Layout: CSystemManager::PDisplayManager (CCountedPointer, object first) at +0x60 (Ego_r.pdb 0x60).
// Shape: the local vector's three words are init-list zeros (immediate stores); the step-down loop sits under an
// if so the index is stored back only when it moved.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
struct CLongVector {                               // STLport vector<long> (12 bytes in retail)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CLongVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CLongVector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    long& operator[](unsigned long n) { return *(_M_start + n); }
};
struct CDisplayManager {
    void GetSupportedAntialiasingLevels(CLongVector& levels);   // 0x009C05C0
};
template <class T> struct CCountedPointer {
    T*    Data;
    void* Info;
    T* operator->() const { return Data; }
};
struct CSystemManager {
    unsigned char    _pad00[0x60];
    CCountedPointer<CDisplayManager> PDisplayManager;   // +0x60
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0
struct CUserProfileManager {
    void ClampAntialiasing(unsigned long* level, long max_level);
};
void CUserProfileManager::ClampAntialiasing(unsigned long* level, long max_level)
{
    CLongVector levels;
    CSystemManager* system = GFGetSystemManager();
    system->PDisplayManager->GetSupportedAntialiasingLevels(levels);
    if (*level >= levels.size())
        *level = 0;
    if (max_level != -1)
    {
        unsigned long index = *level;
        if (levels[index] > max_level)
        {
            for (;;)
            {
                if (levels[index] <= max_level)
                    break;
                --index;
            }
            *level = index;
        }
    }
}
