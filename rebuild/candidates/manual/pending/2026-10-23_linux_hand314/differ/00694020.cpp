// CCreatureActionBase::FrameUpdate() -- per-frame action tick: report progress, dispatch due action events and drop the finished ones, run the movement update at the animation end, finish at TotalTime, advance FramesRunning from the running time, release / start the haste pause, perform the action at ActionTime (with its sound), fire due cut-scene events, and clamp the frame count
// Grade: DIFFER(1042v1032) -- 7 blocks: AddTC retry loop rotated (retail jmp back), cut-scene header loaded once (retail twice), PauseTimer store scheduling
// Leaf: FrameUpdate
// Retail 0x00694020, 1040 bytes, plain `ret` (virtual).
// Callees: For_each<CDispatchActionEventIfActivationFrame> 0x00695B20, find_if 0x00695E30, remove_copy_if 0x00696240,
//   copy 0x006962C0, _Destroy_range 0x00696790 (all LTCG `__fastcall`); UpdateActionMovement 0x00692AA0; __ftol2;
//   CBaseIntelligentPointer::GetPItem 0x00A01B50; CVectorMap::LowerBound 0x0040F020; CTCHaste pause set erase 0x0058119B /
//   insert 0x00580D21 / recalc 0x00800020; CThing::AddTC 0x004C9D60; CCharString 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0;
//   CSoundInit 0x0041C930; PlayRandomSound 0x0066B280; operator new 0x00BFEA1A; CTCEntityEvents::AddEvent 0x008113C0;
//   _Rb_global::_Rebalance_for_erase 0x0042A34E; free 0x00BFEA14. Cut-scene event vtable 0x0125C1CC.
// Layout: local model -- engine/CCreatureActionBase.h has the PDB (debug STL) container sizes; retail uses 12-byte
//   STLport vectors: ActionEvents {start +0x84, finish +0x88}, CutSceneEvents tree {header +0x90, count +0x94}; scalar
//   members as the header (TotalTime +0x10 .. PAnimEntry +0x74). Interfaces 0x8c CTCHaste, 0x42 events, 0x10 sound.
void* __cdecl operator new(unsigned int size);
extern "C" void __cdecl free(void* p);
extern long g_FramesPerSecond;                         // 0x01375550
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};
struct CTCBase { virtual void v00(); };
struct CEventBase { CEventBase() : Unknown(0) { } virtual void v00(); long Unknown; };
struct CEventCutScene : CEventBase {                   // vtable 0x0125C1CC, 0x0c bytes
    CCharString Name;
    CEventCutScene(const CCharString& name) : Name(name) { }
    virtual void v00();
};
struct CTCEntityEvents : CTCBase {
    void AddEvent(CEventBase* event);                  // 0x008113C0
};
struct CCreatureActionBase;
struct SPausePair { CCreatureActionBase* first; long second; SPausePair(CCreatureActionBase* a, long b) : first(a), second(b) { } };
struct SInsertResult { void* it; bool ok; };
struct CPauseSet {
    unsigned long Erase(CCreatureActionBase* const& key);   // 0x0058119B
    SInsertResult Insert(const SPausePair& value);          // 0x00580D21
};
struct CTCHaste : CTCBase {
    unsigned char _pad04[0x18 - 0x04];
    CPauseSet     Paused;                              // +0x18
    void Recalculate();                                // 0x00800020
};
struct CSoundPair { long FirstSound, LastSound; };
struct CSoundInit {
    unsigned char _data[0x20];
    CSoundInit(bool a, long b, long c);                // 0x0041C930
};
struct CTCSound : CTCBase {
    unsigned long PlayRandomSound(const CSoundPair& sounds, const CSoundInit& init);   // 0x0066B280
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x0040F020
    __forceinline CTCBase* FindInline(long key)
    {
        SEntry* end = last;
        SEntry* begin = first;
        long count = end - begin;
        SEntry* it = begin;
        while (count > 0) {
            long half = count / 2;
            if (it[half].type < key) {
                it += half + 1;
                count -= half + 1;
            } else
                count = half;
        }
        return (it == end || key < it->type ? end : it)->component;
    }
};
struct CThing {
    virtual void v00();
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    CTCBase* AddTC(const CCharString& name, long a, long b);   // 0x004C9D60
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    void* Data;
    void* Info;
    CThing* GetPItem() const;                          // 0x00A01B50
};
struct CActionEventPointer { void* Data; void* Info; };
struct CDispatchActionEventIfActivationFrame {
    long                 Frame;
    CCreatureActionBase* Action;
    CDispatchActionEventIfActivationFrame(long frame, CCreatureActionBase* action) : Frame(frame), Action(action) { }
};
struct CIsActionEventDone { bool b; };
struct SRandomTag { SRandomTag() { } };
CDispatchActionEventIfActivationFrame __fastcall For_each(CActionEventPointer* first, CActionEventPointer* last,
                                                          CDispatchActionEventIfActivationFrame f);   // 0x00695B20
CActionEventPointer* __fastcall Find_if(CActionEventPointer* first, CActionEventPointer* last, CIsActionEventDone pred,
                                        const SRandomTag& tag);                                       // 0x00695E30
CActionEventPointer* __fastcall Remove_copy_if(CActionEventPointer* first, CActionEventPointer* last,
                                               CActionEventPointer* out, CIsActionEventDone pred);    // 0x00696240
CActionEventPointer* __fastcall Copy(CActionEventPointer* first, CActionEventPointer* last, CActionEventPointer* out,
                                     const SRandomTag& tag, long* distance);                          // 0x006962C0
void __fastcall Destroy_range(CActionEventPointer* first, CActionEventPointer* last, const SRandomTag& tag);   // 0x00696790
struct CActionEventVector {
    CActionEventPointer* Start;
    CActionEventPointer* Finish;
    CActionEventPointer* End;
    __forceinline void Erase(CActionEventPointer* first, CActionEventPointer* last)
    {
        CActionEventPointer* finish = Copy(last, Finish, first, SRandomTag(), 0);
        Destroy_range(finish, Finish, SRandomTag());
        Finish = finish;
    }
};
struct SCutSceneNode {
    long           Color;
    SCutSceneNode* Parent;
    SCutSceneNode* Left;
    SCutSceneNode* Right;
    long           Frame;                              // +0x10
    CCharString    Name;                               // +0x14
};
SCutSceneNode* __cdecl Rebalance_for_erase(SCutSceneNode* z, SCutSceneNode*& root, SCutSceneNode*& leftmost,
                                           SCutSceneNode*& rightmost);   // 0x0042A34E
extern "C" long __cdecl _ftol2(double);

struct CCreatureActionBase {
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void PerformAction();                      // +0x0c
    virtual void Finish();                             // +0x10
    virtual void v14();
    virtual long GetAnimationLength();                 // +0x18
    virtual void v1c(); virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c(); virtual void v40();
    virtual void v44(); virtual void v48(); virtual void v4c(); virtual void v50(); virtual void v54();
    virtual void v58(); virtual void v5c(); virtual void v60();
    virtual void SetProgress(float progress);          // +0x64
    virtual void v68(); virtual void v6c(); virtual void v70(); virtual void v74(); virtual void v78();
    virtual void v7c(); virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9c(); virtual void va0();
    virtual void va4(); virtual void va8(); virtual void vac(); virtual void vb0(); virtual void vb4();
    virtual void vb8(); virtual void vbc(); virtual void vc0(); virtual void vc4(); virtual void vc8();
    virtual void vcc(); virtual void vd0();
    virtual void UpdateAnimation();                    // +0xd4
    virtual void vd8(); virtual void vdc(); virtual void ve0(); virtual void ve4(); virtual void ve8();
    virtual void vec(); virtual void vf0(); virtual void vf4(); virtual void vf8();
    virtual float GetSpeedScale();                     // +0xfc

    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                 // +0x08
    long                    TotalTime;                 // +0x10
    long                    ActionTime;                // +0x14
    long                    AnimDelayTime;             // +0x18
    long                    FramesRunning;             // +0x1c
    unsigned char           _pad20[0x34 - 0x20];
    CSoundPair              Sounds;                    // +0x34
    unsigned long           PlayingSoundID;            // +0x3c
    float                   RunningTimeSeconds;        // +0x40
    float                   LastRunningTimeSeconds;    // +0x44
    long                    StartFrame;                // +0x48
    long                    PauseFrame;                // +0x4c
    long                    PauseTimer;                // +0x50
    unsigned char           _pad54[0x60 - 0x54];
    bool                    AddedActionAnim;           // +0x60
    bool                    Finished;                  // +0x61
    bool                    PerformedAction;           // +0x62
    bool                    Initialised;               // +0x63
    unsigned char           _pad64[0x74 - 0x64];
    long*                   PAnimEntry;                // +0x74
    unsigned char           _pad78[0x84 - 0x78];
    CActionEventVector      ActionEvents;              // +0x84
    SCutSceneNode*          CutSceneHeader;            // +0x90
    unsigned long           CutSceneCount;             // +0x94

    void UpdateActionMovement();                       // 0x00692AA0
    __forceinline void EraseCutScene(SCutSceneNode* pos)
    {
        SCutSceneNode* erased = Rebalance_for_erase(pos, CutSceneHeader->Parent, CutSceneHeader->Left,
                                                    CutSceneHeader->Right);
        erased->Name.~CCharString();
        if (erased)
            free(erased);
        --CutSceneCount;
    }
    void FrameUpdate();
};

void CCreatureActionBase::FrameUpdate()
{
    LastRunningTimeSeconds = RunningTimeSeconds;
    if (RunningTimeSeconds == 0.0f && TotalTime == 0)
        SetProgress(0.0f);
    else
        SetProgress(RunningTimeSeconds / TotalTime);
    For_each(ActionEvents.Start, ActionEvents.Finish, CDispatchActionEventIfActivationFrame(FramesRunning, this));
    {
        CActionEventPointer* last = ActionEvents.Finish;
        CIsActionEventDone pred;
        pred.b = false;
        CActionEventPointer* first = Find_if(ActionEvents.Start, last, pred, SRandomTag());
        if (first != last)
            first = Remove_copy_if(first + 1, last, first, pred);
        ActionEvents.Erase(first, last);
    }
    UpdateAnimation();
    if (FramesRunning >= GetAnimationLength() + StartFrame && !Finished)
        UpdateActionMovement();
    if (TotalTime >= 0 && FramesRunning >= TotalTime && !Finished) {
        Finished = true;
        Finish();
        return;
    }
    FramesRunning = (long)(g_FramesPerSecond * RunningTimeSeconds);
    if (PauseTimer-- == 0) {
        CTCHaste* haste;
        if (PCreature.GetPItem()->GetInterface(&haste, 0x8c)) {
            CCreatureActionBase* key = this;
            haste->Paused.Erase(key);
            haste->Recalculate();
        }
    }
    if (FramesRunning == PauseFrame) {
        PauseFrame = -1;
        PauseTimer = (long)((1.0f - GetSpeedScale()) * 5.0f);
        CThing* creature;
        for (;;) {
            creature = PCreature.GetPItem();
            if (creature->m_presentBits[0x8c >> 5] & (1UL << (0x8c & 31)))
                break;
            CCharString name("CTCHaste");
            PCreature.GetPItem()->AddTC(name, 0, 0);
        }
        CTCHaste* haste = (CTCHaste*)creature->m_interfaces.FindInline(0x8c);
        haste->Paused.Insert(SPausePair(this, 0));
        haste->Recalculate();
    }
    if (ActionTime >= 0 && FramesRunning >= ActionTime && !PerformedAction &&
        (AddedActionAnim || PAnimEntry == 0 || *PAnimEntry == 0)) {
        PerformAction();
        PerformedAction = true;
        CTCSound* sound;
        if (PCreature.GetPItem()->GetInterface(&sound, 0x10))
            PlayingSoundID = sound->PlayRandomSound(Sounds, CSoundInit(true, 0, 0));
    }
    while (CutSceneCount != 0) {
        SCutSceneNode* node = CutSceneHeader->Left;
        if (FramesRunning < node->Frame)
            break;
        CThing* creature = PCreature.GetPItem();
        if (creature->m_presentBits[0x42 >> 5] & (1UL << (0x42 & 31))) {
            CTCEntityEvents* events = (CTCEntityEvents*)creature->m_interfaces.FindInline(0x42);
            events->AddEvent(new CEventCutScene(CutSceneHeader->Left->Name));
        }
        EraseCutScene(CutSceneHeader->Left);
    }
    if (TotalTime >= 0 && FramesRunning > TotalTime)
        FramesRunning = TotalTime;
}
