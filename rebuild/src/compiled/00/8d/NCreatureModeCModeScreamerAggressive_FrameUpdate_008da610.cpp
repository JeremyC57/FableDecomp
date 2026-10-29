// NCreatureMode::CModeScreamerAggressive::FrameUpdate() -- while its target is alive, a screamer with a look component looks at the target; then the base frame update
// Leaf: ?FrameUpdate@CModeScreamerAggressive
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DA610, 121 bytes (+ int3 padding), `ret`; vtable slot 3.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type
//   0x43 = bit 3 of the bitset dword at +0x28 = CTCLook), CThing::GetPos @ 0x004C73D0,
//   CTCLook::LookAtPos(const C3DVector*, long priority) @ 0x00753E90, CModeBase::FrameUpdate @ 0x0073AC50 (qualified).
// Name: FableWin.pdb `?FrameUpdate@CModeScreamerAggressive@NCreatureMode@@UAEXXZ` (slot 3).
// Layout: Ego_r.pdb CModeScreamerAggressive: CModeStandBase 0x68, Filter +0x68, PEffect +0x74, PTarget +0x7c.
// Shape: IS_ALIVE (GetPItem twice, dead bit +0x91) then the out-parameter GetInterface with the ternary entry pick (the
//   CModeBlock::FrameUpdate 0x008D8EA0 model); the component stays in esi across the GetPItem / GetPos calls.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                     // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);  // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class C3DVector { public: float X, Y, Z; };

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];        // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;            // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;               // +0x91 bit 0: dead
    const C3DVector& GetPos() const;  // 0x004C73D0
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
};

class CTCLook { public:                           // interface 0x43
    void LookAtPos(const C3DVector* position, long priority);   // 0x00753E90
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

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x14];
    void FrameUpdate();                    // 0x0073AC50
};
struct CModeStandBase : CModeBase {        // retail sizeof 0x68
    unsigned char _stand[0x4c];
};

namespace NCreatureMode {
struct CModeScreamerAggressive : CModeStandBase {
    unsigned char           Filter[0x0c];  // +0x68 CAnimFilterOfGroup
    CIntelligentPointer<CThing> PEffect;       // +0x74
    CIntelligentPointer<CThing> PTarget;       // +0x7c
    void FrameUpdate();
};
}

void NCreatureMode::CModeScreamerAggressive::FrameUpdate()
{
    CTCLook* look;
    if (PTarget.GetPItem() != 0 && !(PTarget.GetPItem()->m_flags91 & 1) && ParentThing->GetInterface(&look, 0x43))
        look->LookAtPos(&PTarget.GetPItem()->GetPos(), 0x67);
    CModeBase::FrameUpdate();
}