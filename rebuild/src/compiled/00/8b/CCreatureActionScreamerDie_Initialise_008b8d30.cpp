// CCreatureAction_ScreamerDie::Initialise() -- clear the creature's interface-9 component flag, animation ST_DIE; create the creature's SCREAMER_DEATH special effect at its position, keep it in PDieEffect and attach it to the creature's DUMMY_CENTRE through its particle emitter; then the base Initialise (the sub-def is released last)
// Leaf: ?Initialise@CCreatureAction_ScreamerDie
// Retail: Fable.exe (TLC, VC7.1) @ 0x008B8D30, 499 bytes (512 with padding), virtual Initialise.
// Callees: as CCreatureAction_BanditKingStuckGetHit::Initialise (0x008C87A0): CCharString ctor/dtor 0x0099EBF0/0x0099EAE0,
//   SetAnimationName 0x008574D0, GetPItem 0x00A01B50, CBaseIntelligentPointer() 0x00A01B10 / SetPItem 0x00A01B90 /
//   ~CBaseIntelligentPointer 0x00A01C10, GetSubDef<CSpecialEffectsDef> 0x00669580, CCRC::CalcAndLookup 0x00669D30,
//   CThing::GetPosition 0x004C73D0, Create 0x006E0880 (__fastcall), LowerBound 0x0040F020 (GetInterface 9 and 0x4B),
//   CTCDParticleEmitter::AttachToThing 0x006E0BE0, CCombatActionBase::Initialise 0x00857520.
// Data: "ST_DIE" 0x0127B34C, "SCREAMER_DEATH" 0x0127B33C, "DUMMY_CENTRE" 0x0126F110; CIntelligentPointer<CThing> vtable
//   0x01238C6C.
// Layout (retail, local model): CCombatActionBase is 0x114 bytes (PCreature +0x08); PDieEffect +0x114 (PDB +0x12C);
//   the interface-9 component's flag byte +0x10.
// Shape: the sibling 0x008C87A0 model (block-scoped effect id, implicit string conversions, named-item operator=).
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
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;             // 0x004C73D0
    CDefClassBase* GetDef() const { return m_pDef; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
CThing* __fastcall CreateSpecialEffect(long effect, const C3DVector& position, bool attached);   // 0x006E0880

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CBaseIntelligentPointer();                        // 0x00A01B10
    ~CBaseIntelligentPointer();                       // 0x00A01C10
    CThing* GetPItem() const;                         // 0x00A01B50
    void SetPItem(CThing* item);                      // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};

struct CTCInterface9 {                                // interface 9 component
    unsigned char _pad[0x10];
    bool          m_flag10;                            // +0x10
};
class CTCDParticleEmitter { public:
    void AttachToThing(CThing* thing, long boneIndex, const CCharString& boneName, long a, long b);   // 0x006E0BE0
};

class CCombatActionBase { public:                            // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0x114 - 0x10];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
    void Initialise();                                // 0x00857520
};
class CCreatureAction_ScreamerDie : public CCombatActionBase { public:
    CIntelligentPointerThing PDieEffect;            // +0x114
    void Initialise();
};

void CCreatureAction_ScreamerDie::Initialise()
{
    CTCInterface9* component;
    if (PCreature.GetPItem()->GetInterface(&component, 9))
        component->m_flag10 = false;
    {
        CCharString name("ST_DIE");
        SetAnimationName(name);
    }
    CDefPointer<CSpecialEffectsDef> effects;
    bool found;
    {
        long effect = 0;
        found = PCreature.GetPItem()->GetDef()->GetSubDef(&effects) && effects.Object->CalcAndLookup("SCREAMER_DEATH", &effect);
        if (found) {
            CThing* created = CreateSpecialEffect(effect, PCreature.GetPItem()->GetPosition(), false);
            PDieEffect = CIntelligentPointerThing(created);
        }
    }
    if (found) {
        if (PDieEffect.GetPItem() && !(PDieEffect.GetPItem()->m_flags91 & 1)) {
            CTCDParticleEmitter* emitter = 0;
            PDieEffect.GetPItem()->GetInterface(&emitter, 0x4b);
            emitter->AttachToThing(PCreature.GetPItem(), 0xa, "DUMMY_CENTRE", 0, 0);
        }
    }
    CCombatActionBase::Initialise();
}