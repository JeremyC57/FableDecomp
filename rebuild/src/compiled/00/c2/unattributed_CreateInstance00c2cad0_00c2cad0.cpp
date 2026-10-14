// CCombat_00c2cad0::CreateInstance(void*, void*, const C3DVector*, void*, void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the position is copied to a stack vector (memberwise copy constructor), the instance counter at +0x88c of the stats object (owner at +4, its +4, its +0x38) is post-incremented, and the factory at +0xc of the owner creates the instance from `this`, the old counter, a, b, the vector and d, e (@ 0x00C2BB10); the result survives the lock's destructor in esi
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2CAD0, oracle length 174 bytes. Built under /EHsc.
// Retail bytes:  ... mov eax,[position] ; mov ecx,[eax] ; mov edx,[eax+4] ; mov eax,[eax+8] ; mov [esp+0Ch],ecx ; mov ecx,[esi+4] ; mov [esp+10h],edx ; mov [esp+14h],eax ; mov edx,[ecx+4] ; mov eax,[edx+38h] ; mov ecx,[eax+88Ch] ; lea edx,[ecx+1] ; mov [eax+88Ch],edx ; mov eax,[e] ; mov edx,[d] ; push eax ; push edx ; mov edx,[c] ; lea eax,[esp+14h] ; push eax ; mov eax,[b] ; push edx ; push eax ; push ecx ; mov ecx,[esi+4] ; mov ecx,[ecx+0Ch] ; push esi ; mov dword ptr [esp+3Ch],0 ; call CFactory::Create ; ...
// The manifest label is propagated; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct C3DVector_00c2cad0 {
    float x, y, z;
    C3DVector_00c2cad0(const C3DVector_00c2cad0& other) : x(other.x), y(other.y), z(other.z) { }
};

struct CCombat_00c2cad0;
struct CFactory_00c2cad0 {
    void* Create(CCombat_00c2cad0* combat, long id, void* a, void* b, const C3DVector_00c2cad0* position, void* d, void* e);   // 0x00C2BB10
};

struct CStats_00c2cad0 {
    unsigned char _pad[0x88c];
    long m_nextId;                         // +0x88c
};

struct CWorld_00c2cad0 {
    unsigned char _pad[0x38];
    CStats_00c2cad0* m_pStats;             // +0x38
};

struct COwner_00c2cad0 {
    unsigned char _pad[4];
    CWorld_00c2cad0* m_pWorld;             // +4
    unsigned char _pad2[4];
    CFactory_00c2cad0* m_pFactory;         // +0xc
};

struct CCombat_00c2cad0 {
    unsigned char _pad[4];
    COwner_00c2cad0* m_pOwner;             // +4
    void* CreateInstance(void* a, void* b, const C3DVector_00c2cad0* position, void* d, void* e);
};

void* CCombat_00c2cad0::CreateInstance(void* a, void* b, const C3DVector_00c2cad0* position, void* d, void* e)
{
    CScopedLock lock(&g_lock_0143bb14);
    C3DVector_00c2cad0 local(*position);
    long id = m_pOwner->m_pWorld->m_pStats->m_nextId++;
    return m_pOwner->m_pFactory->Create(this, id, a, b, &local, d, e);
}