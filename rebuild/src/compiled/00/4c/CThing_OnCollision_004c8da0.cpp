// CThing::OnCollision(long phase, long, long, CThing* other) -- on the first collision phase, hands the colliding thing (fourth argument) to the assassin-rush component (type 0xA9) and to the type-0xA8 component when the thing carries them; a live thing is then put back on the manager's update list
// Leaf: OnCollision
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C8DA0, 161 bytes (+15 int3). `ret 10h`.
// Callees: CVectorMap::LowerBound(const long*) 0x0040F020 (x2, the key in a frame word), CTCAssassinRush::SetThingCollisionPtr
//          (CThing*) 0x004CBFB0, the type-0xA8 component's handler 0x004CC000 (manifest "CVillageTaskCarryCrate::
//          AssignCreatureToTask" is propagated; named by its use here), CThingManager::AddToUpdateList(CThing*) 0x0051EC50
//          on g_pThingManager (0x013B8A1C).
// Shape: the phase compare before the prologue; the presence bits for 0xA9/0xA8 both live in bitset word 5 (+0x34,
//        tested through ah: bits 9 and 8); each lookup is the inlined GetInterface (lower bound on the key word, end
//        when absent); Dead is bit 0 of the +0x91 flag byte; the other thing is kept in ebp for both calls.
class CThing;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                               // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);            // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCAssassinRush { public:                             // interface type 0xA9
    void SetThingCollisionPtr(CThing* thing);        // 0x004CBFB0
};
struct CTCCollisionHandler_0xA8 {                    // interface type 0xA8 (descriptive)
    void OnThingCollision(CThing* thing);            // 0x004CC000 (manifest label propagated)
};
class CThingManager { public:
    void AddToUpdateList(CThing* thing);             // 0x0051EC50
};
extern CThingManager* g_pThingManager;               // 0x013B8A1C
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                  // +0x20 UsingComponents (bitset<274>)
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                          // +0x91 bit 0
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_presentBits[wanted >> 5] & (1UL << (wanted & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || wanted < it->first)
                it = map->last;
            *out = (T*)((void*)it->second);
            return true;
        }
        return false;
    }
    void OnCollision(long phase, long a, long b, CThing* other);
};
void CThing::OnCollision(long phase, long, long, CThing* other)
{
    if (phase == 1) {
        CTCAssassinRush* rush;
        if (GetInterface(&rush, 0xa9))
            rush->SetThingCollisionPtr(other);
        CTCCollisionHandler_0xA8* handler;
        if (GetInterface(&handler, 0xa8))
            handler->OnThingCollision(other);
    }
    if (!Dead)
        g_pThingManager->AddToUpdateList(this);
}