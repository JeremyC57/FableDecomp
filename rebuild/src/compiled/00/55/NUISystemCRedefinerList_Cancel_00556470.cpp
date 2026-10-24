// NUISystem::CRedefinerList::Cancel() -- abandon key redefinition: when a redefiner list is active, deactivate it, reload the user profile without re-applying its video values, then reset the volume of sound groups 0x10, 0x80 and 0x40
// Leaf: ?Cancel@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00556470, 126 bytes (128 with padding), plain `ret` (Ego_r.pdb
//   `?Cancel@CRedefinerList@NUISystem@@SIXXZ`: static __fastcall, no arguments).
// Callees: the active list's virtual OnDeactivate (slot 141, +0x234; CRedefinerList's own is the landed 0x00556580),
//   CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0 (three times), CUserProfileManager::Load 0x0040D350 (landed), the
//   sound manager's SetGroupVolume (slot 46, +0xb8, 0x00A3E9D0) three times.
// Callers: NUISystem::CMenuEntry slot 156 0x0053DDD0 (call 0x0053DDED), CTCInventory::ClosePCInventory 0x00587847
//   (0x00587850) and DeleteMenuComponents 0x00587A41 (0x00587A4A).
// Data: CRedefinerList::s_pActiveList 0x013B8AC4 (as the landed `??_G` 0x00557360), the sound manager g_pSoundSystem
//   0x013B8394 (CSoundManagerLH, landed CNewFrontendGameComponent::InitialiseSounds 0x0042E17C).
// Layout: CRedefinerList IsActivated (bool) +0x17c (landed Initialise 0x005566A0); CUserProfileManager
//   ApplyVideoValuesOnLoad (bool) +0x33 (Ego_r.pdb +51). Local views.
// Shape: the active list is loaded once (ecx), the sound manager re-read for each virtual call; Get() is called again for
//   each profile access; the float zeroes are `push 0` immediates.
class CUserProfileManager {
public:
    unsigned char _pad00[0x33];
    bool          ApplyVideoValuesOnLoad;               // +0x33
    static CUserProfileManager& __fastcall Get();       // 0x0040D2A0 (CTBaseSingleton<CUserProfileManager>::Get)
    void Load();                                        // 0x0040D350
};

class CSoundManagerLH {                                 // the object at 0x013B8394 (vtable 0x0129D2C4)
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045();
    virtual void SetGroupVolume(unsigned long groupMask, float volume, float fadeTime);   // slot 46 (+0xb8) 0x00A3E9D0
};
extern CSoundManagerLH* g_pSoundSystem;                 // 0x013B8394

namespace NUISystem {
class CRedefinerList {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084();
    virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109();
    virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140();
    virtual void OnDeactivate();                        // slot 141 (+0x234) 0x00556580 (landed)
    unsigned char _body04[0x17c - 0x04];
    bool          IsActivated;                          // +0x17c
    static CRedefinerList* s_pActiveList;               // 0x013B8AC4
    static void __fastcall Cancel();
};

void __fastcall CRedefinerList::Cancel()
{
    if (s_pActiveList != 0 && s_pActiveList->IsActivated) {
        s_pActiveList->OnDeactivate();
        CUserProfileManager::Get().ApplyVideoValuesOnLoad = false;
        CUserProfileManager::Get().Load();
        CUserProfileManager::Get().ApplyVideoValuesOnLoad = true;
        if (g_pSoundSystem != 0) {
            g_pSoundSystem->SetGroupVolume(0x10, 0.0f, 0.0f);
            g_pSoundSystem->SetGroupVolume(0x80, 0.0f, 0.0f);
            g_pSoundSystem->SetGroupVolume(0x40, 0.0f, 0.0f);
        }
    }
}
}