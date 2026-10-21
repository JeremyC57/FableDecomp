// CMessageEventManager::AddMessage(EMessageEventType type, unsigned long lifespan, const C3DVector* pos, CThing* originator, CThing* target, float sightRadius, float hearingRadius, long extraData) -- build a time-stamped message event with the next message id, the originator/target pointers and UIDs and the target's faction, push a copy on the front of the list and return it
// Grade: DIFFER(486v486) -- register allocation only: retail keeps the originator in esi and the target in edi (ours swaps them), so the target map pointer is `lea esi,[edi+0x44]` vs ours `add esi,0x44`, plus eax/ecx/edx renames in the faction load and the list head read
// Leaf: ?AddMessage@CMessageEventManager@@QAEPAVCMessageEvent@@W4EMessageEventType
struct CBaseObjectPointer;
struct CFaction;
struct C3DVector {
    float X, Y, Z;
};
enum EMessageEventType { EMessageEventType_Force32 = 0x7fffffff };
enum EReactionSpeechType { REACTION_SPEECH_NONE = 0, EReactionSpeechType_Force32 = 0x7fffffff };
struct CBaseObject {
    virtual ~CBaseObject();                             // 0x0099A430
    CBaseObjectPointer* IntelligentPointer;             // +0x04
    CBaseObject();                                      // 0x0099A380
};
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObjectPointer* PData;                          // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
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
struct CThing : CBaseObject {
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
struct CMessageEvent : CBaseObject {                    // vtable 0x0125BE8C, 0x58 bytes (Ego_r.pdb offsets)
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
long __fastcall GetCurrentTime_0049d870();              // 0x0049D870
extern unsigned long g_NextMessageID;                   // 0x0137EDB8
struct CMessageEventDef {
    unsigned char _pad00[0x3c];
    long          SightRadius;                          // +0x3c
    long          HearingRadius;                        // +0x40
    unsigned long Lifespan;                             // +0x44
    EReactionSpeechType ReactionSpeechType;             // +0x48
    EMessageEventType Type;                             // +0x4c
};
struct CWorld;
struct CMessageEventManager {
    CWorld*      World;                                 // +0x00
    CMessageList Messages;                              // +0x04
    CMessageEvent* AddMessage(EMessageEventType type, unsigned long lifespan, const C3DVector* pos, CThing* originator,
        CThing* target, float sightRadius, float hearingRadius, long extraData);
    CMessageEvent* AddMessage(CMessageEventDef* def, const C3DVector* pos, CThing* originator, CThing* target, long extraData);
};

CMessageEvent* CMessageEventManager::AddMessage(EMessageEventType type, unsigned long lifespan, const C3DVector* pos,
    CThing* originator, CThing* target, float sightRadius, float hearingRadius, long extraData)
{
    CMessageEvent event;
    event.Pos = *pos;
    event.Lifespan = lifespan;
    event.TimeStamp = GetCurrentTime_0049d870();
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
        if (target->HasInterface(0x49))
            event.PTargetFaction = target->GetConstInterface<CTCFactionMember>(0x49)->PFaction;
    } else
        event.TargetUID = 0;
    Messages.push_front(event);
    return &*Messages.begin();
}
