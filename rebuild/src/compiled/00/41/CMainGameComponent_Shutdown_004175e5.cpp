// CMainGameComponent::Shutdown() -- tear the game component down: stop the sound system, shut the network session, drop the global game/server hooks, uninitialise the event-package machinery and console, then release every owned subsystem pointer and clear the definition manager
// Leaf: ?Shutdown@CMainGameComponent@@QAEXXZ
// Leaf: Shutdown
// Naming wave 18: PDisplayManager is GPDisplayManager (?GPDisplayManager@@3PAVCDisplayManager@@A); ChatGPT pass 10 audit, raw Ego_r public.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004175E5, length 354 bytes (unaligned TU), tail `jmp`.
// Sound manager (agentSND2, 2026-11-10): PSoundManager of engine/CASoundManager.h -- slot 16 StopAllSounds, slot 18 StopMusic(0),
//   UnloadAllBanks 0x00991750 / CloseAllBanks 0x009918F0 (FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP, Anniversary names); was a local CSoundManager model.
// Callees (several through ILT thunks): the sound manager @ 0x013B8394 (virtual slots 16 +0x40 and 18 +0x48, then 0x00991750 and
//   0x009918F0), the network session's (+0x24, a scoped_ptr with the safe-bool idiom: `neg ; sbb ; test edx,offset get` against
//   scoped_ptr::get 0x00418F47) virtual slot 10 (+0x28), the global counted pointer @ 0x013B8790 (release 0x0041915A), the server
//   @ 0x013B8390 CloseSocket 0x009BF5A0, 0x0049BA20 on +0x16178, CInitBaseClass IsValid 0x0099A330 / Uninitialise 0x004AE990 on
//   +0x13AB8, UninitEventPackageLoading 0x004160A6 / UninitEventPackageSaving 0x00416122, the global Uninitialise 0x00AEBEA0,
//   CConsole::Get 0x00414C90 + Initialise 0x009ED190, the counted-pointer releases 0x00403750 / 0x004190B6 / 0x004057A0 / 0x00419108,
//   the scoped_ptr resets (virtual scalar deleting dtors), CGameDefinitionManager::Set(0) 0x0044C71F (`__fastcall`, the tail jmp).
// Globals: game @ 0x013B86A0 (cleared).
// Layout: local model of the retail component (no engine/ header): owned pointers at +0x10..+0x2c, the session +0x24, the
//   event-package and console counted pointers +0x16144/+0x1614c/+0x161c8, the running flag +0x161e0.
// Shape: each counted pointer is cleared by its out-of-line release followed by `Data = 0` (an inline Clear()); the scoped_ptrs
//   reset with `old = px; px = 0; delete old`; the 1 for the deleting dtors lives in ebp (`xor ebp,ebp ; inc ebp`).
// Pragma: #pragma optimize("s", on) -- a size-optimised TU (unaligned entry; constants materialised in registers).
#include "fable_compiler.h"
#pragma optimize("s", on)
class CPolymorphic {
public:
    virtual ~CPolymorphic();
};

template <class T> struct scoped_ptr {
    T* px;
    typedef T* (scoped_ptr::*unspecified_bool_type)() const;
    T* get() const { return px; }
    operator unspecified_bool_type() const { return px == 0 ? 0 : &scoped_ptr::get; }
    T* operator->() const { return px; }
    void reset()
    {
        T* old = px;
        px = 0;
        if (old)
            delete old;
    }
};

#define COUNTED_POINTER(Name, ReleaseAddr)                                   \
    struct Name {                                                            \
        void* Data;                                                          \
        void* Info;                                                          \
        void Release();                                                      \
        FABLE_FORCEINLINE void Clear() { Release(); Data = 0; }                  \
    };
COUNTED_POINTER(CCountedPointer41915a, 0x0041915A)
#undef COUNTED_POINTER
// NF174 (q5222): +0x14 is the text bank, the shared engine/CCountedPointer.h CCountedPointer<NGameText::CDataBank> (NGameText_CDataBank.h:
//   CMainGameComponent::GetTextBank() reads the counted pointer at +0x14); its release is the out-of-line destructor COMDAT 0x00403750
//   ??1?$CCountedPointer@VCDataBank@NGameText@@@@QAE@XZ (FableWin.pdb; the landed 0x00418B7A / 0x0042F50B bind it; declaration-only
//   dtor, FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY), followed by `Data = 0`, the HEAD row's Clear() sequence (Release(); Data = 0)
//   kept as is with the release spelled as that destructor. Not Reset(0): this TU's Reset<CDataBank> is the out-of-line 0x00403E40
//   (bound by the landed 0x00416832), which this site does not call. Was the local CCountedPointer403750 stand-in binding
//   ?Release@CCountedPointer403750@@QAEXXZ.
// NF221 (family q12226 / q12402): members respelled to Ego_r.pdb CMainGameComponent (PTextBank +0x14, PLUA +0x2c, PDebugFont +0x16144,
//   PCutSceneMainFont +0x1614c, PMainGraphicBank: CCountedPointer<T> with these T). The four releases 0x00403750 / 0x004190B6 / 0x004057A0 / 0x00419108 are the out-of-line
//   ?ReleaseData@?$CCountedPointer@<T>@@QAEXXZ of T = NGameText::CDataBank / CLUA / CFontBank / CGraphicDataBank (FableWin.pdb names;
//   each is a 44/53-byte release with Data zeroed before Info, ~CCountedPointer is the 5-byte jmp thunk where one exists), so +0x2c,
//   +0x16144, +0x1614c and +0x161c8 are the shared CCountedPointer<T> (FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY) and each
//   site is `ReleaseData(); Data = 0;`. Was the destructor call (DTOR_DECL_ONLY) for +0x14 and the local CCountedPointer4190b6 /
//   CCountedPointer4057a0 / CCountedPointer419108 stand-ins (?Release@CCountedPointer<addr>@@QAEXXZ) for the other three.
// Inferred: the source spelling of "release, then Data = 0" is unproven (an explicit ReleaseData call names the retail callee and
//   emits no new COMDAT); T of +0x2c / +0x16144 / +0x161c8 from the callee identity (FableWin.pdb ReleaseData names) only.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
namespace NGameText { class CDataBank; }
class CLUA;
class CFontBank;
class CGraphicDataBank;

class CNetworkSession : public CPolymorphic {
public:
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09();
    virtual void Shutdown();                         // slot 10 (+0x28)
};

#define FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP
#include "engine/CASoundManager.h"
// NF132 (q4019): +0x13AB8 is the component's CNetworkClient Client (Ego_r.pdb CMainGameComponent::Client; the landed
//   CreatePlayers 0x004166A8 reaches the same member), the retail engine/CNetworkClient.h class: IsValid is the shared
//   CInitBaseClass member and 0x004AE990 is ?Uninitialise@CNetworkClient@@QAEXXZ (Ego_r + FableWin; was the local
//   CInitBaseClass model's Uninitialise_004ae990 stand-in).
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CNETWORKCLIENT_UPDATE
#define FABLE_RETAIL_CNETWORKCLIENT_UNINITIALISE
#include "engine/CNetworkClient.h"
// NF132 (q4019): the client's double member makes the class 8-aligned, so the vptr + 12 bytes at +0 are now the retail base
//   engine/CGameComponent.h (Ego_r.pdb CMainGameComponent : CGameComponent, 0x10: CBaseClassNonCopyable +0, CDeviceResetCallback
//   +4); the device-reset callback handed to RemoveDeviceResetCallback is that base (was the `Socket` bytes at +4).
#include "engine/CGameComponent.h"

class CDeviceResetCallback;
class CDisplayManager { public:
    void RemoveDeviceResetCallback(CDeviceResetCallback*a0);   // 0x009BF5A0 (Ego_r.pdb ?RemoveDeviceResetCallback@CDisplayManager@@QAEXPAVCDeviceResetCallback@@@Z)
};
class CLSocketServer {
};
class CDisplayManager;
extern CDisplayManager* GPDisplayManager;              // 0x013B8390
extern CCountedPointer41915a g_GlobalCountedPointer; // 0x013B8790

class CFrameRateSmoother {
public:
    void Uninitialise();                             // 0x0049BA20
};
class CConsole {
public:
    static CConsole* Get();                          // 0x00414C90
    void Initialise(long width, long height, void* font);   // 0x009ED190
};
void UninitialiseParticles();                        // 0x00AEBEA0
class CGameDefinitionManager {
public:
    static void __fastcall Set(CGameDefinitionManager* manager);   // 0x0044C71F
};

// NF47 (round 7): 0x013B86A0 is the class's own protected static ?PMainGameComponent@CMainGameComponent@@1PAV1@A (Ego_r.pdb /
//   FableWin.pdb; engine/CMainGameComponent.h), cleared here in place; was the stand-in `void* g_pGame`. Only the static is respelled
//   inside this row's pre-existing CMainGameComponent model: moving Shutdown onto engine/CMainGameComponent.h needs the header's
//   opaque scoped_ptr / counted-pointer members typed first (a port task).
class CMainGameComponent : public CGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;   // 0x013B86A0
public:
    long                   m_10;                     // +0x10
    CCountedPointer<NGameText::CDataBank> PTextBank;      // +0x14 (Ego_r.pdb: the text bank)
    scoped_ptr<CPolymorphic> m_1c;                   // +0x1c
    scoped_ptr<CPolymorphic> m_20;                   // +0x20
    scoped_ptr<CNetworkSession> NetworkSession;      // +0x24
    scoped_ptr<CPolymorphic> m_28;                   // +0x28
    CCountedPointer<CLUA>  PLUA;                     // +0x2c
    unsigned char          _pad34[0x13ab8 - 0x34];
    CNetworkClient         Client;                   // +0x13ab8 (0x2680 bytes)
    unsigned char          _pad16138[0x16144 - 0x16138];
    CCountedPointer<CFontBank> PDebugFont;          // +0x16144
    CCountedPointer<CFontBank> PCutSceneMainFont;         // +0x1614c
    unsigned char          _pad16154[0x16178 - 0x16154];
    CFrameRateSmoother     FrameRateSmoother;        // +0x16178
    unsigned char          _pad16179[0x161c8 - 0x16179];
    CCountedPointer<CGraphicDataBank> PMainGraphicBank;       // +0x161c8
    unsigned char          _pad161d0[0x161e0 - 0x161d0];
    bool                   Running;                  // +0x161e0

    void UninitEventPackageLoading();                // 0x004160A6
    void UninitEventPackageSaving();                 // 0x00416122
    void Shutdown();
};

void CMainGameComponent::Shutdown()
{
    Running = false;
    if (PSoundManager) {
        PSoundManager->StopAllSounds();
        PSoundManager->StopMusic(0);
        PSoundManager->UnloadAllBanks();
    }
    if (NetworkSession)
        NetworkSession->Shutdown();
    if (PSoundManager)
        PSoundManager->CloseAllBanks();
    m_10 = 0;
    g_GlobalCountedPointer.Clear();
    PMainGameComponent = 0;
    ((CDisplayManager*)(((CLSocketServer*)GPDisplayManager)))->RemoveDeviceResetCallback(this);
    FrameRateSmoother.Uninitialise();
    if (Client.IsValid())
        Client.Uninitialise();
    UninitEventPackageLoading();
    UninitEventPackageSaving();
    UninitialiseParticles();
    CConsole::Get()->Initialise(0x60, 0x29, PDebugFont.Data);
    PTextBank.ReleaseData();
    PTextBank.Data = 0;
    m_1c.reset();
    m_20.reset();
    NetworkSession.reset();
    m_28.reset();
    PLUA.ReleaseData();
    PLUA.Data = 0;
    PDebugFont.ReleaseData();
    PDebugFont.Data = 0;
    PCutSceneMainFont.ReleaseData();
    PCutSceneMainFont.Data = 0;
    PMainGraphicBank.ReleaseData();
    PMainGraphicBank.Data = 0;
    CGameDefinitionManager::Set(0);
}