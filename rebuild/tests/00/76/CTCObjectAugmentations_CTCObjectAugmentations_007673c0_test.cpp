// Behaviour test for the CTCObjectAugmentations constructor (0x007673C0): all containers start empty (the two maps with a
// self-linked header node), counters and flags cleared, the trail pointer set to null, the sub-def fetched from the owner's
// definition and the slot vector resized to NumberOfSlots with null def pointers.
#include <cstdio>
#include <cstdlib>
class CThing;
struct CDefBase { virtual void v00() { } long m_refCount; };
template <class T> class CDefPointer { public:
    const T* Object;
    CDefPointer() : Object(0) { }
};
class CObjectAugmentationsDef : public CDefBase { public: long NumberOfSlots; };
class CThingBaseDef : public CDefBase { public: };
static CObjectAugmentationsDef g_def;
static int g_subDefCalls = 0;
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<T>& out) const { ++g_subDefCalls; out.Object = &g_def; return true; }
};
class CThing { public: CDefClassBase* PDef; };
template <class P> struct CAllocProxy {
    P _M_data;
    CAllocProxy(P p) : _M_data(p) { }
};
static unsigned long g_resizeCount = 0; static const void* g_resizeValue = 0; static int g_resizeNull = -1;
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T*> _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    void resize(unsigned long count, const T& value) { g_resizeCount = count; g_resizeValue = &value; g_resizeNull = value.Object == 0 ? 1 : 0; }
    void resize(unsigned long count) { resize(count, T()); }
};
class CCharString { public: void* PStringData; };
class CObjectAugmentationSoundSet { public: bool Played; CCharString Sound; unsigned long Handle; };
struct SRbTreeNodeBase { bool _M_color; SRbTreeNodeBase* _M_parent; SRbTreeNodeBase* _M_left; SRbTreeNodeBase* _M_right; };
struct SSoundSetNode : SRbTreeNodeBase { long first; CObjectAugmentationSoundSet second; };
template <class N> struct CNodeAllocProxy {
    N* _M_data;
    CNodeAllocProxy(N* p) : _M_data(p) { }
    N* allocate(unsigned int n) { return n != 0 ? (N*)std::malloc(n * sizeof(N)) : 0; }
};
struct CSoundSetTreeBase {
    CNodeAllocProxy<SSoundSetNode> _M_header;
    CSoundSetTreeBase() : _M_header(0) { _M_header._M_data = _M_header.allocate(1); }
};
struct SLess { };
struct CSoundSetMap : CSoundSetTreeBase {
    unsigned long _M_node_count;
    SLess         _M_key_compare;
    CSoundSetMap() : _M_node_count(0)
    {
        _M_header._M_data->_M_color = false;
        _M_header._M_data->_M_parent = 0;
        _M_header._M_data->_M_left = _M_header._M_data;
        _M_header._M_data->_M_right = _M_header._M_data;
    }
};
static int g_setItem = 0;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer() : PData((void*)1) { }
    virtual ~CBaseIntelligentPointer() { }
    void SetPItem(CThing* item) { ++g_setItem; PData = item; }
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) { SetPItem(item); }
    virtual ~CIntelligentPointer() { }
};
struct ParticleAndEndPoints;
struct AttackParticleAndEndPoints;
class CTCBase { public:
    CTCBase(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
    virtual ~CTCBase() { }
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
};
class CTCObjectAugmentations : public CTCBase { public:
    CDefPointer<CObjectAugmentationsDef> PDef;
    bool          LoadedFromSaveGame;
    vector< CDefPointer<CThingBaseDef> > Augmentations;
    CSoundSetMap  InitSounds;
    CSoundSetMap  LoopingSounds;
    vector< vector<ParticleAndEndPoints> > ParticleEffects;
    vector< vector<AttackParticleAndEndPoints> > AttackParticleEffects;
    vector< CIntelligentPointer<CThing> > CurrentRunningAugEffect;
    long          CurrentRunningAugEffectIndex;
    long          CurPeriod;
    bool          InStrikeAction;
    CIntelligentPointer<CThing> ProjectileTrailEffect;
    CTCObjectAugmentations(CThing* thing);
    virtual ~CTCObjectAugmentations() { }
};

CTCObjectAugmentations::CTCObjectAugmentations(CThing* thing)
    : CTCBase(thing), LoadedFromSaveGame(false), CurrentRunningAugEffectIndex(0), CurPeriod(0), InStrikeAction(false),
      ProjectileTrailEffect(0)
{
    ParentThing->PDef->GetSubDef(PDef);
    Augmentations.resize(PDef.Object->NumberOfSlots);
}

int main()
{
    int bad = 0;
    CDefClassBase thingDef; CThing thing; thing.PDef = &thingDef; g_def.NumberOfSlots = 3;
    CTCObjectAugmentations a(&thing);
    if (a.ParentThing != &thing || a.PDef.Object != &g_def || g_subDefCalls != 1 || a.LoadedFromSaveGame) bad |= 1;
    if (g_resizeCount != 3 || g_resizeNull != 1) bad |= 2;
    if (a.Augmentations._M_start || a.ParticleEffects._M_finish || a.AttackParticleEffects._M_end_of_storage._M_data || a.CurrentRunningAugEffect._M_start) bad |= 4;
    SSoundSetNode* h = a.InitSounds._M_header._M_data;
    if (!h || h->_M_color || h->_M_parent || h->_M_left != h || h->_M_right != h || a.InitSounds._M_node_count != 0) bad |= 8;
    SSoundSetNode* h2 = a.LoopingSounds._M_header._M_data;
    if (!h2 || h2 == h || h2->_M_left != h2 || a.LoopingSounds._M_node_count != 0) bad |= 16;
    if (a.CurrentRunningAugEffectIndex != 0 || a.CurPeriod != 0 || a.InStrikeAction || a.ProjectileTrailEffect.PData != 0 || g_setItem != 1) bad |= 32;
    std::free(h); std::free(h2);
    if (bad) { std::printf("007673c0_TEST FAIL\n"); return 1; }
    std::printf("007673c0_TEST PASS\n");
    return 0;
}