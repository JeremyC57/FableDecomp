// Test for 0x00495620 CFrontendGameComponent::~CFrontendGameComponent() on the shared headers (NF236, replaces the HEAD host model):
// the row's defines, includes and (empty) body; the work is the member and base destructors the compiler emits, retail's order.
// Stubbed: the CWideString destructor, CTexture::Uninitialise (0x009F9F70, the inline ~CTexture calls it) and the CBaseClass(NonCopyable) destructors (recording), the class's other out-of-line virtuals. The object
// is a zeroed buffer of the class size whose members are seeded at the CTOR_DTOR offsets (PTempGraphics +0x1DFC, MainFont +0x1E04,
// ProfileNames +0x1E0C, SaveToLoad +0x1E2C, WorldNames +0x1E30, WorldToLoad +0x1E40, SplashScreen +0x1E58), then destroyed in place.
// Case 1: PTempGraphics holds the only reference: its deleter runs once. Case 2: MainFont shares its block: the count drops 2 -> 1.
// Case 3: every CWideString is destroyed once, members in reverse declaration order (WorldToLoad, WorldNames' two, SaveToLoad,
// SaveNames (empty), ProfileNames' one). Case 4: SplashScreen's destructor runs first, the base destructor last, once each.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "fable_compiler.h"
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CGAMECOMPONENT_DTOR_INLINE
#define FABLE_RETAIL_CFRONTENDGAMECOMPONENT_SETQUIT
#define FABLE_RETAIL_CFRONTENDGAMECOMPONENT_CTOR_DTOR
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CTexture.h"
#include "engine/CGameComponent.h"
#include "engine/CFrontendGameComponent.h"
static int g_log[16]; static int g_n = 0;
static void Log(int v) { if (g_n < 16) g_log[g_n++] = v; }
static int g_deletes = 0;
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() { Log(1000); }
CWideString::~CWideString() { Log((int)(size_t)PStringData); }
void CTexture::Uninitialise() { Log(2000); }
void CGameComponent::Init() {}
void CGameComponent::SetQuit() {}
bool CFrontendGameComponent::Run(CGameComponent**) { return false; }
void CFrontendGameComponent::ChangeTextureColourDepth(long) {}
void CFrontendGameComponent::OnPreDeviceReset() {}
bool CFrontendGameComponent::OnPostDeviceReset() { return true; }
void CFrontendGameComponent::SetQuit() {}
long CWideString::NoStrings = 0;
CFrontendGameComponent::~CFrontendGameComponent()
{
}
static void FABLE_FASTCALL Deleter(void*) { ++g_deletes; }
static double g_raw[sizeof(CFrontendGameComponent) / sizeof(double) + 1];
static void SetPtr(unsigned off, void* p) { std::memcpy((char*)g_raw + off, &p, sizeof(p)); }
static void SetStrings(unsigned off, int first, int count)
{
    void** a = (void**)std::malloc(count * sizeof(void*));
    for (int i = 0; i < count; ++i) a[i] = (void*)(size_t)(first + i);
    SetPtr(off, a); SetPtr(off + 4, a + count); SetPtr(off + 8, a + count);
}
int main()
{
    int fails = 0;
    if (sizeof(CFrontendGameComponent) != 0x1E60) fails |= 16;
    int graphics = 0, font = 0;
    CCPPointerInfo* gi = new CCPPointerInfo(1, Deleter, &graphics);
    CCPPointerInfo* fi = new CCPPointerInfo(2, Deleter, &font);
    SetPtr(0x1DFC, &graphics); SetPtr(0x1E00, gi);
    SetPtr(0x1E04, &font); SetPtr(0x1E08, fi);
    SetStrings(0x1E0C, 11, 1);
    SetPtr(0x1E2C, (void*)31);
    SetStrings(0x1E30, 41, 2);
    SetPtr(0x1E40, (void*)51);
    CFrontendGameComponent* c = (CFrontendGameComponent*)g_raw;
    c->CFrontendGameComponent::~CFrontendGameComponent();
    if (g_deletes != 1) fails |= 1;
    if (fi->RefCount != 1) fails |= 2;
    static const int want[] = { 2000, 51, 41, 42, 31, 11, 1000 };
    if (g_n != 7 || std::memcmp(g_log, want, sizeof(want)) != 0) fails |= 4;
    if (g_n < 1 || g_log[0] != 2000 || g_log[g_n - 1] != 1000) fails |= 8;
    delete fi;
    if (fails) { std::printf("00495620_TEST FAIL %d (n=%d)\n", fails, g_n); for (int i = 0; i < g_n; ++i) std::printf(" %d", g_log[i]); std::printf("\n"); return 1; }
    std::printf("00495620_TEST PASS\n");
    return 0;
}
