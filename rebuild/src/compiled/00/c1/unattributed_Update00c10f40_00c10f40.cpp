// CPhysicalPrimitiveMesh_00c10f40::Update(const C3DVector&) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, when the position at +8 differs from the argument (`__fastcall` compare @ 0x00C256F0) it is assigned (implicit memberwise assignment, three dword moves), the owner at +0x7bc is told to update from the three vectors at +8, +0x14 and +0x20 (@ 0x00C1C280) and bit 2 of the flags at +0x7c8 is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C10F40, oracle length 152 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push ebx ; push esi ; push edi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+10h] ; call PKCProtectionInstance::PKCProtectionInstance ; mov ebx,[position] ; lea edi,[esi+8] ; mov edx,ebx ; mov ecx,edi ; mov dword ptr [esp+1Ch],0 ; call VectorsEqual ; test al,al ; jne done ; mov ecx,[ebx] ; mov eax,edi ; mov [eax],ecx ; mov edx,[ebx+4] ; mov [eax+4],edx ; mov ecx,[ebx+8] ; mov [eax+8],ecx ; mov ecx,[esi+7BCh] ; lea edx,[esi+20h] ; push edx ; lea eax,[esi+14h] ; push eax ; push edi ; call COwner::Update ; or dword ptr [esi+7C8h],4 ; done: lea ecx,[esp+0Ch] ; mov dword ptr [esp+1Ch],-1 ; call ~PKCProtectionInstance ; ...
// The callee of the position setter landed at 0x00C110A0. The vector class has a user copy
// constructor (its implicit assignment copies through one base register). The manifest label
// is propagated; the class is named by address.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct C3DVector_00c10f40 {
    float x, y, z;
    C3DVector_00c10f40() { }
    C3DVector_00c10f40(const C3DVector_00c10f40& other) : x(other.x), y(other.y), z(other.z) { }
};

bool __fastcall VectorsEqual_00c256f0(const C3DVector_00c10f40* a, const C3DVector_00c10f40* b);   // 0x00C256F0

struct COwner_00c10f40 {
    void Update(const C3DVector_00c10f40* position, const C3DVector_00c10f40* a, const C3DVector_00c10f40* b);   // 0x00C1C280
};

struct CPhysicalPrimitiveMesh_00c10f40 {
    unsigned char _pad[8];
    C3DVector_00c10f40 m_position;         // +8
    C3DVector_00c10f40 m_14;
    C3DVector_00c10f40 m_20;
    unsigned char _pad2[0x790];
    COwner_00c10f40* m_pOwner;             // +0x7bc
    unsigned char _pad3[8];
    unsigned long m_flags;                 // +0x7c8
    void Update(const C3DVector_00c10f40& position);
};

void CPhysicalPrimitiveMesh_00c10f40::Update(const C3DVector_00c10f40& position)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    if (!VectorsEqual_00c256f0(&m_position, &position)) {
        m_position = position;
        m_pOwner->Update(&m_position, &m_14, &m_20);
        m_flags |= 4;
    }
}