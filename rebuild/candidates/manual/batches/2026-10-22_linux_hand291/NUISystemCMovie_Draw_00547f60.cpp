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

struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct C2DBoxF {
    float Left, Top, Right, Bottom;
    C2DBoxF(float l, float t, float r, float b) : Left(l), Top(t), Right(r), Bottom(b) {}
    void Move(const C2DVector& pos);                 // 0x0041A810
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(const CRGBColour& c) { R = c.R; G = c.G; B = c.B; A = c.A; }
};
struct CTexture {
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

struct CIEngine {
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

struct CComponent {
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
struct CChangingStateComponent : CComponent {
};

struct CMovie : CChangingStateComponent {
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
