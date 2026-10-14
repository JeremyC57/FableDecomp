// CTimedOwner_00c3d290::GetElapsedTicks() -- performance-counter ticks elapsed since the start stamp held at +0x190 of the object at +4; zero when there is no object
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3D290, length 60 bytes.
// Callees: QueryPerformanceCounter through the IAT slot @ 0x0143FE00.
// The manifest label (C3DAnimationBlendState::GetCombinationBlendCount) is a propagated name: the
// body takes a LARGE_INTEGER on the stack (`sub esp,8`), fills it through the import, and returns
// the 64-bit difference in edx:eax (`sub ; sbb`), so the class is named by what it does.
extern "C" __declspec(dllimport) int __stdcall QueryPerformanceCounter(__int64* counter);   // IAT slot 0x0143FE00

struct CTimedObject {
    unsigned char _pad[0x190];
    __int64 m_startTicks;                  // +0x190
};

struct CTimedOwner_00c3d290 {
    void* m_vptr;
    CTimedObject* m_pObject;               // +4
    __int64 GetElapsedTicks();
};

__int64 CTimedOwner_00c3d290::GetElapsedTicks()
{
    CTimedObject* object = m_pObject;      // kept in esi across the import call
    if (object) {
        __int64 now;
        QueryPerformanceCounter(&now);
        return now - object->m_startTicks;
    }
    return 0;
}
