// CTCBerserk::CreateStartingEffect() -- look up the berserk def's "STARTUP_EFFECT" particle, create it at the thing's position, keep it as StartUpEffect and attach its emitter to the thing
// Leaf: CreateStartingEffect
// Retail 0x0078C4C0, 336 bytes, `ret`.
// Callees: CDefClassBase::GetSubDef<T>(CDefPointer<T>*) 0x00669580, the sub-def's named-effect lookup (const CCharString&,
// long*) 0x00669D30, CCharString(const char*, long) 0x0099EBF0 ("STARTUP_EFFECT" 0x0126BC64, "" 0x0122D70E), ~CCharString
// 0x0099EAE0, CThing::GetPos 0x004C73D0, CreateParticleEffect(long, const C3DVector&) 0x0072AB70 (`__fastcall`),
// CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10 (vtable 0x01238C6C),
// CVectorMap::LowerBound 0x0040F020 (interface 0x4B), CTCDParticleEmitter::AttachToThing 0x006E0BE0; the def's slot 1 release.
// Layout: engine/CTCBerserk.h is the PDB layout with a 16-byte DependentThings vector; retail sits 4 lower after it
// (StartUpEffect +0x58, PDef +0x68), so a local model is used; ParentThing +4.
// Advisor fix (Fable 5.1): the GetEffect stand-in. Shape: same-TU auto_inline(off) stand-ins for GetSubDef AND GetEffect (with GetEffect only declared, the &effect out-param
// counts as escaped and its slot stays live, pushing the key/""/emitter homes into the dead def slot); the out CDefPointer and
// the effect id are zeroed together at entry; the sub-def is a CDefPointer with an inline releasing dtor that VC7.1 keeps in
// ebp thanks to the GetSubDef stand-in (`dec [ebp+4] ; jne`); found and effect lookups are nested ifs; the effect name is a temporary in the dead out
// slot; StartUpEffect is assigned from an intelligent-pointer temporary; the emitter is the out-parameter GetInterface.
class CTCBase;
class CBaseObject;
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags;                              // +0x91 bit 0: dead
    const C3DVector& GetPos() const;               // 0x004C73D0
    bool IsDead() const { return (m_flags & 1) != 0; }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
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
    virtual ~CBaseIntelligentPointer();                         // 0x00A01C10
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = ((CThing*)other.GetPItem());
        SetPItem((CBaseObject*)item);
        return *this;
    }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
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
class CDefClassBase : public CDefPointeeBase { public:
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
class CTCBerserk {
public:
    void*                        __vftable;             // +0x00
    CThing*                      ParentThing;           // +0x04
    unsigned char                _pad08[0x58 - 0x08];
    CIntelligentPointer<CThing>  StartUpEffect;         // +0x58
    CIntelligentPointer<CThing>  BerserkBodyEffect;     // +0x60
    CDefClassBase*               PDef;                  // +0x68 (CDefPointer Object)
    void CreateStartingEffect();
};

void CTCBerserk::CreateStartingEffect()
{
    CDefPointer<CBerserkEffectsDef> def;
    long effect = 0;
    if (PDef->GetSubDef(&def)) {
        if (def->GetEffect("STARTUP_EFFECT", &effect)) {
            StartUpEffect = CIntelligentPointer<CThing>(CreateParticleEffect(effect, ParentThing->GetPos()));
            if (IS_THING_ALIVE(StartUpEffect)) {
                CTCDParticleEmitter* emitter;
                ((CThing*)StartUpEffect.GetPItem())->GetInterface(&emitter, 0x4b);
                emitter->AttachToThing(ParentThing, 0x28, "", 0, 0.0f);
            }
        }
    }
}