// CAIStateGroup_BoastingCrowdMember::IsStillValid() -- still valid while the boaster is alive, its crowd component (interface 0x74) is in an active state (neither 0 nor 10) and that component's audience target is alive
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound) re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095E870, length 134 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times on the boaster pointer at +0x14, once on
// the component's pointer at +0x24), the interface-map lower bound @ 0x004365B0 (a second instantiation of the
// 0x0040F020 CVectorMap::LowerBound; interface 0x74 = bitset word 3 bit 20 -> `test dword ptr [eax+2Ch],100000h`).
// Shape: nested ifs with one `return 1` (`mov eax,1`: an int-width result, not bool) and the shared
// `xor eax,eax` tail; the dead flag is bit 0 of the thing's byte at +0x91; the boaster pointer's address is
// formed once into esi and re-fetched for every use; the lookup is the landed GetInterface out-parameter
// template whose bool is tested.
// Model: local (no engine header for this class); the boaster pointer is a CIntelligentPointer<CThing>.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad9[0x3d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

struct CTCBoastingCrowd {                       // interface 0x74
    unsigned char _pad[0x24];
    CIntelligentPointer<CThing> PAudienceTarget;    // +0x24
    long State;                                 // +0x2c
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    void* PBrain;                               // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_BoastingCrowdMember : CAIStateGroupBase {
    CIntelligentPointer<CThing> PBoaster;           // +0x14
    long IsStillValid();
};

long CAIStateGroup_BoastingCrowdMember::IsStillValid()
{
    if (PBoaster.GetPItem() != 0 && !PBoaster.GetPItem()->IsDead()) {
        CTCBoastingCrowd* crowd;
        if (PBoaster.GetPItem()->GetInterface(&crowd, 0x74)) {
            if (crowd->State != 0 && crowd->State != 10) {
                CThing* target = crowd->PAudienceTarget.GetPItem();
                if (target != 0 && !target->IsDead())
                    return 1;
            }
        }
    }
    return 0;
}