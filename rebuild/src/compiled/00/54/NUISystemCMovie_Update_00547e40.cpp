// NUISystem::CMovie::Update(float dt) -- the changing-state update, then drive the shared video system: while the movie is the playing one in state 3 retry a pending play and process a frame; once the component asks to stop outside state 3, close and delete the video system, free the movie textures, clear the primitive handle and notify the owner component
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) @ 0x00547E40, 284 bytes (+ int3 padding), `ret 4`.
// Callees: NUISystem::CChangingStateComponent::Update(float) 0x0052C7E0 (qualified base call), CVideoSys::AttemptToPlay
//   0x00A3B1A0 / Process 0x00A3B430 / Close 0x00A3B380 / ~CVideoSys 0x00A3BC20, operator delete 0x00BFE9BC,
//   CMovie::FreeTextures 0x00547B90 (landed), own virtual slot 137 (+0x224, a bool query), the primitive's slots 3
//   (+0x0C, OnLastHandleReleased(handle)) and 1 (Destroy), the owner component's slot 91 (+0x16C).
// Globals: the video system 0x013B8ABC, the playing movie 0x013B8AB4.
// Layout: retail CMovie as the landed ctor 0x005482D0 (Playing +0x15c, Handle +0x160, AttempToPlay +0x194);
//   CChangingStateComponent::CurrentState +0x144 (PDB +0x174 less the 12-byte container deltas); the owner component
//   pointer at +0xdc. rebuild/include/engine/CMovie.h is a flat pad record, so the bases are modelled locally.
// Shape: the handle reset is the CEnginePrimitiveHandle Clear() of 0x00AF70D0 (IsUnique notification, release plus a
//   second zero store, `__forceinline`); `Playing == true` compares with 1; `delete` of the video system calls its
//   out-of-line destructor then operator delete.
void __cdecl operator delete(void* block);          // 0x00BFE9BC

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

class CVideoSys {
public:
    ~CVideoSys();                                  // 0x00A3BC20
    bool AttemptToPlay();                          // 0x00A3B1A0
    bool Process();                                // 0x00A3B430
    void Close();                                  // 0x00A3B380
};
extern CVideoSys* g_pVideoSys;                     // 0x013B8ABC

namespace NUISystem {

struct CComponent {
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
    virtual bool IsStopRequested();                // slot 137 (+0x224)
    unsigned char _pad04[0xdc - 0x04];
    CComponent*   m_pOwner;                        // +0xdc
    unsigned char _pade0[0x144 - 0xe0];
    unsigned long CurrentState;                    // +0x144 (CChangingStateComponent)
    unsigned char _pad148[0x15c - 0x148];
};
struct CChangingStateComponent : CComponent {
    void Update(float dt);                         // 0x0052C7E0
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
    void FreeTextures();                           // 0x00547B90
    void Update(float dt);
};

void CMovie::Update(float dt)
{
    CChangingStateComponent::Update(dt);
    if (CurrentState == 3 && g_pVideoSys && Playing == true && PlayingMovie == this) {
        if (AttempToPlay && g_pVideoSys->AttemptToPlay())
            AttempToPlay = false;
        g_pVideoSys->Process();
    }
    if (IsStopRequested() && CurrentState != 3 && Playing == true && PlayingMovie == this) {
        g_pVideoSys->Close();
        Playing = false;
        PlayingMovie = 0;
        FreeTextures();
        delete g_pVideoSys;
        g_pVideoSys = 0;
        Handle.Clear();
        if (m_pOwner)
            m_pOwner->OnChildMovieStopped();
    }
}

}