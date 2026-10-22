// CAIStateGroup_BirdFlyToFood::IsRunnable() -- runnable for a flying bird that has not eaten for 20 seconds, on a 74% roll: fly to the nearest dead creature (64% roll) or else the nearest BIRD_FOOD thing, if a flight position near it is found
// Leaf: IsRunnable@CAIStateGroup_BirdFlyToFood
// Model: the landed sibling CAIStateGroup_BirdGroundWander::IsRunnable 0x009143E0 (same TU: interface 7 lookup, mode getter,
//   frame-rate local) and GetRandomFlightPos 0x00912700; layout local: FlightPos +0x24 (C3DVector), FrameLastAte +0x30.
// Shape: the two GetPBestThing<Filter, Compare> instantiations have same-TU out-only stand-in bodies that read the
//   comparator's position by value, so neither the out pointer nor the comparator temporary escapes: VC7.1 then stores
//   the out pointer's zero after loading the creature and loads the whole position before the stores (as retail).
struct CTCBase;

long __fastcall GFGetCurrentFrame();            // 0x0049D870
extern long g_framesPerSecond;                  // 0x01375550
extern long g_flyingMode;                       // 0x0129CBA4 (the mode compared by BirdGroundWander too)

struct CWorld {
    static long GetFramesPerSecond() { return g_framesPerSecond; }
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)


struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

struct C3DVector { float X, Y, Z; };
struct CThingPhysical : CThing {
    const C3DVector& GetPosition() const;       // 0x006A4D00
};

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};
struct CThingFilter_None { CThingFilter_None() {} };
struct CThingFilter_DefName {
    CCharString DefName;
    CThingFilter_DefName(const CCharString& name) : DefName(name) {}
};
struct CThingCompare_Nearest {
    C3DVector Position;
    CThingCompare_Nearest(const C3DVector& position) : Position(position) {}
};
struct CThingSearchTools;
CThingPhysical* FindBest_009142B0(CThingSearchTools* tools, float x, float y, float z);
CThingPhysical* FindBest_00914330(CThingSearchTools* tools, const CThingFilter_DefName& filter, float x, float y, float z);
struct CThingSearchTools {
    bool GetPBestThing(const CThingFilter_None& filter, const CThingCompare_Nearest& compare, CThingPhysical** out);      // 0x009142B0
    bool GetPBestThing(const CThingFilter_DefName& filter, const CThingCompare_Nearest& compare, CThingPhysical** out);   // 0x00914330
};
#pragma auto_inline(off)
// Same-TU out-only stand-ins for the two GetPBestThing<Filter, Compare> instantiations (compiled in the caller's TU):
// they only write the out pointer, so VC7.1 knows the caller's out local does not escape.
__declspec(noinline) bool CThingSearchTools::GetPBestThing(const CThingFilter_None& filter, const CThingCompare_Nearest& compare, CThingPhysical** out)
{
    CThingPhysical* best = FindBest_009142B0(this, compare.Position.X, compare.Position.Y, compare.Position.Z);
    if (best == 0)
        return false;
    *out = best;
    return true;
}
__declspec(noinline) bool CThingSearchTools::GetPBestThing(const CThingFilter_DefName& filter, const CThingCompare_Nearest& compare, CThingPhysical** out)
{
    CThingPhysical* best = FindBest_00914330(this, filter, compare.Position.X, compare.Position.Y, compare.Position.Z);
    if (best == 0)
        return false;
    *out = best;
    return true;
}
#pragma auto_inline(on)

unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}

struct CTCCreatureModeManager {                 // interface 7
    long GetCurrentModeType();                  // 0x006CB6A0
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingPhysical* Creature;                   // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFA0
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
};
struct CAIStateGroup_FlyingThingBase : CAIStateGroupBase {
    unsigned char _14[0x10];
    C3DVector FlightPos;                        // +0x24
    bool GetRandomFlightPos(const C3DVector& from, float radius, C3DVector* out, bool allowGround);   // 0x00912700
};
struct CAIStateGroup_BirdFlyToFood : CAIStateGroup_FlyingThingBase {
    unsigned long FrameLastAte;                 // +0x30
    bool IsRunnable();
};

bool CAIStateGroup_BirdFlyToFood::IsRunnable()
{
    CTCCreatureModeManager* modes;
    PBrain->Creature->GetInterface(&modes, 7);
    if (modes->GetCurrentModeType() == g_flyingMode) {
        long framesPerSecond;
        if ((FrameLastAte == 0
             || (framesPerSecond = GetPWorld()->GetFramesPerSecond(),
                 GFGetCurrentFrame() - FrameLastAte > (unsigned long)(framesPerSecond * 20)))
            && WorldRandomRange(*GetPWorldSeed(), 100) < 74) {
            if (WorldRandomRange(*GetPWorldSeed(), 100) < 64) {
                CThingPhysical* creature = PBrain->Creature;
                CThingPhysical* corpse = 0;
                if (PeekThingSearchTools()->GetPBestThing(CThingFilter_None(), CThingCompare_Nearest(creature->GetPosition()), &corpse)
                    && GetRandomFlightPos(corpse->GetPosition(), 0.5f, &FlightPos, true))
                    return true;
            }
            CThingPhysical* food = 0;
            bool found;
            {
                CCharString name("BIRD_FOOD");
                found = PeekThingSearchTools()->GetPBestThing(CThingFilter_DefName(name), CThingCompare_Nearest(PBrain->Creature->GetPosition()), &food);
            }
            if (found && GetRandomFlightPos(food->GetPosition(), 1.5f, &FlightPos, true))
                return true;
        }
    }
    return false;
}