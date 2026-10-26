// CCombatActionBase::UpdateActionRunningTime() -- while a scripted camera runs on a camera point, advance the running time scaled by the point's speed and drive bullet time; otherwise the base update
// Leaf: ?UpdateActionRunningTime@CCombatActionBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x008579D0, 416 bytes, plain `ret` (virtual).
// Callees: CCreatureActionBase::GetPWorld @ 0x00692940, the current game camera (world +0x34) slot 9 (+0x24, its camera
//   manager by out pointer), CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CGameCameraManager::IsUsingScriptedCameraMode
//   @ 0x00697AE0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound (const copy) @ 0x004365B0, the interface-0x3F component
//   slot 28 (+0x70, its point type), virtual GetAnimDelayTime (slot 6) / GetAnimIndex (slot 7),
//   CTCAnimationComplex::GetAnimDuration(long) @ 0x006627F0 (result dropped), CBulletTimeManager::IsTimeForFullUpdate
//   @ 0x004C6060, SetBulletTimeSlowdownMultiplier(long, CCombatActionBase*, float) @ 0x004C6170, __ftol2 @ 0x00BFEA70,
//   CCreatureActionBase::UpdateActionRunningTime @ 0x00692480.
// Globals: frames per second @ 0x01375550; float pool 1.0f @ 0x0122DED8, 1e-4f @ 0x0129BA3C.
// Layout (retail): CCreatureActionBase is 0xA8 bytes: PCreature +0x08, RunningTimeSeconds +0x40,
//   LastRunningTimeSeconds +0x44; CCombatActionBase PCameraPoint +0xB0 (PDB 0xBC - 0x0C), the strike-interrupt time
//   +0xC4 (PDB StrikeEventInterruptTime 0xD8 - 0x14); CWorld PCurrentGameCamera +0x34, bullet-time manager +0x68;
//   CThing dead flag byte +0x91 bit 0, interface bitset +0x20, interface map +0x44.
// Shape: the point component is fetched through an out-parameter GetInterface left unset when absent; the scripted
//   point is a second uninitialised local in the same frame word.
// Shape (LowerBound): the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation is defined in this TU (noinline),
//   as retail compiled it in the caller's TU; with its body visible VC7.1 packs the lookup keys and temporaries as retail.
class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;     // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)


class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
    template <class T> __forceinline void GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *out = (T*)(it == map->last || type < it->type ? map->last : it)->component;
        }
    }
};

struct CTCCameraPoint {                             // interface 0x3F component
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27();
    virtual long GetPointType();                    // slot 28 (+0x70)
    unsigned char _pad04[0xbc - 4];
    float m_speed;                                  // +0xbc
};

class CGameCameraManager { public:
public:
    bool IsUsingScriptedCameraMode() const;   // 0x00697AE0
};

struct CGameCamera {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** out);   // slot 9 (+0x24)
};

class CBulletTimeManager { public:
    unsigned char _pad0[4];
    long m_active;                                  // +0x04
    unsigned char _pad8[4];
    bool m_slowing;                                 // +0x0c
    __forceinline bool IsRunning() const { return m_slowing || m_active != 0; }
    bool IsTimeForFullUpdate();                     // 0x004C6060
    void SetBulletTimeSlowdownMultiplier(long frames, class CCombatActionBase* source, float multiplier);   // 0x004C6170
};

class CWorld { public:
    unsigned char _pad0[0x34];
    CGameCamera* PCurrentGameCamera;                // +0x34
    unsigned char _pad38[0x68 - 0x38];
    CBulletTimeManager* PBulletTime;                // +0x68
    CGameCamera* GetCurrentGameCamera() const { return PCurrentGameCamera; }
    CBulletTimeManager* GetBulletTime() const { return PBulletTime; }
};

class CThingCreatureBase { public:
    float GetAnimDuration(long animIndex);          // 0x006627F0
};

struct CBaseIntelligentPointer {
    void* __vftable;
    void* m_pItem;
    CThing* GetPItem() const;                       // 0x00A01B50
};
struct CCreaturePointer {
    void* __vftable;
    void* m_pItem;
    CThingCreatureBase* GetPItem() const;           // 0x00A01B50
};

extern long g_framesPerSecond;                      // 0x01375550

class CCreatureActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual long GetAnimDelayTime();                // slot 6 (+0x18)
    virtual long GetAnimIndex();                    // slot 7 (+0x1c)
    unsigned char _pad04[4];
    CCreaturePointer PCreature;                     // +0x08
    unsigned char _pad10[0x40 - 0x10];
    float RunningTimeSeconds;                       // +0x40
    float LastRunningTimeSeconds;                   // +0x44
    unsigned char _pad48[0xa8 - 0x48];
    CWorld* GetPWorld();                            // 0x00692940
    void UpdateActionRunningTime();                 // 0x00692480
};

class CCombatActionBase : public CCreatureActionBase { public:
    unsigned char _pada8[8];
    CBaseIntelligentPointer PCameraPoint;           // +0xb0
    unsigned char _padb8[0xc4 - 0xb8];
    float m_scriptedTime;                           // +0xc4
    void UpdateActionRunningTime();
};

void CCombatActionBase::UpdateActionRunningTime()
{
    CGameCameraManager* cameraManager;
    if (GetPWorld()->GetCurrentGameCamera()->GetCameraManager(&cameraManager)
        && PCameraPoint.GetPItem() != 0
        && !(PCameraPoint.GetPItem()->m_flags91 & 1)
        && cameraManager->IsUsingScriptedCameraMode()) {
        CTCCameraPoint* point;
        PCameraPoint.GetPItem()->GetInterface(0x3f, &point);
        CTCCameraPoint* scripted;
        if (point->GetPointType() == 7)
            scripted = point;
        float speed = scripted->m_speed;
        long framesPerSecond = g_framesPerSecond;
        float fps = (float)framesPerSecond;
        float delay = (float)GetAnimDelayTime() / fps;
        CThingCreatureBase* creature = PCreature.GetPItem();
        creature->GetAnimDuration(GetAnimIndex());
        RunningTimeSeconds = (m_scriptedTime - delay) * speed + delay;
        float step = RunningTimeSeconds - LastRunningTimeSeconds;
        float frameTime = 1.0f / framesPerSecond;
        if (step > 0.0001f) {
            if (!GetPWorld()->PBulletTime->IsRunning() || GetPWorld()->PBulletTime->IsTimeForFullUpdate()) {
                long frames = (long)(1.0f / (step / frameTime));
                if (frames < 1)
                    frames = 1;
                GetPWorld()->GetBulletTime()->SetBulletTimeSlowdownMultiplier(frames, this, 0.5f);
            }
        }
        return;
    }
    CCreatureActionBase::UpdateActionRunningTime();
}