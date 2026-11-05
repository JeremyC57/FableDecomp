// CTCCreatureGroupTag::SetTagger(const CThing* tagger) -- record an active member as the tagger (UID + frame); always returns false
// Leaf: ?SetTagger@CTCCreatureGroupTag@@QAE_NPBVCThing@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008137B0, length 45 bytes (+3 int3). Callers: the minion kid group-tag AI
// state (0x009341AE, 0x00934381).
// Callees: CTCCreatureGroup::IsActiveMember @ 0x00812F70, the world-frame getter @ 0x0049D870 (mov eax,[0x013B89BC]).
// Shape: both exits end in `xor al,al` -- the function's bool result is a constant false (the effect is the two stores).
// TaggerUID (+0x28, unsigned __int64) is copied dword by dword from CThing::UniqueID (+0x18), then TaggedFrame (+0x30)
// takes the current frame. PDB names: CTCCreatureGroupTag {TaggerUID +0x28, TaggedFrame +0x30}, sizeof 0x38.
#include "engine/CThing.h"

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CGroupMemberData { public:
    CBaseIntelligentPointer PMember;        // +0x00
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

class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};                       // 0x0049D870 (returns the global frame counter @ 0x013B89BC)

class CTCCreatureGroupTag : public CTCCreatureGroup { public:
    unsigned __int64 TaggerUID;             // +0x28
    unsigned long TaggedFrame;              // +0x30
    bool ChooseNewTagger();                 // 0x008137E0
    bool SetTagger(const CThing* tagger);
    bool RemoveMember(const CThing* member);
    bool DeactivateMember(const CThing* member);
    CThing* GetTagger();
};

bool CTCCreatureGroupTag::SetTagger(const CThing* tagger)
{
    if (IsActiveMember(tagger)) {
        TaggerUID = tagger->GetUniqueID();
        TaggedFrame = CWorld::GetFrame();
    }
    return false;
}