// CAIStateGroup_MinionSitOnSeat::Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority, const CCharString&)
// -- the base initialisation, then a random sit-down frame within the next 80 seconds.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090F690, length 125 bytes (+ int3 padding), `ret 14h`. Vtable slot 1.
// Callees: CAIStateGroupBase::Initialise @ 0x008FCEC0 (the five arguments forwarded from the frame), CAIStateGroupBase::
// GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890 (__fastcall), CWorld::GetFrame @ 0x0049D870 (__fastcall).
// Global: the frame-rate long @ 0x01375550, read AFTER the base call (`fps*80` = `lea esi,[esi+esi*4] ; shl esi,4`).
// Shape: the landed random-frames step (seed*0x24A1+0x24DF, GFROR13, `range ? seed % range : 0` with an unsigned
// `div`; 0x00934090) then `FrameToSitDown = GFGetCurrentFrame() + r` (+0x3c, engine header name). The class is
// modelled locally (the header flattens the base).
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
    unsigned long HasTC(long interfaceType) const     // the masked word (a bool return narrows to `shr ; test dl,1`)
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
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
    protected:
    unsigned long* GetPWorldSeed() const;             // 0x008FCFD0 (world random seed)
    public:
};

class CWorld { public: static long __fastcall GetFrame(); };            // 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated)
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

class CCharString;
extern long g_framesPerSecond;                  // 0x01375550

struct CAIStateGroupBaseI : CAIStateGroupBase {
    void Initialise(CAIBrain& brain, CThingAICreature& creature, long id, long priority, const CCharString& name);  // 0x008FCEC0
};

struct CAIStateGroup_MinionSitOnSeat : CAIStateGroupBaseI {
    unsigned char _seat[0x28];                  // +0x14..+0x3b UsesSeat members + POwner
    long FrameToSitDown;                        // +0x3c
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long id, long priority, const CCharString& name);
};

void CAIStateGroup_MinionSitOnSeat::Initialise(CAIBrain& brain, CThingAICreature& creature, long id, long priority, const CCharString& name)
{
    CAIStateGroupBaseI::Initialise(brain, creature, id, priority, name);
    long framesPerSecond = g_framesPerSecond;
    unsigned long* seed = GetPWorldSeed();
    unsigned long range = framesPerSecond * 80;
    unsigned long next = *seed * 0x24A1U + 0x24DFU;
    *seed = next;
    unsigned long rotated = GFROR13(next);
    *seed = rotated;
    unsigned long r;
    if (range != 0)
        r = rotated % range;
    else
        r = 0;
    FrameToSitDown = CWorld::GetFrame() + r;
}