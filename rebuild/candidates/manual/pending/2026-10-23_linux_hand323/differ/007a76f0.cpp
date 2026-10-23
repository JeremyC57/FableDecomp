// CTCBattleCharge::CreateStartUpEffect() -- make the charge noise, then from the charge def's effects create the level's shield effect (facing the override direction, the target or the thing's forward) and the left/right feet effects, each kept in its intelligent pointer and attached to the charging thing
// Grade: DIFFER(1237v1239) -- whole flow in retail order; retail frame is 0x40 v our 0x34 (a separate named direction local beside the orientation), the thing is homed in a stack slot (ebx then holds the found flags), and the def/effect out slots are swapped; with the same-TU GetSubDef/GetEffect stand-ins dropped the diff grows (1232v1239)
// Leaf: CreateStartUpEffect
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A76F0, 1248 bytes, `ret`.
// Literals: "SIMPLE_NOISE" 0x01268A5C, "SHIELD_EFFECT_01".."_04" 0x0126CCD0/0x0126CC94/0x0126CCA8/0x0126CCBC, "LEFT_FEET_EFFECT" 0x0126CC80,
//   "RIGHT_FEET_EFFECT" 0x0126CC6C, "" 0x0122D70E. Model taken from the landed CTCBerserk::CreateStartingEffect 0x0078C4C0.
struct CTCBase;
struct CBaseObject;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags;                              // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;               // 0x004C73D0
    void PlaySoundCriteria(const CCharString& name);    // 0x00662110
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
CThing* __fastcall CreateParticleEffect(long effect, const C3DVector& position, long flags);   // 0x006E0880
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
    void SetOrientation(const struct SOrientation& orientation);   // 0x006E0480
    void SetActive(bool active);                       // 0x006E0590
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
struct CDefPointeeBase {
    virtual void Destroy();                             // slot 0
    virtual void Delete();                              // slot 1 (+0x04)
    long m_refCount;                                    // +0x04
};
template <class T> struct CDefPointer {
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
struct CDefClassBase : CDefPointeeBase {
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
struct SOrientation {
    C3DVector Up; C3DVector Forward;
    SOrientation(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) { }
};
struct CThingController {
    virtual void s000(); virtual void s004(); virtual void s008(); virtual void s00c(); virtual void s010(); virtual void s014(); virtual void s018(); virtual void s01c();
    virtual void s020(); virtual void s024(); virtual void s028(); virtual void s02c(); virtual void s030(); virtual void s034(); virtual void s038(); virtual void s03c();
    virtual void s040(); virtual void s044(); virtual void s048(); virtual void s04c(); virtual void s050(); virtual void s054(); virtual void s058(); virtual void s05c();
    virtual void s060(); virtual void s064(); virtual void s068(); virtual void s06c(); virtual void s070(); virtual void s074(); virtual void s078(); virtual void s07c();
    virtual void s080(); virtual void s084(); virtual void s088(); virtual void s08c(); virtual void s090(); virtual void s094(); virtual void s098(); virtual void s09c();
    virtual void s0a0(); virtual void s0a4(); virtual void s0a8(); virtual void s0ac(); virtual void s0b0(); virtual void s0b4(); virtual void s0b8(); virtual void s0bc();
    virtual void s0c0(); virtual void s0c4(); virtual void s0c8(); virtual void s0cc(); virtual void s0d0(); virtual void s0d4(); virtual void s0d8(); virtual void s0dc();
    virtual void s0e0(); virtual void s0e4(); virtual void s0e8(); virtual void s0ec(); virtual void s0f0(); virtual void s0f4(); virtual void s0f8(); virtual void s0fc();
    virtual void s100(); virtual void s104(); virtual void s108(); virtual void s10c(); virtual void s110(); virtual void s114();
    virtual C3DVector GetForward() const;               // +0x118
};
struct CThingWithController : CThing { };
#pragma pack(push, 1)
class CTCBattleCharge {
public:
    void*                        __vftable;             // +0x00
    CThing*                      ParentThing;           // +0x04
    unsigned char                _pad08[0x0c - 0x08];
    CIntelligentPointer<CThing>  Target;                // +0x0c
    CDefClassBase*               PDef;                  // +0x14
    unsigned char                _pad18[0x2e - 0x18];
    bool                         UseDirectionOverride;  // +0x2e
    C3DVector                    DirectionOverride;     // +0x2f
    unsigned char                _pad3b[0x40 - 0x3b];
    long                         Level;                 // +0x40
    unsigned char                _pad44[0x50 - 0x44];
    CIntelligentPointer<CThing>  ShieldEffect;          // +0x50
    CIntelligentPointer<CThing>  LeftFeetEffect;        // +0x58
    CIntelligentPointer<CThing>  RightFeetEffect;       // +0x60
    unsigned char                _pad68[0x88 - 0x68];
    bool                         Charging;              // +0x88
    void CreateStartUpEffect();
};
#pragma pack(pop)
static inline CThingController* ControllerOf(CThing* thing) { return *(CThingController**)((char*)thing + 0x60); }

void CTCBattleCharge::CreateStartUpEffect()
{
    CThing* thing = ParentThing;
    thing->PlaySoundCriteria("SIMPLE_NOISE");
    long effect = 0;
    CDefPointer<CBerserkEffectsDef> def;
    if (PDef->GetSubDef(&def)) {
        C3DVector dir;
        if (UseDirectionOverride && !Charging)
            dir = DirectionOverride;
        else if (IS_THING_ALIVE(Target)) {
            CThing* target = Target.GetPItem();
            const C3DVector& from = thing->GetPosition();
            const C3DVector& to = target->GetPosition();
            dir = C3DVector(to.X - from.X, to.Y - from.Y, to.Z - from.Z);
        } else
            dir = ControllerOf(thing)->GetForward();
        SOrientation orientation(C3DVector(0.0f, 0.0f, 1.0f), dir);
        CCharString name("SHIELD_EFFECT_01");
        switch (Level) {
        case 1: name = "SHIELD_EFFECT_02"; break;
        case 2: name = "SHIELD_EFFECT_03"; break;
        case 3: name = "SHIELD_EFFECT_04"; break;
        }
        if (def->GetEffect(name, &effect)) {
            ShieldEffect = CIntelligentPointer<CThing>(CreateParticleEffect(effect, thing->GetPosition(), 0));
            if (IS_THING_ALIVE(ShieldEffect)) {
                CTCDParticleEmitter* emitter = 0;
                ShieldEffect.GetPItem()->GetInterface(&emitter, 0x4b);
                emitter->AttachToThing(thing, 0x28, "", 0, 0.0f);
                emitter->SetOrientation(orientation);
                emitter->SetActive(true);
            }
        }
        bool foundL = def->GetEffect("LEFT_FEET_EFFECT", &effect);
        if (foundL) {
            LeftFeetEffect = CIntelligentPointer<CThing>(CreateParticleEffect(effect, ParentThing->GetPosition(), 0));
            if (IS_THING_ALIVE(LeftFeetEffect)) {
                CTCDParticleEmitter* emitter = 0;
                LeftFeetEffect.GetPItem()->GetInterface(&emitter, 0x4b);
                emitter->AttachToThing(thing, 8, "", 0, 0.0f);
                emitter->SetOrientation(orientation);
                emitter->SetActive(true);
            }
        }
        bool foundR = def->GetEffect("RIGHT_FEET_EFFECT", &effect);
        if (foundR) {
            RightFeetEffect = CIntelligentPointer<CThing>(CreateParticleEffect(effect, ParentThing->GetPosition(), 0));
            if (IS_THING_ALIVE(RightFeetEffect)) {
                CTCDParticleEmitter* emitter = 0;
                RightFeetEffect.GetPItem()->GetInterface(&emitter, 0x4b);
                emitter->AttachToThing(thing, 8, "", 0, 0.0f);
                emitter->SetOrientation(orientation);
                emitter->SetActive(true);
            }
        }
    }
}
