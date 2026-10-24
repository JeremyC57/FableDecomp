// CPhysicalPrimitiveMesh_00c10fe0::SetFrame(const C3DVector*, const C3DVector*, const C3DVector*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the three vector arguments are copied to frame locals (three-float constructors: x as a dword, y and z through the x87 stack) and passed by reference to the out-of-line update (@ 0x00C10E50)
// Leaf: CPersistInfo>
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C10FE0, oracle length 178 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,2Ch ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call CScopedLock::CScopedLock ; mov eax,[a] ; fld dword ptr [eax+8] ; mov dword ptr [esp+38h],0 ; fld dword ptr [eax+4] ; mov eax,[eax] ; fstp dword ptr [esp+28h] ; mov [esp+24h],eax ; mov eax,[b] ; mov ecx,[eax] ; fstp dword ptr [esp+2Ch] ; fld dword ptr [eax+8] ; mov [esp+18h],ecx ; fld dword ptr [eax+4] ; mov eax,[c] ; mov edx,[eax] ; fstp dword ptr [esp+1Ch] ; lea ecx,[esp+18h] ; fstp dword ptr [esp+20h] ; mov [esp+0Ch],edx ; fld dword ptr [eax+8] ; lea edx,[esp+24h] ; fld dword ptr [eax+4] ; lea eax,[esp+0Ch] ; push eax ; fstp dword ptr [esp+14h] ; push ecx ; push edx ; fstp dword ptr [esp+20h] ; mov ecx,esi ; call Update ; ...
// The three-argument sibling of the position setter landed at 0x00C110A0. The manifest label
// is propagated; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct C3DVector_00c10fe0 {
    float x, y, z;
    C3DVector_00c10fe0(float X, float Y, float Z) : x(X), y(Y), z(Z) { }
};

struct CPhysicalPrimitiveMesh_00c10fe0 {
    void Update(const C3DVector_00c10fe0& a, const C3DVector_00c10fe0& b, const C3DVector_00c10fe0& c);   // 0x00C10E50
    void SetFrame(const C3DVector_00c10fe0* a, const C3DVector_00c10fe0* b, const C3DVector_00c10fe0* c);
};

void CPhysicalPrimitiveMesh_00c10fe0::SetFrame(const C3DVector_00c10fe0* a, const C3DVector_00c10fe0* b, const C3DVector_00c10fe0* c)
{
    CScopedLock lock(&g_lock_0143bb14);
    C3DVector_00c10fe0 first(a->x, a->y, a->z);
    C3DVector_00c10fe0 second(b->x, b->y, b->z);
    C3DVector_00c10fe0 third(c->x, c->y, c->z);
    Update(first, second, third);
}