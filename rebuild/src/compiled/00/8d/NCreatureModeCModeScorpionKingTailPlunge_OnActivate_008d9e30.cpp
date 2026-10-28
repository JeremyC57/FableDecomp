// NCreatureMode::CModeScorpionKingTailPlunge::OnActivate() -- arm the recovery / spike / spike-decision countdowns from the battle def (seconds x frames per second), attach-mode activation, find the EMITTER_TAIL dummy (kept at ground height) as the tail position, and spawn the TAIL_STRIKE_IDLE effect (the tail emitter) and the TAIL_STRIKE_TRAVEL effect (its particle emitter attached to itself; the dust trail) there
// Leaf: ?OnActivate@CModeScorpionKingTailPlunge
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D9E30, 689 bytes (`ret`).
// Callees: CDefClassBase::GetSubDef(CDefPointer<CScorpionKingBattleDef>*) 0x008C5A50 (manifest "GetSubDef<CAICreatureWillPowerIndicatorDef>"
//   is a byte-twin label), __ftol2 0x00BFEA70, CModeBase::OnActivate 0x0073AAC0, CThing::GetPos 0x004C73D0,
//   CTCGraphicAppearance::GetDummyObject 0x004C08C0, CCharString 0x0099EBF0 / ~ 0x0099EAE0, GetSubDef<CSpecialEffectsDef>
//   0x00669580, CSpecialEffectsDef::GetEffectFromString 0x00669D30, CreateEffectThing 0x006E0880 (__fastcall),
//   CBaseIntelligentPointer 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / ~ 0x00A01C10, CVectorMap::LowerBound
//   0x0040F020 (inline GetTC 0x4b), CTCDParticleEmitter::AttachToThing 0x006E0BE0; def release slot 1.
// Data: "EMITTER_TAIL" 0x01285A74, "TAIL_STRIKE_IDLE" 0x01285A60, "TAIL_STRIKE_TRAVEL" 0x01285A4C, "" 0x0122D70E,
//   the frames-per-second long 0x01375550, CIntelligentPointerThing vtable 0x01238C6C.
// Layout: Ego_r.pdb CModeScorpionKingTailPlunge (packed, as the landed Spike 0x008DA0F0): RecoveryCount +0x68,
//   DustEffectCounter +0x6c, SpikeTime +0x70, SpikeDecisionTime +0x74, Filter +0x78, PCreature +0x84, bRecovering +0x88,
//   TargetPos +0x89, TailPos +0x95, PTarget +0xa1, PTailEmitter +0xa9, PDustTrailEmitter +0xb1. Battle def floats +0x44/+0x48/+0x4c
//   named descriptively (the def's base sits below the PDB's). Thing: graphic +0x64, def +0x70, dead flag +0x91 bit 0.
// Shape: same-TU out-only stand-ins for both GetSubDef instantiations, GetEffectFromString and LowerBound (as Spike); both
//   defs released by hand without a null test on the battle def; the particle emitter is a named local so GetTC runs before
//   the "" temporary is built.
class C3DVector { public:
    float X, Y, Z;
};
class CRightHandedSet { public: C3DVector Up, Forward; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CDefBase {
    virtual ~CDefBase();                        // slot 0
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};
template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() : m_p(0) { }
};
class CSpecialEffectsDef : public CDefBase { public:
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const
{
    long found = Lookup_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}
#pragma auto_inline(on)
class CScorpionKingBattleDef : public CDefBase { public:
    unsigned char _pad08[0x44 - 0x08];
    float RecoveryTime;                         // +0x44
    float SpikeTime;                            // +0x48
    float SpikeDecisionTime;                    // +0x4c
};
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
    bool GetSubDef(CDefPointer<CScorpionKingBattleDef>* out);   // 0x008C5A50
};
void* Find_CDefClassBase_GetSubDef(const void* owner);
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (T*)def;
    return true;
}
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CScorpionKingBattleDef>* out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out->m_p = (CScorpionKingBattleDef*)def;
    return true;
}
#pragma auto_inline(on)
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
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
class CThing;
class CTCDParticleEmitter { public:
    void AttachToThing(CThing* thing, long boneIndex, const CCharString& boneName, long a, float b);   // 0x006E0BE0
};
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position,
                        CRightHandedSet* orientation, float scale);   // 0x004C08C0
};
class CThing { public:
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];            // +0x20
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* Graphic;              // +0x64
    unsigned char  _pad68[0x70 - 0x68];
    CDefClassBase* PDef;                        // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  m_flags91;                   // +0x91 bit 0: dead
    const C3DVector& GetPos() const;       // 0x004C73D0
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
inline bool IS_THING_ALIVE(const CThing* thing)
{
    if (thing && !(thing->m_flags91 & 1))
        return true;
    return false;
}
class CThingCreatureBase : public CThing { public: };
CThing* __fastcall CreateEffectThing(long type, const C3DVector& position, bool flag);   // 0x006E0880

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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { T* item = other.GetPItem(); SetPItem(item); return *this; }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

extern long g_framesPerSecond;             // 0x01375550

struct CModeBase {
    virtual ~CModeBase();
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x14];
    void OnActivate();                          // 0x0073AAC0
};
namespace NCreatureMode {
#pragma pack(push, 1)
class CModeScorpionKingTailPlunge : public CModeBase { public:
    unsigned char       _pad1c[0x68 - 0x1c];
    long                RecoveryCount;          // +0x68
    long                DustEffectCounter;      // +0x6c
    long                SpikeTime;              // +0x70
    long                SpikeDecisionTime;      // +0x74
    unsigned char       Filter[0xc];            // +0x78
    CThingCreatureBase* PCreature;              // +0x84
    bool                bRecovering;            // +0x88
    C3DVector           TargetPos;              // +0x89
    C3DVector           TailPos;                // +0x95
    CIntelligentPointer<CThing> PTarget;           // +0xa1
    CIntelligentPointer<CThing> PTailEmitter;      // +0xa9
    CIntelligentPointer<CThing> PDustTrailEmitter; // +0xb1
    void OnActivate();
};
#pragma pack(pop)

void CModeScorpionKingTailPlunge::OnActivate()
{
    CDefPointer<CScorpionKingBattleDef> def;
    ParentThing->PDef->GetSubDef(&def);
    RecoveryCount = (long)(g_framesPerSecond * def.m_p->RecoveryTime);
    SpikeTime = (long)(RecoveryCount - g_framesPerSecond * def.m_p->SpikeTime);
    SpikeDecisionTime = (long)(RecoveryCount - g_framesPerSecond * def.m_p->SpikeDecisionTime);
    CModeBase::OnActivate();
    C3DVector pos = ParentThing->GetPos();
    CRightHandedSet axes;
    PCreature->Graphic->GetDummyObject("EMITTER_TAIL", 0, 0, &pos, &axes, 1.0f);
    pos.Z = ParentThing->GetPos().Z;
    TailPos = pos;
    CDefPointer<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    ParentThing->PDef->GetSubDef(effectsSlot);
    CSpecialEffectsDef* effects = effectsSlot.m_p;
    if (effects) {
        bool found = effects->GetEffectFromString("TAIL_STRIKE_IDLE", effectType);
        if (found) {
            CThing* fx = CreateEffectThing(effectType, pos, false);
            if (IS_THING_ALIVE(fx))
                PTailEmitter = CIntelligentPointer<CThing>(fx);
        }
        found = effects->GetEffectFromString("TAIL_STRIKE_TRAVEL", effectType);
        if (found) {
            CThing* fx = CreateEffectThing(effectType, pos, false);
            if (IS_THING_ALIVE(fx)) {
                if (fx->HasTC(0x4b)) {
                    CTCDParticleEmitter* emitter = fx->GetTC<CTCDParticleEmitter>(0x4b);
                    emitter->AttachToThing(fx, 10, "", 0, 0.0f);
                }
                PDustTrailEmitter = CIntelligentPointer<CThing>(fx);
            }
        }
        if (--effects->m_refCount == 0)
            effects->Destroy();
    }
    if (--def.m_p->m_refCount == 0)
        def.m_p->Destroy();
}
}