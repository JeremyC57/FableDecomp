// SPair_00c20150::~SPair_00c20150() -- EH-framed pair destructor: the CDebugStream second at +0x18 (@ 0x00C0BA40, state 0), then the first's 0x10-byte member at +8 (@ 0x00C34B80, state 1) and the first's polymorphic base (vtable 0x012B3420) with its empty inline destructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C20150, oracle length 83 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; lea ecx,[esi+18h] ; mov dword ptr [esp+10h],0 ; call ~CDebugStream ; lea ecx,[esi+8] ; mov dword ptr [esp+10h],1 ; call ~CMember8 ; mov ecx,[esp+8] ; mov dword ptr [esi],offset CBase::`vftable' ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// Destructor of a map value pair whose first member is a polymorphic 0x18-byte class (vptr, a
// pointer, a 0x10-byte member destroyed out of line @ 0x00C34B80) and whose second is the
// shared-buffer CDebugStream (@ 0x00C0BA40): members go in reverse order under EH states 0
// and 1, and the first's empty inline base destructor restores the base vtable last; `this` is
// kept in the frame for the unwind funclets. Built under /EHsc (the `__ehhandler` is not
// compared; the funclets land through the funclet pass). The manifest label
// (`_Dest_val<allocator<pair<...>>>`) is propagated; the types are modelled by layout. One of
// 3 bodies of this shape.
struct CMember8 {                          // +8, 0x10 bytes, destroyed out of line
    unsigned char _body[0x10];
    ~CMember8();                           // 0x00C34B80
};

struct CDebugStream {                      // the shared-buffer stream
    char* m_pBuffer;
    ~CDebugStream();                       // 0x00C0BA40
};

struct CBase {                             // vtable 0x012B3420
    virtual ~CBase() { }
};

struct CFirst : CBase {                    // 0x18 bytes: vptr, a pointer, the member
    void* m_4;
    CMember8 m_member8;                    // +8
};

struct SPair_00c20150 {
    CFirst first;                          // +0
    CDebugStream second;                   // +0x18
    ~SPair_00c20150();
};

SPair_00c20150::~SPair_00c20150()
{
}
