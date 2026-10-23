// CAIStateGroup_Drunk::Update() -- while the creature's drunk component says it is drunk: sometimes (15%) find the nearest live village "discharge bodily fluids" marker, book it, walk there, face along it and puke or piss (sobering up by 0.6), then wander to a nearby spot and maybe idle a while; any failure or sobriety terminates the group
// Grade: DIFFER(1620v1612) -- first pass: control flow and calls in retail order; frame-slot sharing differs (retail keeps every TestTC bool and the reached flag in one byte, the 0xc3 key in the best-distance word, the navigation pointer uninitialised in the drunk-component word), so most esp offsets shift
// Retail: Fable.exe (TLC, VC7.1) @ 0x00922860, 1612 bytes, plain ret, 8-aligned ebp frame.
// Callees: interface LowerBound 0x0040F020 (types 0xc3 drunk, 0x23 village member, 0x2f village task, 7 navigation),
//   GetPWorldSeed 0x008FCFD0, GFFloatRandom 0x004978A0 (__fastcall), GFROR13 0x00497890, CBaseIntelligentPointer ctor
//   0x00A01B10 / GetPItem 0x00A01B50 / dtor 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C), CCharString
//   0x0099EBF0 / ~ 0x0099EAE0, CGameDefinitionManager::Get 0x0044C6B0, GetDefGlobalIndexFromName 0x009AD410, the village
//   member's village accessor 0x007EE540, IS_THING_ALIVE copies 0x00919040 / 0x004CC340 (__fastcall), the village-task
//   filter 0x006A8AF0, CThing::GetPosition 0x004C73D0, CThingCreatureBase::GetPosition 0x006A4D00, CIntelligentPointer<CThing>
//   (CThing*) 0x004A96F0 / operator= 0x004A9760, AssignCreatureToTask 0x00916640, TalkToSelfUpdate 0x00922750,
//   AIAction_MoveToPos 0x0097EC00, physics GetOrientation (vtable slot 72), AIAction_LookAtPos 0x0097D580,
//   CCreatureAction_PlayAnimation 0x008424F0 / 0x00843A80, ~CCreatureActionBase 0x00693EF0, AIAction_PerformActionAndWait
//   0x0097DB70, GetNearbyWanderPosition 0x006CB6E0, Terminate 0x008FCF40.
// Constants: 0.15f @ 0x0129000C, 0.6f @ 0x01290010, 0.0f @ 0x0122DEDC.
#include "engine/CAIStateGroup_Drunk.h"

struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
};
struct CTCPhysicsBase {
    virtual void p00(); virtual void p01(); virtual void p02(); virtual void p03(); virtual void p04(); virtual void p05();
    virtual void p06(); virtual void p07(); virtual void p08(); virtual void p09(); virtual void p10(); virtual void p11();
    virtual void p12(); virtual void p13(); virtual void p14(); virtual void p15(); virtual void p16(); virtual void p17();
    virtual void p18(); virtual void p19(); virtual void p20(); virtual void p21(); virtual void p22(); virtual void p23();
    virtual void p24(); virtual void p25(); virtual void p26(); virtual void p27(); virtual void p28(); virtual void p29();
    virtual void p30(); virtual void p31(); virtual void p32(); virtual void p33(); virtual void p34(); virtual void p35();
    virtual void p36(); virtual void p37(); virtual void p38(); virtual void p39(); virtual void p40(); virtual void p41();
    virtual void p42(); virtual void p43(); virtual void p44(); virtual void p45(); virtual void p46(); virtual void p47();
    virtual void p48(); virtual void p49(); virtual void p50(); virtual void p51(); virtual void p52(); virtual void p53();
    virtual void p54(); virtual void p55(); virtual void p56(); virtual void p57(); virtual void p58(); virtual void p59();
    virtual void p60(); virtual void p61(); virtual void p62(); virtual void p63(); virtual void p64(); virtual void p65();
    virtual void p66(); virtual void p67(); virtual void p68(); virtual void p69(); virtual void p70(); virtual void p71();
    virtual const CRightHandedSet& GetOrientation() const;   // slot 72 (+0x120)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
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
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PPhysics;                   // +0x60
    unsigned char _pad64[0x8c - 0x64];
    unsigned short DefGlobalIndex;              // +0x8c
    unsigned char _pad8e[0x91 - 0x8e];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    __forceinline bool TestTC(long type) const { return ((m_presentBits[type >> 5] >> (type & 31)) & 1) != 0; }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
};
struct CThingCreatureBase : CThing {
    const C3DVector& GetPosition() const;       // 0x006A4D00
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {    // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing() { }
    CIntelligentPointerThing(CThing* item);     // 0x004A96F0
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other);   // 0x004A9760
};
struct CIntelligentPointerMarker : CBaseIntelligentPointer {   // the village's marker list element
    virtual void v00();
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointerThing* pointer);    // 0x004CC340
bool __fastcall IS_THING_ALIVE(const CIntelligentPointerMarker* pointer);   // 0x00919040
struct CMarkerListNode {
    CMarkerListNode* _M_next;
    CMarkerListNode* _M_prev;
    CIntelligentPointerMarker _M_data;          // +8
};
struct CMarkerListIterator {
    CMarkerListNode* _M_node;
    CMarkerListIterator(CMarkerListNode* node) : _M_node(node) { }
    CMarkerListIterator(const CMarkerListIterator& other) : _M_node(other._M_node) { }
    CIntelligentPointerMarker& operator*() const { return _M_node->_M_data; }
    CMarkerListIterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const CMarkerListIterator& other) const { return _M_node != other._M_node; }
};
struct CTCVillage {
    unsigned char _pad[0x70];
    CMarkerListNode* m_markersNode;             // +0x70 std::list<CIntelligentPointer<...>> sentinel
    CMarkerListIterator MarkersBegin() { return CMarkerListIterator(m_markersNode->_M_next); }
    CMarkerListIterator MarkersEnd() { return CMarkerListIterator(m_markersNode); }
};
struct CTCVillageMember {
    CTCVillage* GetVillage();                   // 0x007EE540
};
struct CTCVillageTask {
    void AssignCreatureToTask(CThing* creature);   // 0x00916640
};
struct CTCDrunk {
    unsigned char _pad[0x0c];
    bool Drunk;                                 // +0x0c
    unsigned char _pad0d[3];
    float Drunkness;                            // +0x10
};
struct CTCCreatureNavigation {
    bool GetNearbyWanderPosition(const C3DVector& centre, float radius, C3DVector* result, bool onMap);   // 0x006CB6E0
};
struct CThingFilter_HasVillageTask {
    bool operator()(CThing* thing) const;       // 0x006A8AF0
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CGameDefinitionManager {
    static CDefinitionManager* Get();           // 0x0044C6B0
};
struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CCreatureActionBase {
    void* __vftable;
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThing* owner, const CCharString& animationName, const CSoundPair& sound,
                                  bool useAnimationMovement, bool usePhysics, bool cameraUpdate, long c, long d);   // 0x008424F0
    CCreatureAction_PlayAnimation(CThing* owner, const CCharString& animationName, long loops, const CSoundPair& sound,
                                  bool useAnimationMovement, bool usePhysics, bool cameraUpdate, long c, long d);   // 0x00843A80
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
float __fastcall GFFloatRandom(unsigned long* seed, float range);   // 0x004978A0
extern float g_DrunkDischargeChance;            // 0x0129000C (0.15f)

struct CAIStateGroupDrunkMethods : CAIStateGroup_Drunk {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThingCreatureBase* GetCreature() const { return Brain()->PCreature; }
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    void Terminate();                           // 0x008FCF40
    void TalkToSelfUpdate();                    // 0x00922750
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long movement, bool a, bool b,
        CThing* thing, bool c, float speed);    // 0x0097EC00
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float turnSpeed);   // 0x0097D580
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    __forceinline unsigned long Random()
    {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24A1U + 0x24DFU;
        return *seed = GFROR13(*seed);
    }
    void Update();
};

void CAIStateGroupDrunkMethods::Update()
{
    CThingCreatureBase* creature = GetCreature();
    bool has;
    has = creature->TestTC(0xc3);
    if (!has) {
        Terminate();
        return;
    }
    CTCDrunk* drunk = (CTCDrunk*)creature->GetTC(0xc3);
    if (!drunk->Drunk) {
        Terminate();
        return;
    }
    float drunkness = drunk->Drunkness;
    if (GFFloatRandom(GetPWorldSeed(), 1.0f) < g_DrunkDischargeChance) {
        CIntelligentPointerThing target;
        float bestDistanceSquared = 0.0f;
        long markerDef;
        {
            CCharString name("MARKER_DISCHARGE_BODILY_FLUIDS");
            markerDef = CGameDefinitionManager::Get()->GetDefGlobalIndexFromName(name);
        }
        CThingCreatureBase* me = GetCreature();
        has = me->TestTC(0x23);
        if (has) {
            CTCVillageMember* member = (CTCVillageMember*)me->GetTC(0x23);
            if (member->GetVillage()) {
                CTCVillage* village = member->GetVillage();
                for (CMarkerListIterator it = village->MarkersBegin(); it != village->MarkersEnd(); ++it) {
                    CIntelligentPointerMarker& marker = *it;
                    if (IS_THING_ALIVE(&marker) && marker.GetPItem()->DefGlobalIndex == markerDef
                        && CThingFilter_HasVillageTask()(marker.GetPItem())) {
                        CThingCreatureBase* self = GetCreature();
                        CThing* thing = marker.GetPItem();
                        float distanceSquared = (thing->GetPosition() - self->CThing::GetPosition()).GetLengthSquared();
                        if (!IS_THING_ALIVE(&target) || distanceSquared < bestDistanceSquared) {
                            target = marker.GetPItem();
                            bestDistanceSquared = distanceSquared;
                        }
                    }
                }
            }
        }
        if (target.GetPItem() && !(target.GetPItem()->m_flags91 & 1)) {
            if (target.GetPItem() && !(target.GetPItem()->m_flags91 & 1)) {
                has = target.GetPItem()->TestTC(0x2f);
                if (has)
                    ((CTCVillageTask*)target.GetPItem()->GetTC(0x2f))->AssignCreatureToTask(GetCreature());
            }
            long result;
            do {
                TalkToSelfUpdate();
                result = AIAction_MoveToPos(2, target.GetPItem()->GetPosition(), &has, 3, true, true, 0, true, 0.0f);
            } while (result == 4);
            const CRightHandedSet& orientation = target.GetPItem()->PPhysics->GetOrientation();
            const C3DVector& position = target.GetPItem()->GetPosition();
            AIAction_LookAtPos(1, C3DVector(position.X + orientation.Forward.X, position.Y + orientation.Forward.Y,
                position.Z + orientation.Forward.Z), 0x11, 0.0027777f);
            long which = Random() & 1;
            CCharString animation(which ? "ST_PUKING" : "ST_PISSING");
            result = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), animation, CSoundPair(),
                false, true, false, 0, 0), false);
            if (target.GetPItem() && !(target.GetPItem()->m_flags91 & 1)) {
                has = target.GetPItem()->TestTC(0x2f);
                if (has)
                    ((CTCVillageTask*)target.GetPItem()->GetTC(0x2f))->AssignCreatureToTask(0);
            }
            if (result != 2) {
                Terminate();
                return;
            }
            drunkness -= 0.6f;
        }
    }
    if (drunkness < 0.0f)
        drunkness = 0.0f;
    drunk->Drunkness = drunkness;
    if (!drunk->Drunk) {
        Terminate();
        return;
    }
    CThingCreatureBase* walker = GetCreature();
    CTCCreatureNavigation* navigation;
    has = walker->TestTC(7);
    if (has)
        navigation = (CTCCreatureNavigation*)walker->GetTC(7);
    C3DVector wanderPosition;
    if (!navigation->GetNearbyWanderPosition(walker->GetPosition(), 5.0f, &wanderPosition, true)) {
        Terminate();
        return;
    }
    has = false;
    long result;
    do {
        TalkToSelfUpdate();
        result = AIAction_MoveToPos(2, wanderPosition, &has, 3, true, true, 0, true, 0.0f);
    } while (result == 4);
    if (result == 2 && Random() % 100 < 50) {
        unsigned long idles = Random() & 1;
        for (unsigned long i = 0; i < idles; ) {
            TalkToSelfUpdate();
            long idle = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), CCharString("ST_IDLE"),
                0, CSoundPair(), false, true, false, 0, 0), false);
            if (idle == 4)
                continue;
            if (idle != 2)
                break;
            ++i;
        }
    }
    Terminate();
}
