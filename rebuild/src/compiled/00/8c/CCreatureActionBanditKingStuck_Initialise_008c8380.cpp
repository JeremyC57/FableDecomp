// CCreatureAction_BanditKingStuck::Initialise() -- animation BK_STUCK; unless a live stuck effect exists, create the creature's STUCKLOOP special effect at its position, keep it in PStuckEffect and attach it to the creature through its particle emitter; then the base Initialise
// Leaf: ?Initialise@CCreatureAction_BanditKingStuck
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C8380, 455 bytes (464 with padding), virtual Initialise.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CCombatActionBase::SetCombatAnimation
//   0x008574D0, CBaseIntelligentPointer::GetPItem 0x00A01B50 / CBaseIntelligentPointer() 0x00A01B10 / SetPItem 0x00A01B90 /
//   ~CBaseIntelligentPointer 0x00A01C10, CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580, CCRC::CalcAndLookup
//   0x00669D30, CThing::GetPos 0x004C73D0, the special-effect Create(long, const C3DVector&, bool) 0x006E0880
//   (__fastcall), CVectorMap::LowerBound 0x0040F020 (inline GetInterface 0x4B = CTCDParticleEmitter),
//   CTCDParticleEmitter::AttachToThing(CThing*, long, const CCharString&, long, float) 0x006E0BE0,
//   CCombatActionBase::Initialise 0x00857520.
// Data: "BK_STUCK" 0x01281AA0, "STUCKLOOP" 0x01281A94, "" 0x0122D70E; CIntelligentPointer<CThing> vtable 0x01238C6C.
// Layout (retail, local model): CCombatActionBase is 0x114 bytes (PCreature +0x08); PStuckEffect +0x118 (PDB +0x130,
//   the engine header carries PDB offsets); CThing def +0x70, dead flag +0x91 bit 0.
// Shape: the sub-def lookup and the name lookup are one `&&` (the name is a conditional temporary with its flag word);
//   the created thing goes through a CIntelligentPointer<CThing> temporary assigned with GetPItem/SetPItem, built
//   straight from the CreateSpecialEffect call (a named `created` local puts the temporary over the effect word);
//   CalcAndLookup / GetSubDef have same-TU out-only stand-ins, LowerBound a same-TU body, GetInterface the ternary pick.
class C3DVector { public: float X, Y, Z; };
class CTCBase;
class CThing;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};

class CSpecialEffectsDef { public:
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    bool CalcAndLookup(const CCharString& name, long* value);   // 0x00669D30
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
long Lookup_CSpecialEffectsDef_CalcAndLookup(const void* def, const CCharString& name);   // stand-in callee
// Stand-in body (out of line in retail @ 0x00669D30): it writes the out value only, so VC7.1 knows the local's address
// does not escape the call (the 0x008D9740 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::CalcAndLookup(const CCharString& name, long* value)
{
    long found = Lookup_CSpecialEffectsDef_CalcAndLookup(this, name);
    if (found < 0)
        return false;
    *value = found;
    return true;
}
#pragma auto_inline(on)
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out);   // 0x00669580
};
void* FindDef_CDefClassBase_GetSubDef_0();             // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x008C4170 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CSpecialEffectsDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
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

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPos() const;             // 0x004C73D0
    CDefClassBase* GetDef() const { return m_pDef; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
CThing* __fastcall CreateSpecialEffect(long effect, const C3DVector& position, bool attached);   // 0x006E0880

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CBaseIntelligentPointer();                        // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                       // 0x00A01C10
    CThing* GetPItem() const;                         // 0x00A01B50
    void SetPItem(CThing* item);                      // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};

class CTCDParticleEmitter { public:
    void AttachToThing(CThing* thing, long boneIndex, const CCharString& boneName, long a, long b);   // 0x006E0BE0
};

class CCombatActionBase { public:                            // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0x114 - 0x10];
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
    void Initialise();                                // 0x00857520
};
class CCreatureAction_BanditKingStuck : public CCombatActionBase { public:
    long                     TimesToLoop;             // +0x114
    CIntelligentPointerThing PStuckEffect;            // +0x118
    void Initialise();
};

void CCreatureAction_BanditKingStuck::Initialise()
{
    {
        CCharString name("BK_STUCK");
        SetCombatAnimation(name);
    }
    if (PStuckEffect.GetPItem() == 0 || (PStuckEffect.GetPItem()->m_flags91 & 1)) {
        CDefPointer<CSpecialEffectsDef> effects;
        bool found;
        {
            long effect = 0;
            found = PCreature.GetPItem()->GetDef()->GetSubDef(&effects) && effects.Object->CalcAndLookup("STUCKLOOP", &effect);
            if (found) {
                PStuckEffect = CIntelligentPointerThing(CreateSpecialEffect(effect, PCreature.GetPItem()->GetPos(), false));
            }
        }
        if (found) {
            if (PStuckEffect.GetPItem() && !(PStuckEffect.GetPItem()->m_flags91 & 1)) {
                CTCDParticleEmitter* emitter = 0;
                PStuckEffect.GetPItem()->GetInterface(&emitter, 0x4b);
                emitter->AttachToThing(PCreature.GetPItem(), 0x2a, "", 0, 0);
            }
        }
    }
    CCombatActionBase::Initialise();
}