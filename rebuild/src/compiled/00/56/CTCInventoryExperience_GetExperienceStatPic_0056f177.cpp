// CTCInventoryExperience::GetExperienceStatPic(EHeroTrainableStatType stat) -- creates the ring picture component for a trainable stat
// Retail 0x0056F177, 248 bytes, ret 4 (`this` unused). The picture name is a default CCharString local
// (0x0099E4B0) reassigned by an if-chain (operator=(const char*) 0x0099EFE0) for the stat ids 0..5 and 9
// (PHYSIQUE, HEALTH, TOUGHNESS, SPEED, ACCURACY, GUILE, MAGIC_POWER). The front-end manager
// (CFrontEndManager::GetInstance 0x0041E5F2, __fastcall) creates a "UI_RING_PIC" component
// (CManager::CreateComponent(const CCharString&, bool) 0x0041DB1D): the literal converts IMPLICITLY to the
// const CCharString& (CCharString(const char*, long) 0x0099EBF0), so VC7.1 builds the temporary in the dead
// `stat` argument slot and re-addresses it with `lea`, destroying it (0x0099EAE0) right after the call. The
// manager's graphics bank counted pointer (+0x10, see the landed CManager::SetGraphicsBank 0x00419134) is copied
// into a local (out-of-line copy ctor 0x0041BA30, dtor 0x00419108); the bank's FindEntry(const CCharString&)
// 0x009CCDC0 (manifest "CQuestManager::OnQuestStartScreenDisplayed" is propagated) gives the graphic index
// handed to component virtual slot 145 (+0x244, named SetGraphic here descriptively).
// Frame-pointer size TU: `optimize("y", off)` + `("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CCharString { public:
    void* m_pStringData;
    CCharString();                                                // 0x0099E4B0
    CCharString(const char* text, long length = -1);              // 0x0099EBF0
    ~CCharString();                                               // 0x0099EAE0
    CCharString& operator=(const char* text);                     // 0x0099EFE0
};

class CBankFile
{
public:
    unsigned long FindIndexBySymbol(const CCharString&) const;   // 0x009CCDC0
};
class CBankFileAsync : public CBankFile
{
public:
};

class CGraphicDataBank : public CBankFileAsync { public:
};

template<class T> class CCountedPointer { public:
    T*    m_pObject;
    void* m_pCount;
    CCountedPointer(const CCountedPointer& other);                // 0x0041BA30
    ~CCountedPointer();                                           // 0x00419108
    T* operator->() const { return m_pObject; }
};

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
    virtual void v091();
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
    virtual void v143();
    virtual void v144();
    virtual void SetGraphic(long graphicIndex);                   // slot 145 (+0x244)
};
class CManager { public:
    unsigned char _pad_000[0x10];
    CCountedPointer<CGraphicDataBank> GraphicsBank;               // +0x10
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
};
class CFrontEndManager : public CManager { public:
    static CFrontEndManager* __fastcall GetInstance();            // 0x0041E5F2
};
}

enum EHeroTrainableStatType { EHeroTrainableStatType_Force32 = 0x7fffffff };

class CTCInventoryExperience { public:
    NUISystem::CComponent* GetExperienceStatPic(EHeroTrainableStatType stat);
};

NUISystem::CComponent* CTCInventoryExperience::GetExperienceStatPic(EHeroTrainableStatType stat)
{
    CCharString picName;
    if (stat == 0) picName = "UI_RING_PIC_PHYSIQUE";
    if (stat == 1) picName = "UI_RING_PIC_HEALTH";
    if (stat == 2) picName = "UI_RING_PIC_TOUGHNESS";
    if (stat == 3) picName = "UI_RING_PIC_SPEED";
    if (stat == 4) picName = "UI_RING_PIC_ACCURACY";
    if (stat == 5) picName = "UI_RING_PIC_GUILE";
    if (stat == 9) picName = "UI_RING_PIC_MAGIC_POWER";
    NUISystem::CFrontEndManager* manager = NUISystem::CFrontEndManager::GetInstance();
    NUISystem::CComponent* pic = manager->CreateComponent("UI_RING_PIC", false);
    CCountedPointer<CGraphicDataBank> bank(manager->GraphicsBank);
    pic->SetGraphic(bank->FindIndexBySymbol(picName));
    return pic;
}