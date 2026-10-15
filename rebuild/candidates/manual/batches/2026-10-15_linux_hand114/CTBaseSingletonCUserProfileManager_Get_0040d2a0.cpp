// CTBaseSingleton<CUserProfileManager>::Get() -- lazily create the profile manager singleton (0x140 bytes) into its counted-pointer holder and return the object
// Retail: Fable.exe @ 0x0040D2A0, 69 bytes (+ int3 padding).
// Callees: operator new @ 0x00BFEA1A (non-throwing, null-checked), CUserProfileManager::CUserProfileManager @ 0x0040CEC0,
// CCountedPointer<CUserProfileManager>::Reset/operator=(T*) @ 0x0040F1B0 (manifest label propagated from another
// CCountedPointer instantiation) on the holder @ 0x013B7D4C (object word first, count block second).
// Shape: `if (!holder.object) holder = new CUserProfileManager;` -- the null-checked new duplicates the assignment
// and the trailing re-read of the object word into both arms (`push eax` / `xor eax,eax ; push eax`), the fast path
// returns the first read straight away. Same body as the landed 0x0040D2A0 source (fable_user_profile.h model).
void* __cdecl operator new(unsigned int size);   // 0x00BFEA1A

struct CUserProfileManager {
    unsigned char _body[0x140];
    CUserProfileManager();                       // 0x0040CEC0
};

template <class T>
struct CCountedPointer {
    T* m_pObject;
    void* m_pCount;
    void operator=(T* object);                   // 0x0040F1B0
};

template <class T>
struct CTBaseSingleton {
    static CCountedPointer<T> s_instance;        // 0x013B7D4C
    static T* Get();
};

template <>
CUserProfileManager* CTBaseSingleton<CUserProfileManager>::Get()
{
    if (s_instance.m_pObject == 0)
        s_instance = new CUserProfileManager;
    return s_instance.m_pObject;
}
