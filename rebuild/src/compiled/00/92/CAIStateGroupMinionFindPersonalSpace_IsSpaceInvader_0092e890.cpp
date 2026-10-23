// CAIStateGroup_MinionFindPersonalSpace::IsSpaceInvader(CThing*) -- a live thing standing still (its motion block's
// current and previous positions agree within 1e-4 on every axis) that is not in creature mode 8 or 9.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092E890, length 179 bytes (+ int3 padding). `ret 4`, `this` unused.
// Callees: CVectorMap::LowerBound @ 0x004365B0 (inlined HasTC + entry lookup, type 0x31 = CTCCreatureModeManager,
// bit 17 of the bitset dword at +0x24), CTCCreatureModeManager::HasMode @ 0x0073A8A0 (twice); 1e-4f @ 0x0129BA3C.
// Shape: `(float)fabs(a - b) <= 1e-4f` gives `fcomp ; test ah,41h ; jp`; the motion block is the thing's +0x60
// pointer with two C3DVectors at +0x0c / +0x18. The lookup key is stored in the dead argument slot ([esp+8]).
// No engine header for this class.
#include <math.h>
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;       // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
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
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->type <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5  (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CBaseIntelligentPointerCtor {            // CBaseIntelligentPointer (8 bytes: vptr + item)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointerCtor();             // 0x00A01C10
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CThingMotion {
    unsigned char _pad[0xc];
    float PosX, PosY, PosZ;                     // +0x0c
    float LastPosX, LastPosY, LastPosZ;         // +0x18
};
struct CThingWithMotion {
    unsigned char _pad[0x60];
    CThingMotion* PMotion;                      // +0x60
};

struct CTCCreatureModeManager {
    bool HasMode(long mode);                    // 0x0073A8A0
};

struct CAIStateGroup_MinionFindPersonalSpace : CAIStateGroupBase {
    bool IsSpaceInvader(CThing* thing);
};

bool CAIStateGroup_MinionFindPersonalSpace::IsSpaceInvader(CThing* thing)
{
    if (thing != 0 && !(thing->m_flags91 & 1)) {
        CThingMotion* motion = ((CThingWithMotion*)thing)->PMotion;
        if ((float)fabs(motion->PosX - motion->LastPosX) <= 1e-4f &&
            (float)fabs(motion->PosY - motion->LastPosY) <= 1e-4f &&
            (float)fabs(motion->PosZ - motion->LastPosZ) <= 1e-4f && thing->HasTC(0x31)) {
            CTCCreatureModeManager* modes = (CTCCreatureModeManager*)thing->FindEntry(0x31)->component;
            if (!modes->HasMode(8) && !modes->HasMode(9))
                return true;
        }
    }
    return false;
}