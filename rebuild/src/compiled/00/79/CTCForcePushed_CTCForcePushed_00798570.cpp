// CTCForcePushed::CTCForcePushed(CThing& parent) -- constructor: CTCBase, empty pusher/particle pointers, not initialised, no def, no emitters; a live AI-creature owner has its animation suspended; then load SPECIAL_ABILITIES_FORCE_PUSH_DEF
// Leaf: ??0CTCForcePushed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00798570, length 170 bytes (+ int3 padding). `ret 4`; vtable 0x0126C09C.
//         Ego_r.pdb: `??0CTCForcePushed@@QAE@AAVCThing@@@Z`.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800; CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (x2,
//          CIntelligentPointer<CThing> vtable 0x01238C6C); CThingAnimatable::SetSuspendAnimation(bool, long) @ 0x00833A10;
//          CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0; CGameDefinitionManager::Get @ 0x0044C6B0
//          (__fastcall, twice); CDefinitionManager::GetGlobalIndexFromClassIndex(const CCharString&, long) const @ 0x009AD390;
//          CDefinitionManager::GetDef<CSpecialAbilitiesForcePushDef>(long, CDefPointer<const ..>*) const @ 0x00798C10.
// Shape: the members with inline ctors follow the vtable store in declaration order (one eax zero for the flag, the def
//        pointer and the STLport vector); the constant 1 lives in ebx (dead-bit test, AI-class compare, both `true`/`1`
//        pushes); the def name is a named CCharString in its own block, homed in the dead `parent` slot.
// Layout: Ego_r.pdb CTCForcePushed matches retail up to PDef +0x40; Emitters_created is a 12-byte STLport vector at +0x44;
//         retail CThing ThingClass +0x10 (1 = AI creature), Dead +0x91 bit 0.
class CThing;
class CBaseObject;
class CTCBase;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

template <class T> struct SRetailAllocProxy {       // STLport _STLP_alloc_proxy
    T* _M_data;
    SRetailAllocProxy(T* data) : _M_data(data) {}
};
template <class T> struct CRetailVector {           // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    SRetailAllocProxy<T> _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};

class CSpecialAbilitiesForcePushDef {
public:
    virtual void v00();
    long RefCount;                                  // +0x04
    unsigned char _pad08[0x98 - 0x08];
    CRetailVector<float> TimeDelayBetweenCastsSec;  // +0x98
};

template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
    T* operator->() const { return Object; }
};

class CDefinitionManager {
public:
    long GetGlobalIndexFromClassIndex(const CCharString& className, long classIndex) const;   // 0x009AD390
    template<class T> bool GetDef(long index, CDefPointer<const T>* out) const;            // 0x00798C10 for the force-push def
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();                                     // 0x0044C6B0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() {}
};

class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char ThingClass;                       // +0x10 (1: AI creature)
    unsigned char _pad11[0x20 - 0x11];
    unsigned char _pad20[0x91 - 0x20];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
};
class CThingAnimatable : public CThing {
public:
    void SetSuspendAnimation(bool suspend, long reason);   // 0x00833A10
};

class CTCBase {
public:
    CTCBase(CThing* parent);                        // 0x00686800
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    unsigned char _pad08[0x4];                      // ToBeRemoved, IsInGlobalUpdateTCs
};

class CTCForcePushed : public CTCBase {             // vtable 0x0126C09C
public:
    CIntelligentPointer<CThing> PThingCausingForcePush;     // +0x0C
    CIntelligentPointer<CThing> PParticle;          // +0x14
    float PositionForcePushStarted[3];              // +0x1C C3DVector
    float DistanceBeingPushed;                      // +0x28
    float DirectionBeingPushed[3];                  // +0x2C C3DVector
    float DistanceMovingPerFrame;                   // +0x38
    bool Initialised;                               // +0x3C
    bool StartedMoving;                             // +0x3D
    CDefPointer<const CSpecialAbilitiesForcePushDef> PDef;  // +0x40
    CRetailVector<CThing*> Emitters_created;        // +0x44
    CTCForcePushed(CThing& parent);
    virtual void v00();
};

CTCForcePushed::CTCForcePushed(CThing& parent)
    : CTCBase(&parent), Initialised(false)
{
    if (ParentThing && !(ParentThing->m_flags91 & 1) && ParentThing->ThingClass == 1)
        ((CThingAnimatable*)ParentThing)->SetSuspendAnimation(true, 0);
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_FORCE_PUSH_DEF", -1);
        index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &PDef);
}