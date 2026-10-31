// CMessageEventManager::AddMessage(EMessageEventType type, unsigned long lifespan, const C3DVector& pos, CThing* originator, CThing* target, float sightRadius, float hearingRadius, long extraData) -- build a time-stamped message event with the next message id, the originator/target pointers and UIDs and the target's faction, push a copy on the front of the list and return it
// Grade: DIFFER(486v486) -- register allocation only: retail keeps the intelligent-pointer vtable constant in edi, the originator in esi and the target in edi; ours swaps esi/edi (so `add esi,44h` v `lea esi,[edi+44h]`), plus the faction load `mov eax,[eax+4]` (ours edx) and the list-head read in eax (ours ecx). Wave 25 (agent647): PDB parameter kinds and class keys (const C3DVector&, protected CBaseObject ctor/dtor, CWorld::GetFrame), a named faction-member local and the if/else-if `sel` interface lookup: identical.
// Leaf: ?AddMessage@CMessageEventManager@@QAEPAVCMessageEvent@@W4EMessageEventType
class CBaseObjectPointer;
class CFaction;
class C3DVector {
public:
    float X, Y, Z;
};
enum EMessageEventType { EMessageEventType_Force32 = 0x7fffffff };
enum EReactionSpeechType { REACTION_SPEECH_NONE = 0, EReactionSpeechType_Force32 = 0x7fffffff };
class CBaseObject {
protected:
    CBaseObject();                                      // 0x0099A380
    virtual ~CBaseObject();                             // 0x0099A430
public:
    CBaseObjectPointer* IntelligentPointer;             // +0x04
};
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObjectPointer* PData;                          // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:   // vtable 0x01238C6C
    CIntelligentPointer() {}
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CTCFactionMember {                               // interface 0x49 (descriptive)
    unsigned char _pad00[0x18];
    CFaction*     PFaction;                             // +0x18
};
class CThing : public CBaseObject {
public:
    unsigned char    _pad08[0x18 - 0x08];
    unsigned __int64 UID;                               // +0x18
    unsigned long    m_presentBits[9];                  // +0x20
    CInterfaceMap    m_interfaces;                      // +0x44
    template<class T> __forceinline T* GetConstInterface(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    __forceinline unsigned long HasInterface(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
};
class CMessageEvent : public CBaseObject {
public:                    // vtable 0x0125BE8C, 0x58 bytes (Ego_r.pdb offsets)
    EMessageEventType Type;                             // +0x08
    C3DVector Pos;                                      // +0x0c
    float SightRadius;                                  // +0x18
    float HearingRadius;                                // +0x1c
    unsigned __int64 OriginatorUID;                     // +0x20
    unsigned __int64 TargetUID;                         // +0x28
    CFaction* PTargetFaction;                           // +0x30
    long TimeStamp;                                     // +0x34
    unsigned long Lifespan;                             // +0x38
    long ExtraData;                                     // +0x3c
    unsigned long MessageID;                            // +0x40
    EReactionSpeechType ReactionSpeechType;             // +0x44
    CIntelligentPointer<CThing> POriginator;            // +0x48
    CIntelligentPointer<CThing> PTarget;                // +0x50
    CMessageEvent() {}
    CMessageEvent(const CMessageEvent& other);          // 0x00687310
};
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
struct SMessageNode {
    SMessageNode* m_pNext;
    SMessageNode* m_pPrev;
    CMessageEvent m_value;                              // +0x08
};
struct CMessageList {                                   // STLport list<CMessageEvent>: the sentinel pointer only
    SMessageNode* m_pHead;
    __forceinline void push_front(const CMessageEvent& value)
    {
        SMessageNode* position = m_pHead->m_pNext;
        SMessageNode* node = (SMessageNode*)malloc(sizeof(SMessageNode));
        new (&node->m_value) CMessageEvent(value);
        SMessageNode* prev = position->m_pPrev;
        node->m_pNext = position;
        node->m_pPrev = prev;
        node->m_pPrev->m_pNext = node;
        position->m_pPrev = node;
    }
    struct iterator {
        SMessageNode* m_pNode;
        iterator(SMessageNode* node) : m_pNode(node) {}
        CMessageEvent& operator*() const { return m_pNode->m_value; }
    };
    iterator begin() { return iterator(m_pHead->m_pNext); }
};
class CWorld {
public:
    static long __fastcall GetFrame();                  // 0x0049D870
};
extern unsigned long g_NextMessageID;                   // 0x0137EDB8
class CMessageEventDef {
public:
    unsigned char _pad00[0x3c];
    long          SightRadius;                          // +0x3c
    long          HearingRadius;                        // +0x40
    unsigned long Lifespan;                             // +0x44
    EReactionSpeechType ReactionSpeechType;             // +0x48
    EMessageEventType Type;                             // +0x4c
};
class CMessageEventManager {
public:
    CWorld*      World;                                 // +0x00
    CMessageList Messages;                              // +0x04
    CMessageEvent* AddMessage(EMessageEventType type, unsigned long lifespan, const C3DVector& pos, CThing* originator,
        CThing* target, float sightRadius, float hearingRadius, long extraData);
    CMessageEvent* AddMessage(const CMessageEventDef& def, const C3DVector& pos, CThing* originator, CThing* target, long extraData);
};

CMessageEvent* CMessageEventManager::AddMessage(EMessageEventType type, unsigned long lifespan, const C3DVector& pos,
    CThing* originator, CThing* target, float sightRadius, float hearingRadius, long extraData)
{
    CMessageEvent event;
    event.Pos = pos;
    event.Lifespan = lifespan;
    event.TimeStamp = CWorld::GetFrame();
    event.HearingRadius = hearingRadius;
    event.SightRadius = sightRadius;
    event.ExtraData = extraData;
    event.POriginator = originator;
    event.PTarget = target;
    event.Type = type;
    event.MessageID = g_NextMessageID++;
    event.ReactionSpeechType = REACTION_SPEECH_NONE;
    event.PTargetFaction = 0;
    event.OriginatorUID = originator ? originator->UID : 0;
    if (target) {
        event.TargetUID = target->UID;
        if (target->HasInterface(0x49)) {
            CTCFactionMember* member = target->GetConstInterface<CTCFactionMember>(0x49);
            event.PTargetFaction = member->PFaction;
        }
    } else
        event.TargetUID = 0;
    Messages.push_front(event);
    return &*Messages.begin();
}
