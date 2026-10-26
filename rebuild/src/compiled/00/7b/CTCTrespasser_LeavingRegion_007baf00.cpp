// CTCTrespasser::LeavingRegion() -- the trespasser has left: while the trespassed thing lives, end and drop the open trespass deeds in the owner's deed log (the main one, index 2, is ended but kept), forget the informed-occupants index and mark the trespassable as no longer trespassed (stamping the frame); then clear the trespassed thing and detach the opinion source if one was attached
// Leaf: LeavingRegion
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BAF00, length 368 bytes (plain `ret`, no frame pointer; Ego_r.pdb
//   `?LeavingRegion@CTCTrespasser@@UAEXXZ`).
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (inline IS_THING_ALIVE, the trespassable lookup, the
//   temporary), CBaseIntelligentPointer() 0x00A01B10, SetPItem 0x00A01B90 and ~CBaseIntelligentPointer 0x00A01C10 (the
//   inline CIntelligentPointer<CThing> ctor / operator=, vtable 0x01238C6C, as landed); CVectorMap LowerBound 0x0040F020
//   (interface 0x51 = CTCHeroOpinionDeedLog, bit 17 of +0x28; 0xBD = CTCTrespassable, bit 29 of +0x34);
//   CTCHeroOpinionDeedLog deed-ending call 0x006C5920 (descriptive EndDeed: stamps FrameHappened, clears
//   IsStillHappening, drops the handle from the witness search list) and RemoveDeed(long) 0x006C59E0 (map erase; manifest
//   "AIState_DestroyWithPointerCleanup"); CWorld::GetWorldFrame() 0x0049D870 (static); CTCBase::GetWorld() const
//   0x00686A70; COpinionReactionManager::ClearOpinionSource(CThing&, bool) 0x007008C0 (Ego_r.pdb signature; world +0x74).
// Layout: as the landed ForceTrespass 0x007BB070 (Ego_r.pdb = retail up to the list): InsideBuildingNotTrespassingDeedHandle
//   +0x0c, DeedHandles[3] +0x10, OccupantsKnowOfDeedHandleIndex +0x20, FramePlayerLeft +0x28, AttachedOpinionSource
//   +0x2c, TrespassingThing +0x2d (pack(1)); CTCTrespassable trespassed flag +0x0d (descriptive).
// Shape: the deed log is the plain GetTC pick; the trespassable comes through the out-parameter GetInterface on a
//   named `trespassed` thing (one GetPItem; the plain pick reloads the component into ecx instead of eax); the deed loop
//   indexes the handle array (pointer in edi, index in ebp) with the keep-index-2 test inside; the clear is an
//   assignment from a null CIntelligentPointer temporary sharing the lookup key's frame word; the opinion source is
//   cleared through an inline helper taking the parent thing and an inline CWorld getter for the reaction manager
//   (the parent loads into edi before GetWorld; with the field read directly VC7.1 pushes the arguments first and the
//   whole function's register assignment changes: `this` lands in ebp).
class CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad_0x4c[0x45];
    unsigned char m_flags;                              // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            *ppOut = (T*)GetTC(interfaceType);
            return true;
        }
        return false;
    }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return ((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
class CTCHeroOpinionDeedLog { public:                          // interface 0x51
    void EndDeed(long handle);                          // 0x006C5920 (descriptive)
    void RemoveDeed(long handle);                       // 0x006C59E0
};
class CTCTrespassable { public:                                // interface 0xBD
    unsigned char _pad00[0x0d];
    bool          BeingTrespassed;                      // +0x0d (descriptive)
};
class COpinionReactionManager { public:
    void ClearOpinionSource(CThing& thing, bool immediately);   // 0x007008C0
};
class CWorld {
public:
    unsigned char            _pad00[0x74];
    COpinionReactionManager* OpinionReactionManager;    // +0x74
    static unsigned long GetWorldFrame();               // 0x0049D870
    COpinionReactionManager* GetOpinionReactionManager() const { return OpinionReactionManager; }
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {                        // 8 bytes: vptr + item
    virtual void v00();
    CThing* PData;
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {  // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item = 0) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
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

class CTCTrespasser { public:
    void*         __vftable;                            // +0x00
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    bool          ToBeRemoved;                          // +0x08
    bool          IsInGlobalUpdateTCs;                  // +0x09
    unsigned char _pad0a[2];
    long          InsideBuildingNotTrespassingDeedHandle;   // +0x0c
    long          DeedHandles[3];                       // +0x10
    long          CurrentDeedHandleIndex;               // +0x1c
    long          OccupantsKnowOfDeedHandleIndex;       // +0x20
    unsigned long FrameSomeoneWasInformedOfLastDeed;    // +0x24
    unsigned long FramePlayerLeft;                      // +0x28
    bool          AttachedOpinionSource;                // +0x2c
    CIntelligentPointer TrespassingThing;               // +0x2d
    CWorld& GetWorld() const;                           // 0x00686A70 (CTCBase)
    void ClearOpinionSource(CThing* thing) { GetWorld().GetOpinionReactionManager()->ClearOpinionSource(*thing, true); }
    void LeavingRegion();
};

void CTCTrespasser::LeavingRegion()
{
    if (IS_THING_ALIVE(TrespassingThing)) {
        if (ParentThing->HasTC(0x51)) {
            CTCHeroOpinionDeedLog* log = (CTCHeroOpinionDeedLog*)ParentThing->GetTC(0x51);
            for (long i = 0; i < 3; ++i) {
                if (DeedHandles[i] != -1) {
                    log->EndDeed(DeedHandles[i]);
                    if (i != 2)
                        log->RemoveDeed(DeedHandles[i]);
                    DeedHandles[i] = -1;
                }
            }
            if (InsideBuildingNotTrespassingDeedHandle != -1) {
                log->EndDeed(InsideBuildingNotTrespassingDeedHandle);
                log->RemoveDeed(InsideBuildingNotTrespassingDeedHandle);
                InsideBuildingNotTrespassingDeedHandle = -1;
            }
        }
        OccupantsKnowOfDeedHandleIndex = -1;
        CThing* trespassed = TrespassingThing.GetPItem();
        CTCTrespassable* trespassable;
        if (trespassed->GetInterface(&trespassable, 0xbd)) {
            trespassable->BeingTrespassed = false;
            FramePlayerLeft = CWorld::GetWorldFrame();
        }
    }
    TrespassingThing = CIntelligentPointer(0);
    if (AttachedOpinionSource) {
        ClearOpinionSource(ParentThing);
        AttachedOpinionSource = false;
    }
}