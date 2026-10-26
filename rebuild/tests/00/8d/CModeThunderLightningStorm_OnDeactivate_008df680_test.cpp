#include <stdio.h>
// ---- source under test (pasted) ----
// NCreatureMode::CModeThunderLightningStorm::OnDeactivate, retail 0x008DF680.
// PDB identity: egor 200?? / debug address pending; assigned RTTI slot 6.
class CThing;
class CThingCreatureBase;
class CTCSoundPlayer;
class CBaseObject { public: virtual ~CBaseObject() {} };
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;
    void SetPItem(CBaseObject*); // 0x00A01B90
    void* PData;
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
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
class CThing : public CBaseObject { public:
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
class CCreatureActionBase { public: virtual ~CCreatureActionBase() {} };
class CCombatActionBase : public CCreatureActionBase { public: char body[0x110]; virtual ~CCombatActionBase(); };
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCreatureActionBase&); // 0x006644F0
};
class CCharString { public:
    void* PStringData;
    CCharString(const char*, long); // 0x0099EBF0
    ~CCharString();                 // 0x0099EAE0
};
class CCreatureAction_PlayCombatAnimation : public CCombatActionBase { public:
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
class CTCSoundPlayer { public:
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

// ---- end source ----

long g_InvalidSoundHandle_0129D25C = -1;
static int calls[8];
static int setItemCalls;static int actionDestroyed,stringDestroyed,baseCleanupOrdered;
static CTCSoundPlayer soundPlayer;
static bool soundExists;static CModeThunderLightningStorm* lookupMode;static bool validActionFields;
static long lastSound;
static long removed;
static CCreatureAction_PlayCombatAnimation* lastAction;

CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++calls[0]; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const
{ return (CBaseObject*)PData; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* object)
{ PData = object; ++setItemCalls; }
void CThing::Kill(bool kill) { ++calls[1]; if (kill) Killed = 1; }
CTCMap::Entry* CTCMap::LowerBound(const long* key) { if(lookupMode)lookupMode->LoopingSound=888;Entry* it=First;while(it!=Last && it->Type<*key)++it;return it; }
void CThing::RemoveTC(unsigned long tc) { removed = tc; ++calls[2]; }
void CThing::SetSuspendAnimation(bool suspend, long flags)
{ calls[3] = suspend ? 1 : 2; calls[4] = flags; }
void CTCSoundPlayer::StopAttachedSound(unsigned long id)
{ lastSound = id; ++calls[5]; }
CCharString::CCharString(const char* text, long) { PStringData=(void*)text;++calls[6]; }
CCharString::~CCharString() { ++calls[7];++stringDestroyed; }
CCombatActionBase::~CCombatActionBase(){++actionDestroyed;}
CCreatureAction_PlayCombatAnimation::CCreatureAction_PlayCombatAnimation(
    CThingCreatureBase&, const CCharString&, bool usePhysics) { FaceTarget=false;UsePhysics=usePhysics;AlwaysUseMovementInAnim=false;lastAction = this; }
bool CThingCreatureBase::SetCurrentAction(const CCreatureActionBase& action)
{ const CCreatureAction_PlayCombatAnimation& a=static_cast<const CCreatureAction_PlayCombatAnimation&>(action);validActionFields=!a.FaceTarget && a.UsePhysics && !a.AlwaysUseMovementInAnim; ++calls[6]; return true; }
void CModeStandBase::OnDeactivate() { baseCleanupOrdered=actionDestroyed==1 && stringDestroyed==1;++calls[7]; }

int main()
{
    CThing* former = new CThing;
    CThing* retained = new CThing;
    CIntelligentPointer<CThing>* tailItems = new CIntelligentPointer<CThing>[2];
    CVector<CIntelligentPointer<CThing> > tailVector;
    tailItems[0].PData = former;
    tailItems[1].PData = retained;
    tailVector._First = tailItems;
    tailVector._Last = tailItems + 2;
    tailVector._End = tailItems + 2;
    tailVector.erase(tailItems, tailItems + 1);
    if (setItemCalls != 1 || tailItems[0].PData != retained ||
        tailVector._Last != tailItems + 1) return 1;

    CModeThunderLightningStorm* mode = new CModeThunderLightningStorm;
    CThingCreatureBase* parent = new CThingCreatureBase;
    CThing* live = new CThing;
    CThing* dead = new CThing;
    CThing* sky = new CThing;
    CEffect* effect = new CEffect;
    CThing* hand = new CThing;
    CThing* hit = new CThing;
    CIntelligentPointer<CThing>* beamItems = new CIntelligentPointer<CThing>[2];
    CIntelligentPointer<CThing>* endItems = new CIntelligentPointer<CThing>[1];
    CIntelligentPointer<CThing>* hitItems = new CIntelligentPointer<CThing>[1];
    live->Killed = sky->Killed = hand->Killed = effect->Killed = hit->Killed = 0;
    mode->ParentThing = parent;
    mode->LightningBeamParticleEmitters._First = beamItems;
    mode->LightningBeamParticleEmitters._Last = beamItems + 2;
    mode->LightningBeamParticleEmitters._End = beamItems + 2;
    mode->EndEmitters._First = endItems;
    mode->EndEmitters._Last = endItems + 1;
    mode->EndEmitters._End = endItems + 1;
    mode->HitThings._First = hitItems;
    mode->HitThings._Last = hitItems + 1;
    mode->HitThings._End = hitItems + 1;
    beamItems[0].PData = live;
    beamItems[1].PData = dead; dead->Killed = 1;
    endItems[0].PData = live;
    hitItems[0].PData = hit;
    mode->PSkyBeam.PData = sky;
    mode->PCreatorEffect.PData = effect;
    mode->PHandGlow.PData = hand;
    mode->LoopingSound = 77;
    parent->UsingComponents[7] = 1UL << 14;
    parent->UsingComponents[0] = 1UL << 16;
    static CTCMap::Entry soundEntries[2];soundEntries[0].Type=0x10;soundEntries[0].Component=&soundPlayer;soundEntries[1].Component=0;
    parent->Interfaces.First=soundEntries;parent->Interfaces.Last=soundEntries+1;
    soundExists = true;lookupMode=mode;
    mode->OnDeactivate();
    if (calls[1] != 4 || calls[2] != 1 || removed != 0xee ||
        calls[3] != 2 || calls[4] != 0 || lastSound != 77 || calls[5] != 1 ||
        mode->LoopingSound != -1 || mode->LightningBeamParticleEmitters._Last != beamItems ||
        mode->EndEmitters._Last != endItems || mode->HitThings._Last != hitItems ||
        !validActionFields || !baseCleanupOrdered || !live->Killed || hit->Killed || calls[6] != 2 || calls[7] < 2)
        return 1;
    puts("008df680_TEST PASS");
    return 0;
    puts("008df680_TEST PASS");
    return 0;
}