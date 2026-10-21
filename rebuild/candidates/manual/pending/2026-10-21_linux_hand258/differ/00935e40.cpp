// CAIStateGroup_MinionPutOutFires::PutOutFire() -- while the burning building lives and is still on fire, run to within 3 units of it; once there (and it is still alive and burning) play ST_IDLE; true when the animation completed
// Grade: DIFFER(476v468) -- inline lower_bound loads first into esi directly (retail eax then mov esi,eax) and the PlayAnimation zero args are push 0 immediates (retail xor ecx,ecx; push ecx); all else matches
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00935E40, length 480 bytes (+ int3 padding), bool in al.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PBuildingOnFire), CTCFlammable::IsOnFire @ 0x00780400
// (interface 0x6d, bit 13 of +0x2c; the first lookup is an inline lower_bound, the second the const
// CInterfaceMap::LowerBound @ 0x004365B0), CThing::GetPosition @ 0x004C73D0, AIAction_MoveToPosProximity @ 0x0097E890,
// CCharString(const char*, long) @ 0x0099EBF0 ("ST_IDLE" @ 0x0127138C) / ~CCharString @ 0x0099EAE0,
// CCreatureAction_PlayAnimation ctor @ 0x008424F0, AIAction_PerformAction @ 0x0097D8B0, ~CCreatureActionBase @ 0x00693EF0.
// Layout: Ego_r.pdb CAIStateGroup_MinionPutOutFires (PBuildingOnFire +0x14); retail CThing interface bitset +0x20 /
// map +0x44.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    bool HasTC(long interfaceType) const { return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        bool present = HasTC(interfaceType);
        if (present) {
            CInterfaceMap::SEntry* first = m_interfaces.first;
            CInterfaceMap::SEntry* last = m_interfaces.last;
            int count = last - first;
            while (count > 0) {
                int half = count / 2;
                CInterfaceMap::SEntry* middle = first + half;
                if (middle->type < interfaceType) {
                    first = middle + 1;
                    count = count - half - 1;
                } else
                    count = half;
            }
            *ppOut = (T*)((first == last || interfaceType < first->type) ? last : first)->component;
        }
        return present;
    }
    bool TestTC(unsigned int pos) const { return (m_presentBits[pos / 32] & ((unsigned long)1 << pos % 32)) != 0; }
    template<class T> bool GetInterfaceConst(T** ppOut, long interfaceType) const
    {
        if (TestTC(interfaceType)) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingObject : CThing { };
struct CThingCreatureBase : CThing { };
struct CThingBuilding : CThingObject { };

struct CTCFlammable {                           // interface 0x6d
    bool IsOnFire() const;                      // 0x00780400
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
inline bool IsThingAlive(const CBaseIntelligentPointer& thing) { return thing.GetPItem() != 0 && !(thing.GetPItem()->m_flags91 & 1); }

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
struct CSoundPair {
    long FirstSound;
    long LastSound;
    CSoundPair(long first, long last) : FirstSound(first), LastSound(last) { }
};
struct CCreatureActionBase {
    virtual ~CCreatureActionBase();             // 0x00693EF0
    unsigned char _pad04[0xb0];
};
struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float proximity, bool& arrived, long movement,
                                     bool a, bool b, bool avoidDynamicObstacles, const CThing* ignore, bool c, float d);   // 0x0097E890
    long AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
};
struct CAIStateGroup_MinionPutOutFires : CAIStateGroupBase {
    CIntelligentPointer<CThingBuilding> PBuildingOnFire;   // +0x14
    bool PutOutFire();
};

bool CAIStateGroup_MinionPutOutFires::PutOutFire()
{
    bool arrived = false;
    for (;;) {
        if (!IsThingAlive(PBuildingOnFire))
            return false;
        CTCFlammable* flammable;
        if (!PBuildingOnFire.GetPItem()->GetInterface(&flammable, 0x6d))
            return false;
        if (!flammable->IsOnFire())
            return false;
        long result = AIAction_MoveToPosProximity(2, PBuildingOnFire.GetPItem()->GetPosition(), 3.0f, arrived, 5, true, false,
                                                  true, 0, true, 0.0f);
        if (result == 4)
            continue;
        if (result != 2)
            return false;
        break;
    }
    if (IsThingAlive(PBuildingOnFire)) {
        CTCFlammable* flammable;
        if (PBuildingOnFire.GetPItem()->GetInterfaceConst(&flammable, 0x6d) && flammable->IsOnFire()) {
            if (AIAction_PerformAction(&CCreatureAction_PlayAnimation(GetCreature(), "ST_IDLE", CSoundPair(0, 0), false, true,
                                                                      false, 0, 0)) == 2)
                return true;
        }
    }
    return false;
}
