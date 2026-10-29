// CAIStateGroup_TellStory::Update() -- find the nearest story place marker, then while the villager's village says it is cook time: gather the other creatures within 10 units of the marker, walk to the marker facing a random listener (or the marker's facing point), and tell the story (talk for 2-4 seconds, then act for 3-6); terminates when stopped or cook time ends
// Leaf: ?Update@CAIStateGroup_TellStory
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00942EE0, length 767 bytes (0x68 frame, virtual slot 12).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 ("STORY_PLACE_MARKER") / ~CCharString @ 0x0099EAE0,
//   CGameDefinitionManager::Get @ 0x0044C6B0 / GetDefGlobalIndexFromName @ 0x009AD410, CThing::GetPos @ 0x004C73D0,
//   PeekThingSearchTools @ 0x008FD000, GetPBestThing<HasDefGlobalIndex, Nearest> @ 0x009107B0 and the creature-filter
//   FindThings @ 0x00943830 (both same-TU templates: noinline stand-ins here), the marker body's virtual slot 70 (+0x118,
//   C3DVector by hidden pointer), CVectorMap::LowerBound const @ 0x004365B0 (interface 0x23), GFGetPVillageInterface
//   @ 0x007EE540, CTCVillage::IsCookTime @ 0x00658A80, IsStopping @ 0x008FD010, GetPWorldSeed @ 0x008FCFD0 + GFROR13
//   @ 0x00497890 (inline random step x3), MoveAndFace @ 0x009425F0, StandAndTalk @ 0x00942C70, StandAndAct @ 0x00942920
//   (landed as StandAndAct(long); the caller passes a float), Terminate @ 0x008FCF40, free @ 0x00BFEA14 (vector storage).
// Layout: the landed CAIStateGroup_TellStory::IsRunnable 0x00942D40 model; the marker's physics body (+0x60) holds its
//   position at +0x0c.
// Shape: C3DVector has a user copy ctor (the marker position stays in the frame); the 0x23 present bit is a named bool
//   (`shr ; and cl,1 ; mov [esp+3],cl`) and the empty creature filter has NO user ctor, so both share the byte at +3;
//   FindThings / GetPBestThing get same-TU noinline stand-ins (the listener vector's address does not escape, so the
//   start pointer stays in ebp for the free); the vector is declared first in the loop body (its zero register is
//   scheduled before the filter stores); `start` is a named copy of _M_start.
extern "C" void __cdecl free(void* p);          // 0x00BFEA14
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const; // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class C3DVector { public:
    float x, y, z;
    C3DVector() {}
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(x + o.x, y + o.y, z + o.z); }
};
struct CThingPhysicsBody {
    virtual void v00();
    unsigned char _pad04[0x0c - 0x04];
    C3DVector Pos;                              // +0x0c
};
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
struct CThingBody {                             // the physics body's vtable view
    V16(a) V16(b) V16(c) V16(d) V4(e) virtual void f0(); virtual void f1();
    virtual C3DVector GetForward() const;       // slot 70 (+0x118)
    unsigned char _pad04[0x0c - 0x04];
    C3DVector Pos;                              // +0x0c
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CThingBody* PBody;                          // +0x60
    const C3DVector& GetPos() const;       // 0x004C73D0
    bool TestTC(long interfaceType) const
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType) const
    {
        bool has = TestTC(interfaceType);
        if (has) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
        }
        return has;
    }
};
struct CTCVillager;
class CTCVillage { public:
    bool IsCookTime();                          // 0x00658A80
};
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);  // 0x007EE540
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CGameDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();                 // 0x0044C6B0
    long GetDefGlobalIndexFromName(const CCharString& name);        // 0x009AD410
};
class CThingFilter_HasDefGlobalIndex { public:
    long DefGlobalIndex;
    CThingFilter_HasDefGlobalIndex(long index) : DefGlobalIndex(index) {}
};
class CThingCompare_Nearest { public:
    C3DVector Pos;
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) {}
};
struct CThingFilter_IsCreature { };
struct CThingFilter_NotThing {
    CThing* Thing;
    CThingFilter_NotThing(CThing* thing) : Thing(thing) {}
};
struct CThingFilter_InRadius {
    C3DVector Pos;
    float Radius;
    CThingFilter_InRadius(const C3DVector& pos, float radius) : Pos(pos), Radius(radius) {}
};
struct CThingFilter_And3 {
    const CThingFilter_IsCreature* A;
    const CThingFilter_NotThing* B;
    const CThingFilter_InRadius* C;
    CThingFilter_And3(const CThingFilter_IsCreature& a, const CThingFilter_NotThing& b, const CThingFilter_InRadius& c)
        : A(&a), B(&b), C(&c) {}
};
struct CThingPtrVector {                        // std::vector<CThing*> (STLport, 12 bytes)
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThingPtrVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CThingPtrVector() { if (_M_start) free(_M_start); }
};
class CThingSearchTools { public:
    bool GetPBestThing(const CThingFilter_HasDefGlobalIndex& filter, const CThingCompare_Nearest& compare, CThing** out);   // 0x009107B0
    void FindThings(const CThingFilter_And3& filter, CThingPtrVector& out);   // 0x00943830
};
long BestSink_9107B0(const void* self, long index, float x);
#pragma auto_inline(off)
__declspec(noinline) bool CThingSearchTools::GetPBestThing(const CThingFilter_HasDefGlobalIndex& filter, const CThingCompare_Nearest& compare, CThing** out)
{
    long best = BestSink_9107B0(this, filter.DefGlobalIndex, compare.Pos.x);
    *out = (CThing*)best;
    return best != 0;
}
#pragma auto_inline(on)
long FindSink_943830(const void* self, float radius);
#pragma auto_inline(off)
__declspec(noinline) void CThingSearchTools::FindThings(const CThingFilter_And3& filter, CThingPtrVector& out)
{
    long found = FindSink_943830(this, filter.C->Radius);
    out._M_finish = out._M_start + found;
}
#pragma auto_inline(on)
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    void Terminate();                           // 0x008FCF40
    bool IsStopping();                          // 0x008FD010
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
};
struct CAIStateGroup_TellStory : CAIStateGroupBase {
    CThing* GetCreature() const { return PBrain->PCreature; }
    bool MoveAndFace(C3DVector pos, float radius, C3DVector facePos);   // 0x009425F0
    bool StandAndTalk(float seconds);           // 0x00942C70
    void StandAndAct(float seconds);            // 0x00942920
    virtual void Update();
};

void CAIStateGroup_TellStory::Update()
{
    long index;
    {
        CCharString name("STORY_PLACE_MARKER");
        index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
    }
    CThing* marker;
    {
        CThingCompare_Nearest compare(GetCreature()->GetPos());
        CThingFilter_HasDefGlobalIndex filter(index);
        PeekThingSearchTools()->GetPBestThing(filter, compare, &marker);
    }
    CThingBody* body = marker->PBody;
    C3DVector markerPos = body->Pos;
    C3DVector forward = body->GetForward();
    CTCVillager* villager;
    {
        const CThing* creature = GetCreature();
        bool hasVillager = ((creature->m_presentBits[0x23 >> 5] >> (0x23 & 31)) & 1) != 0;
        if (hasVillager) {
            ETCInterfaceType type = (ETCInterfaceType)(0x23);
            const CInterfaceMap* map = &creature->m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || 0x23 < it->first)
                it = map->last;
            villager = (CTCVillager*)it->second;
        }
    }
    while (GFGetPVillageInterface(villager)->IsCookTime()) {
        if (IsStopping())
            break;
        CThingPtrVector listeners;
        CThingFilter_InRadius inRadius(markerPos, 10.0f);
        CThingFilter_IsCreature isCreature;
        CThingFilter_NotThing notMe(GetCreature());
        PeekThingSearchTools()->FindThings(CThingFilter_And3(isCreature, notMe, inRadius), listeners);
        CThing** start = listeners._M_start;
        long count = listeners._M_finish - start;
        C3DVector facePos;
        if (count != 0)
            facePos = start[Random(count)]->GetPos();
        else
            facePos = forward + markerPos;
        MoveAndFace(markerPos, 0.0f, facePos);
        if (start != listeners._M_finish) {
            StandAndTalk((float)(Random(3) + 2));
            StandAndAct((float)Random(4) + 3.0f);
        }
    }
    Terminate();
}