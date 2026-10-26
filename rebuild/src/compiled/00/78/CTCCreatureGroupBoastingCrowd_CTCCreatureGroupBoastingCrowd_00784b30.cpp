// CTCCreatureGroupBoastingCrowd::CTCCreatureGroupBoastingCrowd(CThing* parent) -- boasting-crowd group component: the inline CTCCreatureGroup base (counts, shutdown flag, empty member map), a null hero pointer, crowd state 0 / encouragement state 2, no deed handle; forces a frame update and clears the base's shutdown-on-empty flag
// Leaf: ??0CTCCreatureGroupBoastingCrowd
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00784B30, length 158 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, malloc @ 0x00BFEA0E (the 0x30-byte map header node),
// CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, CThing::SetForceFrameUpdate(bool) @ 0x004C7C60.
// Vtables: 0x0124326C CTCCreatureGroup, 0x0126B5DC CTCCreatureGroupBoastingCrowd, 0x01238C6C CIntelligentPointer<CThing>.
// Layout: engine/CTCCreatureGroupBoastingCrowd.h is the PDB layout; retail's STLport map in CTCCreatureGroup is 12 bytes
// (PDB 16), so every member here sits 4 bytes lower: PHero +0x24, CrowdState +0x2C, CurrentEncouragementState +0x30,
// HeroBoasted +0x34, HeroMadeGoodBoast +0x35, CurrentOpinionDeedHandle +0x38, HeroOpinionSourcesPushed +0x3C,
// FrameToClearRemainingOpinionSources +0x40 (FrameToLeavePostBoastState +0x44 is left unset). A local model is used.
// Shape: the CTCCreatureGroup ctor is an inline base (`__forceinline`) whose map is the STLport tree (header nulled, malloc'd,
// count zeroed, `_M_empty_initialize`); the intelligent pointer is an inline derived ctor over the out-of-line base ctor;
// the parent's +0x70 test reuses the 2 held in ecx.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

struct CThingGraphic_0x70 {
    unsigned char _pad[0x41];
    unsigned char m_flags;                         // +0x41 (bit 1: already frame-updating)
};
class CThing { public:
    unsigned char       _pad[0x70];
    CThingGraphic_0x70* m_pState;                  // +0x70
    void SetForceFrameUpdate(bool force);          // 0x004C7C60
};

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
    CTCBase(CThing* parent);                       // 0x00686800
};

struct SMemberNode {                               // 0x30 bytes: rb links, key (8), CGroupMemberData (20)
    unsigned char m_color;                         // +0x00 (0 = red)
    SMemberNode*  m_pParent;                       // +0x04
    SMemberNode*  m_pLeft;                         // +0x08
    SMemberNode*  m_pRight;                        // +0x0c
    unsigned char m_value[0x20];                   // +0x10
};
struct CMemberTreeBase {
    SMemberNode* m_pHeader;
    __forceinline CMemberTreeBase() : m_pHeader(0)
    {
        m_pHeader = (SMemberNode*)malloc(sizeof(SMemberNode));
    }
};
struct CMemberTree : CMemberTreeBase {             // map<unsigned __int64, CGroupMemberData> (12 bytes in retail)
    unsigned long m_nodeCount;
    unsigned char m_compare;                       // empty std::less
    __forceinline CMemberTree() : m_nodeCount(0)
    {
        m_pHeader->m_color = 0;
        m_pHeader->m_pParent = 0;
        m_pHeader->m_pLeft = m_pHeader;
        m_pHeader->m_pRight = m_pHeader;
    }
};

class CTCCreatureGroup : public CTCBase { public:
    unsigned long MemberCount;                     // +0x0c
    unsigned long ActiveMemberCount;               // +0x10
    bool          ShutdownOnEmpty;                 // +0x14
    CMemberTree   Members;                         // +0x18
    virtual void v01();
    __forceinline CTCCreatureGroup(CThing* parent)
        : CTCBase(parent), MemberCount(0), ActiveMemberCount(0), ShutdownOnEmpty(true)
    {
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                 // +0x04
    CBaseIntelligentPointer();                     // 0x00A01B10
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    virtual void v01();
    CIntelligentPointerThing() {}
};

class CTCCreatureGroupBoastingCrowd : public CTCCreatureGroup { public:
    CIntelligentPointerThing PHero;                // +0x24
    long CrowdState;                               // +0x2c EBoastingCrowdState
    long CurrentEncouragementState;                // +0x30 EBoastingCrowdState
    bool HeroBoasted;                              // +0x34
    bool HeroMadeGoodBoast;                        // +0x35
    long CurrentOpinionDeedHandle;                 // +0x38
    long HeroOpinionSourcesPushed;                 // +0x3c
    long FrameToClearRemainingOpinionSources;      // +0x40
    long FrameToLeavePostBoastState;               // +0x44
    virtual void v02();
    CTCCreatureGroupBoastingCrowd(CThing* parent);
};

CTCCreatureGroupBoastingCrowd::CTCCreatureGroupBoastingCrowd(CThing* parent)
    : CTCCreatureGroup(parent)
    , CrowdState(0)
    , CurrentEncouragementState(2)
    , HeroBoasted(false)
    , HeroMadeGoodBoast(false)
    , CurrentOpinionDeedHandle(-1)
    , HeroOpinionSourcesPushed(0)
    , FrameToClearRemainingOpinionSources(0)
{
    if (parent->m_pState && !(parent->m_pState->m_flags & 2))
        parent->SetForceFrameUpdate(true);
    ShutdownOnEmpty = false;
}