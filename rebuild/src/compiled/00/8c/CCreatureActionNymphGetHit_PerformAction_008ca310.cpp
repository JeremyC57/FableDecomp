// CCreatureAction_NymphGetHit::PerformAction() -- when the nymph's definition has a special-effects sub-def that maps "TELEPORT_POS" to an effect, spawn that effect at the nymph and attach its particle emitter to the nymph's "DUMMY_CENTRE" helper; then tell the CTCNymph component to fly around for the frames the action has left (TotalTime - FramesRunning)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CA310, length 387 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature +8), CDefClassBase::GetSubDef<CSpecialEffectsDef> @ 0x00669580 on the
// thing's definition (+0x70), CCharString ctor/dtor @ 0x0099EBF0 / 0x0099EAE0, CSpecialEffectsDef::GetEffectFromString(const CCharString&,
// long&) @ 0x00669D30, CThing::GetPos @ 0x004C73D0, CreateEffectThing(long, const C3DVector*, bool) @ 0x006E0880 (`__fastcall`),
// CVectorMap::LowerBound @ 0x0040F020 (inlined lookups: 0x4B = CTCDParticleEmitter, bit 11 of +0x28, `p = 0; if (HasTC) p = GetTC`;
// 0xF5 = CTCNymph, bit 21 of +0x3c, tested), CTCDParticleEmitter::AttachToThing(CThing*, long, const CCharString&, long, float)
// @ 0x006E0BE0, CTCNymph::StartFlyingAround(long) @ 0x0080F5E0.
// Layout: CCreatureActionBase TotalTime +0x10, FramesRunning +0x1c (engine/CCreatureActionBase.h), PCreature +0x08.
// Shape: the landed CCreatureAction_TeleportIn 0x008B9F20 model -- the GetSubDef out-slot is a plain counted slot released by hand
// at the end (`dec [esi+4] ; call [edx+4]`); the lookup string is the implicit conversion inside a `found && ...` condition, so VC
// keeps a construction flag in bl and destroys the string before branching on the saved bool; the spawned effect is tested alive
// (null and bit 0 of +0x91) before the emitter lookup (`p = 0; if (HasTC) p = GetTC`). The nymph is found through the out-parameter
// GetInterface(T**, type) template (component left in eax, `mov ecx,eax` after the argument). GetSubDef and GetEffectFromString
// get same-TU stand-in bodies (write only the out slot / the `long&`), so neither the def slot nor the effect-type word escapes.
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                            // +0x04
};
class CSpecialEffectsDef : public CDefBase { public:
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
long Find_CSpecialEffectsDef_GetEffectFromString(const CSpecialEffectsDef* def, const CCharString& name);   // stand-in callee
// Stand-in body (the real one is out of line in retail): it writes the effect type only, so VC7.1 knows the
// effect-type word's address does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const
{
    long found = Find_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}
#pragma auto_inline(on)
template <class T> class CDefPointer { public:      // the out-slot GetSubDef fills (counted reference handed over)
    T* m_p;
    CDefPointer() : m_p(0) { }
};
template <class T> __forceinline void ReleaseDef(T* def)
{
    if (def && --def->m_refCount == 0)
        def->Destroy();
}
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};
void* FindSubDef_CDefClassBase_GetSubDef(CDefClassBase* def);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    void* def = FindSubDef_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (T*)def;
    return true;
}
#pragma auto_inline(on)
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                      // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;       // 0x004C73D0
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880
class CTCDParticleEmitter { public:
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
class CTCNymph { public:
    void StartFlyingAround(long frames);        // 0x0080F5E0
};
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CCreatureActionBase { public:                    // retail sizeof 0xa8
    virtual void v00();
    unsigned char           _pad04[4];
    CIntelligentPointer<CThing> PCreature;          // +0x08
    long                    TotalTime;          // +0x10
    long                    ActionTime;         // +0x14
    long                    AnimDelayTime;      // +0x18
    long                    FramesRunning;      // +0x1c
    unsigned char           _pad20[0xa8 - 0x20];
};
class CCreatureAction_NymphGetHit : public CCreatureActionBase { public:
    void PerformAction();
};

void CCreatureAction_NymphGetHit::PerformAction()
{
    CDefPointer<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    CDefClassBase* def = PCreature.GetPItem()->m_pDef;
    bool found = def->GetSubDef(effectsSlot);
    if (found && effectsSlot.m_p->GetEffectFromString("TELEPORT_POS", effectType)) {
        CThing* effect = CreateEffectThing(effectType, &PCreature.GetPItem()->GetPos(), false);
        if (effect && !(effect->m_flags91 & 1)) {
            CTCDParticleEmitter* emitter = 0;
            if (effect->HasTC(0x4b))
                emitter = effect->GetTC<CTCDParticleEmitter>(0x4b);
            emitter->AttachToThing(PCreature.GetPItem(), 10, "DUMMY_CENTRE", 0, 0.0f);
        }
    }
    CThing* creature = PCreature.GetPItem();
    CTCNymph* nymph;
    if (creature->GetInterface(&nymph, 0xf5))
        nymph->StartFlyingAround(TotalTime - FramesRunning);
    ReleaseDef(effectsSlot.m_p);
}