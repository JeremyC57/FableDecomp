// CAIStateGroup_MinionKidGroupTag::Cleanup() -- leave the tag game, drop the stand mode if this state group added it,
// and remember a randomised "last played" frame (now + up to 2 seconds of frames).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009347B0, length 171 bytes (+ int3 padding). Vtable slot 9.
// Callees: the group-tag component's virtual slot 29 (+0x74) with the creature, CVectorMap::LowerBound @ 0x0040F020
// (inlined GetInterface, type 0x31 = CTCCreatureModeManager, bit 17 of the bitset dword at +0x24; miss path reads
// the never-written out slot), CTCCreatureModeManager::RemoveMode @ 0x0073ABA0, CAIStateGroupBase::GetPWorldSeed
// @ 0x008FCFD0, GFROR13 @ 0x00497890, GFGetCurrentFrame @ 0x0049D870; frames-per-second long @ 0x01375550.
// Shape: the landed InitStateGroup sibling's inline RandomFramesWithin(seconds) (global read before the seed call,
// `add edi,edi` = 2 seconds), `FrameLastPlayedIn = GFGetCurrentFrame() + RandomFramesWithin(2)`.
// Members are the engine header names; the header flattens the base, so the class is modelled locally.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->first <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
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

class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};            // 0x0049D870
long __fastcall GFROR13(unsigned long);   // 0x00497890
extern long g_framesPerSecond;                  // 0x01375550

class CTCCreatureModeManager { public:
public:
    void RemoveMode(unsigned long);   // 0x0073ABA0
};

class CTCCreatureGroupTag { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual void RemoveMember(CThing* creature);    // slot 29 (+0x74)
};

struct CAIStateGroup_MinionKidGroupTag : CAIStateGroupBase {
    bool InitiatedTag;                          // +0x14
    unsigned char _pad15[3];
    CTCCreatureGroupTag* PTCGroupTag;           // +0x18
    float InitialTagPos_X, InitialTagPos_Y, InitialTagPos_Z;    // +0x1c
    bool StandModeAddedToCreature;              // +0x28
    unsigned char _pad29[3];
    long FrameLastPlayedIn;                     // +0x2c
    unsigned long* GetPWorldSeed();             // 0x008FCFD0 (CAIStateGroupBase)
    unsigned long RandomFramesWithin(long seconds)
    {
        long framesPerSecond = g_framesPerSecond;
        unsigned long* seed = GetPWorldSeed();
        unsigned long range = framesPerSecond * seconds;
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        if (range != 0)
            return rotated % range;
        return 0;
    }
    virtual void Cleanup();
};

void CAIStateGroup_MinionKidGroupTag::Cleanup()
{
    PTCGroupTag->RemoveMember(PBrain->PCreature);
    if (StandModeAddedToCreature) {
        CTCCreatureModeManager* modes;
        PBrain->PCreature->GetInterface(&modes, 0x31);
        modes->RemoveMode(1);
    }
    FrameLastPlayedIn = CWorld::GetFrame() + RandomFramesWithin(2);
}