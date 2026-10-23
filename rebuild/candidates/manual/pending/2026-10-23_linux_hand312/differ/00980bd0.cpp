// Grade: DIFFER(1212v1233) -- repeat==1 half byte-exact; in the later-call half retail cross-jumps 'return 3' and the second 'return 1' (IsThingAlive form) into shared cold blocks at the end, ours inlines their epilogues; the second CTCTalk lookup uses eax/edx where retail uses edx/eax
// CAIStateGroupBase::AIAction_SpeakToTarget(repeat, speaker, target, speech, interrupt, priority, wait) -- on the first call finish the creature's action for a creature target and look at the target while it is speaking, then play the speech (AI speech when waiting) and, while the creature speaks, keep looking at the target; on later calls keep looking while speaking, or start the speech if the target is quiet
// Retail: Fable.exe (TLC, VC7.1) @ 0x00980BD0, 1248 bytes, ret 0x24.
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10
//   (CIntelligentPointer<const CThing> vtable 0x01237F1C), CThingCreatureBase::IsFreeToFinishAction 0x006621C0 /
//   FinishCurrentAction 0x00662650, AIAction_LookAtThing 0x0097D6F0, IsSpeaking 0x008644E0 (__fastcall),
//   StopSpeaking 0x00864580, CTCTalk::PlayAISpeech 0x006E3B60 / PlaySpeech 0x006E3B10, IsStopping 0x008FD010,
//   IS_THING_ALIVE 0x006961F0 (__fastcall), CVectorMap::LowerBound 0x0040F020, the brain's virtual slot 2 (+8).
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad0[0x10];
    unsigned char ThingType;                    // +0x10
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline bool TestTC(long type) const { return ((m_presentBits[type >> 5] >> (type & 31)) & 1) != 0; }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
};
struct CThingCreatureBase : CThing {
    bool IsFreeToFinishAction();                // 0x006621C0
    void FinishCurrentAction();                 // 0x00662650
    void StopSpeaking();                        // 0x00864580
};
bool __fastcall IsSpeaking(CThing* creature);   // 0x008644E0
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
bool __fastcall IsThingAlive(CBaseIntelligentPointer& pointer);   // 0x006961F0
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    return p.GetPItem() != 0 && !(p.GetPItem()->m_flags91 & 1);
}
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerCreature(const CIntelligentPointerCreature& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    CThingCreatureBase* Get() const { return (CThingCreatureBase*)GetPItem(); }
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    __forceinline CIntelligentPointerConstThing(const CIntelligentPointerConstThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CTCTalk {
    long PlayAISpeech(unsigned long sampleID, bool force, long layer, bool b1, bool b2);   // 0x006E3B60
    long PlaySpeech(unsigned long textId, long layer, bool a, bool b);                    // 0x006E3B10
};
struct CAIBrain {
    virtual void v0(); virtual void v1();
    virtual void OnActionUpdate();              // slot 2 (+8)
    unsigned char _pad04[0x18 - 0x04];
    CThingCreatureBase* Creature;               // +0x18
};
struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    bool IsStopping();                          // 0x008FD010
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long frames, float speed);   // 0x0097D6F0
    long AIAction_SpeakToTarget(long repeat, CIntelligentPointerCreature speaker, CIntelligentPointerConstThing target,
                                unsigned long speech, bool interrupt, long priority, bool wait);
};

long CAIStateGroupBase::AIAction_SpeakToTarget(long repeat, CIntelligentPointerCreature speaker, CIntelligentPointerConstThing target,
                                               unsigned long speech, bool interrupt, long priority, bool wait)
{
    long result = 4;
    if (repeat == 1) {
        if (IS_THING_ALIVE(target) && target.GetPItem()->ThingType == 2 && GetCreature()->IsFreeToFinishAction())
            GetCreature()->FinishCurrentAction();
        if (IS_THING_ALIVE(target)) {
            long look = AIAction_LookAtThing(1, target, priority | 0x10, 0.0027777778f);
            if (look != 4 && look != 2)
                return look;
        }
        while (IS_THING_ALIVE(target) && !interrupt && target.GetPItem()->ThingType == 1 && IsSpeaking(target.GetPItem())) {
            long look = AIAction_LookAtThing(1, target, priority | 0x10, 0.0027777778f);
            if (look != 4 && look != 2)
                return look;
        }
        CThingCreatureBase* creature = GetCreature();
        CTCTalk* talk;
        bool hasTalk = creature->TestTC(0xf);
        if (hasTalk)
            talk = (CTCTalk*)creature->GetTC(0xf);
        for (;;) {
            long played = wait ? talk->PlayAISpeech(speech, false, 1, false, true) : talk->PlaySpeech(speech, 0, false, true);
            if (played == 2) {
                PBrain->OnActionUpdate();
                if (IsStopping()) {
                    if (IS_THING_ALIVE(speaker))
                        speaker.Get()->StopSpeaking();
                    return 1;
                }
                continue;
            }
            if (played == 1)
                return 3;
            break;
        }
        while (IsSpeaking(GetCreature()) && IS_THING_ALIVE(target)) {
            long look = AIAction_LookAtThing(1, target, priority | 0x10, 0.0027777778f);
            if (look != 4 && look != 2) {
                GetCreature()->StopSpeaking();
                return look;
            }
        }
        return 2;
    }
    if (IsStopping()) {
        if (IS_THING_ALIVE(speaker))
            speaker.Get()->StopSpeaking();
        return 1;
    }
    do {
        if (!IsSpeaking(GetCreature())) {
            if (IS_THING_ALIVE(target) && !interrupt && target.GetPItem()->ThingType == 1 && IsSpeaking(target.GetPItem())) {
                PBrain->OnActionUpdate();
                result = 4;
                break;
            }
            CThingCreatureBase* creature = GetCreature();
            CTCTalk* talk = 0;
            if (creature->HasTC(0xf))
                talk = (CTCTalk*)creature->GetTC(0xf);
            long played = wait ? talk->PlayAISpeech(speech, false, 1, false, true) : talk->PlaySpeech(speech, 0, false, true);
            if (played == 1)
                return 3;
            if (played == 2) {
                PBrain->OnActionUpdate();
                result = 4;
                break;
            }
        } else if (IS_THING_ALIVE(target))
            result = AIAction_LookAtThing(2, target, priority | 0x10, 0.0027777778f);
        if (IsStopping()) {
            if (IsThingAlive(speaker))
                speaker.Get()->StopSpeaking();
            return 1;
        }
        if (!IsSpeaking(GetCreature()))
            result = 2;
    } while (0);
    return result;
}
