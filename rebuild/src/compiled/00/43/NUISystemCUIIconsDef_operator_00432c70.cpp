// NUISystem::CUIIconsDef::operator=(const CUIIconsDef&) -- def copy with the whole base chain inlined: CDefClassBase (root base, PDefManager, GlobalIndex, three flag bits), CParentDefClassBase (InstantiationName, SubDefInfoMap), then the 32 icon graphic indices
// Leaf: ??4CUIIconsDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x00432C70, 389 bytes, `ret 4`.
// Identity: reached through the `jmp` thunk at 0x00432C60 in slot 19 of vtable 0x01230F54 = NUISystem::CUIIconsDef
//   (rtti-vtables.tsv); the manifest label "operator=" (module CUIIconsDef) is right.
// Callees: the CDefClassBase root base's operator= @ 0x00431F60, SubDefInfoMap's vector operator= @ 0x00431F90.
// Layout (PDB names, retail offsets): the base chain exactly as the landed CFrontEndDef::operator= (0x00432E60, same TU);
//   CUIIconsDef: IconFriendRequestReceived .. IconListHighlight at retail +0x3c..+0xb8 (Ego_r.pdb +0x40..+0xbc; retail
//   def layouts sit 4 below the PDB after the 12-byte STLport vector).
// Shape: same TU as the CDefClassBase out-of-line copy, so the implicit CDefClassBase/CParentDefClassBase/CVectorMap
//   assignments are inlined (`__forceinline` user copies) and the 32 dwords copied member by member.
#pragma pack(push, 1)
class CDefPointeeBase { public:                                   // 28-byte polymorphic root of CDefClassBase
    virtual void v00();
    unsigned long m_words[6];                           // +0x04..+0x18
    CDefPointeeBase& operator=(const CDefPointeeBase& other); // 0x00431F60
};

class CDefinitionManager;

class CDefClassBase : public CDefPointeeBase { public:
    CDefinitionManager* PDefManager;                    // +0x1c
    unsigned long GlobalIndex;                          // +0x20
    unsigned char Setup : 1;                            // +0x24 bit 0
    unsigned char Template : 1;                         // +0x24 bit 1
    unsigned char DefaultValsApplied : 1;               // +0x24 bit 2
    __forceinline CDefClassBase& operator=(const CDefClassBase& other)
    {
        CDefPointeeBase::operator=(other);
        PDefManager = other.PDefManager;
        GlobalIndex = other.GlobalIndex;
        Setup = other.Setup;
        Template = other.Template;
        DefaultValsApplied = other.DefaultValsApplied;
        return *this;
    }
};
#pragma pack(pop)

template<class T> struct CRetailVector {                // begin/end/capacity
    T* m_first; T* m_last; T* m_end;
    CRetailVector& operator=(const CRetailVector& other);
};

struct CKeyPairCompareLess {};

template<class K, class V> class CVectorMap { public:          // retail: 14 bytes
    CRetailVector<V>    m_vector;                       // +0x00
    CKeyPairCompareLess m_compare;                      // +0x0c (empty)
    bool                m_sorted;                       // +0x0d
    __forceinline CVectorMap& operator=(const CVectorMap& other)
    {
        m_vector = other.m_vector;
        m_compare = other.m_compare;
        m_sorted = other.m_sorted;
        return *this;
    }
};

class CDefString { public: long TablePos; };
struct CSubDefInfo;
class CCharString { public: void* m_pStringData; };

#pragma pack(push, 1)
class CParentDefClassBase : public CDefClassBase { public:
    CDefString                             InstantiationName;   // +0x25
    CVectorMap<unsigned long, CSubDefInfo> SubDefInfoMap;       // +0x29
    __forceinline CParentDefClassBase& operator=(const CParentDefClassBase& other)
    {
        CDefClassBase::operator=(other);
        InstantiationName = other.InstantiationName;
        SubDefInfoMap = other.SubDefInfoMap;
        return *this;
    }
};
#pragma pack(pop)

namespace NUISystem {
class CUIIconsDef : public CParentDefClassBase { public:
    unsigned long IconFriendRequestReceived;            // +0x3c
    unsigned long IconFriendRequestReceivedOn;          // +0x40
    unsigned long IconFriendRequestSent;                // +0x44
    unsigned long IconFriendRequestSentOn;              // +0x48
    unsigned long IconGameInviteReceived;               // +0x4c
    unsigned long IconGameInviteReceivedOn;             // +0x50
    unsigned long IconGameInviteSent;                   // +0x54
    unsigned long IconGameInviteSentOn;                 // +0x58
    unsigned long IconMute;                             // +0x5c
    unsigned long IconMuteOn;                           // +0x60
    unsigned long IconOnline;                           // +0x64
    unsigned long IconOnlineOn;                         // +0x68
    unsigned long IconPasscodeBlank;                    // +0x6c
    unsigned long IconPasscodeFilled;                   // +0x70
    unsigned long IconTV;                               // +0x74
    unsigned long IconTVOn;                             // +0x78
    unsigned long IconVoice;                            // +0x7c
    unsigned long IconVoiceOn;                          // +0x80
    unsigned long IconWait1;                            // +0x84
    unsigned long IconWait2;                            // +0x88
    unsigned long IconWait3;                            // +0x8c
    unsigned long IconWait4;                            // +0x90
    unsigned long IconProgress;                         // +0x94
    unsigned long IconProgressOn;                       // +0x98
    unsigned long IconA;                                // +0x9c
    unsigned long IconB;                                // +0xa0
    unsigned long IconX;                                // +0xa4
    unsigned long IconY;                                // +0xa8
    unsigned long IconBlank;                            // +0xac
    unsigned long IconUpArrow;                          // +0xb0
    unsigned long IconDownArrow;                        // +0xb4
    unsigned long IconListHighlight;                    // +0xb8
    CUIIconsDef& operator=(const CUIIconsDef& other);
};

CUIIconsDef& CUIIconsDef::operator=(const CUIIconsDef& other)
{
    CParentDefClassBase::operator=(other);
    IconFriendRequestReceived = other.IconFriendRequestReceived;
    IconFriendRequestReceivedOn = other.IconFriendRequestReceivedOn;
    IconFriendRequestSent = other.IconFriendRequestSent;
    IconFriendRequestSentOn = other.IconFriendRequestSentOn;
    IconGameInviteReceived = other.IconGameInviteReceived;
    IconGameInviteReceivedOn = other.IconGameInviteReceivedOn;
    IconGameInviteSent = other.IconGameInviteSent;
    IconGameInviteSentOn = other.IconGameInviteSentOn;
    IconMute = other.IconMute;
    IconMuteOn = other.IconMuteOn;
    IconOnline = other.IconOnline;
    IconOnlineOn = other.IconOnlineOn;
    IconPasscodeBlank = other.IconPasscodeBlank;
    IconPasscodeFilled = other.IconPasscodeFilled;
    IconTV = other.IconTV;
    IconTVOn = other.IconTVOn;
    IconVoice = other.IconVoice;
    IconVoiceOn = other.IconVoiceOn;
    IconWait1 = other.IconWait1;
    IconWait2 = other.IconWait2;
    IconWait3 = other.IconWait3;
    IconWait4 = other.IconWait4;
    IconProgress = other.IconProgress;
    IconProgressOn = other.IconProgressOn;
    IconA = other.IconA;
    IconB = other.IconB;
    IconX = other.IconX;
    IconY = other.IconY;
    IconBlank = other.IconBlank;
    IconUpArrow = other.IconUpArrow;
    IconDownArrow = other.IconDownArrow;
    IconListHighlight = other.IconListHighlight;
    return *this;
}
} // namespace NUISystem