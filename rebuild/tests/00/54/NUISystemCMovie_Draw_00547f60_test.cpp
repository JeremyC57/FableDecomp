// Behaviour test for 00547f60 NUISystem::CMovie::Draw: the source verbatim, every callee stubbed, and the drawn sprite
// (box, layer, texture, colour, angles), the add path and the handle clear observed.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#define __int64 long long
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NUISystem::CMovie::Draw(CIEngine& engine, CEnginePrimitiveHandle* primitives, long layer, long* count, CComponent* drawer) -- while this is the playing movie in state 3: when visible, size the box from the video (or the component size) times the scale, centre it if asked, pick the front-end layer, wait up to 100 ms for a decoded frame, copy it into the movie texture under the video lock and add a 2D sprite (indexed when the caller passes a handle array and count); when invisible, clear the handle and notify the owner; otherwise just clear the handle
// Leaf: ?Draw@CMovie
// Retail: Fable.exe (TLC, VC7.1) @ 0x00547F60, 875 bytes (`ret 14h`). Vtable slot 2.
// Callees: C2DBoxF::Move 0x0041A810, CFrontEndManager::GetInstance 0x0041E5F2 (`__fastcall`) and its slot 36 GetLayer,
//   WaitForSingleObjectEx / EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE08 / 0x0143FE0C / 0x0143FE10),
//   CTexture::CopyFromTexture 0x009FA4E0, CEnginePrimitive2DSprite ctor 0x0041BF60, the engine's slots 23 (+0x5C) and 28 (+0x70),
//   own slots 101 / 104 and the owner's slot 91.
// Globals: the playing movie 0x013B8AB4, the video system 0x013B8ABC (frame lock +0x64, frame event +0x7C, frame texture +0x80),
//   the movie texture 0x013B8AC0.
// Layout: the landed CMovie::Update 0x00547E40 model; the component's draw scale +0x7C, colour +0x94 (B,G,R,A), size +0xCC,
//   position +0xF8, scale +0x108, layer +0x12F.
// Shape (advisor fix): C2DVector has no user copy ctor; the selected extent is a multi-def vector and the scaled size a
//   separate ctor-built vector, so the box right edge is `fld size.X ; fadd pos.X` (VC7.1 loads the single-definition
//   operand first) and size reuses extent's dead slot; the colour copy ctor assigns R, G, B, A in the body; the two engine
//   adds have distinct names (same-name virtual overloads swap their vtable slots).
struct CRITICAL_SECTION_18 { unsigned char _body[0x18]; };
extern "C" __declspec(dllimport) unsigned long __stdcall WaitForSingleObjectEx(void* handle, unsigned long ms, int alertable);   // IAT 0x0143FE08
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION_18* cs);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION_18* cs);   // IAT 0x0143FE10

class C2DVector { public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class C2DBoxF { public:
    float Left, Top, Right, Bottom;
    C2DBoxF(float l, float t, float r, float b) : Left(l), Top(t), Right(r), Bottom(b) {}
    void Move(const C2DVector& pos);                 // 0x0041A810
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(const CRGBColour& c) { R = c.R; G = c.G; B = c.B; A = c.A; }
};
class CTexture { public:
    void CopyFromTexture(CTexture* source, unsigned long mip);   // 0x009FA4E0
};
struct CEnginePrimitive2DSprite {
    unsigned char _body[0x54];
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DBoxF* box, long layer, CTexture* texture, long blend,
                             long interpolation, float angleStart, float angleEnd, unsigned char alpha, unsigned char red,
                             unsigned char green, unsigned char blue, unsigned long flags, const C2DVector& uvStart,
                             const C2DVector& uvEnd);  // 0x0041BF60
};

class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    long RefCount;                                                   // +0x04
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};
class CIVCountedPointer {
public:
    CEnginePersistentPrimitiveBase* Object;
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    __forceinline CIVCountedPointer& operator=(int)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = 0;
        return *this;
    }
};
class CEnginePrimitiveHandle {                     // vtable 0x0122F598
public:
    virtual void v000();
    CIVCountedPointer Data;                        // +0x04
    __forceinline void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
};

class CIEngine { public:
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void AddPrimitive(CEnginePrimitiveHandle* handle, const CEnginePrimitive2DSprite& primitive, long flags);   // slot 23 (+0x5C)
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void AddPrimitiveAt(CEnginePrimitiveHandle* handles, long index, const CEnginePrimitive2DSprite& primitive, long flags);   // slot 28 (+0x70)
};

class CVideoSys {
public:
    unsigned char       _pad00[0x64];
    CRITICAL_SECTION_18 FrameLock;                 // +0x64
    void*               FrameReadyEvent;           // +0x7c
    CTexture*           FrameTexture;              // +0x80
};
extern CVideoSys* g_pVideoSys;                     // 0x013B8ABC
extern CTexture*  g_pMovieTexture;                 // 0x013B8AC0

namespace NUISystem {
class CFrontEndManager {
public:
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual long GetLayer(long depth);             // slot 36 (+0x90)
};

class CComponent { public:
    virtual void v000();
    virtual void v001();
    virtual void Draw(CIEngine& engine, CEnginePrimitiveHandle* primitives, long layer, long* count, CComponent* drawer);   // slot 2
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual void v087();
    virtual void v088();
    virtual void v089();
    virtual void v090();
    virtual void OnChildMovieStopped();            // slot 91 (+0x16C)
    virtual void v092();
    virtual void v093();
    virtual void v094();
    virtual void v095();
    virtual void v096();
    virtual void v097();
    virtual void v098();
    virtual void v099();
    virtual void v100();
    virtual bool IsIndependent();                  // slot 101 (+0x194)
    virtual void v102();
    virtual void v103();
    virtual bool IsLayerIndependent();             // slot 104 (+0x1A0)
    unsigned char _pad04[0x7c - 0x04];
    C2DVector     DrawScale;                       // +0x7c
    unsigned char _pad84[0x94 - 0x84];
    CRGBColour    Colour;                          // +0x94
    unsigned char _pad98[0xcc - 0x98];
    C2DVector     Size;                            // +0xcc
    unsigned char _padd4[0xdc - 0xd4];
    CComponent*   m_pOwner;                        // +0xdc
    unsigned char _pade0[0xf8 - 0xe0];
    C2DVector     Position;                        // +0xf8
    unsigned char _pad100[0x108 - 0x100];
    C2DVector     Scale;                           // +0x108
    unsigned char _pad110[0x12f - 0x110];
    char          Layer;                           // +0x12f
    unsigned char _pad130[0x144 - 0x130];
    unsigned long CurrentState;                    // +0x144
    unsigned char _pad148[0x15c - 0x148];
};
class CChangingStateComponent : public CComponent { public:
};

class CMovie : public CChangingStateComponent { public:
    bool                   Playing;                // +0x15c
    CEnginePrimitiveHandle Handle;                 // +0x160
    unsigned long          Width;                  // +0x168
    unsigned long          Height;                 // +0x16c
    bool                   PositionIsCenter;       // +0x170
    void*                  Movie;                  // +0x174 CWideString
    unsigned char          Playback[0x18];         // +0x178 CXMVPlaybackInfo
    float                  Angle;                  // +0x190
    bool                   AttempToPlay;           // +0x194
    static CMovie*         PlayingMovie;           // 0x013B8AB4
    virtual void Draw(CIEngine& engine, CEnginePrimitiveHandle* primitives, long layer, long* count, CComponent* drawer);
};

void CMovie::Draw(CIEngine& engine, CEnginePrimitiveHandle* primitives, long layer, long* count, CComponent* drawer)
{
    if (PlayingMovie == this && CurrentState == 3 && Playing) {
        if (Colour.A > 0 && (!(DrawScale.X <= 0.0f) || !(DrawScale.Y <= 0.0f))) {
            C2DVector extent;
            if (Width == 0)
                extent.X = Size.X;
            else
                extent.X = (float)Width;
            if (Height == 0)
                extent.Y = Size.Y;
            else
                extent.Y = (float)Height;
            C2DVector pos = Position;
            C2DVector size(extent.X * Scale.X, extent.Y * Scale.Y);
            C2DBoxF box(pos.X, pos.Y, pos.X + size.X, pos.Y + size.Y);
            CRGBColour colour = Colour;
            if (PositionIsCenter)
                box.Move(C2DVector(pos.X - size.X * 0.5f, pos.Y - size.Y * 0.5f));
            bool independent = IsIndependent();
            bool layerIndependent = IsLayerIndependent();
            long renderLayer;
            if (!independent && !layerIndependent)
                renderLayer = CFrontEndManager::GetInstance()->GetLayer(Layer + layer);
            else
                renderLayer = CFrontEndManager::GetInstance()->GetLayer(Layer);
            if (WaitForSingleObjectEx(g_pVideoSys->FrameReadyEvent, 100, 1) != 0)
                return;
            if (!g_pMovieTexture)
                return;
            EnterCriticalSection(&g_pVideoSys->FrameLock);
            g_pMovieTexture->CopyFromTexture(g_pVideoSys->FrameTexture, (unsigned long)-1);
            LeaveCriticalSection(&g_pVideoSys->FrameLock);
            CEnginePrimitive2DSprite sprite(0, &box, renderLayer, g_pMovieTexture, 2, 0, Angle, Angle,
                                            colour.A, colour.R, colour.G, colour.B, 2,
                                            C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            if (primitives && count)
                engine.AddPrimitiveAt(primitives, (*count)++, sprite, 0x40);
            else
                engine.AddPrimitive(&Handle, sprite, 0x40);
            return;
        }
        Handle.Clear();
        if (m_pOwner)
            m_pOwner->OnChildMovieStopped();
        return;
    }
    Handle.Clear();
}
}

// ---- stubs ----
void CIEngine::v000() {}
void CIEngine::v001() {}
void CIEngine::v002() {}
void CIEngine::v003() {}
void CIEngine::v004() {}
void CIEngine::v005() {}
void CIEngine::v006() {}
void CIEngine::v007() {}
void CIEngine::v008() {}
void CIEngine::v009() {}
void CIEngine::v010() {}
void CIEngine::v011() {}
void CIEngine::v012() {}
void CIEngine::v013() {}
void CIEngine::v014() {}
void CIEngine::v015() {}
void CIEngine::v016() {}
void CIEngine::v017() {}
void CIEngine::v018() {}
void CIEngine::v019() {}
void CIEngine::v020() {}
void CIEngine::v021() {}
void CIEngine::v022() {}
void CIEngine::v024() {}
void CIEngine::v025() {}
void CIEngine::v026() {}
void CIEngine::v027() {}
void NUISystem::CFrontEndManager::v000() {}
void NUISystem::CFrontEndManager::v001() {}
void NUISystem::CFrontEndManager::v002() {}
void NUISystem::CFrontEndManager::v003() {}
void NUISystem::CFrontEndManager::v004() {}
void NUISystem::CFrontEndManager::v005() {}
void NUISystem::CFrontEndManager::v006() {}
void NUISystem::CFrontEndManager::v007() {}
void NUISystem::CFrontEndManager::v008() {}
void NUISystem::CFrontEndManager::v009() {}
void NUISystem::CFrontEndManager::v010() {}
void NUISystem::CFrontEndManager::v011() {}
void NUISystem::CFrontEndManager::v012() {}
void NUISystem::CFrontEndManager::v013() {}
void NUISystem::CFrontEndManager::v014() {}
void NUISystem::CFrontEndManager::v015() {}
void NUISystem::CFrontEndManager::v016() {}
void NUISystem::CFrontEndManager::v017() {}
void NUISystem::CFrontEndManager::v018() {}
void NUISystem::CFrontEndManager::v019() {}
void NUISystem::CFrontEndManager::v020() {}
void NUISystem::CFrontEndManager::v021() {}
void NUISystem::CFrontEndManager::v022() {}
void NUISystem::CFrontEndManager::v023() {}
void NUISystem::CFrontEndManager::v024() {}
void NUISystem::CFrontEndManager::v025() {}
void NUISystem::CFrontEndManager::v026() {}
void NUISystem::CFrontEndManager::v027() {}
void NUISystem::CFrontEndManager::v028() {}
void NUISystem::CFrontEndManager::v029() {}
void NUISystem::CFrontEndManager::v030() {}
void NUISystem::CFrontEndManager::v031() {}
void NUISystem::CFrontEndManager::v032() {}
void NUISystem::CFrontEndManager::v033() {}
void NUISystem::CFrontEndManager::v034() {}
void NUISystem::CFrontEndManager::v035() {}
void NUISystem::CComponent::v000() {}
void NUISystem::CComponent::v001() {}
void NUISystem::CComponent::v003() {}
void NUISystem::CComponent::v004() {}
void NUISystem::CComponent::v005() {}
void NUISystem::CComponent::v006() {}
void NUISystem::CComponent::v007() {}
void NUISystem::CComponent::v008() {}
void NUISystem::CComponent::v009() {}
void NUISystem::CComponent::v010() {}
void NUISystem::CComponent::v011() {}
void NUISystem::CComponent::v012() {}
void NUISystem::CComponent::v013() {}
void NUISystem::CComponent::v014() {}
void NUISystem::CComponent::v015() {}
void NUISystem::CComponent::v016() {}
void NUISystem::CComponent::v017() {}
void NUISystem::CComponent::v018() {}
void NUISystem::CComponent::v019() {}
void NUISystem::CComponent::v020() {}
void NUISystem::CComponent::v021() {}
void NUISystem::CComponent::v022() {}
void NUISystem::CComponent::v023() {}
void NUISystem::CComponent::v024() {}
void NUISystem::CComponent::v025() {}
void NUISystem::CComponent::v026() {}
void NUISystem::CComponent::v027() {}
void NUISystem::CComponent::v028() {}
void NUISystem::CComponent::v029() {}
void NUISystem::CComponent::v030() {}
void NUISystem::CComponent::v031() {}
void NUISystem::CComponent::v032() {}
void NUISystem::CComponent::v033() {}
void NUISystem::CComponent::v034() {}
void NUISystem::CComponent::v035() {}
void NUISystem::CComponent::v036() {}
void NUISystem::CComponent::v037() {}
void NUISystem::CComponent::v038() {}
void NUISystem::CComponent::v039() {}
void NUISystem::CComponent::v040() {}
void NUISystem::CComponent::v041() {}
void NUISystem::CComponent::v042() {}
void NUISystem::CComponent::v043() {}
void NUISystem::CComponent::v044() {}
void NUISystem::CComponent::v045() {}
void NUISystem::CComponent::v046() {}
void NUISystem::CComponent::v047() {}
void NUISystem::CComponent::v048() {}
void NUISystem::CComponent::v049() {}
void NUISystem::CComponent::v050() {}
void NUISystem::CComponent::v051() {}
void NUISystem::CComponent::v052() {}
void NUISystem::CComponent::v053() {}
void NUISystem::CComponent::v054() {}
void NUISystem::CComponent::v055() {}
void NUISystem::CComponent::v056() {}
void NUISystem::CComponent::v057() {}
void NUISystem::CComponent::v058() {}
void NUISystem::CComponent::v059() {}
void NUISystem::CComponent::v060() {}
void NUISystem::CComponent::v061() {}
void NUISystem::CComponent::v062() {}
void NUISystem::CComponent::v063() {}
void NUISystem::CComponent::v064() {}
void NUISystem::CComponent::v065() {}
void NUISystem::CComponent::v066() {}
void NUISystem::CComponent::v067() {}
void NUISystem::CComponent::v068() {}
void NUISystem::CComponent::v069() {}
void NUISystem::CComponent::v070() {}
void NUISystem::CComponent::v071() {}
void NUISystem::CComponent::v072() {}
void NUISystem::CComponent::v073() {}
void NUISystem::CComponent::v074() {}
void NUISystem::CComponent::v075() {}
void NUISystem::CComponent::v076() {}
void NUISystem::CComponent::v077() {}
void NUISystem::CComponent::v078() {}
void NUISystem::CComponent::v079() {}
void NUISystem::CComponent::v080() {}
void NUISystem::CComponent::v081() {}
void NUISystem::CComponent::v082() {}
void NUISystem::CComponent::v083() {}
void NUISystem::CComponent::v084() {}
void NUISystem::CComponent::v085() {}
void NUISystem::CComponent::v086() {}
void NUISystem::CComponent::v087() {}
void NUISystem::CComponent::v088() {}
void NUISystem::CComponent::v089() {}
void NUISystem::CComponent::v090() {}
void NUISystem::CComponent::v092() {}
void NUISystem::CComponent::v093() {}
void NUISystem::CComponent::v094() {}
void NUISystem::CComponent::v095() {}
void NUISystem::CComponent::v096() {}
void NUISystem::CComponent::v097() {}
void NUISystem::CComponent::v098() {}
void NUISystem::CComponent::v099() {}
void NUISystem::CComponent::v100() {}
void NUISystem::CComponent::v102() {}
void NUISystem::CComponent::v103() {}
void CEnginePersistentPrimitiveBase::v000() {}
void CEnginePersistentPrimitiveBase::v002() {}
void CEnginePrimitiveHandle::v000() {}

void CEnginePersistentPrimitiveBase::Destroy() {}
void CEnginePersistentPrimitiveBase::OnLastHandleReleased(CEnginePrimitiveHandle*) {}
long NUISystem::CFrontEndManager::GetLayer(long) { return 0; }
void CIEngine::AddPrimitive(CEnginePrimitiveHandle*, const CEnginePrimitive2DSprite&, long) {}
void CIEngine::AddPrimitiveAt(CEnginePrimitiveHandle*, long, const CEnginePrimitive2DSprite&, long) {}
static int g_waitResult = 0;
static int g_lockDepth = 0, g_lockCalls = 0, g_copyCalls = 0;
static void* g_waitHandle = 0; static unsigned long g_waitMs = 0; static int g_waitAlert = 0;
extern "C" unsigned long __stdcall WaitForSingleObjectEx(void* handle, unsigned long ms, int alertable) { g_waitHandle = handle; g_waitMs = ms; g_waitAlert = alertable; return g_waitResult; }
extern "C" void __stdcall EnterCriticalSection(CRITICAL_SECTION_18*) { ++g_lockDepth; ++g_lockCalls; }
extern "C" void __stdcall LeaveCriticalSection(CRITICAL_SECTION_18*) { --g_lockDepth; }
static C2DVector g_moveTo; static int g_moveCalls = 0;
void C2DBoxF::Move(const C2DVector& pos) { ++g_moveCalls; g_moveTo = pos; float w = Right - Left, h = Bottom - Top; Left = pos.X; Top = pos.Y; Right = pos.X + w; Bottom = pos.Y + h; }
static CTexture* g_copySrc = 0; static unsigned long g_copyMip = 0; static int g_copyLocked = 0;
void CTexture::CopyFromTexture(CTexture* source, unsigned long mip) { ++g_copyCalls; g_copySrc = source; g_copyMip = mip; g_copyLocked = g_lockDepth; }
struct SpriteRec { C2DBoxF box; long layer; CTexture* tex; long blend, interp; float a0, a1; unsigned char c[4]; unsigned long flags; float uv[4]; SpriteRec() : box(0,0,0,0) {} };
static SpriteRec g_sprite;
CEnginePrimitive2DSprite::CEnginePrimitive2DSprite(unsigned __int64, const C2DBoxF* box, long layer, CTexture* texture, long blend,
    long interpolation, float angleStart, float angleEnd, unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue,
    unsigned long flags, const C2DVector& uvStart, const C2DVector& uvEnd)
{
    g_sprite.box = *box; g_sprite.layer = layer; g_sprite.tex = texture; g_sprite.blend = blend; g_sprite.interp = interpolation;
    g_sprite.a0 = angleStart; g_sprite.a1 = angleEnd; g_sprite.c[0] = alpha; g_sprite.c[1] = red; g_sprite.c[2] = green; g_sprite.c[3] = blue;
    g_sprite.flags = flags; g_sprite.uv[0] = uvStart.X; g_sprite.uv[1] = uvStart.Y; g_sprite.uv[2] = uvEnd.X; g_sprite.uv[3] = uvEnd.Y;
    _body[0] = 0x5a;
}
struct TestEngine : CIEngine {
    int addCalls, addAtCalls; CEnginePrimitiveHandle* handle; long index; long flags; const CEnginePrimitive2DSprite* sprite;
    TestEngine() : addCalls(0), addAtCalls(0), handle(0), index(-1), flags(0), sprite(0) {}
    void AddPrimitive(CEnginePrimitiveHandle* h, const CEnginePrimitive2DSprite& p, long f) { ++addCalls; handle = h; flags = f; sprite = &p; }
    void AddPrimitiveAt(CEnginePrimitiveHandle* h, long i, const CEnginePrimitive2DSprite& p, long f) { ++addAtCalls; handle = h; index = i; flags = f; sprite = &p; }
};
struct TestFEM : NUISystem::CFrontEndManager { long GetLayer(long depth) { return depth * 10 + 7; } };
static TestFEM g_fem;
NUISystem::CFrontEndManager* __fastcall NUISystem::CFrontEndManager::GetInstance() { return &g_fem; }
struct TestPrim : CEnginePersistentPrimitiveBase {
    int destroyed, lastReleased; TestPrim() : destroyed(0), lastReleased(0) { RefCount = 1; }
    void Destroy() { ++destroyed; }
    void OnLastHandleReleased(CEnginePrimitiveHandle*) { ++lastReleased; }
};
static bool g_indep = false, g_layerIndep = false; static int g_stopped = 0;
void NUISystem::CComponent::Draw(CIEngine&, CEnginePrimitiveHandle*, long, long*, CComponent*) {}
void NUISystem::CComponent::OnChildMovieStopped() { ++g_stopped; }
bool NUISystem::CComponent::IsIndependent() { return g_indep; }
bool NUISystem::CComponent::IsLayerIndependent() { return g_layerIndep; }
CVideoSys* g_pVideoSys = 0;
CTexture* g_pMovieTexture = 0;
NUISystem::CMovie* NUISystem::CMovie::PlayingMovie = 0;

static CVideoSys g_video; static CTexture g_tex, g_frameTex;
static void Setup(NUISystem::CMovie& m)
{
    std::memset(&g_sprite, 0, sizeof(g_sprite)); g_moveCalls = 0; g_lockCalls = 0; g_copyCalls = 0; g_waitResult = 0; g_stopped = 0;
    g_indep = false; g_layerIndep = false;
    g_pVideoSys = &g_video; g_pMovieTexture = &g_tex; g_video.FrameTexture = &g_frameTex; g_video.FrameReadyEvent = (void*)&g_video;
    NUISystem::CMovie::PlayingMovie = &m;
    m.CurrentState = 3; m.Playing = true;
    m.Colour.B = 1; m.Colour.G = 2; m.Colour.R = 3; m.Colour.A = 200;
    m.DrawScale.X = 1.0f; m.DrawScale.Y = 1.0f;
    m.Width = 0; m.Height = 0; m.Size.X = 40.0f; m.Size.Y = 20.0f;
    m.Position.X = 100.0f; m.Position.Y = 50.0f; m.Scale.X = 2.0f; m.Scale.Y = 0.5f;
    m.Layer = 3; m.PositionIsCenter = false; m.Angle = 0.25f; m.m_pOwner = 0; m.Handle.Data.Object = 0;
}
static bool Near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    NUISystem::CMovie m;
    TestEngine engine;
    // 1. not the playing movie: only the handle is cleared
    Setup(m); TestPrim prim; m.Handle.Data.Object = &prim; NUISystem::CMovie::PlayingMovie = 0;
    m.Draw(engine, 0, 5, 0, 0);
    CHECK(prim.lastReleased == 1 && prim.destroyed == 1 && m.Handle.Data.Object == 0);
    CHECK(engine.addCalls == 0 && engine.addAtCalls == 0 && g_stopped == 0);
    // 2. invisible (alpha 0): handle cleared and the owner notified
    Setup(m); NUISystem::CMovie owner; m.m_pOwner = &owner; m.Colour.A = 0;
    m.Draw(engine, 0, 5, 0, 0);
    CHECK(g_stopped == 1 && engine.addCalls == 0);
    // 3. zero draw scale on both axes behaves like invisible
    Setup(m); m.DrawScale.X = 0.0f; m.DrawScale.Y = 0.0f;
    m.Draw(engine, 0, 5, 0, 0);
    CHECK(engine.addCalls == 0 && g_copyCalls == 0);
    // 4. normal draw from Size, layered, added to the movie's own handle
    Setup(m); m.DrawScale.X = 0.0f;
    m.Draw(engine, 0, 5, 0, 0);
    CHECK(g_waitMs == 100 && g_waitAlert == 1 && g_waitHandle == (void*)&g_video);
    CHECK(g_copyCalls == 1 && g_copySrc == &g_frameTex && g_copyMip == (unsigned long)-1 && g_copyLocked == 1 && g_lockCalls == 1);
    CHECK(engine.addCalls == 1 && engine.handle == &m.Handle && engine.flags == 0x40);
    CHECK(Near(g_sprite.box.Left, 100.0f) && Near(g_sprite.box.Top, 50.0f) && Near(g_sprite.box.Right, 180.0f) && Near(g_sprite.box.Bottom, 60.0f));
    CHECK(g_sprite.layer == (3 + 5) * 10 + 7 && g_sprite.tex == &g_tex && g_sprite.blend == 2 && g_sprite.interp == 0);
    CHECK(Near(g_sprite.a0, 0.25f) && Near(g_sprite.a1, 0.25f) && g_sprite.flags == 2 && g_moveCalls == 0);
    CHECK(g_sprite.c[0] == 200 && g_sprite.c[1] == 3 && g_sprite.c[2] == 2 && g_sprite.c[3] == 1);
    CHECK(g_sprite.uv[0] == 0.0f && g_sprite.uv[1] == 0.0f && g_sprite.uv[2] == 0.0f && g_sprite.uv[3] == 0.0f);
    // 5. explicit width/height, centred, independent layer, indexed add
    Setup(m); m.Width = 10; m.Height = 30; m.PositionIsCenter = true; g_indep = true;
    long handles[4]; long count = 2;
    m.Draw(engine, (CEnginePrimitiveHandle*)handles, 5, &count, 0);
    CHECK(g_moveCalls == 1 && Near(g_moveTo.X, 100.0f - 10.0f) && Near(g_moveTo.Y, 50.0f - 7.5f));
    CHECK(Near(g_sprite.box.Right - g_sprite.box.Left, 20.0f) && Near(g_sprite.box.Bottom - g_sprite.box.Top, 15.0f));
    CHECK(g_sprite.layer == 3 * 10 + 7 && engine.addAtCalls == 1 && engine.index == 2 && count == 3);
    // 6. the frame is not ready: nothing copied or added
    Setup(m); g_waitResult = 0x102; int adds = engine.addCalls;
    m.Draw(engine, 0, 5, 0, 0);
    CHECK(g_copyCalls == 0 && engine.addCalls == adds);
    // 7. no movie texture: nothing copied
    Setup(m); g_pMovieTexture = 0;
    m.Draw(engine, 0, 5, 0, 0);
    CHECK(g_copyCalls == 0 && engine.addCalls == adds);
    if (fails == 0) printf("00547f60_TEST PASS\n");
    return fails;
}