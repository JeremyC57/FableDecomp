// CTCCreatureGroupBoastingCrowd::OnHeroAttackCrowdMember() -- the hero attacked a crowd member: schedule clearing the remaining opinion sources 0.5..1 minutes ahead and, unless the crowd is already hostile (state 10), pop one pushed hero opinion source, drop the pending opinion deed from the hero's deed log, push the "hero attacked crowd" opinion source onto the hero and enter state 10
// Leaf: OnHeroAttackCrowdMember
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007849C0, 352 bytes (`ret`).
// Callees: CTCBase::GetPRandomSeed @ 0x00686DA0, GFFloatRandom(float, ulong*) @ 0x004978A0 (__fastcall), CWorld::GetFrame
// @ 0x0049D870 (__fastcall), __ftol2 @ 0x00BFEA70, CTCBase::GetWorld @ 0x00686A70, CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50, COpinionReactionManager::ClearOpinionSource(CThing*, bool) @ 0x007008C0 and AttachOpinionSource(CThing*,
// const CCharString&, CTCOpinionSource**) @ 0x00700780, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020
// (interface 0x51, the hero opinion deed log), CTCHeroOpinionDeedLog::RemoveDeedStillSearchingForWitnesses(long) @ 0x006C5920,
// CCharString(const char*, long) @ 0x0099EBF0, operator=(const char*) @ 0x0099EFE0, operator!=(const char*) @ 0x0099E960,
// ~CCharString @ 0x0099EAE0. Globals: frames per second @ 0x01375550, 0.5f @ 0x0122F59C, 60.0f @ 0x0126B458, "" @ 0x0122D70E,
// "OPINION_SOURCE_HERO_BOASTING_HERO_ATTACKED_CROWD" @ 0x0126B460.
// Layout: as the landed OnHeroBoast 0x00784920 / OnHeroApproachBoastingArea 0x00784600 (retail = engine header - 4): PHero +0x24,
// CrowdState +0x2c, CurrentOpinionDeedHandle +0x38, HeroOpinionSourcesPushed +0x3c, FrameToClearRemainingOpinionSources +0x40;
// CWorld opinion reaction manager +0x74.
// Shape: the delay and the string each in their own block (retail packs fps with the string and the delay with the interface key
// and the out pointer: two stack slots); managers loaded into named locals before the GetPItem argument; the deed log bound as
// `CTCHeroOpinionDeedLog* const&` (retail loads the component into eax, then the handle, then `mov ecx,eax`).
class CCharString {
public:
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    const CCharString& operator=(const char* text);    // 0x0099EFE0
    bool operator!=(const char* text) const;           // 0x0099E960
private:
    void* m_pData;
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
extern long g_framesPerSecond;                                      // 0x01375550

class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};
class CTCHeroOpinionDeedLog { public:                  // interface 0x51
    void RemoveDeedStillSearchingForWitnesses(long deed);   // 0x006C5920
};
class CTCOpinionSource;
class COpinionReactionManager { public:
    void ClearOpinionSource(CThing* thing, bool flag);                                      // 0x007008C0
    bool AttachOpinionSource(CThing* thing, const CCharString& name, CTCOpinionSource** source);   // 0x00700780
};
class CWorld { public:
    unsigned char _pad00[0x74];
    COpinionReactionManager* OpinionReactionManager;   // +0x74
public:
    static long __fastcall GetFrame();   // 0x0049D870
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CTCBase
{
public:
    virtual void v00();
protected:
    unsigned long* GetPWorldSeed() const;   // 0x00686DA0
    CWorld& GetWorld() const;   // 0x00686A70
};
class CTCCreatureGroup : public CTCBase
{
public:
};

class CTCCreatureGroupBoastingCrowd : public CTCCreatureGroup { public:
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x24 - 0x08];
    CIntelligentPointer<CThing> PHero;              // +0x24
    long CrowdState;                            // +0x2c
    long CurrentEncouragementState;             // +0x30
    bool HeroBoasted;                           // +0x34
    bool HeroMadeGoodBoast;                     // +0x35
    long CurrentOpinionDeedHandle;              // +0x38
    long HeroOpinionSourcesPushed;              // +0x3c
    long FrameToClearRemainingOpinionSources;   // +0x40
    long FrameToLeavePostBoastState;            // +0x44
    void OnHeroAttackCrowdMember();
};

void CTCCreatureGroupBoastingCrowd::OnHeroAttackCrowdMember()
{
    {
        long framesPerSecond = g_framesPerSecond;
        float delay = (GFFloatRandom(0.5f, GetPWorldSeed()) + 0.5f) * framesPerSecond * 60.0f;
        FrameToClearRemainingOpinionSources = (long)(CWorld::GetFrame() + delay);
    }
    if (CrowdState == 10)
        return;
    if (HeroOpinionSourcesPushed > 0) {
        COpinionReactionManager* manager = GetWorld().OpinionReactionManager;
        manager->ClearOpinionSource(PHero.GetPItem(), true);
        --HeroOpinionSourcesPushed;
    }
    if (CurrentOpinionDeedHandle != -1) {
        if (PHero.GetPItem() && !PHero.GetPItem()->IsDead()) {
            CThing* hero = PHero.GetPItem();
            if (hero->HasTC(0x51)) {
                CTCHeroOpinionDeedLog* const& deedLog = hero->GetTC<CTCHeroOpinionDeedLog>(0x51);
                deedLog->RemoveDeedStillSearchingForWitnesses(CurrentOpinionDeedHandle);
            }
        }
        CurrentOpinionDeedHandle = -1;
    }
    {
        CCharString name("", -1);
        name = "OPINION_SOURCE_HERO_BOASTING_HERO_ATTACKED_CROWD";
        if (name != "") {
            CTCOpinionSource* source;
            COpinionReactionManager* manager = GetWorld().OpinionReactionManager;
            if (manager->AttachOpinionSource(PHero.GetPItem(), name, &source))
                ++HeroOpinionSourcesPushed;
        }
        CrowdState = 10;
    }
}