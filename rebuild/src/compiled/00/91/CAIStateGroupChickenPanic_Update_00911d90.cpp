// CAIStateGroup_ChickenPanic::Update() -- while the last attacker is within 8 units, run to the nearest flee position; then terminate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00911D90, length 332 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetPosition @ 0x004C73D0 and the const copy @ 0x006A4D00
//          (manifest "CVertexBufferWin32::DoSizeof" is propagated), CTCCreatureNavigation::GetNearestFleePosition(const C3DVector&,
//          float, C3DVector*) @ 0x006CB840, CAIStateGroupBase::AIAction_MoveToPosProximity @ 0x0097E890, AIAction_DoNothing
//          @ 0x0097E200, Terminate @ 0x008FCF40 (manifest "CXMVPlaybackInfo" is propagated).
// Shape: the navigation component is a function-scope out-parameter (its never-written home is loaded into ebp at entry
//          and survives the passes); the presence test is the bitset::test form (`shr ; and al,1` into a frame byte) and
//          the vector-map lower_bound is inlined in this TU (the `cdq ; sub ; sar 1` halving loop on the constant key 7);
//          the XY distance is a named C2DVector delta whose squared length is a named float compared with 64.0f
//          (`fstp st(2) ; fstp st(0) ; fcomp`); the hit/end pick is one ternary (the lower_bound result register is
//          reused); the creature test goes through an inline brain accessor (`lea ecx,[eax+0F4h]`); the attacker is
//          fetched inside the call (after the two constant pushes); the flee target and the reached flag share frame
//          slots with the test byte.
// Layout: creature LastAttacker intelligent pointer +0xF4 (CThingCreatureBase), bitset +0x20, interface map +0x44.
//          No engine header exists for CAIStateGroup_ChickenPanic; the base follows engine/CAIStateGroupBase.h.
class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)        // STLport lower_bound over the 8-byte {type, component} entries (inlined here)
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

class C3DVector { public:
    float x, y, z;
    C3DVector() { }
    C3DVector(const C3DVector& v) { x = v.x; y = v.y; z = v.z; }
};
class C2DVector { public:
    float x, y;
    float GetLengthSquared() const { return x * x + y * y; }
};

class CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0xf4 - 0x4c];
    CBaseIntelligentPointer LastAttacker;       // +0xf4 (CThingCreatureBase)
    const C3DVector& GetPosition();             // 0x004C73D0
    const C3DVector& GetPosition_006a4d00() const;       // 0x006A4D00
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

class CTCCreatureNavigation { public:                  // interface 7
    bool GetNearestFleePosition(const C3DVector& centre, float radius, C3DVector* result);   // 0x006CB840
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
    CThing* GetPCreature() const { return PCreature; }
};

class CAIStateGroupBase { public:
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
};

struct CAIStateGroup_ChickenPanic : CAIStateGroupBase {
    virtual void Update();
};

void CAIStateGroup_ChickenPanic::Update()
{
    CTCCreatureNavigation* navigation;
    do {
        if (!PBrain->GetPCreature()->LastAttacker.GetPItem())
            break;
        CThing* creature = PBrain->PCreature;
        const C3DVector& attackerPos = creature->LastAttacker.GetPItem()->GetPosition();
        const C3DVector& pos = creature->GetPosition();
        C2DVector delta;
        delta.x = pos.x - attackerPos.x;
        delta.y = pos.y - attackerPos.y;
        float distanceSquared = delta.GetLengthSquared();
        if (!(distanceSquared < 64.0f))
            break;
        CThing* chicken = PBrain->PCreature;
        chicken->GetInterfaceT(&navigation, 7);
        C3DVector fleePos;
        if (navigation->GetNearestFleePosition(((const CThing*)chicken->LastAttacker.GetPItem())->GetPosition_006a4d00(), 8.0f, &fleePos)) {
            bool reached = false;
            AIAction_MoveToPosProximity(1, fleePos, 1.0f, &reached, 5, true, false, true, 0, true, 0.0f);
        }
    } while (AIAction_DoNothing() == 4);
    Terminate();
}