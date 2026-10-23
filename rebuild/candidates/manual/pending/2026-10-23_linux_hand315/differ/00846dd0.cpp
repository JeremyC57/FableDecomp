// Grade: DIFFER(1685v1685) -- register priority (weighted-use rank; for the R&D register round): register name only: in the !ReplacedAction emote/sound block retail keeps &PCreature in ebp, ours in ebx (7 instructions); advisor requested
// CCreatureAction_PerformExpression::FinishAction() -- end of an expression: an extendable held expression is replaced by a PerformExpressionExtended (moving the held item); otherwise clear emote/sound/script, post the expression event and FOLLOW/WAIT opinion deeds to the target, then play the out-of animation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00846DD0, length 1685 bytes.
// Tried: QFE, PCreature.GetPItem() vs Creature(), CIntelligentPointer& alias, __forceinline helper for the block, if-order swap (worse), no frame local (worse).
// Leaf: FinishAction
struct CTCBase;
struct C3DVector;
struct CThing;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CGamePlayerInterface {
    bool IsAssignableSpecialMoveButtonInQueue(long index, long eventType, long player) const;   // 0x004460C0
    bool IsContextSensitiveEventInQueue(long index, long eventType, long player) const;        // 0x004461D0
};
struct CPlayer {
    unsigned char _pad0[0xc];
    CGamePlayerInterface* PInterface;                   // +0x0c
    long GetPlayerNumber() const;                       // 0x00487510
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned char m_flags6c;                            // +0x6c (0x10 creature, 0x20 hero)
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char m_flags91;                            // +0x91 bit0 Dead
    unsigned char _pad92[0x1c9 - 0x92];
    unsigned char m_flags1c9;                           // +0x1c9
    const C3DVector* GetPosition() const;               // 0x004C73D0
    CPlayer* GetPlayer() const;                         // 0x004C7A30
    void RemoveTC(long type);                           // 0x004C9840
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterfaceConst(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CCreatureActionBase;
struct CThingCreatureBase : CThing {
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
struct CIntelligentPointer {
    void* m_a; void* m_b;
    CIntelligentPointer(CThing* p);                     // 0x004A96F0
    CIntelligentPointer(const CIntelligentPointer& o);
    ~CIntelligentPointer();                             // 0x00A01C10
    CIntelligentPointer& operator=(const CIntelligentPointer& o);   // 0x004A9760
    CThing* GetPItem() const;                           // 0x00A01B50
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer& thing);   // 0x004CC340
struct CCharString {
    void* m_pData;
    ~CCharString();                                     // 0x0099EAE0
};
struct CWideString {
    void* m_pData;
    CWideString(const unsigned short* text);                   // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
struct CDefString {
    long m_index;
    CDefString(long index);
    bool operator==(const char* text) const;            // 0x005B3440
    bool operator!=(const char* text) const;            // 0x005FA740
};
struct CDefStringTable {
    CCharString GetString(long id);                     // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828
struct CQuestManager {
    void DeactivateQuest(const CCharString& questName, unsigned long reason);   // 0x004B43D0
};
extern CQuestManager* g_pQuestManager;                  // 0x013B89FC
struct CTCInventory {
    long GetQuickAccessItemButtonIndex(long item);      // 0x00581FD1
    long GetContextSensitiveItemButtonIndex(long item); // 0x00581D4E
};
struct CTCEmoteIcon {
    void SetEmoteIcon(long priority, long icon, float seconds, float scale, unsigned char alpha);   // 0x0071AAB0
    void SetEmoteText(long priority, const CWideString& text, float activeTime, float scale);      // 0x0071AC70
};
struct CTCSound {
    void StopSound(unsigned long id);                   // 0x0066AD90
};
struct CSoundPair {
    long m_first; long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CExpressionDef {
    virtual void d00(); virtual void d01(); virtual void d02(); virtual void d03(); virtual void d04(); virtual void d05();
    virtual void d06(); virtual void d07(); virtual void d08(); virtual void d09();
    virtual CDefString GetName() const;                 // slot 10 (+0x28)
    unsigned char _pad4[0x48 - 4];
    CDefString AnimationOutOf;                          // +0x48
    unsigned char _pad4c[0x58 - 0x4c];
    CDefString AnimationToOtherOutOf;                   // +0x58
    unsigned char _pad5c[0x78 - 0x5c];
    CDefString ScriptName;                              // +0x78
    bool ScriptLoadResources;                           // +0x7c
    bool DeactivateScriptAtEnd;                         // +0x7d
};
struct CDefPointer_CExpressionDef {
    CExpressionDef* m_pDef;
    CExpressionDef* operator->() const { return m_pDef; }
};
// Layout: retail CCreatureActionBase is 0xa8 bytes (PDB 0xb4: the three STLport containers are 12 bytes, not 16).
struct CCreatureActionBase {
    virtual void v00();
    long _pad04;
    CIntelligentPointer PCreature;                      // +0x08
    long TotalTime, ActionTime, AnimDelayTime, FramesRunning;   // +0x10..+0x1c
    // +0x20 is read as the running frame count
    long InterruptionGroupID, InterruptionPriority, ActionID;   // +0x20..+0x28
    CIntelligentPointer PItemRemoved;                   // +0x2c
    unsigned char _pad34[0x60 - 0x34];
    bool AddedActionAnim;                               // +0x60
    bool Finished;                                      // +0x61
    unsigned char _pad62[0xa8 - 0x62];
    ~CCreatureActionBase();                             // 0x00693EF0
    void FinishAction();                                // 0x00694430
    long GetEnvironmentFrames();                        // 0x006925C0
};
struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};
struct CCreatureAction_PerformExpressionExtended : CCreatureActionBase {
    unsigned char _expr[0xc8 - 0xa8];
    CCreatureAction_PerformExpressionExtended(CThingCreatureBase* creature, CThing* performTo, const CDefPointer_CExpressionDef& def,
        long itemIndex, bool emoteIconAdded, unsigned long soundEffectID, long frameCount, long startFrame);   // 0x00843310
    ~CCreatureAction_PerformExpressionExtended();       // 0x0084ADC0
    void SetItem(CIntelligentPointer item);             // 0x0072D360
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CBaseIntelligentPointer();                          // 0x00A01B10
    void SetPItem(void* item);                          // 0x00A01B90
    CThing* GetPItem() const;                           // 0x00A01B50
    ~CBaseIntelligentPointer();                         // 0x00A01C10
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(void* item) { SetPItem(item); }
};
struct CEventBase { };
struct CEventExpressionPerformedTo : CEventBase {
    unsigned char _pad[0x20];
    CEventExpressionPerformedTo(void* target, long expression, const CDefPointer_CExpressionDef& data, long a, long b);   // 0x0084B010
};
struct CTCEntityEvents {
    void AddEvent(CEventBase* e);                       // 0x008113C0
};
struct CTCFollowed {
    bool IsCreatureValidFollower(CThing* creature, bool* addToFront);   // 0x006D4E60
    bool IsFollower(CThing* creature);                  // 0x006D4D70
    bool IsAIFollower(CThing* creature);                // 0x006D4DB0
};
struct CTCHeroOpinionDeedLog {
    long PostDeed(long deed, const C3DVector* pos, CThing* thing, CThing* creature, long postType,
                  bool a, bool b, void* faction, bool c, long defIndex);   // 0x006C6990
};
// Layout: retail CCreatureAction_PerformExpression = PDB minus 0xc (base containers): PExpressionDef +0xa8, PPerformToThing +0xac,
// ExpressionItemDefIndex +0xb4, EmoteIconAdded +0xb8, ReplacedAction +0xb9, Extended +0xba, ButtonHeld +0xbb, SoundEffectID +0xbc,
// ExtraData1 +0xc0, ExtraData2 +0xc4.
struct CCreatureAction_PerformExpression : CCreatureActionBase {
    CDefPointer_CExpressionDef PExpressionDef;          // +0xa8
    CIntelligentPointer PPerformToThing;                // +0xac
    long ExpressionItemDefIndex;                        // +0xb4
    bool EmoteIconAdded;                                // +0xb8
    bool ReplacedAction;                                // +0xb9
    bool Extended;                                      // +0xba
    bool ButtonHeld;                                    // +0xbb
    unsigned long SoundEffectID;                        // +0xbc
    long ExtraData1;                                    // +0xc0
    long ExtraData2;                                    // +0xc4
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual bool IsHoldingItem();
    void FinishAction();
    CThingCreatureBase* Creature() const { return (CThingCreatureBase*)PCreature.GetPItem(); }
};

void CCreatureAction_PerformExpression::FinishAction()
{
    if (ReplacedAction)
        return;
    if (FramesRunning >= TotalTime) {
        if (Extended && ButtonHeld) {
            ReplacedAction = true;
            CThing* heldItem = 0;
            if (IsHoldingItem() && IS_THING_ALIVE(PItemRemoved)) {
                heldItem = PItemRemoved.GetPItem();
                CIntelligentPointer none(0);
                PItemRemoved = none;
            }
            long frame = InterruptionGroupID;
            CThing* performTo = PPerformToThing.GetPItem();
            CThingCreatureBase* creature = Creature();
            CCreatureAction_PerformExpressionExtended action(creature, performTo, PExpressionDef, ExpressionItemDefIndex,
                EmoteIconAdded, SoundEffectID, GetEnvironmentFrames(), frame + 1);
            if (heldItem)
                action.SetItem(CIntelligentPointer(heldItem));
            Creature()->SetCurrentAction(action);
        }
    } else {
        ((CBaseIntelligentPointer&)PItemRemoved).SetPItem(CIntelligentPointerThing(0).GetPItem());
    }
    CCreatureActionBase::FinishAction();
    if (!ReplacedAction) {
        if (Creature() && !(Creature()->m_flags91 & 1)) {
            if (EmoteIconAdded) {
                CTCEmoteIcon* emote = 0;
                Creature()->GetInterface(&emote, 0x45);
                emote->SetEmoteIcon(5, 0, 0.0f, 1.0f, 255);
                CWideString empty((const unsigned short*)L"");
                emote->SetEmoteText(7, empty, 0.0f, 1.0f);
            }
            Creature()->RemoveTC(0x8f);
        }
        if (SoundEffectID > 0) {
            CTCSound* sound;
            if (Creature()->GetInterface(&sound, 0x10))
                sound->StopSound(SoundEffectID);
        }
        if (PExpressionDef->DeactivateScriptAtEnd && PExpressionDef->ScriptName != "") {
            CCharString name = g_DefStringTable.GetString(PExpressionDef->ScriptName.m_index);
            g_pQuestManager->DeactivateQuest(name, 0);
        }
    }
    if (PPerformToThing.GetPItem()) {
        if (PPerformToThing.GetPItem()->m_flags6c & 0x10) {
            if (PExpressionDef->GetName() == "EXPRESSION_FOLLOW" || PExpressionDef->GetName() == "EXPRESSION_WAIT") {
                if (!(PPerformToThing.GetPItem()->m_flags1c9 & 4))
                    goto anim;
            }
        }
        {
            CTCEntityEvents* events;
            if (PPerformToThing.GetPItem()->GetInterface(&events, 0x42))
                events->AddEvent(new CEventExpressionPerformedTo(Creature(), GetEnvironmentFrames(), PExpressionDef, ExtraData1, ExtraData2));
        }
        if ((PPerformToThing.GetPItem()->m_flags6c & 0x10) && (Creature()->m_flags6c & 0x20)) {
            CTCFollowed* followed;
            if (Creature()->GetInterfaceConst(&followed, 0xbc)) {
                if (PExpressionDef->GetName() == "EXPRESSION_FOLLOW") {
                    CThing* target = PPerformToThing.GetPItem();
                    CTCHeroOpinionDeedLog* log;
                    Creature()->GetInterface(&log, 0x51);
                    bool addToFront = false;
                    bool valid = followed->IsCreatureValidFollower(target, &addToFront);
                    if (addToFront && !followed->IsFollower(target)) {
                        if (valid)
                            log->PostDeed(0x2e, Creature()->GetPosition(), target, target, 1, false, false, 0, false, 0);
                        else
                            log->PostDeed(0x2f, Creature()->GetPosition(), target, target, 1, false, false, 0, false, 0);
                    }
                } else if (PExpressionDef->GetName() == "EXPRESSION_WAIT") {
                    CThing* target = PPerformToThing.GetPItem();
                    if (followed->IsAIFollower(target)) {
                        CTCHeroOpinionDeedLog* log;
                        Creature()->GetInterface(&log, 0x51);
                        log->PostDeed(0x30, Creature()->GetPosition(), target, target, 1, false, false, 0, false, 0);
                    }
                }
            }
        }
    }
anim:
    if (PPerformToThing.GetPItem() && PExpressionDef->AnimationToOtherOutOf != "") {
        CCharString anim = g_DefStringTable.GetString(PExpressionDef->AnimationToOtherOutOf.m_index);
        CSoundPair sound;
        Creature()->SetCurrentAction(CCreatureAction_PlayAnimation(Creature(), anim, sound, false, true, false, 0, 0));
    } else if (PExpressionDef->AnimationOutOf != "") {
        CCharString anim = g_DefStringTable.GetString(PExpressionDef->AnimationOutOf.m_index);
        CSoundPair sound;
        Creature()->SetCurrentAction(CCreatureAction_PlayAnimation(Creature(), anim, sound, false, true, false, 0, 0));
    }
}
