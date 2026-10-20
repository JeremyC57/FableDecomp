// CTCInventoryExperience::OpenInventory() -- opens the experience screen: menu bookkeeping, front-end notification, fade reset, HUD hook, sound category refresh, then snapshots the hero's experience values and opens the base inventory view
// Retail 0x0056FAD7, 278 bytes. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: the global open-menus counter 0x0041D122 (__fastcall, no arguments; landed as an asm bake),
// CFrontEndManager::GetInstance 0x0041E5F2 and its slot 26 (+0x68) given the parent thing, the base screen hook
// 0x005BC6ED on `this` (manifest "_Dest_val<...CRegisteredMusicEntry>" is a propagated label), the main game
// component (0x013B86A0) chain +0x28 -> +0x2C and that object's slot 76 (+0x130, given false; names from
// position), std::bitset::test 0x00410DE0 + CVectorMap::LowerBound 0x0040F020 (the inline CThing::GetInterface for
// the hero experience component 0x68), the sound system (0x013B8394) slot 46 (+0xB8, RefreshCategory(long, float,
// float) as named in the landed CThingSoundEmitter rows) for categories 0x10 and 0x80 with (0.0f, 500.0f), the
// tutorial sound handle map clear 0x005795C3 (+0x134; manifest "CTCCarryable::OnKill" is propagated),
// CTCHeroExperience::GetStatExperience(long) 0x006D6510, this's slot 87 (+0x15C) given 1.
// Shape: the experience component pointer is an uninitialised out local (the miss path re-reads the key's frame
// word, as in the landed SaveExperienceValuesForCountDown 0x00570161); the map clear sits inside the sound block.
// Layout (retail; Ego_r.pdb names, the PDB's 16-byte containers are 12 in retail): TutorialSoundHandles +0x134,
// OrigGeneral..OrigWill +0x1AC..+0x1B8, AccumFadeTime +0x254; CTCHeroExperience GeneralExperience +0x14.
#pragma optimize("y", off)
#pragma optimize("s", on)

void __fastcall OnOpenMenus();                       // 0x0041D122
class CThing;
struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
};
class CThing {
public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCHeroExperience {                           // interface type 0x68
    unsigned char _pad_0x00[0x14];
    long GeneralExperience;                          // +0x14
    long GetStatExperience(long stat);               // 0x006D6510
};

struct CFrontEndManager {
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
    virtual void OnThingInventoryOpened(CThing* thing);   // slot 26 (+0x68)
    static CFrontEndManager* __fastcall GetInstance();    // 0x0041E5F2
};
struct CHudOwner {
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
    virtual void SetEnabled(bool enabled);           // slot 76 (+0x130)
};
struct CWorldPart {
    unsigned char _pad_0x00[0x2c];
    CHudOwner* Hud;                                  // +0x2c
};
struct CMainGameComponent {
    unsigned char _pad_0x00[0x28];
    CWorldPart* World;                               // +0x28
};
extern CMainGameComponent* g_pMainGameComponent;     // 0x013B86A0
struct CSoundSystem {
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
    virtual void RefreshCategory(long category, float a, float b);   // slot 46 (+0xb8)
};
extern CSoundSystem* g_pSoundSystem;                 // 0x013B8394
struct CTutorialSoundHandleMap {                     // map<CCharString, unsigned long>
    void* _M_header; long _M_node_count; long _M_compare;
    void clear();                                    // 0x005795C3
};

struct CTCInventoryExperience {
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
    virtual void SetScreenActive(long active);       // slot 87 (+0x15c)
    CThing* ParentThing;                             // +0x04
    unsigned char _pad_0x08[0x134 - 0x08];
    CTutorialSoundHandleMap TutorialSoundHandles;    // +0x134
    unsigned char _pad_0x140[0x1ac - 0x140];
    long OrigGeneral;                                // +0x1ac
    long OrigStrength;                               // +0x1b0
    long OrigSkill;                                  // +0x1b4
    long OrigWill;                                   // +0x1b8
    unsigned char _pad_0x1bc[0x254 - 0x1bc];
    float AccumFadeTime;                             // +0x254
    void PrepareScreen();                            // 0x005BC6ED (CTCInventoryBase)
    void OpenInventory();
};

void CTCInventoryExperience::OpenInventory()
{
    OnOpenMenus();
    CFrontEndManager::GetInstance()->OnThingInventoryOpened(ParentThing);
    PrepareScreen();
    AccumFadeTime = 0.0f;
    g_pMainGameComponent->World->Hud->SetEnabled(false);
    CTCHeroExperience* experience;
    ParentThing->GetInterface(&experience, 0x68);
    if (g_pSoundSystem) {
        g_pSoundSystem->RefreshCategory(0x10, 0.0f, 500.0f);
        g_pSoundSystem->RefreshCategory(0x80, 0.0f, 500.0f);
        TutorialSoundHandles.clear();
    }
    OrigGeneral = experience->GeneralExperience;
    OrigStrength = experience->GetStatExperience(0);
    OrigSkill = experience->GetStatExperience(1);
    OrigWill = experience->GetStatExperience(2);
    SetScreenActive(1);
}
