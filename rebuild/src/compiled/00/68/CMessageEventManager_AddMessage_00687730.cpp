// CMessageEventManager::AddMessage(const CMessageEventDef& def, const C3DVector& pos, CThing* originator, CThing* target, long extraData) -- build a time-stamped message event from the definition's type, radii, lifespan and reaction speech with the next message id, the originator/target pointers and UIDs and the target's faction, push a copy on the front of the list and return it
// Leaf: ?AddMessage@CMessageEventManager@@QAEPAVCMessageEvent@@ABVCMessageEventDef@@ABVC3DVector@@PAVCThing@@2J@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00687730, 477 bytes; Ego_r.pdb ?AddMessage@CMessageEventManager@@QAEPAVCMessageEvent@@ABVCMessageEventDef@@ABVC3DVector@@PAVCThing@@2J@Z
//   (def and pos are const references; the old draft had pointers).
// Callees: CWorld::GetFrame 0x0049D870 (static __fastcall, ?GetFrame@CWorld@@SIJXZ), CBaseObject ctor/dtor 0x0099A380/0x0099A430,
//   CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10, CVectorMap<ETCInterfaceType,
//   CTCBase*>::LowerBound const 0x004365B0, CMessageEvent copy ctor 0x00687310, malloc 0x00BFEA0E.
// Fix (chunk 759): the 0x49 component is TCI_ENEMY (engine/ETCInterfaceType.h); Ego_r CTCEnemy::PFaction sits at +0x28 with
//   12-byte lists, +0x18 with retail's 4-byte STLport lists, the offset retail reads. The lookup is the PeekTC(ETCInterfaceType, T**)
//   const shape over the real engine/CVectorMap.h (key copied into a local, explicit LowerBound / end() test), and `event.Pos = pos`
//   is assigned straight after the time stamp: then VC7.1 puts the 0x49 key temporary in the dead `pos` slot ([esp+0x78]) as
//   retail does (pos copied after the radii: the dead `def` slot, 2 diff lines; the old HasInterface + GetConstInterface form: 14).
#include "fable_compiler.h"
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
class CBaseObjectPointer;
class CFaction;
#include "engine/C3DVector.h"
enum EMessageEventType { EMessageEventType_Force32 = 0x7fffffff };
enum EReactionSpeechType { REACTION_SPEECH_NONE = 0, EReactionSpeechType_Force32 = 0x7fffffff };
class CBaseObject { public:
    virtual ~CBaseObject();                             // 0x0099A430
    CBaseObjectPointer* IntelligentPointer;             // +0x04
protected:
    CBaseObject();                                      // 0x0099A380 ??0CBaseObject@@IAE@XZ (protected in both PDBs)
};
class CBaseIntelligentPointer { public:
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObjectPointer* PData;                          // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    CIntelligentPointer() {}
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
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
    template<class T> FABLE_FORCEINLINE bool PeekTC(ETCInterfaceType id, T** ptc) const
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
    FABLE_FORCEINLINE void push_front(const CMessageEvent& value)
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

CMessageEvent* CMessageEventManager::AddMessage(const CMessageEventDef& def, const C3DVector& pos, CThing* originator, CThing* target,
    long extraData)
{
    CMessageEvent event;
    event.TimeStamp = CWorld::GetFrame();
    event.Pos = pos;
    event.HearingRadius = (float)def.HearingRadius;
    event.SightRadius = (float)def.SightRadius;
    event.Lifespan = def.Lifespan;
    event.POriginator = originator;
    event.PTarget = target;
    event.Type = def.Type;
    event.MessageID = g_NextMessageID++;
    event.ReactionSpeechType = def.ReactionSpeechType;
    event.PTargetFaction = 0;
    event.OriginatorUID = originator ? originator->UID : 0;
    if (target) {
        event.TargetUID = target->UID;
        CTCEnemy* enemy;
        if (target->PeekTC(TCI_ENEMY, &enemy))
            event.PTargetFaction = enemy->PFaction;
    } else
        event.TargetUID = 0;
    event.ExtraData = extraData;
    Messages.push_front(event);
    return &*Messages.begin();
}