// CMessageEventManager::AddMessage(EMessageEventType type, unsigned long lifespan, const C3DVector& pos, CThing* originator, CThing* target, float sightRadius, float hearingRadius, long extraData) -- build a time-stamped message event with the next message id, the originator/target pointers and UIDs and the target's faction, push a copy on the front of the list and return it
// Leaf: ?AddMessage@CMessageEventManager@@QAEPAVCMessageEvent@@W4EMessageEventType@@KABVC3DVector@@PAVCThing@@2MMJ@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00687540, 486 bytes; Ego_r.pdb ?AddMessage@CMessageEventManager@@QAEPAVCMessageEvent@@
//   W4EMessageEventType@@KABVC3DVector@@PAVCThing@@2MMJ@Z (pos is a const reference). Model shared with the landed-ready sibling
//   0x00687730 (chunk 759): CWorld::GetFrame 0x0049D870, TCI_ENEMY PeekTC over engine/CVectorMap.h (CTCEnemy::PFaction +0x18).
// Fix (chunk 762): engine/CBaseObject.h + engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN) instead of the
//   local CBaseObject / CBaseIntelligentPointer models: VC7.1 then caches the CIntelligentPointer vtable constant in edi and keeps
//   the originator in esi, as retail does (the local models swapped them, 17 lines).
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
class CFaction;
class C3DVector { public:
    float X, Y, Z;
};
enum EMessageEventType { EMessageEventType_Force32 = 0x7fffffff };
enum EReactionSpeechType { REACTION_SPEECH_NONE = 0, EReactionSpeechType_Force32 = 0x7fffffff };
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#include "engine/CIntelligentPointer.h"
class CTCBase { public: virtual ~CTCBase(); };
class CTCEnemy : public CTCBase { public:                             // TCI_ENEMY (73); Ego_r CTCEnemy, STLport lists 4 bytes in retail
    unsigned char _pad04[0x18 - 0x04];
    CFaction*     PFaction;                             // +0x18
};
class CThing : public CBaseObject { public:
    unsigned char    _pad08[0x18 - 0x08];
    unsigned __int64 UID;                               // +0x18
    std::bitset<274> UsingComponents;                   // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    template<class T> __forceinline bool PeekTC(ETCInterfaceType id, T** ptc) const
    {
        if (HasTC(id)) {
            ETCInterfaceType type = id;
            const CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            const std::pair<ETCInterfaceType, CTCBase*>* it = map->LowerBound(type);
            if (it == map->end() || id < it->first)
                it = map->end();
            *ptc = static_cast<T*>(it->second);
            return true;
        }
        return false;
    }
};
class CMessageEvent : public CBaseObject { public:                    // vtable 0x0125BE8C, 0x58 bytes (Ego_r.pdb offsets)
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
extern unsigned long g_NextMessageID;                   // 0x0137EDB8
class CMessageEventDef { public:
    unsigned char _pad00[0x3c];
    long          SightRadius;                          // +0x3c
    long          HearingRadius;                        // +0x40
    unsigned long Lifespan;                             // +0x44
    EReactionSpeechType ReactionSpeechType;             // +0x48
    EMessageEventType Type;                             // +0x4c
};
class CWorld { public:
    static long __fastcall GetFrame();                  // 0x0049D870
};
class CMessageEventManager { public:
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
        CTCEnemy* enemy;
        if (target->PeekTC(TCI_ENEMY, &enemy))
            event.PTargetFaction = enemy->PFaction;
    } else
        event.TargetUID = 0;
    Messages.push_front(event);
    return &*Messages.begin();
}