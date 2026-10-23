// CAIStateGroup_MinionReactToCorpse::Update() -- walk up to the corpse, mark it and every dead creature within 5 units discovered (noting whether the player killed one); a creature in life state 3 then plays the alert-to-player action when the player was the killer, any other posts STANDARD_MESSAGE_EVENT_FOUND_CORPSE to a guard
// Grade: DIFFER(1031v1037) -- structure follows retail (sibling 0x009378F0 model); residue: list end/iterator register roles (retail end in edi, iterator spilled), &PCorpse ebx v ebp, x87 order of the 2D offset/length, killer-pointer lea
// Layout: local model (the engine header flattens the base); PCorpse +0x14; the dead creature's killer pointer at +0xd8.
#include <stdlib.h>
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
struct CThing;
struct C2DBoxI {
    long X0, Y0, X1, Y1;
    C2DBoxI(long x0, long y0, long x1, long y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) { }
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    template<class V> C2DVector(const V& v) : X(v.X), Y(v.Y) { }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
};
struct CThingFilter_None { };
struct CDeadList {                              // STLport list<CThingDeadCreature*>
    struct SNode { SNode* next; SNode* prev; CThing* data; };
    SNode* node;
    CDeadList() { node = (SNode*)malloc(sizeof(SNode)); node->next = node; node->prev = node; }
    ~CDeadList();                               // 0x00936910
};
struct CThingSearchTools {
    void FindThingsInRoughArea(const C2DBoxI& area, const CThingFilter_None& filter, CDeadList& things);   // 0x00937080
};
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    V16(a) V16(b) V16(c) V16(d) V4(e0) V4(e1) V4(e2) virtual void s76();
    virtual bool IsPlayer() const;              // slot 77 (+0x134)
    unsigned char _pad4[0x1c];
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
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
struct CThingCreatureBase : CThing {
    long GetLifeState();                        // 0x00662010
    const C3DVector& GetPosition() const;       // 0x006A4D00
};
struct CThingDeadCreature : CThingCreatureBase {
    unsigned char _pad94[0xd8 - 0x94];
    CBaseIntelligentPointer PKiller;         // +0xd8
};
struct CTCDiscovery {                           // interface 0x40
    unsigned char _pad[0x0c];
    bool Discovered;                            // +0x0c
    void SetDiscovered(bool discovered, CThing* by);    // 0x007509D0
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer { };
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
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};
struct CMessageEventDef {                   // CDefBase: vptr, then the reference count at +4
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
struct CDefinitionManager {
    bool GetDef(const CCharString& name, CDefPointer& def);   // 0x007923C0
};
struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                 // 0x00693EF0
};
struct CCreatureAction_AlertToPlayer : CCreatureActionBase {
    CCreatureAction_AlertToPlayer(CThingCreatureBase* creature);  // 0x009110F0
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;          // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CDefinitionManager* GetPDefinitionManager();
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000    // 0x008FCFB0
    void Terminate();                           // 0x008FCF40
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long frames, float speed);   // 0x0097D6F0
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerConstThing target, float distance, bool* reached,
        long movement, bool a, bool b, bool c, bool d, float speed);         // 0x0097E380
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    long AIAction_FindAGuardAndGiveMessage(CDefPointer def, C3DVector position, long flags);   // 0x009810B0
};
struct CAIStateGroup_MinionReactToCorpse : CAIStateGroupBase {
    CBaseIntelligentPointer PCorpse;            // +0x14 CIntelligentPointer<CThingDeadCreature>
    virtual void Update();
};

void CAIStateGroup_MinionReactToCorpse::Update()
{
    bool killedByPlayer = false;
    if (IS_THING_ALIVE(PCorpse)) {
        bool reached = false;
        if (AIAction_MoveToThingProximity(1, CIntelligentPointerConstThing(PCorpse), 2.0f, &reached, 5, true, false, true, true, 0.0f) == 2
            && IS_THING_ALIVE(PCorpse)) {
            CTCDiscovery* discovery;
            if (IS_THING_ALIVE(PCorpse) && PCorpse.GetPItem()->GetInterface(&discovery, 0x40) && !discovery->Discovered) {
                discovery->SetDiscovered(true, GetCreature());
                if (((CThingDeadCreature*)PCorpse.GetPItem())->PKiller.GetPItem() != 0
                    && ((CThingDeadCreature*)PCorpse.GetPItem())->PKiller.GetPItem()->IsPlayer())
                    killedByPlayer = true;
            }
            CDeadList corpses;
            CDeadList::SNode* const end = corpses.node;
            {
                const C3DVector& pos = ((CThingDeadCreature*)PCorpse.GetPItem())->GetPosition();
                long y = (long)pos.Y;
                long x = (long)pos.X;
                PeekThingSearchTools()->FindThingsInRoughArea(C2DBoxI(x - 5, y - 5, x + 5, y + 5), CThingFilter_None(), corpses);
            }
            for (CDeadList::SNode* it = end->next; it != end; it = it->next) {
                CThingDeadCreature* dead = (CThingDeadCreature*)it->data;
                if (dead != 0 && !(dead->m_flags91 & 1)) {
                    if (!dead->GetInterface(&discovery, 0x40) && !discovery->Discovered) {
                        C2DVector offset = C2DVector(dead->GetPosition()) - C2DVector(((CThingDeadCreature*)PCorpse.GetPItem())->GetPosition());
                        if (offset.X * offset.X + offset.Y * offset.Y <= 25.0f) {
                            discovery->SetDiscovered(true, GetCreature());
                            CBaseIntelligentPointer& killer = dead->PKiller;
                            if (killer.GetPItem() != 0 && killer.GetPItem()->IsPlayer())
                                killedByPlayer = true;
                        }
                    }
                }
            }
            if (GetCreature()->GetLifeState() == 3) {
                if (killedByPlayer) {
                    if (AIAction_PerformActionAndWait(&CCreatureAction_AlertToPlayer(GetCreature()), false) != 2) {
                        Terminate();
                        return;
                    }
                }
            } else {
                CDefPointer def;
                bool found;
                {
                    CCharString name("STANDARD_MESSAGE_EVENT_FOUND_CORPSE", -1);
                    found = GetPDefinitionManager()->GetDef(name, def);
                }
                CMessageEventDef* p = def.m_p;
                if (found)
                    AIAction_FindAGuardAndGiveMessage(CDefPointer(p), ((CThingDeadCreature*)PCorpse.GetPItem())->GetPosition(), 0);
                if (p) {
                    if (--p->m_count == 0)
                        p->Destroy();
                }
            }
            return;
        }
    }
    Terminate();
}
