// CTCBerserk::CancelMove() -- cancel the berserk move: when the creature's berserk state component is in state 8 and the ability is not initialised, reset it, kill the start-up effect, spawn the def's "BERSERK_EXIT_EFFECT" and remove TC 0x84; otherwise rescale or clamp the timer
// Leaf: CancelMove
// Retail 0x0078C610, 368 bytes, `ret`. Same TU as CreateStartingEffect 0x0078C4C0 (shared model).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (interface 0xA7), CBaseIntelligentPointer::GetPItem
// 0x00A01B50, CThing::Kill(bool) 0x004C9B80, CDefClassBase::GetSubDef<T> 0x00669580, the effects def's named lookup
// 0x00669D30, CCharString(const char*, long) 0x0099EBF0 ("BERSERK_EXIT_EFFECT" 0x0126BC74), ~CCharString 0x0099EAE0,
// CThing::GetPosition 0x004C73D0, CreateParticleEffect 0x0072AB70 (`__fastcall`), CThing::RemoveTC(long) 0x004C9840,
// __ftol2 0x00BFEA70; the def's slot 1 release.
// Layout: retail CTCBerserk (local model, see CreateStartingEffect): Timer +0x0C, ReleasedAnimationTimer +0x1C (float),
// AnimTimer +0x20, AbilityInitialised +0x49, StartUpEffect +0x58, PDef +0x68 (max timer +0x40); the 0xA7 component's
// active flag +0x0C and state +0x10.
// Shared-TU model with CreateStartingEffect 0x0078C4C0 (same GetSubDef/GetEffect stand-ins). Shape: HasTC(0xA7) is a sign test of the bitset byte; the exit-effect test is `GetSubDef(&def) && def->GetEffect("...",
// &effect)` with a conditional CCharString temporary (flag zeroed at entry); the def is a CDefPointer released by its inline
// dtor (register-held thanks to the same-TU GetSubDef stand-in) and the berserk def is a named local read before it (so
// `mov ecx,[esi+68h]` precedes the out address); IS_THING_ALIVE must be `__forceinline` here; the anim timer is converted
// once into a float local and the rescale is written `max * (t / released)` (VC7.1 keeps `fdiv ; fimul`; `t / released *
// max` is reassociated into `fild max ; fdiv ; fmul`).
class CTCBase;
class CBaseObject;
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags;                              // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;               // 0x004C73D0
    void Kill(bool immediately);                        // 0x004C9B80
    void RemoveTC(long type);                           // 0x004C9840
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    bool IsDead() const { return (m_flags & 1) != 0; }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
CThing* __fastcall CreateParticleEffect(long effect, const C3DVector& position);   // 0x0072AB70
class CTCDParticleEmitter { public:
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
class CDefPointeeBase { public:
    virtual void Destroy();                             // slot 0
    virtual void Delete();                              // slot 1 (+0x04)
    long m_refCount;                                    // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
template <class T> struct CDefHolder {
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
struct CBerserkEffectsDef : CDefPointeeBase {
    long FindEffect(const CCharString& name);
    bool GetEffect(const CCharString& name, long* effect);   // 0x00669D30
};
struct CBerserkDef : CDefPointeeBase {
    unsigned char _pad08[0x40 - 0x08];
    long          MaxTimer;                             // +0x40
};
class CDefClassBase : public CBerserkDef { public:
    CDefPointeeBase* FindSubDef();
    bool GetSubDef(CDefPointer<CBerserkEffectsDef>* out);     // 0x00669580
};
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef<CBerserkEffectsDef> (its instantiation lives in this TU in retail): VC7.1 then knows the
// out pointer does not escape, keeps the def in a register and reuses the out slot.
bool CDefClassBase::GetSubDef(CDefPointer<CBerserkEffectsDef>* out)
{
    CDefPointeeBase* p = FindSubDef();
    out->Object = (CBerserkEffectsDef*)p;
    if (p)
        ++p->m_refCount;
    return p != 0;
}
bool CBerserkEffectsDef::GetEffect(const CCharString& name, long* effect)
{
    long e = FindEffect(name);
    *effect = e;
    return e != 0;
}
#pragma auto_inline(on)
struct CTCBerserkState {
    unsigned char _pad00[0x0c];
    bool          Active;                               // +0x0c
    long          State;                                // +0x10
};
class CTCBerserk {
public:
    void*                        __vftable;             // +0x00
    CThing*                      ParentThing;           // +0x04
    unsigned char                _pad08[0x0c - 0x08];
    long                         Timer;                 // +0x0c
    long                         DarkenEnvTimer;        // +0x10
    float                        StrengthPercentage;    // +0x14
    float                        MaxIncreaseInStrength; // +0x18
    float                        ReleasedAnimationTimer; // +0x1c
    long                         AnimTimer;             // +0x20
    unsigned char                _pad24[0x48 - 0x24];
    bool                         JustReleased;          // +0x48
    bool                         AbilityInitialised;    // +0x49
    unsigned char                _pad4a[0x58 - 0x4a];
    CIntelligentPointer<CThing>  StartUpEffect;         // +0x58
    CIntelligentPointer<CThing>  BerserkBodyEffect;     // +0x60
    CDefClassBase*               PDef;                  // +0x68 (CDefPointer Object)
    void CancelMove();
};

void CTCBerserk::CancelMove()
{
    CThing* thing = ParentThing;
    if (!thing->HasTC(0xa7))
        return;
    CTCBerserkState* state = thing->GetTC<CTCBerserkState>(0xa7);
    if (!AbilityInitialised && state->State == 8) {
        state->Active = false;
        state->State = 0;
        if (IS_THING_ALIVE(StartUpEffect))
            StartUpEffect.GetPItem()->Kill(true);
        CDefClassBase* berserkDef = PDef;
        CDefPointer<CBerserkEffectsDef> def;
        long effect = 0;
        if (berserkDef->GetSubDef(&def) && def->GetEffect("BERSERK_EXIT_EFFECT", &effect))
            CreateParticleEffect(effect, thing->GetPosition());
        thing->RemoveTC(0x84);
    } else {
        float animTimer = (float)AnimTimer;
        if (animTimer < ReleasedAnimationTimer)
            Timer = (long)(PDef->MaxTimer * (animTimer / ReleasedAnimationTimer));
        else if (Timer > PDef->MaxTimer)
            Timer = PDef->MaxTimer;
    }
}