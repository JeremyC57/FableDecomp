// NUISystem::CMovie::ChangeState(EState newState) -- base state change; on entering state 3 (from another target state) create the shared video system if needed, stop the movie that is playing, open this movie's full path and, on success, size the component from the video, reset the primitive handle, notify the owner, recreate the 8888 movie texture and mark the movie playing; this becomes the playing movie
// Leaf: ?ChangeState@CMovie
// Retail: Fable.exe (TLC, VC7.1) @ 0x00547C90, length 432 bytes (427 + int3 padding), `ret 4`, no EH frame.
// Callees: NUISystem::CChangingStateComponent::ChangeState 0x0052CF40 (qualified base call), operator new 0x00BFEA1A (0x8c),
//   CVideoSys::CVideoSys 0x00A3BC70 / Close 0x00A3B380 / Init 0x00A3B9D0, CAFile::ConvertPathToFullPath 0x00999110 (`__fastcall`,
//   hidden result in ecx, path in edx; manifest "_global"), CPixelFormat::Initialise(long, long, long, long, bool) 0x009E3790,
//   ~CWideString 0x0099B510; the playing movie's slot 143 (+0x23C), the owner's slot 91 (+0x16C), the primitive's slots 3/1,
//   the render device's slots 70 (+0x118, release texture) and 69 (+0x114, create texture).
// Globals: the playing movie 0x013B8AB4, the render device 0x013B8AB8, the video system 0x013B8ABC, the movie texture 0x013B8AC0.
// Layout: the landed NUISystem::CMovie::Update 0x00547E40 model (Playing +0x15c, Handle +0x160, Movie +0x174, AttempToPlay
//   +0x194, owner +0xdc); CChangingStateComponent::TargetState +0x148 (PDB +0x178 less 0x30); the component's float size at
//   +0xcc/+0xd0; the video system's frame description (+0x20) holds the long width/height at +0x16c/+0x170.
// Shape: the full path is a CWideString temporary homed in the dead `newState` slot (destroyed after `PlayingMovie = this`);
//   CPixelFormat has no constructor store here; the extents are built in a frame local before the create call.
void* __cdecl operator new(unsigned int size);      // 0x00BFEA1A

class CWideString { public:
    void* m_pData;
    ~CWideString();                                // 0x0099B510
};
class CAFile { public:
    static CWideString __fastcall ConvertPathToFullPath(const CWideString& path);   // 0x00999110
};
class C2DExtentsI { public:
    long X, Y;
    C2DExtentsI(long x, long y) : X(x), Y(y) { }
};
class CPixelFormat { public:
    long FormatIndex;
    void Initialise(long r, long g, long b, long a, bool alpha);   // 0x009E3790
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

struct CVideoFrameInfo {
    unsigned char _pad00[0x16c];
    long          Width;                           // +0x16c
    long          Height;                          // +0x170
};
class CVideoSys {                                  // 0x8c bytes
public:
    CVideoSys();                                   // 0x00A3BC70
    void Close();                                  // 0x00A3B380
    long Init(const CWideString& filename, long flags, bool loop);   // 0x00A3B9D0
    unsigned char    _pad00[0x20];
    CVideoFrameInfo* m_pFrameInfo;                 // +0x20
    long             m_24;                         // +0x24
    unsigned char    _pad28[0x8c - 0x28];
};
extern CVideoSys* g_pVideoSys;                     // 0x013B8ABC

struct CRenderDevice {
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
    virtual void* CreateTexture(unsigned long flags, const C2DExtentsI* size, unsigned long levels, const CPixelFormat* format);   // slot 69 (+0x114)
    virtual void ReleaseTexture(void* texture);   // slot 70 (+0x118)
};
extern CRenderDevice* g_pRenderDevice;             // 0x013B8AB8
extern void* g_pMovieTexture;                      // 0x013B8AC0

namespace NUISystem {

class CComponent { public:
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
    virtual void v101();
    virtual void v102();
    virtual void v103();
    virtual void v104();
    virtual void v105();
    virtual void v106();
    virtual void v107();
    virtual void v108();
    virtual void v109();
    virtual void v110();
    virtual void v111();
    virtual void v112();
    virtual void v113();
    virtual void v114();
    virtual void v115();
    virtual void v116();
    virtual void v117();
    virtual void v118();
    virtual void v119();
    virtual void v120();
    virtual void v121();
    virtual void v122();
    virtual void v123();
    virtual void v124();
    virtual void v125();
    virtual void v126();
    virtual void v127();
    virtual void v128();
    virtual void v129();
    virtual void v130();
    virtual void v131();
    virtual void v132();
    virtual void v133();
    virtual void v134();
    virtual void v135();
    virtual void v136();
    virtual void v137();
    virtual void v138();
    virtual void v139();
    virtual void v140();
    virtual void v141();
    virtual void v142();
    virtual bool IsMoviePlaying();                 // slot 143 (+0x23C)
    unsigned char _pad04[0xcc - 0x04];
    float         Width;                           // +0xcc
    float         Height;                          // +0xd0
    unsigned char _padd4[0xdc - 0xd4];
    CComponent*   m_pOwner;                        // +0xdc
    unsigned char _pade0[0x144 - 0xe0];
    unsigned long CurrentState;                    // +0x144 (CChangingStateComponent)
    unsigned long TargetState;                     // +0x148
    unsigned char _pad14c[0x15c - 0x14c];
};
class CChangingStateComponent : public CComponent { public:
    void ChangeState(unsigned long newState);      // 0x0052CF40
};

class CMovie : public CChangingStateComponent { public:
    bool                   Playing;                // +0x15c
    CEnginePrimitiveHandle Handle;                 // +0x160
    unsigned long          MovieWidth;             // +0x168
    unsigned long          MovieHeight;            // +0x16c
    bool                   PositionIsCenter;       // +0x170
    CWideString            Movie;                  // +0x174
    unsigned char          Playback[0x18];         // +0x178 CXMVPlaybackInfo
    float                  Angle;                  // +0x190
    bool                   AttempToPlay;           // +0x194
    static CMovie*         PlayingMovie;           // 0x013B8AB4
    void ChangeState(unsigned long newState);
};

void CMovie::ChangeState(unsigned long newState)
{
    unsigned long oldState = TargetState;
    CChangingStateComponent::ChangeState(newState);
    if (newState == 3 && oldState != newState) {
        if (g_pVideoSys == 0) {
            g_pVideoSys = new CVideoSys;
            g_pVideoSys->m_24 = 0;
        }
        if (PlayingMovie && PlayingMovie->IsMoviePlaying() && g_pVideoSys)
            g_pVideoSys->Close();
        Playing = false;
        CWideString fullPath = CAFile::ConvertPathToFullPath(Movie);
        if (g_pVideoSys->Init(fullPath, 0, true) >= 0) {
            Width = (float)g_pVideoSys->m_pFrameInfo->Width;
            Height = (float)g_pVideoSys->m_pFrameInfo->Height;
            Handle.Clear();
            if (m_pOwner)
                m_pOwner->OnChildMovieStopped();
            if (g_pMovieTexture) {
                g_pRenderDevice->ReleaseTexture(g_pMovieTexture);
                g_pMovieTexture = 0;
            }
            CVideoFrameInfo* frame = g_pVideoSys->m_pFrameInfo;
            C2DExtentsI size(frame->Width, frame->Height);
            CPixelFormat format;
            format.Initialise(8, 8, 8, 8, false);
            g_pMovieTexture = g_pRenderDevice->CreateTexture(0, &size, 1, &format);
            AttempToPlay = true;
            Playing = true;
        }
        PlayingMovie = this;
    }
}

}