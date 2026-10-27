// CScriptHost::Post() -- hand the host, as an event, to its owning event queue at +8 for processing
// Leaf: ?Post@CScriptHost@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0E970, 10 bytes, thiscall: `push ecx ; mov ecx,[ecx+8] ; call 0x00C08940 ; ret`.
// Identity (descriptive): virtual slot 0 of the script host vtable 0x012B25A8. The host is itself an event of the landed
//   CEventQueue_00c08940::Process 0x00C08940 (payload at +4 released through slot 3, then deleted through the slot-6
//   destructor), so slot 0 posts `this` to the queue it holds at +8. libpk has no PDB or RTTI.
struct CEvent_00c08940 {
    virtual void Post();                        // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual ~CEvent_00c08940();                 // slot 6
    void* m_pPayload;                           // +4
};

struct CEventQueue_00c08940 {
    void Process(CEvent_00c08940* event);       // 0x00C08940
};

struct CScriptHost : public CEvent_00c08940 {   // vtable 0x012B25A8
    CEventQueue_00c08940* m_pQueue;             // +8
    virtual void Post();
};

void CScriptHost::Post()
{
    m_pQueue->Process(this);
}