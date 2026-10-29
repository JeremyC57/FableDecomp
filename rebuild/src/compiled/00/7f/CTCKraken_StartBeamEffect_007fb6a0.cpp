// CTCKraken::StartBeamEffect() -- spawn the BEAM_ATTACK effect at the head's MOUTH_DUMMY and attach its particle emitter to the kraken
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FB6A0, length 457 bytes (+ int3 padding), esp frame, plain `ret`.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CTCGraphicAppearance::GetDummyObject @
// 0x004C08C0, CDefClassBase::GetSubDef<CSpecialEffectsDef> @ 0x00669580, CSpecialEffectsDef::GetEffectFromString @ 0x00669D30
// (both same-TU noinline stand-ins, as the landed CCreatureActionJackFloatUp::PerformAction 0x008C0FB0), CreateEffectThing @
// 0x006E0880 (`__fastcall`), CVectorMap::LowerBound @ 0x0040F020 (inline GetInterface 0x4B = CTCDParticleEmitter, bitset word
// +0x28 bit 11), CTCDParticleEmitter::AttachToThing @ 0x006E0BE0, CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 /
// dtor @ 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C).
// Strings: "MOUTH_DUMMY" @ 0x0126FA88, "BEAM_ATTACK" @ 0x0127203C.
// Layout (local model): retail CTCKraken ParentThing +0x04, PBeamWeaponEffect +0x34, KillBeamCounter +0x58 (PDB +0x3C / +0x68:
// the two STLport lists before them are 4 bytes in retail, 12 in the PDB). CThing GraphicAppearance +0x64, def +0x70, dead bit
// +0x91. The emitter attaches with type 0x2A at MOUTH_DUMMY.
// Shape: the def pointer and the zeroed effect type are declared BEFORE the GetDummyObject call (their stores lead the frame);
// the sub-def / effect lookup is one `&&` condition over the CDefClassReference and string temporaries (the ebx 1/3 flags and
// the bool in [esp+0Fh]); the new effect is assigned through a CIntelligentPointer temporary whose operator= names the item
// in a local (an inline `SetPItem(other.GetPItem())` schedules the `lea esi,[edi+34h]` before GetPItem); then IS_THING_ALIVE
// (IsDead() member form) and the out-parameter GetInterface.
class CTCBase;
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector First; C3DVector Second; };
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
struct CDefVirtual {
    virtual ~CDefVirtual();                           // slot 0
    virtual void Destroy();                           // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                                  // +0x04
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
template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (m_p && --m_p->m_refCount == 0) m_p->Destroy(); }
    T* operator->() const { return m_p; }
};
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
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
struct CDefClassReference {                           // a counted CDefClassBase reference (AddRef / Release)
    CDefClassBase* m_p;
    CDefClassReference(CDefClassBase* p) : m_p(p) { if (m_p) ++m_p->m_refCount; }
    ~CDefClassReference() { if (m_p && --m_p->m_refCount == 0) m_p->Destroy(); }
    CDefClassBase* operator->() const { return m_p; }
};
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long zero1, unsigned long zero2, C3DVector* position,
                        CRightHandedSet* orientation, float scale);   // 0x004C08C0
};
class CThing;
class CTCDParticleEmitter { public:                          // interface 0x4B
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                  // +0x20 interface-present bitset
    CInterfaceMap  m_interfaces;                      // +0x44
    unsigned char  _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* PGraphicAppearance;         // +0x64
    unsigned char  _pad68[0x70 - 0x68];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  m_flags91;                         // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
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
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        T* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }

public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

class CTCKraken { public:
    void*               __vftable;                    // +0x00
    CThing*             ParentThing;                  // +0x04
    unsigned char       _pad08[0x34 - 0x08];
    CIntelligentPointer<CThing> PBeamWeaponEffect;            // +0x34
    void StartBeamEffect();
};

void CTCKraken::StartBeamEffect()
{
    CDefPointer<CSpecialEffectsDef> effects;
    long effectType = 0;
    C3DVector mouthPosition;
    CRightHandedSet orientation;
    ParentThing->PGraphicAppearance->GetDummyObject("MOUTH_DUMMY", 0, 0, &mouthPosition, &orientation, 1.0f);
    if (CDefClassReference(ParentThing->m_pDef)->GetSubDef(effects) &&
        effects->GetEffectFromString("BEAM_ATTACK", effectType)) {
        PBeamWeaponEffect = CIntelligentPointer<CThing>(CreateEffectThing(effectType, &mouthPosition, false));
        if (IS_THING_ALIVE(PBeamWeaponEffect)) {
            CTCDParticleEmitter* emitter;
            if (PBeamWeaponEffect.GetPItem()->GetInterface(&emitter, 0x4b))
                emitter->AttachToThing(ParentThing, 0x2a, "MOUTH_DUMMY", 0, 0.0f);
        }
    }
}