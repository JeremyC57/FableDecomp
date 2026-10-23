// NCreatureMode::CModeThunderLightningStorm::OnDeactivate, retail 0x008DF680.
// PDB identity: egor 200?? / debug address pending; assigned RTTI slot 6.
struct CThing;
struct CThingCreatureBase;
struct CTCSoundPlayer;
struct CBaseObject { virtual ~CBaseObject() {} };
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;
    void SetPItem(CBaseObject*); // 0x00A01B90
    void* PData;
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) {
        SetPItem(other.GetPItem());
        return *this;
    }
    virtual ~CIntelligentPointer() {}
};
template<class T> struct CVector {
    T* _First;
    T* _Last;
    T* _End;
    T* begin() const { return _First; }
    T* end() const { return _Last; }
    unsigned long size() const { return (unsigned long)(_Last - _First); }
    static __forceinline T* CopyPtrs(T* first, T* last, T* result) {
        for (long n = (long)(last - first); n > 0; --n) {
            *result = *first;
            ++first;
            ++result;
        }
        return result;
    }
    static __forceinline void Destroy(T* first, T* last) {
        for (; first != last; ++first)
            ((CBaseIntelligentPointer*)first)->~CBaseIntelligentPointer();
    }
    __forceinline T* erase(T* first, T* last) {
        T* i = CopyPtrs(last, _Last, first);
        Destroy(i, _Last);
        _Last = i;
        return first;
    }
    __forceinline void clear() { erase(_First, _Last); }
};
struct CTCMap {
    struct Entry { long Type; void* Component; };
    Entry *First,*Last;
    Entry* LowerBound(const long*); // original 0040f020
};
struct CThing : CBaseObject {
    virtual ~CThing() {}
    unsigned char pad00[0x20 - sizeof(CBaseObject)];
    unsigned long UsingComponents[9]; // component membership bitmap at +0x20
    CTCMap Interfaces;
    unsigned char pad4c[0x91 - 0x4c];
    unsigned char Killed : 1;
    void Kill(bool); // 0x004C9B80
    unsigned long HasTC(long type) const
    { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCSoundPlayer* GetTC(long interfaceType) {
        CTCMap::Entry* first=Interfaces.First;
        CTCMap::Entry* last=Interfaces.Last;
        long len=(long)(last-first);
        while (len > 0) {
            long half=len/2;
            CTCMap::Entry* middle=first+half;
            if (middle->Type < interfaceType) { first=middle+1; len=len-half-1; }
            else len=half;
        }
        if (first==last || interfaceType<first->Type) first=last;
        return (CTCSoundPlayer*)first->Component;
    }
    void RemoveTC(unsigned long); // 0x004C9840
    void SetSuspendAnimation(bool, long); // 0x00833A10
};
struct CEffect : CThing {};
struct CCreatureActionBase { virtual ~CCreatureActionBase() {} };
struct CCombatActionBase : CCreatureActionBase { char body[0x110]; virtual ~CCombatActionBase(); };
struct CThingCreatureBase : CThing {
    bool SetCurrentAction(const CCreatureActionBase&); // 0x006644F0
};
struct CCharString {
    void* PStringData;
    CCharString(const char*, long); // 0x0099EBF0
    ~CCharString();                 // 0x0099EAE0
};
struct CCreatureAction_PlayCombatAnimation : CCombatActionBase {
    bool FaceTarget,UsePhysics,AlwaysUseMovementInAnim; // PDB94325/94401-3; retail114/115/116
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase&, const CCharString&, bool);
};
struct CModeStandBase {
    virtual ~CModeStandBase() {}
    CThingCreatureBase* ParentThing; // +4
    unsigned char _pad08[0x6c];
    CVector<CIntelligentPointer<CThing> > LightningBeamParticleEmitters; // +74
    CVector<CIntelligentPointer<CThing> > EndEmitters; // +80
    CIntelligentPointer<CThing> PSkyBeam; // +8c
    CIntelligentPointer<CEffect> PCreatorEffect; // +94
    CIntelligentPointer<CThing> PHandGlow; // +9c
    unsigned char _padA4[4];
    CVector<CIntelligentPointer<CThing> > HitThings; // +a8
    unsigned char _padB4[0x10];
    long LoopingSound; // +c4
    void OnDeactivate(); // CModeStandBase::OnDeactivate, 0x00861D00
};
struct CModeThunderLightningStorm : CModeStandBase {
    void OnDeactivate();
};
struct CTCSoundPlayer {
    void StopAttachedSound(unsigned long); // 0x0066ADD0
};
extern long g_InvalidSoundHandle_0129D25C;

template<class T> static void KillLive(CIntelligentPointer<T>& item)
{
    if (item.GetPItem() && !item.GetPItem()->Killed)
        item.GetPItem()->Kill(true);
}

void CModeThunderLightningStorm::OnDeactivate()
{
    long count = (long)LightningBeamParticleEmitters.size();
    for (long i = 0; i < count; ++i) {
        CIntelligentPointer<CThing>& item=LightningBeamParticleEmitters.begin()[i];
        if(item.GetPItem() && !item.GetPItem()->Killed)
            LightningBeamParticleEmitters.begin()[i].GetPItem()->Kill(true);
    }
    LightningBeamParticleEmitters.clear();

    count = (long)EndEmitters.size();
    for (long j = 0; j < count; ++j) {
        CIntelligentPointer<CThing>& item=EndEmitters.begin()[j];
        if(item.GetPItem() && !item.GetPItem()->Killed)
            EndEmitters.begin()[j].GetPItem()->Kill(true);
    }
    EndEmitters.clear();

    KillLive(PSkyBeam);
    KillLive(PCreatorEffect);
    KillLive(PHandGlow);

    if (ParentThing->HasTC(0xee))
        ParentThing->RemoveTC(0xee);

    long sound=LoopingSound;
    if (sound != g_InvalidSoundHandle_0129D25C) {
        if (ParentThing->HasTC(0x10))
            ParentThing->GetTC(0x10)->StopAttachedSound(sound);
        LoopingSound = g_InvalidSoundHandle_0129D25C;
    }

    HitThings.clear();
    {
        CThingCreatureBase* owner=ParentThing;
        owner->SetSuspendAnimation(false, 0);
        CCharString name("LIGHTNING_STORM_OUTOF", -1);
        owner->SetCurrentAction(CCreatureAction_PlayCombatAnimation(*owner, name, true));
    }
    CModeStandBase::OnDeactivate();
}