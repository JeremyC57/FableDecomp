// CAIStateGroupBase::AIAction_SpeakAISpeechToTarget(EAIActionRepeat repeat, CIntelligentPointer<CThingCreatureBase> speaker, CIntelligentPointer<const CThing> target, unsigned long speech, bool interrupt, long priority) -- forwards to the seven-argument AIAction_SpeakAISpeechToTarget with a trailing `true`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00981EA0, length 130 bytes, `ret 20h`.
// Callees: CAIStateGroupBase::AIAction_SpeakAISpeechToTarget(...) @ 0x00981350 (thiscall, seven arguments),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, ::GetPItem @ 0x00A01B50, ::SetPItem @ 0x00A01B90,
// ::~CBaseIntelligentPointer @ 0x00A01C10.
// Shape: arguments evaluated right to left -- the literal `true`, the three scalar arguments (two of them loaded
// before the prologue pushes), then the two by-value intelligent pointers copy-constructed in their argument slots
// (`sub esp,8 ; mov esi,esp`, inline family-44 copy constructor: base ctor, own vtable 0x01237F1C / 0x0123E7D8,
// SetPItem(other.GetPItem())), the repeat mode last. The callee-destroyed by-value parameters are released after
// the call through the base destructor (the CIntelligentPointer<T> classes declare none); the result rides in esi.
// The two rows 0x009812C0 / 0x00981EA0 are this same forwarder over two different seven-argument targets
// (0x009812C0 -> 0x00980BD0 AIAction_SpeakToTarget, 0x00981EA0 -> 0x00981350 AIAction_SpeakAISpeechToTarget). Parameter types from the manifest prototype; the 4-byte speech argument is modelled as an id.
class CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                      // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                         // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};

struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerCreature(const CIntelligentPointerCreature& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {      // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerThing(const CIntelligentPointerThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

enum EAIActionRepeat { AI_ACTION_REPEAT_NONE };
enum EAIActionReturn { AI_ACTION_RETURN_NONE };

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    EAIActionReturn AIAction_SpeakAISpeechToTarget(EAIActionRepeat repeat, CIntelligentPointerCreature speaker,
        CIntelligentPointerThing target, unsigned long speech, bool interrupt, long priority, bool wait);   // 0x00981350
    EAIActionReturn AIAction_SpeakAISpeechToTarget(EAIActionRepeat repeat, CIntelligentPointerCreature speaker,
        CIntelligentPointerThing target, unsigned long speech, bool interrupt, long priority);
};

EAIActionReturn CAIStateGroupBase::AIAction_SpeakAISpeechToTarget(EAIActionRepeat repeat, CIntelligentPointerCreature speaker,
    CIntelligentPointerThing target, unsigned long speech, bool interrupt, long priority)
{
    return AIAction_SpeakAISpeechToTarget(repeat, speaker, target, speech, interrupt, priority, true);
}