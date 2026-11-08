// CThing::OnCollision(long phase, long, long, CThing* other) -- on the first collision phase, hands the colliding thing (fourth argument) to the assassin-rush component (type 0xA9) and to the type-0xA8 component when the thing carries them; a live thing is then put back on the manager's update list
// Leaf: OnCollision
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Naming wave 18: the global stand-in g_pThingManager (0x013B8A1C) is the private static CThing::PCurrentThingManager, read through the inline
//   static accessor GetPCurrentThingManager() as retail code outside CThing does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C8DA0, 161 bytes (+15 int3). `ret 10h`.
// Callees: CVectorMap::LowerBound(const long*) 0x0040F020 (x2, the key in a frame word), CTCAssassinRush::SetThingCollisionPtr
//          (CThing*) 0x004CBFB0, the type-0xA8 component's handler 0x004CC000 (manifest "CVillageTaskCarryCrate::
//          AssignCreatureToTask" is propagated; named by its use here), CThingManager::AddToUpdateList(CThing*) 0x0051EC50
//          on CThing::GetPCurrentThingManager() (0x013B8A1C).
// Shape: the phase compare before the prologue; the presence bits for 0xA9/0xA8 both live in bitset word 5 (+0x34,
//        tested through ah: bits 9 and 8); each lookup is the inlined GetInterface (lower bound on the key word, end
//        when absent); Dead is bit 0 of the +0x91 flag byte; the other thing is kept in ebp for both calls.
#include "fable_compiler.h"
class CThing;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
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
class CThing {
private:
    static CThingManager* PCurrentThingManager;      // 0x013B8A1C ?PCurrentThingManager@CThing@@0PAVCThingManager@@A (private static)
public:
    static CThingManager* __fastcall GetPCurrentThingManager() { return PCurrentThingManager; }   // Ego_r / FableWin ?GetPCurrentThingManager@CThing@@SIPAVCThingManager@@XZ (inline)
private: public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                  // +0x20 UsingComponents (bitset<274>)
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad54[0x3d];
    unsigned char Dead : 1;                          // +0x91 bit 0
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** out, long wanted)
    {
        if (m_presentBits[wanted >> 5] & (1UL << (wanted & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || wanted < it->first)
                it = map->end();
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
        CThing::GetPCurrentThingManager()->AddToUpdateList(this);
}