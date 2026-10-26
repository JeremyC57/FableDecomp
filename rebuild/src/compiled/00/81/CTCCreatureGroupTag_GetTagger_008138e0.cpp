// CTCCreatureGroupTag::GetTagger() -- the tagger's thing when a tagger UID is set and its map entry is still active, else null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008138E0, length 50 bytes. 14 callers in the minion kid group-tag AI state
// (0x00934107..0x009348DA; the landed IsStillValid 0x00934100 calls it as "CheckValid").
// Callees: the Members map find @ 0x00812790, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (tail jump).
// Shape: `TaggerUID > 0` on the unsigned 64-bit member is the `test edx,edx ; ja` + `cmp [eax],0 ; jbe` pair; the member's
// address (`lea eax,[ecx+28h]`) is formed once because find takes the key by reference. The node's IsActive byte
// (+0x20) gates the GetPItem tail call on the node's PMember (+0x18).
class CThing { public:
    unsigned char _pad00[0x18];
    unsigned __int64 UniqueID;              // +0x18
    unsigned char _pad20[0x71];
    unsigned char Dead : 1;                 // +0x91 bit 0
    unsigned __int64 GetUID() const { return UniqueID; }
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

class CGroupMemberData { public:
    CIntelligentPointer<CThing> PMember;        // +0x00
    bool IsActive;                          // +0x08
    unsigned long JoinedFrame;              // +0x0c
    unsigned long DeactivatedFrame;         // +0x10
};

struct SGroupMemberNode {                   // STLport _Rb_tree_node<pair<const unsigned __int64, CGroupMemberData> >
    long Color;                             // +0x00
    SGroupMemberNode* Parent;               // +0x04
    SGroupMemberNode* Left;                 // +0x08
    SGroupMemberNode* Right;                // +0x0c
    unsigned __int64 Key;                   // +0x10
    CGroupMemberData Value;                 // +0x18
};

struct CGroupMemberMap {                    // STLport map<unsigned __int64, CGroupMemberData>
    SGroupMemberNode* Header;               // +0x00
    unsigned long NodeCount;                // +0x04
    unsigned long _compare;                 // +0x08
    SGroupMemberNode* find(const unsigned __int64& key);   // 0x00812790
    SGroupMemberNode* end() { return Header; }
};

class CTCCreatureGroup { public:
    virtual void v00();
    CThing* ParentThing;                    // +0x04 (CTCBase)
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
    unsigned long MemberCount;              // +0x0c
    unsigned long ActiveMemberCount;        // +0x10
    bool ShutdownOnEmpty;                   // +0x14
    CGroupMemberMap Members;                // +0x18
    bool IsActiveMember(const CThing* member);          // 0x00812F70
    bool RemoveMember(const CThing* member);            // vtable slot 29: 0x00813100
    bool DeactivateMember(const CThing* member);        // vtable slot 30: 0x00812E20
};

long GetWorldFrame();                       // 0x0049D870 (returns the global frame counter @ 0x013B89BC)

class CTCCreatureGroupTag : public CTCCreatureGroup { public:
    unsigned __int64 TaggerUID;             // +0x28
    unsigned long TaggedFrame;              // +0x30
    bool ChooseNewTagger();                 // 0x008137E0
    bool SetTagger(const CThing* tagger);
    bool RemoveMember(const CThing* member);
    bool DeactivateMember(const CThing* member);
    CThing* GetTagger();
};

CThing* CTCCreatureGroupTag::GetTagger()
{
    if (TaggerUID > 0) {
        SGroupMemberNode* it = Members.find(TaggerUID);
        if (it != Members.end() && it->Value.IsActive)
            return it->Value.PMember.GetPItem();
    }
    return 0;
}