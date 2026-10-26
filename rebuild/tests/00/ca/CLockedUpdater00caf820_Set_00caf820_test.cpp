// Behaviour test for 00caf820: src/00caf820.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CLockedUpdater_00caf820::Set(long first, long second)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CLockedUpdater_00caf820::Set(long first, long second) -- under the object's critical section (+8): store the two values (+0x68, +0x6c), raise the dirty flag (+0x64 = 1), run the update at 0x00CAF750; returns 0
// Leaf: Set
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CAF820, 104 bytes (`ret 8`, fs:[0] frame).
// Label: the catalog's "CModifierGraphic::CModifierGraphic" is propagated: the body is no constructor (no vtable, it
// returns 0 in eax, not `this`), and the retail CModifierGraphic is a 12-byte record; class and name are descriptive.
// Callees: EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10), the update @ 0x00CAF750
// (catalog "CModifierGraphic_UpdatePosition", `__fastcall`).
// Shape: a scoped lock object (its section pointer in the `push ecx` frame word, EH state 0 while it lives) whose
// inline destructor leaves the section; the second argument is loaded and stored first (source in that order).
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);   // IAT 0x0143FE10

struct CScopedSectionLock {
    void* m_pSection;
    CScopedSectionLock(void* section) : m_pSection(section) { EnterCriticalSection(section); }
    ~CScopedSectionLock() { LeaveCriticalSection(m_pSection); }
};

class C2DCoordI { public:                          // the two values form one small value struct with a user operator=
    long X, Y;
    C2DCoordI() {}
    C2DCoordI(long x, long y) : X(x), Y(y) {}
    C2DCoordI& operator=(const C2DCoordI& o) { X = o.X; Y = o.Y; return *this; }
};
struct CLockedUpdater_00caf820 {
    unsigned char _pad00[8];
    unsigned char m_section[0x18];          // +0x08 CRITICAL_SECTION
    unsigned char _pad20[0x44];
    long          m_dirty;                  // +0x64
    C2DCoordI     m_values;                 // +0x68 (+0x68 first, +0x6c second)
    void Update();                          // 0x00CAF750
    long Set(long first, long second);
};

long CLockedUpdater_00caf820::Set(long first, long second)
{
    CScopedSectionLock lock(m_section);
    m_values = C2DCoordI(first, second);
    m_dirty = 1;
    Update();
    return 0;
}

// ---- stubs and driver ----

extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(void* section);
extern "C" __declspec(dllimport) int __stdcall TryEnterCriticalSection(void* section);
static int g_updates = 0; static long g_seenX, g_seenY, g_seenDirty; static long g_owner;
void CLockedUpdater_00caf820::Update()
{
    ++g_updates; g_seenX = m_values.X; g_seenY = m_values.Y; g_seenDirty = m_dirty;
    g_owner = *(long*)(m_section + 8);       // RecursionCount: 1 while Set holds the section
}
int main()
{
    static CLockedUpdater_00caf820 u; memset(&u, 0, sizeof u);
    InitializeCriticalSection(u.m_section);
    long r = u.Set(12, -34);
    CHECK(r == 0); CHECK(u.m_values.X == 12); CHECK(u.m_values.Y == -34); CHECK(u.m_dirty == 1);
    CHECK(g_updates == 1); CHECK(g_seenX == 12 && g_seenY == -34 && g_seenDirty == 1);
    CHECK(g_owner == 1);                        // held during the update
    CHECK(*(long*)(u.m_section + 8) == 0);      // released afterwards
    u.m_dirty = 0; r = u.Set(1, 2); CHECK(r == 0 && u.m_values.X == 1 && u.m_values.Y == 2 && u.m_dirty == 1 && g_updates == 2);
    if (fails == 0) printf("00caf820_TEST PASS\n"); else printf("00caf820_TEST FAIL\n");
    return fails;
}