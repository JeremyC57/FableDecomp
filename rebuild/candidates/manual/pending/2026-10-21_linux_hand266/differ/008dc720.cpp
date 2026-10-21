// Grade: DIFFER(498v478) -- register allocation: the lane pushes ebp as a zero register (the null inits and both GetDummyObject zero arguments) and caches the special-effects def pointer in it, where retail uses immediate zeros and re-reads the def slot (three callee-saved pushes); the zero stores also land in different slots (retail B+18h twice, B+10h). Frame size (0x38, the 24-byte CRightHandedSet), the conditional-temporary flags for the counted PDef copy and the BURNINATE_ATTACK name, the beam set-up and the 0x4B emitter attach are in retail order.
// NCreatureMode::CModeDragonStrafeBurninate::StartBeamEffect() -- find the dragon's mouth dummy, look up the def's
// "BURNINATE_ATTACK" special effect and, when there is one, note the start time, create the effect thing at the mouth,
// keep it as the beam, and attach its particle emitter (type 0x4B) to the dragon's mouth dummy
// Leaf: StartBeamEffect
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DC720, 0x1DE bytes (padded to 480), plain `ret`.
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0 ("DRAGON_MOUTH_DUMMY" 0x01285B4C, "BURNINATE_ATTACK" 0x01285BA4),
//   CTCGraphicAppearance::GetDummyObject 0x004C08C0, CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580,
//   CSpecialEffectsDef::GetEffectFromString 0x00669D30, the world-frame getter 0x0049D870 (__fastcall, mislabelled
//   GFGetPreMainMemoryUsed), CreateEffectThing 0x006E0880 (__fastcall), CBaseIntelligentPointer ctor 0x00A01B10 /
//   SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C),
//   CVectorMap::LowerBound 0x0040F020 (inline lookup 0x4B), CTCDParticleEmitter::AttachToThing 0x006E0BE0.
// Layout: local model. Mode: owner +4, Beam +0x98 (CIntelligentPointer<CThing>), BeamStartTime +0xA0, BeamActive
//   +0xBC. Thing: TC-present bits +0x20, interface map +0x44, graphic TC +0x64, PDef +0x70, killed flag +0x91 bit 0.
// Shape: stand-in GetSubDef / GetEffectFromString bodies as in the parked 0x008DA0F0 (they only write their out slots).
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Forward, Up; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CDefVirtual {
    virtual ~CDefVirtual();
    virtual void Destroy();                     // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                            // +0x04
    void Release() { if (--m_refCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (m_p) m_p->Release(); }
};
template <class T> struct CCountedDefPointer {
    T* m_p;
    CCountedDefPointer(T* p) : m_p(p) { if (m_p) ++m_p->m_refCount; }
    ~CCountedDefPointer() { if (m_p) m_p->Release(); }
    T* operator->() const { return m_p; }
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
struct CDefClassBase : CDefBase {
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
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString* name, long index, unsigned long flags, C3DVector* position,
                        CRightHandedSet* orientation, float scale);   // 0x004C08C0
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing;
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long attachType, const CCharString* dummyName, long index, float offset);   // 0x006E0BE0
};
struct CThing {
    unsigned char         _pad0[0x20];
    unsigned long         m_presentBits[9];     // +0x20
    CInterfaceMap         m_interfaces;         // +0x44
    unsigned char         _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* PGraphic;             // +0x64
    unsigned char         _pad68[0x70 - 0x68];
    CDefClassBase*        PDef;                 // +0x70
    unsigned char         _pad74[0x91 - 0x74];
    unsigned char         Killed;               // +0x91 (bit 0)
    CCountedDefPointer<CDefClassBase> GetDef() const { return CCountedDefPointer<CDefClassBase>(PDef); }
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
long __fastcall GetWorldFrame();                                                          // 0x0049D870
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880
struct CBaseIntelligentPointer {
    virtual void _v0();
    void* PItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void _v0();
    CIntelligentPointer(CThing* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
namespace NCreatureMode {
class CModeDragonStrafeBurninate {
public:
    void*               _vptr;                  // +0x00
    CThing*             Owner;                  // +0x04
    unsigned char       _pad08[0x98 - 0x08];
    CIntelligentPointer Beam;                   // +0x98
    long                BeamStartTime;          // +0xA0
    unsigned char       _padA4[0xbc - 0xa4];
    bool                BeamActive;             // +0xBC
    void StartBeamEffect();
};
void CModeDragonStrafeBurninate::StartBeamEffect()
{
    CDefPointer<CSpecialEffectsDef> effects;
    long effectType = 0;
    C3DVector mouth;
    CRightHandedSet orientation;
    {
        CCharString dummy("DRAGON_MOUTH_DUMMY");
        Owner->PGraphic->GetDummyObject(&dummy, 0, 0, &mouth, &orientation, 1.0f);
    }
    bool found = Owner->GetDef()->GetSubDef(effects) && effects.m_p->GetEffectFromString("BURNINATE_ATTACK", effectType);
    if (!found)
        return;
    BeamStartTime = GetWorldFrame();
    Beam = CIntelligentPointer(CreateEffectThing(effectType, &mouth, false));
    BeamActive = true;
    if (Beam.GetPItem() && !(Beam.GetPItem()->Killed & 1)) {
        CThing* beam = Beam.GetPItem();
        if (beam->HasTC(0x4b)) {
            CCharString dummy("DRAGON_MOUTH_DUMMY");
            beam->GetTC<CTCDParticleEmitter>(0x4b)->AttachToThing(Owner, 0x2a, &dummy, 0, 0.0f);
        }
    }
}
}
