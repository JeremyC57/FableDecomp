// CActivationReceptor_00c38b20::~CActivationReceptor() -- EH-framed destructor of a vptr-less receptor (the manifest's "CTCActivationReceptor_Update" is propagated): drives the +0xf8 state machine down -- a queued receptor (1) is detached from the +0xfc registry (@ 0x00C48560 with the +0xd8 entry) and stopped (@ 0x00C38AF0) when the query @ 0x00C38A40 or its flag says so, an acknowledged one (3) hands its current item (@ 0x00C38AC0, +8 cleared) to the +0x16c sink's slot 0, a pending one (2) is detached, a stopping one (4) stops and deletes the sink (virtual destructor at slot 6), an idle one (0) unregisters the +0xc entry (@ 0x00C48430) and a finished one (5) runs the final step (@ 0x00C38B10); the compiler then destroys the +0x138, +0xfc, +0xcc, +0x94, +0x60 and +0x2c members (states 6..1) and the base (@ 0x00C39BA0; state 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C38B20, oracle length 423 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,8 ; push esi ; mov esi,ecx ; mov [esp+8],esi ; cmp dword ptr [esi+0F8h],1 ; mov dword ptr [esp+14h],6 ; jne s3 ; push 0 ; lea eax,[esp+0Bh] ; push eax ; call CActivationReceptor::Query ; test al,al ; jne stop ; mov al,[esp+7] ; test al,al ; je idle ; stop: lea ecx,[esi+0D8h] ; push ecx ; lea ecx,[esi+0FCh] ; call CRegistry::Detach ; mov ecx,esi ; call CActivationReceptor::Stop ; idle: mov dword ptr [esi+0F8h],0 ; s3: cmp dword ptr [esi+0F8h],3 ; jne s2 ; mov ecx,esi ; call CActivationReceptor::Current ; mov dword ptr [eax+8],0 ; mov ecx,[esi+16Ch] ; mov edx,[ecx] ; push eax ; call [edx] ; mov dword ptr [esi+0F8h],2 ; s2: cmp dword ptr [esi+0F8h],2 ; jne s4 ; lea eax,[esi+0D8h] ; push eax ; lea ecx,[esi+0FCh] ; call CRegistry::Detach ; mov dword ptr [esi+0F8h],4 ; s4: cmp dword ptr [esi+0F8h],4 ; jne s0 ; mov ecx,esi ; call CActivationReceptor::Stop ; mov ecx,[esi+16Ch] ; test ecx,ecx ; je a ; mov edx,[ecx] ; push 1 ; call [edx+18h] ; a: mov dword ptr [esi+16Ch],0 ; mov dword ptr [esi+0F8h],0 ; s0: mov eax,[esi+0F8h] ; test eax,eax ; jne s5 ; lea eax,[esi+0Ch] ; push eax ; lea ecx,[esi+0FCh] ; call CRegistry::Unregister ; mov dword ptr [esi+0F8h],5 ; s5: cmp dword ptr [esi+0F8h],5 ; jne tail ; mov ecx,esi ; call CActivationReceptor::Final ; mov dword ptr [esi+0F8h],6 ; tail: lea ecx,[esi+138h] ; mov byte ptr [esp+14h],5 ; call ~CPart138 ; lea ecx,[esi+0FCh] ; mov byte ptr [esp+14h],4 ; call ~CRegistry ; lea ecx,[esi+0CCh] ; mov byte ptr [esp+14h],3 ; call ~CPartCC ; lea ecx,[esi+94h] ; mov byte ptr [esp+14h],2 ; call ~CPart94 ; lea ecx,[esi+60h] ; mov byte ptr [esp+14h],1 ; call ~CPart60 ; lea ecx,[esi+2Ch] ; mov byte ptr [esp+14h],0 ; call ~CPart2C ; mov ecx,esi ; mov dword ptr [esp+14h],-1 ; call ~CReceptorBase ; ... ret
// The classes are named by address.
struct ISink_00c38b20 {
    virtual void Take(void* item);         // slot 0
    virtual void Slot04(); virtual void Slot08(); virtual void Slot0C(); virtual void Slot10(); virtual void Slot14();
    virtual ~ISink_00c38b20();             // slot 0x18
};
struct SItem_00c38b20 { long m_0; long m_4; long m_8; };

struct CPart2C_00c38b20 { unsigned char _body[0x34]; ~CPart2C_00c38b20(); };   // 0x00C39BF0
struct CPart60_00c38b20 { unsigned char _body[0x34]; ~CPart60_00c38b20(); };   // 0x00C39C40
struct CPart94_00c38b20 { unsigned char _body[0x38]; ~CPart94_00c38b20(); };   // 0x00C39C90
struct CPartCC_00c38b20 { unsigned char _body[0xc]; ~CPartCC_00c38b20(); };    // 0x00C39CE0
struct CRegistry_00c38b20 {                // +0xfc
    unsigned char _body[0x3c];
    void Detach(void* entry);              // 0x00C48560
    void Unregister(void* entry);          // 0x00C48430
    ~CRegistry_00c38b20();                 // 0x00C48290
};
struct CPart138_00c38b20 { unsigned char _body[0x34]; ~CPart138_00c38b20(); };   // 0x00C3A7A0

struct CReceptorBase_00c38b20 {            // 0x2c bytes, no vptr
    unsigned char _0[0xc];
    unsigned char m_entry[0x20];           // +0xc
    ~CReceptorBase_00c38b20();             // 0x00C39BA0
};

struct CActivationReceptor_00c38b20 : CReceptorBase_00c38b20 {
    CPart2C_00c38b20 m_2c;
    CPart60_00c38b20 m_60;
    CPart94_00c38b20 m_94;
    CPartCC_00c38b20 m_cc;
    unsigned char m_d8[0x20];              // +0xd8
    long m_state;                          // +0xf8
    CRegistry_00c38b20 m_registry;         // +0xfc
    CPart138_00c38b20 m_138;
    ISink_00c38b20* m_pSink;               // +0x16c
    bool Query(bool* flag, long a);        // 0x00C38A40
    SItem_00c38b20* Current();             // 0x00C38AC0
    void Stop();                           // 0x00C38AF0
    void Final();                          // 0x00C38B10
    ~CActivationReceptor_00c38b20();
};

CActivationReceptor_00c38b20::~CActivationReceptor_00c38b20()
{
    if (m_state == 1) {
        bool flag;
        if (Query(&flag, 0) || flag) {
            m_registry.Detach(m_d8);
            Stop();
        }
        m_state = 0;
    }
    if (m_state == 3) {
        SItem_00c38b20* item = Current();
        item->m_8 = 0;
        m_pSink->Take(item);
        m_state = 2;
    }
    if (m_state == 2) {
        m_registry.Detach(m_d8);
        m_state = 4;
    }
    if (m_state == 4) {
        Stop();
        if (m_pSink)
            delete m_pSink;
        m_pSink = 0;
        m_state = 0;
    }
    if (m_state == 0) {
        m_registry.Unregister(m_entry);
        m_state = 5;
    }
    if (m_state == 5) {
        Final();
        m_state = 6;
    }
}