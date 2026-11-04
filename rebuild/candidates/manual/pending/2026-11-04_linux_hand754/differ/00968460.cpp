// CAIStateGroup_TavernKeeper::GetPaymentFinishedMessage() -- while the payee is alive, wait for its "payment finished" message (type 0x30) aimed at this creature; on a hit withdraw that message from the world's message manager and report success
// Grade: DIFFER(172v172) -- name GetPaymentFinishedMessage (type 0x30); &PPayee/payee web in ebx and the creature in edi where retail has edi/ebx (dl 18 same length, best try t)
// Leaf: ?GetPaymentFinishedMessage@CAIStateGroup_TavernKeeper@@QAE_NXZ
// Flags: none
class CBaseObject;
class CThing { public:
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
};
class CBaseIntelligentPointer { public:
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseObject* GetPItem() const;              // 0x00A01B50
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* Creature;                           // +0x18
};
class CMessageEvent;
class CMessageEventManager { public:
    void RemoveMessage(const CMessageEvent* message);   // 0x00493B5B
};
class CWorld { public:
    unsigned char _pad[0x60];
    CMessageEventManager* PMessageEventManager_Data;    // +0x60 CCountedPointer<CMessageEventManager>
    CMessageEventManager& GetMessageEventManager() { return *PMessageEventManager_Data; }
};
class CMessageEventFilter_IsOfType { public:
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};
class CMessageEventFilter_IsTargettedAtThing { public:
    long m_handle[2];
    CMessageEventFilter_IsTargettedAtThing(const CThing& thing);   // 0x006AF290
};
class CMessageEventFilter_IsOriginatedByThing { public:
    long m_handle[2];
    CMessageEventFilter_IsOriginatedByThing(const CThing& thing);  // 0x006AF2B0
};
template<class A, class B, class C> class CFilter_Bind3 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    CFilter_Bind3(const A& a, const B& b, const C& c) : m_pFirst(&a), m_pSecond(&b), m_pThird(&c) { }
};
typedef CFilter_Bind3<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatedByThing, CMessageEventFilter_IsTargettedAtThing> CMessageFilter;
enum EAIActionRepeat { AI_ACTION_REPEAT_0, AI_ACTION_REPEAT_1, AI_ACTION_REPEAT_2 };
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    long AIAction_WaitForMessage(EAIActionRepeat repeat, const CMessageFilter& filter, CMessageEvent** ppMessage);   // 0x0091A670
protected:
    CWorld& GetWorld() const;                   // 0x008FCFC0
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if ((CThing*)p.GetPItem() != 0) {
        if (!(((CThing*)p.GetPItem())->m_flags91 & 1))
            return true;
    }
    return false;
}
class CAIStateGroup_TavernKeeper : public CAIStateGroupBase { public:
    CBaseIntelligentPointer PJug;               // +0x14 CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PBar;               // +0x1c CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PPub;               // +0x24 CIntelligentPointer<CThingBuilding>
    CBaseIntelligentPointer PPayee;             // +0x2c CIntelligentPointer<CThingAICreature>
    bool GetPaymentFinishedMessage();
};
long TmplSink_00968460(const void* self, long seen);   // stand-in: the message search AIAction_WaitForMessage does
#pragma auto_inline(off)
__declspec(noinline) long CAIStateGroupBase::AIAction_WaitForMessage(EAIActionRepeat repeat, const CMessageFilter& filter, CMessageEvent** ppMessage)
{
    long result;
    do {
        *ppMessage = (CMessageEvent*)TmplSink_00968460(this, *(const long*)&filter);
        if (*ppMessage) {
            result = 2;
            break;
        }
        result = 4;
    } while (repeat == AI_ACTION_REPEAT_1);
    return result;
}
#pragma auto_inline(on)
bool CAIStateGroup_TavernKeeper::GetPaymentFinishedMessage()
{
    CMessageEvent* message = 0;
    if (IS_THING_ALIVE(PPayee)) {
        CAIBrain* brain = PBrain;
        CThing* creature = brain->Creature;
        CThing* payee = (CThing*)PPayee.GetPItem();
        if (AIAction_WaitForMessage(AI_ACTION_REPEAT_2,
                CMessageFilter(CMessageEventFilter_IsOfType(0x30), CMessageEventFilter_IsOriginatedByThing(*payee),
                    CMessageEventFilter_IsTargettedAtThing(*creature)),
                &message) == 2) {
            GetWorld().GetMessageEventManager().RemoveMessage(message);
            return true;
        }
    }
    return false;
}
