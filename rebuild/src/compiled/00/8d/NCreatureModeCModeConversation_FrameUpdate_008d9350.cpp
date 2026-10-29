// NCreatureMode::CModeConversation::FrameUpdate() -- follow the talk component's attitude: when it changes, make it current, re-target the filter at the attitude's animation group and ask for an idle reset; then the base frame update
// Leaf: ?FrameUpdate@CModeConversation
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D9350, 99 bytes (+ int3 padding), `ret`; vtable 0x01285990 slot 3.
// Callees: CVectorMap::LowerBound (const) @ 0x004365B0 (inlined CTCTalk lookup, interface 15 = bit 15 of the +0x20 dword),
//   CTCTalk::GetAttitude @ 0x006E3180, GetAnimGroupName(EConversationAttitude) @ 0x004CD9C0 (`__fastcall`),
//   CAnimFilterOfGroup::SetGroup @ 0x0073DFF0, CModeBase::FrameUpdate @ 0x0073AC50 (qualified).
// Name: FableWin.pdb `?FrameUpdate@CModeConversation@NCreatureMode@@UAEXXZ` (slot 3).
// Layout: Ego_r.pdb CModeConversation (0x34, matches retail): CurrentAttitude +0x1c, InitialAttitude +0x20, Filter +0x24,
//   ResetIdleAnim +0x30.
// Shape: the landed ctor 0x008D9220 attitude block (out-parameter GetInterface, named attitude local); the presence test of
//   bit 15 is `mov ecx,[eax+20h] ; test ch,ch ; jns`.
class CCharString;
class CTCBase;

typedef long EConversationAttitude;

const CCharString& __fastcall GetAnimGroupName(EConversationAttitude attitude);   // 0x004CD9C0

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                       // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;    // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];          // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;              // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CTCTalk { public:                             // interface 15
    EConversationAttitude GetAttitude();     // 0x006E3180
};

class CAnimFilterOfGroup { public:                  // vtable 0x01267568
    virtual void v00();
    void* Group;                             // +4 CCharString
    unsigned long GroupCRC;                  // +8
    void SetGroup(const CCharString& group); // 0x0073DFF0
};

struct CModeBase {                           // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing* ParentThing;                     // +4
    unsigned char _body[0x14];               // WeaponBasedFilter, FramesBeforeRemovingSelf, Active
    void FrameUpdate();                      // 0x0073AC50
};

namespace NCreatureMode {
struct CModeConversation : CModeBase {       // vtable 0x01285990
    EConversationAttitude CurrentAttitude;   // +0x1c
    EConversationAttitude InitialAttitude;   // +0x20
    CAnimFilterOfGroup Filter;               // +0x24
    bool ResetIdleAnim;                      // +0x30
    void FrameUpdate();
};
}

void NCreatureMode::CModeConversation::FrameUpdate()
{
    CTCTalk* talk;
    if (ParentThing->GetInterface(&talk, 15)) {
        EConversationAttitude attitude = talk->GetAttitude();
        if (attitude != CurrentAttitude) {
            CurrentAttitude = attitude;
            Filter.SetGroup(GetAnimGroupName(attitude));
            ResetIdleAnim = true;
        }
    }
    CModeBase::FrameUpdate();
}