// CAIStateGroup_ChickenPanic::IsRunnable() -- panic when an attacker is within 8 units and attacked recently, or when hit by an air-guitar expression
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00911C20, length 361 bytes (same TU as Update 0x00911D90).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetPosition @ 0x004C73D0, CAIStateGroupBase::GetPFrameWorld
//          @ 0x008FCFC0 (only evaluated for the static frame call), GFGetCurrentFrame @ 0x0049D870, CVectorMap::LowerBound const
//          @ 0x004365B0, CTCEntityEvents::GetEvent<const CEventHitBy, CEventFilter_None> @ 0x008AF0A0, the hit def's virtual slot
//          10 (+0x28, CDefString by value), CDefString::operator==(const char*) const @ 0x005B3440 ("EXPRESSION_AIR_GUITAR"
//          @ 0x0124F5A4), the def's slot 1 release, CThingCreatureBase::SetAttackedByCreature @ 0x006624F0.
// Globals: panic memory (frames / 10) @ 0x01375550 (int), 64.0f @ 0x0126973C, 10.0f @ 0x01237F0C, the unsigned-conversion
//          bias 4294967296.0f @ 0x0122DCB4.
// Shape: the attacker test is the inline XY compare (no named float); the frame world is fetched first, then the
//          attack frame and the panic memory are named locals read before the static frame call (esi / a frame slot);
//          the elapsed frame count is unsigned. The event half is its own block and the hit lookup a nested block, so
//          the lookup key, the panic memory and the hit share one frame slot (0xC frame); the entity-events component
//          starts null (HasTC + one-ternary GetTC on the const map); the hit def is held through an add-ref'd local with
//          the short `dec [esi+4] ; jne` release and the bool lives in a mid-function `push ebx`; the creature is a named
//          local before SetAttackedByCreature. Two same-TU stand-in bodies (const LowerBound 0x004365B0 and GetEvent
//          0x008AF0A0, both seen by retail's LTCG) tell VC7.1 the key and the hit pointer do not escape.
long __fastcall GFGetCurrentFrame();            // 0x0049D870
struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0 (the const copy, called out of line)
    SEntry* LowerBound(const long* type)        // STLport lower_bound over the 8-byte {type, component} entries (inlined in Update)
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
};

#pragma auto_inline(off)
// Same-TU stand-in for the const CVectorMap::LowerBound (0x004365B0, STLport lower_bound; retail's LTCG saw its body): the
// key's address does not escape, so the lookup key shares its frame slot with the hit-event out pointer as in retail.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
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

struct C3DVector {
    float x, y, z;
    C3DVector() { }
    C3DVector(const C3DVector& v) { x = v.x; y = v.y; z = v.z; }
};
struct C2DVector {
    float x, y;
    float GetLengthSquared() const { return x * x + y * y; }
};

struct CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0xf4 - 0x4c];
    CBaseIntelligentPointer LastAttacker;       // +0xf4 (CThingCreatureBase)
    long FrameLastAttacked;                     // +0xfc
    void SetAttackedByCreature(CThing* attacker);   // 0x006624F0
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    const C3DVector& GetPosition();             // 0x004C73D0
    const C3DVector& GetPosition() const;       // 0x006A4D00
    bool TestTC(long interfaceType) const
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return has;
    }
};

struct CTCCreatureNavigation {                  // interface 7
    bool GetNearestFleePosition(const C3DVector& centre, float radius, C3DVector* result);   // 0x006CB840
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
    CThing* GetPCreature() const { return PCreature; }
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float distance, bool* reached, long movement,
        bool a, bool b, bool c, CThing* avoid, bool d, float speed);        // 0x0097E890
    long AIAction_DoNothing();                  // 0x0097E200
    void Terminate();                           // 0x008FCF40
    struct CThingWorld* GetPFrameWorld();       // 0x008FCFC0
};

struct CThingWorld {
    static long GetCurrentFrame() { return GFGetCurrentFrame(); }
};
extern long g_ChickenPanicMemory;               // 0x01375550

struct CDefString {
    long Index;
    bool operator==(const char* text) const;    // 0x005B3440
};
struct CHitDef {
    virtual void v00();
    virtual void Release();                     // slot 1 (+0x04)
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09();
    virtual CDefString GetEmoteName() const;    // slot 10 (+0x28)
    long RefCount;                              // +0x04
};
struct CHitDefPointer {
    CHitDef* Object;
    CHitDefPointer(CHitDef* object) : Object(object) { if (Object) ++Object->RefCount; }
    ~CHitDefPointer() { if (--Object->RefCount == 0) Object->Release(); }
    CHitDef* operator->() const { return Object; }
};
namespace NEntityEvents {
    struct CEventHitBy {
        unsigned char _00[8];
        CBaseIntelligentPointer Attacker;       // +0x08
        CHitDef* Def;                           // +0x10
    };
    struct CEventFilter_None { CEventFilter_None() { } };
}
struct CTCEntityEvents {                        // interface 0x42
    unsigned char _00[0x0c];
    const NEntityEvents::CEventHitBy* FirstHit; // stand-in view of the event list
    bool GetEvent(const NEntityEvents::CEventHitBy** out, const NEntityEvents::CEventFilter_None& filter);   // 0x008AF0A0 (stand-in body below)
};
#pragma auto_inline(off)
// Same-TU stand-in for CTCEntityEvents::GetEvent<const CEventHitBy, CEventFilter_None> (0x008AF0A0; retail's LTCG saw its
// body): it only writes the out pointer, so `hit` stays in a register (edi) after the call as in retail.
__declspec(noinline) bool CTCEntityEvents::GetEvent(const NEntityEvents::CEventHitBy** out, const NEntityEvents::CEventFilter_None&)
{
    if (FirstHit) {
        *out = FirstHit;
        return true;
    }
    return false;
}
#pragma auto_inline(on)

struct CAIStateGroup_ChickenPanic : CAIStateGroupBase {
    virtual bool IsRunnable();
};

bool CAIStateGroup_ChickenPanic::IsRunnable()
{
    if (PBrain->GetPCreature()->LastAttacker.GetPItem()) {
        CThing* creature = PBrain->PCreature;
        const C3DVector& attackerPos = creature->LastAttacker.GetPItem()->GetPosition();
        const C3DVector& pos = creature->GetPosition();
        C2DVector delta;
        delta.x = pos.x - attackerPos.x;
        delta.y = pos.y - attackerPos.y;
        if (delta.GetLengthSquared() < 64.0f) {
            CThingWorld* world = GetPFrameWorld();
            long attacked = PBrain->PCreature->FrameLastAttacked;
            long memory = g_ChickenPanicMemory;
            if ((float)(unsigned long)(world->GetCurrentFrame() - attacked) < (float)memory * 10.0f)
                return true;
        }
    }
    {
        CThing* creature = PBrain->PCreature;
        CTCEntityEvents* events = 0;
        if (creature->HasTC(0x42))
            events = creature->GetTC<CTCEntityEvents>(0x42);
        {
            const NEntityEvents::CEventHitBy* hit = 0;
            if (events->GetEvent(&hit, NEntityEvents::CEventFilter_None())) {
                bool airGuitar;
                {
                    CHitDefPointer def(hit->Def);
                    airGuitar = def->GetEmoteName() == "EXPRESSION_AIR_GUITAR";
                }
                if (airGuitar) {
                    CThing* chicken = PBrain->PCreature;
                    chicken->SetAttackedByCreature(hit->Attacker.GetPItem());
                    return true;
                }
            }
        }
    }
    return false;
}
