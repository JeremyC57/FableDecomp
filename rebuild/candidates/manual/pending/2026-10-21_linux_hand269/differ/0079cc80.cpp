// Grade: DIFFER(496v512) -- zero-register gap only: retail keeps 0 in ebx (xor ebx,ebx; the slot/effectType stores, cmp esi/eax,ebx, push ebx) across the no-effect arm; ours uses immediates (same code otherwise)
// CTCBulletTime::PlayEffect(CIntelligentPointer<CThing>* effect, const CCharString& name, CThingCreatureBase* target, long attachType) -- spawn the named special effect at the target and attach its particle emitter to the target (keeping the effect thing in *effect when asked)
// Retail 0x0079CC80, 510 bytes (`ret 10h`). Ego_r.pdb: ?PlayEffect@CTCBulletTime@@AAEXPAV?$CIntelligentPointer@VCThing@@@@ABVCCharString@@PAVCThingCreatureBase@@J@Z.
// Callees: CDefClassBase::GetSubDef<CSpecialEffectsDef> @ 0x00669580 (on the bullet-time def +0x20),
// CSpecialEffectsDef::GetEffectFromString @ 0x00669D30, CThing::GetPosition @ 0x004C73D0, CreateParticleEffect @ 0x0072AB70
// (__fastcall), LowerBound @ 0x0040F020 (inlined GetInterface, CTCDParticleEmitter = 0x4b, result unchecked),
// CCharString(const char*, long) @ 0x0099EBF0 ("" @ 0x0122D70E), CTCDParticleEmitter::AttachToThing @ 0x006E0BE0,
// ~CCharString @ 0x0099EAE0, CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor
// 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C), the effects def's slot 1 (destroy).
// Layout: engine/CTCBulletTime.h PDef_Object +0x20.
struct CTCBase;
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CDefVirtual {
    virtual ~CDefVirtual();
    virtual void Destroy();                           // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                                  // +0x04
};
struct CSpecialEffectsDef : CDefBase {
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
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (m_p && --m_p->m_refCount == 0) m_p->Destroy(); }
};
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580
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
#pragma auto_inline(on)
class CThing;
class CTCDParticleEmitter {
public:
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                  // +0x20
    CInterfaceMap  m_interfaces;                      // +0x44
    unsigned char  _pad4c[0x91 - 0x4c];
    unsigned char  m_flags91;                         // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;             // 0x004C73D0
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { };
CThing* __fastcall CreateParticleEffect(long type, const C3DVector& position);   // 0x0072AB70
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                        // 0x00A01B10
    ~CBaseIntelligentPointer();                       // 0x00A01C10
    CThing* GetPItem() const;                         // 0x00A01B50
    void SetPItem(CThing* item);                      // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C for <CThing>
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CTCBulletTime {
    virtual void v00();
    unsigned char _pad04[0x20 - 0x04];
    CDefClassBase* PDef;                              // +0x20 CDefPointer<const CSpecialAbilitiesBulletTimeDef>
    void PlayEffect(CIntelligentPointer<CThing>* effect, const CCharString& name, CThingCreatureBase* target, long attachType);
};

void CTCBulletTime::PlayEffect(CIntelligentPointer<CThing>* effect, const CCharString& name, CThingCreatureBase* target, long attachType)
{
    CDefPointer<CSpecialEffectsDef> effects;
    long effectType = 0;
    bool found = PDef->GetSubDef(effects);
    if (effect == 0) {
        if (found && effects.m_p->GetEffectFromString(name, effectType)) {
            CThing* thing = CreateParticleEffect(effectType, target->GetPosition());
            if (thing && !(thing->m_flags91 & 1)) {
                CTCDParticleEmitter* emitter;
                thing->GetInterface(&emitter, 0x4b);
                emitter->AttachToThing(*target, attachType, "", 0, 0);
            }
        }
    } else {
        if (found && effects.m_p->GetEffectFromString(name, effectType)) {
            CThing* thing = CreateParticleEffect(effectType, target->GetPosition());
            *effect = CIntelligentPointer<CThing>(thing);
            if (IS_THING_ALIVE(*effect)) {
                CTCDParticleEmitter* emitter;
                effect->GetPItem()->GetInterface(&emitter, 0x4b);
                emitter->AttachToThing(*target, attachType, "", 0, 0);
            }
        }
    }
}
