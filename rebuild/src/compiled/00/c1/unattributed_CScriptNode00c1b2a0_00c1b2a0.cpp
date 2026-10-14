// CScriptNode_00c1b2a0::~CScriptNode_00c1b2a0() -- EH-framed destructor: the pair member at +0x14 destroyed inline (its second at +0x30 @ 0x00C29FB0 at state 1, its first at +0x14 @ 0x00C1C820 at state 0), then the out-of-line base destructor (@ 0x00C1AEA0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1B2A0, oracle length 104 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push edi ; mov [esp+8],esi ; lea edi,[esi+14h] ; mov dword ptr [esp+18h],0 ; mov [esp+0Ch],edi ; lea ecx,[edi+1Ch] ; mov byte ptr [esp+18h],1 ; call ~CSecond ; mov ecx,edi ; mov byte ptr [esp+18h],0 ; call ~CFirst ; mov ecx,esi ; mov dword ptr [esp+18h],-1 ; call ~CBase ; mov ecx,[esp+0Ch] ; pop edi ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret
// The pair's implicit destructor is inlined (the pair's address is kept in edi and in the frame
// for its funclets); EH state 0 marks the pair as a whole, 1 the pair with its first still alive.
// No vtable store: the base destructor is out of line and the class adds none. The manifest label
// is a propagated `_Dest_val` name; the classes are named by address.
struct CFirst_00c1b2a0 {                   // 0x1c bytes (a CCharString)
    unsigned char _body[0x1c];
    ~CFirst_00c1b2a0();                    // 0x00C1C820
};

struct CSecond_00c1b2a0 {
    unsigned char _body[4];
    ~CSecond_00c1b2a0();                   // 0x00C29FB0
};

struct SPair_00c1b2a0 {                    // implicit destructor, inlined into the owner
    CFirst_00c1b2a0 first;
    CSecond_00c1b2a0 second;
};

struct CScriptNodeBase_00c1b2a0 {
    unsigned char _body[0x14];
    ~CScriptNodeBase_00c1b2a0();           // 0x00C1AEA0
};

struct CScriptNode_00c1b2a0 : CScriptNodeBase_00c1b2a0 {
    SPair_00c1b2a0 m_value;                // +0x14
    ~CScriptNode_00c1b2a0();
};

CScriptNode_00c1b2a0::~CScriptNode_00c1b2a0()
{
}