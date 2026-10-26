// NCreatureMode::CModeConversation::CModeConversation(CThing*) -- conversation mode with the default attitude (1); adopts the owner's CTCTalk attitude when the thing has one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D9220, 160 bytes (padding included).
// Shape: CModeBase ctor (0x0073EAC0), vtable 0x01285990 (rtti NCreatureMode::CModeConversation),
// CurrentAttitude (+0x1c) = InitialAttitude (+0x20) = 1, the CAnimFilterOfGroup Filter (+0x24) built
// out of line (0x0073E8F0) from GetAnimGroupName(1) (0x004CD9C0, __fastcall, returns the group's
// CCharString), ResetIdleAnim (+0x30) = true. The body looks up the owner's interface 15 (CTCTalk)
// with the inlined out-parameter GetInterface (present bit 15 of the +0x20 bitset, LowerBound
// 0x004365B0 on the +0x44 vector map with the key in the dead argument slot); when present it reads
// GetAttitude (0x006E3180) into InitialAttitude and, when that differs from CurrentAttitude, makes
// it current and re-targets the filter (0x0073DFF0) at the new group. Field names from the PDB
// (struct_layouts_egor.tsv CModeConversation, 52 bytes).
class CCharString;
class CTCBase;

typedef long EConversationAttitude;

const CCharString& __fastcall GetAnimGroupName(EConversationAttitude attitude);   // 0x004CD9C0

struct CInterfaceMap {                       // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;    // 0x004365B0
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];          // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;              // +0x44
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CTCTalk { public:
    EConversationAttitude GetAttitude();     // 0x006E3180
};

class CAnimFilterOfGroup { public:                  // vtable 0x01267568
    virtual void v00();
    void* Group;                             // +4 CCharString
    unsigned long GroupCRC;                  // +8
    CAnimFilterOfGroup(const CCharString& group);   // 0x0073E8F0
    void SetGroup(const CCharString& group);        // 0x0073DFF0
};

struct CModeBase {
    virtual void v00();
    CThing* ParentThing;                     // +4
    unsigned char _body[0x14];               // WeaponBasedFilter, FramesBeforeRemovingSelf, Active
    CModeBase(CThing* thing);                // 0x0073EAC0
};

namespace NCreatureMode {

struct CModeConversation : CModeBase {
    EConversationAttitude CurrentAttitude;   // +0x1c
    EConversationAttitude InitialAttitude;   // +0x20
    CAnimFilterOfGroup Filter;               // +0x24
    bool ResetIdleAnim;                      // +0x30
    virtual void v00();
    CModeConversation(CThing* thing);
};

CModeConversation::CModeConversation(CThing* thing)
    : CModeBase(thing)
    , CurrentAttitude(1)
    , InitialAttitude(1)
    , Filter(GetAnimGroupName(1))
    , ResetIdleAnim(true)
{
    CTCTalk* talk;
    if (ParentThing->GetInterface(&talk, 15)) {
        EConversationAttitude attitude = talk->GetAttitude();
        InitialAttitude = attitude;
        if (attitude != CurrentAttitude) {
            CurrentAttitude = attitude;
            Filter.SetGroup(GetAnimGroupName(attitude));
        }
    }
}

}