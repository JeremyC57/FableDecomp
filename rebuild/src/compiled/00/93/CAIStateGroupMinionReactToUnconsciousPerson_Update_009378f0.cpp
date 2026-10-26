// CAIStateGroup_MinionReactToUnconsciousPerson::Update() -- walk up to the (living) unconscious person, mark it discovered by this creature, then either (creature in life state 3) look at it and play the alert-to-player action, or post the STANDARD_MESSAGE_EVENT_FOUND_UNCONSCIOUS_PERSON message to a guard at the person's position; always Terminate()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009378F0, length 535 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 (the by-value
// CIntelligentPointer<const CThing> target, vtable 0x01237F1C, inline at the first site), its out-of-line converting
// ctor @ 0x0091F990 (second site), AIAction_MoveToThingProximity @ 0x0097E380, CVectorMap::LowerBound @ 0x0040F020
// (inlined GetInterface 0x40 = CTCDiscovery, bit 0 of +0x28), CTCDiscovery::SetDiscovered @ 0x007509D0, the
// creature's life-state getter @ 0x00662010, AIAction_LookAtThing @ 0x0097D6F0, CCreatureAction_AlertToPlayer ctor
// @ 0x009110F0 / ~CCreatureActionBase @ 0x00693EF0, AIAction_PerformActionAndWait @ 0x0097DB70, CCharString(const
// char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, GetPDefinitionManager @ 0x008FCFB0, CDefinitionManager::
// GetDef<CMessageEventDef> @ 0x007923C0, CThing::GetPosition @ 0x004C73D0, AIAction_FindAGuardAndGiveMessage
// @ 0x009810B0 (def and position by value), Terminate @ 0x008FCF40.
// Data: "STANDARD_MESSAGE_EVENT_FOUND_UNCONSCIOUS_PERSON" @ 0x01292318; 0.0027777f look speed (immediate).
// Layout: engine/CAIStateGroup_MinionReactToUnconsciousPerson.h flattens the base, so the model is local (PBrain +4,
// PPerson +0x14); the alert action is 0xa8 bytes (retail CCreatureActionBase size).
// Shape: the discovery lookup's HasTC reads the bitset word into a named local and shifts it (`mov ecx,[eax+28h] ; and
// cl,bl`; the shifted-byte form narrows to a byte load and swaps this/&PPerson between ebp/edi), bound to a `const bool&`
// (the byte spill at +0x0b); the message def is released by hand through a pointer local (landed 0x00849140 shape) and
// passed by value through an add-ref copy; a named person local puts the flags push after GetPItem.
class CTCBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    bool HasTC(long interfaceType) const
    {
        unsigned long word = m_presentBits[interfaceType >> 5];
        return ((word >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        const bool& present = HasTC(interfaceType);
        if (present) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return present != 0;
    }
};
class CThingCreatureBase : public CThing { public:
    long GetLifeState();                        // 0x00662010
};
class CTCDiscovery { public:                           // interface 0x40
    unsigned char _pad[0x0c];
    bool Discovered;                            // +0x0c
    void SetDiscovered(bool discovered, CThing* by);    // 0x007509D0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: };
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {    // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    __forceinline CIntelligentPointerConstThing(const CBaseIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    template<class U> CIntelligentPointerConstThing(const CIntelligentPointer<U>& other);   // 0x0091F990
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};
class CMessageEventDef { public:                   // CDefBase: vptr, then the reference count at +4
    virtual void v00();
    virtual void Destroy();                 // slot 1 (+4)
    long m_count;                           // +4
};
struct CDefPointer {                        // CDefPointer<CMessageEventDef>: one word, zero-initialised; released by hand
    CMessageEventDef* m_p;
    CDefPointer() : m_p(0) { }
    CDefPointer(CMessageEventDef* p) : m_p(p) { if (p) ++p->m_count; }
    CDefPointer(const CDefPointer& other) : m_p(other.m_p) { if (m_p) ++m_p->m_count; }
};
class CDefinitionManager { public:
    bool GetDef(const CCharString& name, CDefPointer& def);   // 0x007923C0
};
class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa4];
    virtual ~CCreatureActionBase();                 // 0x00693EF0
};
class CCreatureAction_AlertToPlayer : public CCreatureActionBase { public:
    CCreatureAction_AlertToPlayer(CThingCreatureBase* creature);  // 0x009110F0
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;          // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CDefinitionManager* GetPDefinitionManager();    // 0x008FCFB0
    void Terminate();                           // 0x008FCF40
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long frames, float speed);   // 0x0097D6F0
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerConstThing target, float distance, bool* reached,
        long movement, bool a, bool b, bool c, bool d, float speed);         // 0x0097E380
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    long AIAction_FindAGuardAndGiveMessage(CDefPointer def, C3DVector position, long flags);   // 0x009810B0
};
struct CAIStateGroup_MinionReactToUnconsciousPerson : CAIStateGroupBase {
    CIntelligentPointer<CThingCreatureBase> PPerson;    // +0x14
    virtual void Update();
};

void CAIStateGroup_MinionReactToUnconsciousPerson::Update()
{
    if (IS_THING_ALIVE(PPerson)) {
        bool reached = false;
        if (AIAction_MoveToThingProximity(1, CIntelligentPointerConstThing(static_cast<const CBaseIntelligentPointer&>(PPerson)),
                1.0f, &reached, 5, true, false, true, true, 0.0f) == 2 && IS_THING_ALIVE(PPerson)) {
            CTCDiscovery* discovery;
            if (IS_THING_ALIVE(PPerson) && PPerson.GetPItem()->GetInterface(&discovery, 0x40) && !discovery->Discovered)
                discovery->SetDiscovered(true, GetCreature());
            if (GetCreature()->GetLifeState() == 3) {
                if (AIAction_LookAtThing(1, CIntelligentPointerConstThing(PPerson), 11, 0.0027777f) == 2) {
                    if (AIAction_PerformActionAndWait(&CCreatureAction_AlertToPlayer(GetCreature()), false) != 2) {
                        Terminate();
                        return;
                    }
                    Terminate();
                    return;
                }
            } else {
                CDefPointer def;
                bool found;
                {
                    CCharString name("STANDARD_MESSAGE_EVENT_FOUND_UNCONSCIOUS_PERSON", -1);
                    found = GetPDefinitionManager()->GetDef(name, def);
                }
                CMessageEventDef* p = def.m_p;
                if (found) {
                    CThing* person = PPerson.GetPItem();
                    AIAction_FindAGuardAndGiveMessage(CDefPointer(p), person->GetPosition(), 0);
                }
                if (p) {
                    if (--p->m_count == 0)
                        p->Destroy();
                }
            }
        }
    }
    Terminate();
}