// CAIStateGroup_MinionGossip::InitStateGroup(const CWorld&, CThingAICreature&) -- pretend the villager last
// gossiped a random 0..GetTimeToInitiate() seconds ago.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090C800, length 100 bytes (+ int3 padding), `ret 8` (both
// reference arguments unused). Vtable slot 13 of CAIStateGroup_MinionGossip.
// Callees: CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0, virtual slot 14 GetTimeToInitiate (+0x38, 0x0090C7E0
// returns 70 seconds), GFROR13 @ 0x00497890 (__fastcall), GFGetCurrentFrame @ 0x0049D870 (__fastcall).
// Global: the frame-rate long @ 0x01375550, read into ebp BEFORE the seed call.
// Shape: the same random-frames step as the landed CAIStateGroup_MinionKidGroupTag::InitStateGroup @ 0x00934090
// (`seed = seed*0x24A1 + 0x24DF; seed = GFROR13(seed); r = range ? seed % range : 0`, unsigned `div`), but the
// range is `GetTimeToInitiate() * fps` with the virtual call made after the seed pointer is fetched (the
// multiply is scheduled between the seed loads). FrameOfLastGossip is +0x14 (engine header name; the header
// flattens the base, so the class is modelled locally like its landed siblings).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool HasTC(long interfaceType) const
    {
        return (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
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

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
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
    unsigned long* GetPWorldSeed();             // 0x008FCFD0 (world random seed)
};

class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};            // 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated)
long __fastcall GFROR13(unsigned long);   // 0x00497890

extern long g_framesPerSecond;                  // 0x01375550

struct CAIStateGroup_MinionGossip : CAIStateGroupBase {
    long FrameOfLastGossip;                     // +0x14
    bool Critical;                              // +0x18
    bool NowRelyingOnGossipee;                  // +0x19
    virtual void InitStateGroup(const CWorld& world, CThingAICreature& creature);
};

void CAIStateGroup_MinionGossip::InitStateGroup(const CWorld& world, CThingAICreature& creature)
{
    long framesPerSecond = g_framesPerSecond;
    unsigned long* seed = GetPWorldSeed();
    unsigned long range = GetTimeToInitiate() * framesPerSecond;
    unsigned long next = *seed * 0x24A1U + 0x24DFU;
    *seed = next;
    unsigned long rotated = GFROR13(next);
    *seed = rotated;
    unsigned long r;
    if (range != 0)
        r = rotated % range;
    else
        r = 0;
    FrameOfLastGossip = CWorld::GetFrame() - r;
}