// CNavigationHost_00c13310::LoadLayer(void*, void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, a 0x50-byte navigation request is built on the stack by its inline constructor (a message-list owner at +0 @ 0x00C203A0, words zeroed, two 1.0f scales at +0x24/+0x28, a CNavigationLayer at +0x30 @ 0x00C20390, a short at +0x44), bound to the second argument (@ 0x00C27C10) and loaded through the host (@ 0x00C131C0) with the first; the result survives the unlock in esi
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C13310, oracle length 182 bytes. Built under /EHsc.
// Retail bytes:  ... xor esi,esi ; lea ecx,[esp+10h] ; mov [esp+68h],esi ; call CMessageListOwner::CMessageListOwner ; lea ecx,[esp+40h] ; mov [esp+2Ch],esi ; mov [esp+30h],esi ; mov dword ptr [esp+34h],3F800000h ; mov dword ptr [esp+38h],3F800000h ; mov [esp+3Ch],esi ; call CNavigationLayer::CNavigationLayer ; mov eax,[esp+74h] ; push eax ; lea ecx,[esp+14h] ; mov [esp+54h],esi ; mov [esp+58h],si ; mov [esp+5Ch],esi ; mov [esp+60h],esi ; call Bind ; mov edx,[esp+70h] ; lea ecx,[esp+10h] ; push ecx ; push edx ; mov ecx,edi ; call Load ; lea ecx,[esp+8] ; mov esi,eax ; mov dword ptr [esp+68h],-1 ; call ~PKCProtectionInstance ; ...
// The request has no destructor (the lock is the only EH state); its member stores are
// scheduled around the two member constructors. The manifest label is a description; the
// classes are named by address except the two PDB-named members.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CMessageListOwner_00c13310 {        // 0x1c bytes
    unsigned char _body[0x1c];
    CMessageListOwner_00c13310();          // 0x00C203A0
};

struct CNavigationLayer_00c13310 {         // 12 bytes
    long m_a, m_b, m_c;
    CNavigationLayer_00c13310();           // 0x00C20390
};

struct SNavigationRequest_00c13310 {       // 0x50 bytes, built on the stack
    CMessageListOwner_00c13310 m_owner;    // +0
    long m_1c;
    long m_20;
    float m_scaleX;                        // +0x24
    float m_scaleY;                        // +0x28
    long m_2c;
    CNavigationLayer_00c13310 m_layer;     // +0x30
    long m_3c;
    long m_40;
    short m_44;
    long m_48;
    long m_4c;
    SNavigationRequest_00c13310() : m_1c(0), m_20(0), m_scaleX(1.0f), m_scaleY(1.0f), m_2c(0), m_40(0), m_44(0), m_48(0), m_4c(0) { }
    void Bind(void* target);               // 0x00C27C10
};

struct CNavigationHost_00c13310 {
    long Load(void* source, SNavigationRequest_00c13310* request);   // 0x00C131C0
    long LoadLayer(void* source, void* target);
};

long CNavigationHost_00c13310::LoadLayer(void* source, void* target)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    SNavigationRequest_00c13310 request;
    request.Bind(target);
    return Load(source, &request);
}