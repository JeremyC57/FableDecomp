// NUISystem::CMenuEntry::ProcessEvent(EEvent event) -- menu entry input: ignored while the front end is busy (or for event 25 when the front end blocks it) and inside the input delay; runs the CClickable handler when the def enables it, then events 2/5 select the entry and 3/4 unselect it unless the parent list is a type-8 list that keeps its selection with pending list entries
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053D200, 401 bytes + a 4-entry jump table (432 with padding). Vtable 0x01246604 slot 1:
//   the event receiver base sits at +4, so `this` is the full object + 4 and the primary virtuals go through `lea edi,[esi-4]`.
// Callees: CManager::GetInstance 0x0041E5F2 (its virtual slot 11 +0x2c, the busy byte +0xa4), GFGetTime 0x009E1BC0,
//   CClickable::ProcessEvent 0x0055AD60, primary slots 108 GetDef (CDefPointer by hidden pointer), 155 (+0x26c), 156 (+0x270);
//   the parent component's slots 65 (+0x104) and 108. Globals: 0x013B8AA8 (bool), 0x013B8AA0 (double time stamp),
//   0x013B8A98 (list sentinel pointer).
// Layout: local model in the landed CClickable::ProcessEvent 0x0055AD60 convention (retail = Ego_r.pdb - 0x30): Time +0x30,
//   InputDelay +0x18c, the entry's own time stamp +0x194, the parent component pointer +0xc8; def bytes +0x221 / +0x20a.
// Shape: the CClickable call is guarded by a bool read through a CDefPointer temporary (homed in the dead `event` slot); the
//   unselect test is one && chain stored in a named bool (homed in the dead `event` slot) whose GetDef temporary is conditionally
//   destroyed (flag word zeroed at entry; the forceinline destructor nulls the pointer); the parent is re-read through an inline
//   accessor after the GetType call (a plain `PParent` re-read loads eax and copies to ecx).
namespace NUISystem {

enum EEvent { EVENT_2 = 2, EVENT_3 = 3, EVENT_4 = 4, EVENT_5 = 5, EVENT_25 = 25 };

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                // slot 1
};
struct CComponentDef : CDefVirtual, CRefCounted {
    unsigned char _pad08[0x20a - 8];
    bool          KeepSelection;           // +0x20a (descriptive)
    unsigned char _pad20b[0x221 - 0x20b];
    bool          ClickableEnabled;        // +0x221 (descriptive)
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {
    CComponentDef* m_p;
    CDefPointer() { }
    __forceinline ~CDefPointer()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
    CComponentDef* operator->() const { return m_p; }
};
struct CComponentVirtuals {
    virtual void VSlot0();
    virtual void VSlot1();
    virtual void VSlot2();
    virtual void VSlot3();
    virtual void VSlot4();
    virtual void VSlot5();
    virtual void VSlot6();
    virtual void VSlot7();
    virtual void VSlot8();
    virtual void VSlot9();
    virtual void VSlot10();
    virtual void VSlot11();
    virtual void VSlot12();
    virtual void VSlot13();
    virtual void VSlot14();
    virtual void VSlot15();
    virtual void VSlot16();
    virtual void VSlot17();
    virtual void VSlot18();
    virtual void VSlot19();
    virtual void VSlot20();
    virtual void VSlot21();
    virtual void VSlot22();
    virtual void VSlot23();
    virtual void VSlot24();
    virtual void VSlot25();
    virtual void VSlot26();
    virtual void VSlot27();
    virtual void VSlot28();
    virtual void VSlot29();
    virtual void VSlot30();
    virtual void VSlot31();
    virtual void VSlot32();
    virtual void VSlot33();
    virtual void VSlot34();
    virtual void VSlot35();
    virtual void VSlot36();
    virtual void VSlot37();
    virtual void VSlot38();
    virtual void VSlot39();
    virtual void VSlot40();
    virtual void VSlot41();
    virtual void VSlot42();
    virtual void VSlot43();
    virtual void VSlot44();
    virtual void VSlot45();
    virtual void VSlot46();
    virtual void VSlot47();
    virtual void VSlot48();
    virtual void VSlot49();
    virtual void VSlot50();
    virtual void VSlot51();
    virtual void VSlot52();
    virtual void VSlot53();
    virtual void VSlot54();
    virtual void VSlot55();
    virtual void VSlot56();
    virtual void VSlot57();
    virtual void VSlot58();
    virtual void VSlot59();
    virtual void VSlot60();
    virtual void VSlot61();
    virtual void VSlot62();
    virtual void VSlot63();
    virtual void VSlot64();
    virtual long GetType();                          // slot 65 (+0x104)
    virtual void VSlot66();
    virtual void VSlot67();
    virtual void VSlot68();
    virtual void VSlot69();
    virtual void VSlot70();
    virtual void VSlot71();
    virtual void VSlot72();
    virtual void VSlot73();
    virtual void VSlot74();
    virtual void VSlot75();
    virtual void VSlot76();
    virtual void VSlot77();
    virtual void VSlot78();
    virtual void VSlot79();
    virtual void VSlot80();
    virtual void VSlot81();
    virtual void VSlot82();
    virtual void VSlot83();
    virtual void VSlot84();
    virtual void VSlot85();
    virtual void VSlot86();
    virtual void VSlot87();
    virtual void VSlot88();
    virtual void VSlot89();
    virtual void VSlot90();
    virtual void VSlot91();
    virtual void VSlot92();
    virtual void VSlot93();
    virtual void VSlot94();
    virtual void VSlot95();
    virtual void VSlot96();
    virtual void VSlot97();
    virtual void VSlot98();
    virtual void VSlot99();
    virtual void VSlot100();
    virtual void VSlot101();
    virtual void VSlot102();
    virtual void VSlot103();
    virtual void VSlot104();
    virtual void VSlot105();
    virtual void VSlot106();
    virtual void VSlot107();
    virtual CDefPointer GetDef();                    // slot 108 (+0x1b0)
    virtual void VSlot109();
    virtual void VSlot110();
    virtual void VSlot111();
    virtual void VSlot112();
    virtual void VSlot113();
    virtual void VSlot114();
    virtual void VSlot115();
    virtual void VSlot116();
    virtual void VSlot117();
    virtual void VSlot118();
    virtual void VSlot119();
    virtual void VSlot120();
    virtual void VSlot121();
    virtual void VSlot122();
    virtual void VSlot123();
    virtual void VSlot124();
    virtual void VSlot125();
    virtual void VSlot126();
    virtual void VSlot127();
    virtual void VSlot128();
    virtual void VSlot129();
    virtual void VSlot130();
    virtual void VSlot131();
    virtual void VSlot132();
    virtual void VSlot133();
    virtual void VSlot134();
    virtual void VSlot135();
    virtual void VSlot136();
    virtual void VSlot137();
    virtual void VSlot138();
    virtual void VSlot139();
    virtual void VSlot140();
    virtual void VSlot141();
    virtual void VSlot142();
    virtual void VSlot143();
    virtual void VSlot144();
    virtual void VSlot145();
    virtual void VSlot146();
    virtual void VSlot147();
    virtual void VSlot148();
    virtual void VSlot149();
    virtual void VSlot150();
    virtual void VSlot151();
    virtual void VSlot152();
    virtual void VSlot153();
    virtual void VSlot154();
    virtual void OnUnselected();                     // slot 155 (+0x26c) (descriptive)
    virtual void OnSelected();                       // slot 156 (+0x270) (descriptive)
};
struct CEventReceiver {
    virtual void VSlot0();
    virtual void ProcessEvent(EEvent event);
    virtual void VSlot2();
    unsigned char _data[0x10];
};
class CComponent : public CComponentVirtuals, public CEventReceiver {
public:
    unsigned char _pad18[0x18];
    float         Time;                    // +0x30
    unsigned char _pad34[0x94];
    CComponent*   PParent;                 // +0xc8 (descriptive)
    unsigned char _padcc[0x15c - 0xcc];
};
class CClickable : public CComponent {
public:
    unsigned char _pad15c[0x18c - 0x15c];
    float         InputDelay;              // +0x18c
    float         LastInputTime;           // +0x190
    virtual void ProcessEvent(EEvent event);    // 0x0055AD60
};
class CMenuEntry : public CClickable {
public:
    float         LastEventTime;           // +0x194 (descriptive)
    CComponent* GetParentList() const { return PParent; }   // inline accessor: its re-read lands straight in ecx
    virtual void ProcessEvent(EEvent event);
};
class CManager { public:
    virtual void VSlot0(); virtual void VSlot1(); virtual void VSlot2(); virtual void VSlot3(); virtual void VSlot4();
    virtual void VSlot5(); virtual void VSlot6(); virtual void VSlot7(); virtual void VSlot8(); virtual void VSlot9();
    virtual void VSlot10();
    virtual bool IsBlockingEvent25();      // slot 11 (+0x2c) (descriptive)
    unsigned char _pad04[0xa0];
    bool          Busy;                    // +0xa4 (descriptive)
    static CManager* __fastcall GetInstance();   // 0x0041E5F2
};
struct SListNode { SListNode* _M_next; SListNode* _M_prev; };
extern bool       g_IgnoreInputDelay;      // 0x013B8AA8
extern double     g_LastInputTime;         // 0x013B8AA0
extern SListNode* g_PendingList;           // 0x013B8A98
double __fastcall GFGetTime();             // 0x009E1BC0

void CMenuEntry::ProcessEvent(EEvent event)
{
    if (CManager::GetInstance()->IsBlockingEvent25() && event == EVENT_25)
        return;
    if (CManager::GetInstance()->Busy)
        return;
    if (event != EVENT_25 && !g_IgnoreInputDelay) {
        if (Time - LastEventTime < InputDelay)
            return;
        if (GFGetTime() - g_LastInputTime < InputDelay)
            return;
    }
    if (GetDef()->ClickableEnabled)
        CClickable::ProcessEvent(event);
    switch (event) {
    case EVENT_2:
    case EVENT_5:
        OnSelected();
        return;
    case EVENT_3:
    case EVENT_4:
        {
            bool keep = PParent && PParent->GetType() == 8 && GetParentList() && !GetParentList()->GetDef()->KeepSelection && g_PendingList->_M_next != g_PendingList;
            if (!keep)
                OnUnselected();
        }
        break;
    }
}

}