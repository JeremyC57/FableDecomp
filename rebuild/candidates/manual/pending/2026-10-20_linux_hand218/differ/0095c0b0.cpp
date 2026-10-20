// CAIStateGroup_HoverHornetQueen::WaitWhileBabiesFight() -- keep out of the way while the target lives, the first circle marker (if any) lives, the group is not stopping, the boss still has living helper creatures and is not ready for a phase change
// Grade: DIFFER(ours v retail) -- register rotation only: the loop-carried in-loop boss lookup gets esi in ours (retail edi) and the first circle marker pointer gets edi (retail esi); code, frame and control flow otherwise identical
// Retail: Fable.exe (TLC, VC7.1) @ 0x0095C0B0, 307 bytes (`ret`). Ego_r.pdb: ?WaitWhileBabiesFight@CAIStateGroup_HoverHornetQueen@@IAE?AW4EAIActionReturn@@XZ.
// Callees: CVectorMap::LowerBound 0x0040F020 (the boss lookup before the loop), GetPItem 0x00A01B50, IsStopping 0x008FD010,
// CTCBoss::CountLivingHelperCreatures 0x007AEEC0 (manifest "CountEffects"), CTCBoss::IsReadyForPhaseChange 0x007AEE90,
// KeepOutOfWay 0x0095B6F0 (this class, result ignored); returns 2.
// Shape: the 0xad (CTCBoss) lookup twice -- once before the loop through the out-of-line LowerBound (key and result
// share one frame word), once in the loop as the in-place binary search; CircleMarkers is the retail 12-byte vector at +0x50.
// Leaf: WaitWhileBabiesFight@CAIStateGroup_HoverHornetQueen
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;
struct CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const     // the masked word (a bool return narrows to `shr ; test dl,1`)
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01244344
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
};


// The component lookup of this TU inlines the STLport __lower_bound over the {type, component} vector.
struct CInlineInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
};
struct CThingI {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInlineInterfaceMap m_interfaces;           // +0x44
    bool HasTC(long type) const { if (m_presentBits[type >> 5] & (1UL << (type & 31))) return true; return false; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long type)
    {
        if (HasTC(type)) {
            CInlineInterfaceMap::SEntry* last = m_interfaces.last;
            CInlineInterfaceMap::SEntry* it = m_interfaces.first;
            long len = last - it;
            while (len > 0) {
                long half = len / 2;
                if (it[half].type < type) {
                    it += half + 1;
                    len = len - half - 1;
                }
                else
                    len = half;
            }
            if (it == last || type < it->type)
                it = last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};


struct CTCBoss {                                // interface 0xad
    long CountLivingHelperCreatures() const;    // 0x007AEEC0
    bool IsReadyForPhaseChange() const;         // 0x007AEE90
};

enum EAIActionReturn { AI_ACTION_RETURN_0, AI_ACTION_RETURN_1, AI_ACTION_RETURN_2 };

struct CStateGroupStop : CAIStateGroupBase {
    bool IsStopping();                          // 0x008FD010
};

#pragma pack(push, 1)
struct CAIStateGroup_HoverHornetQueen : CStateGroupStop {
    CBaseIntelligentPointer PTarget;            // +0x14
    unsigned char _pad1c[0x34];                 // +0x1c StartPos .. Things .. CurrentCirclePoint
    CBaseIntelligentPointer* CircleMarkers_First;   // +0x50 retail 12-byte vector<CIntelligentPointer<CThing> >
    CBaseIntelligentPointer* CircleMarkers_Last;    // +0x54
    CBaseIntelligentPointer* CircleMarkers_End;     // +0x58
    unsigned long CircleMarkersSize() const { return CircleMarkers_Last - CircleMarkers_First; }
    EAIActionReturn KeepOutOfWay();             // 0x0095B6F0
    EAIActionReturn WaitWhileBabiesFight();
};
#pragma pack(pop)

EAIActionReturn CAIStateGroup_HoverHornetQueen::WaitWhileBabiesFight()
{
    CTCBoss* boss;
    PBrain->PCreature->GetInterface(&boss, 0xad);
    CTCBoss* current = boss;
    while (IS_THING_ALIVE(PTarget)) {
        if (CircleMarkersSize() > 0 && !IS_THING_ALIVE(CircleMarkers_First[0]))
            break;
        if (IsStopping())
            break;
        ((CThingI*)PBrain->PCreature)->GetInterface(&current, 0xad);
        if (current->CountLivingHelperCreatures() <= 0 || boss->IsReadyForPhaseChange())
            break;
        KeepOutOfWay();
    }
    return AI_ACTION_RETURN_2;
}
