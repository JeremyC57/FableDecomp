// CCreatureActionBase::InitialiseActionEvents() -- unless the global system says not to, for each embedded event name of
// Grade: DIFFER(455v455) -- counted-event release: retail loads the count block into edi and tests the dec flags (dec [edi]; jne); ours loads eax, copies to edi and re-compares [edi] with the zero register (then the loop-alignment padding differs); everything else identical
// the action's animation look up the registered action-event factory by the name's first word, create the event at
// (anim delay + frames-per-second * event time) for the creature and keep it in ActionEvents when it accepts the name
// Leaf: InitialiseActionEvents
// Tried: direct GetPItem (a), inline GetCreature + cached count (b), // Layout: CCreatureActionBase PCreature +0x08, ActionEvents +0x84// Layout: CCreatureActionBase PCreature +0x08, ActionEvents +0x84 release + _Destroy(first,last) + inline GetPart (c),
//   forceinline Release() (d, this).
// Layout: CCreatureActionBase PCreature +0x08, ActionEvents +0x84 (vector<CCountedPointer<CActionEvent>>).
extern "C" void __cdecl free(void* block);            // 0x00BFEA14
void __cdecl operator delete(void* block);            // 0x00BFE9BC
struct CCharStringData { char* PData; unsigned long StringLength; };
struct CCharString {
    CCharStringData* PStringData;
    ~CCharString();                                   // 0x0099EAE0
    long FindCharPos(char c) const;                   // 0x0099E5A0
    CCharString GetSubString(long start, long length) const;   // 0x0099EC70
    __forceinline long Length() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
};
struct CStringVector {                                // STLport vector<CCharString>
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    CStringVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    static __forceinline void _Destroy(CCharString* first, CCharString* last)
    {
        for (; first != last; ++first)
            first->~CCharString();
    }
    __forceinline ~CStringVector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};
struct CAnimationManager {
    void GetEventNames(long animation, CStringVector& names);           // 0x006F9B20
    float GetEventTimeInSeconds(long animation, const CCharString& name);   // 0x006F9790
};
CAnimationManager* GetAnimationManager();             // 0x006F90B0
extern long g_FramesPerSecond;                        // 0x01375550
struct CGlobalSystem {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool IsSuppressingActionEvents();         // slot 9
};
extern CGlobalSystem* g_pGlobalSystem;                // 0x013B86A0
struct CWorldPart {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void GetValue(long* out);                 // slot 9
};
struct CDisplayEngine {
    unsigned char _pad[0x34];
    CWorldPart* Part;                                 // +0x34
    CWorldPart* GetPart() const { return Part; }
};
struct CThing {
    CDisplayEngine* GetDisplayEngine() const;         // 0x004C7AA0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                         // 0x00A01B50
};
struct CActionEvent {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual bool Accepts(const CCharString& name);    // slot 4
};
struct CActionEventFactory {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CActionEvent* Create(long frame, CThing* creature, long value);   // slot 3
};
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
    __forceinline bool Release() { return --m_references == 0; }
};
struct CCountedPointerActionEvent {
    CActionEvent* m_pObject;
    SCountBlock*  m_pCount;
    CCountedPointerActionEvent() : m_pObject(0), m_pCount(0) { }
    __forceinline ~CCountedPointerActionEvent()
    {
        SCountBlock* count = m_pCount;
        if (count && count->Release()) {
            count->m_pfnDestroy(count->m_pObject);
            delete count;
        }
        m_pObject = 0;
        m_pCount = 0;
    }
    void Reset(CActionEvent* object);                 // 0x00695A00
    CActionEvent* operator->() const { return m_pObject; }
};
struct CActionEventVector {
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    void push_back(const CCountedPointerActionEvent& value);   // 0x00696FE0
};
struct SActionEventNode {
    unsigned char _pad[0x10];
    CCharString Key;                                  // +0x10
    CActionEventFactory* Factory;                     // +0x14
};
struct CActionEventFactoryMap {
    SActionEventNode* Header;                         // end()
    long Count;
    SActionEventNode* Find(const CCharString& key);   // 0x00696340
};
extern CActionEventFactoryMap g_ActionEventFactories; // 0x013BA8BC

struct CCreatureActionBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual long GetAnimDelayTime() const;            // slot 6
    virtual long GetAnimationIndex() const;           // slot 7
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0x84 - 0x10];
    CActionEventVector ActionEvents;                  // +0x84
    CThing* GetCreature() const { return PCreature.GetPItem(); }
    void InitialiseActionEvents();
};

void CCreatureActionBase::InitialiseActionEvents()
{
    if (g_pGlobalSystem->IsSuppressingActionEvents())
        return;
    long value;
    PCreature.GetPItem()->GetDisplayEngine()->GetPart()->GetValue(&value);
    long animation = GetAnimationIndex();
    if (animation <= 0)
        return;
    long framesPerSecond = g_FramesPerSecond;
    CAnimationManager* manager = GetAnimationManager();
    CStringVector names;
    manager->GetEventNames(animation, names);
    for (CCharString* it = names._M_start; it != names._M_finish; ++it) {
        long end = it->FindCharPos(' ');
        if (end == -1)
            end = it->Length();
        CCharString prefix = it->GetSubString(0, end);
        SActionEventNode* node = g_ActionEventFactories.Find(prefix);
        if (node != g_ActionEventFactories.Header) {
            float seconds = manager->GetEventTimeInSeconds(animation, *it);
            long frame = (long)(GetAnimDelayTime() + framesPerSecond * seconds);
            CActionEventFactory* factory = node->Factory;
            CCountedPointerActionEvent event;
            event.Reset(factory->Create(frame, GetCreature(), value));
            if (event->Accepts(*it))
                ActionEvents.push_back(event);
        }
    }
}
