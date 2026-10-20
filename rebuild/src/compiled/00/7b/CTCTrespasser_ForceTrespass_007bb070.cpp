// CTCTrespasser::ForceTrespass() -- when the trespassed thing is alive and trespassable, posts the trespass deed to the owner's opinion deed log, keeping the handle when no trespass deed is open yet
// Leaf: ForceTrespass
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BB070, 219 bytes (plain `ret`). Ego_r.pdb
// `?ForceTrespass@CTCTrespasser@@QAEXXZ`.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (IS_THING_ALIVE, the trespassable test and the deed
//          target); CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (interface 0x51 = word 2 bit 17, the
//          opinion deed log); CThing::GetPosition() const 0x004C73D0; CTCHeroOpinionDeedLog::PostDeed(EOpinionDeedType,
//          const C3DVector&, const CThing*, CThingAICreature*, EOpinionPostDeedType, bool, bool, const CFaction*, bool,
//          long) 0x006C6990 (Ego_r.pdb signature; the landed callers spell it four different ways).
// Layout (Ego_r.pdb CTCTrespasser, retail equal up to the list): DeedHandles[3] +0x10, TrespassingThing +0x2d (an
//         intelligent pointer at an odd offset: the pointer classes are `#pragma pack(1)`); the thing's dead flag +0x91
//         bit 0, interface bitset +0x20 (0xBD, trespassable = word 5 bit 29), map +0x44.
// Shape: `T* log = 0;` preloads edi, and the out-parameter GetInterface result is `&&`-ed with the open-handle test, so
//        the absent component skips straight to the else call; both arms are complete PostDeed calls (deed type 2,
//        post type 0x12) differing in the seventh argument, the first storing the new handle.
struct CTCBase;
struct CBaseObject;
struct CFaction;
struct CThingAICreature;
struct C3DVector { float X, Y, Z; };

enum EOpinionDeedType { OPINION_DEED_TYPE_TRESPASS = 2 };
enum EOpinionPostDeedType { OPINION_POST_DEED_TYPE_0x12 = 0x12 };

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad_0x4c[0x45];
    unsigned char m_flags;                      // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
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
    const C3DVector& GetPosition() const;       // 0x004C73D0
};

struct CTCHeroOpinionDeedLog {                  // interface 0x51
    long PostDeed(EOpinionDeedType deedType, const C3DVector& position, const CThing* target, CThingAICreature* victim,
                  EOpinionPostDeedType postType, bool b1, bool b2, const CFaction* faction, bool b3, long extra);   // 0x006C6990
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {                // 8 bytes: vptr + item
    void* __vftable;
    CBaseObject* PData;
    CThing* GetPItem() const;                   // 0x00A01B50
};
#pragma pack(pop)

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

struct CTCTrespasser {
    void*         __vftable;                    // +0x00
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    bool          ToBeRemoved;                  // +0x08
    bool          IsInGlobalUpdateTCs;          // +0x09
    unsigned char _pad0a[2];
    long          InsideBuildingNotTrespassingDeedHandle;   // +0x0c
    long          DeedHandles[3];               // +0x10
    long          CurrentDeedHandleIndex;       // +0x1c
    long          OccupantsKnowOfDeedHandleIndex;   // +0x20
    unsigned long FrameSomeoneWasInformedOfLastDeed;    // +0x24
    unsigned long FramePlayerLeft;              // +0x28
    bool          AttachedOpinionSource;        // +0x2c
    CBaseIntelligentPointer TrespassingThing;   // +0x2d CIntelligentPointer<CThing>
    void ForceTrespass();
};

void CTCTrespasser::ForceTrespass()
{
    if (IS_THING_ALIVE(TrespassingThing) && TrespassingThing.GetPItem()->HasTC(0xbd)) {
        CTCHeroOpinionDeedLog* log = 0;
        if (ParentThing->GetInterface(&log, 0x51) && DeedHandles[2] == -1)
            DeedHandles[2] = log->PostDeed(OPINION_DEED_TYPE_TRESPASS, ParentThing->GetPosition(), TrespassingThing.GetPItem(), 0,
                                           OPINION_POST_DEED_TYPE_0x12, true, true, 0, false, 0);
        else
            log->PostDeed(OPINION_DEED_TYPE_TRESPASS, ParentThing->GetPosition(), TrespassingThing.GetPItem(), 0,
                          OPINION_POST_DEED_TYPE_0x12, true, false, 0, false, 0);
    }
}