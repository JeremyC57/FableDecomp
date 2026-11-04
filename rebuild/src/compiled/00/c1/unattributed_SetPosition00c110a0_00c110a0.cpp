// CPhysicalPrimitiveMesh_00c110a0::SetPosition(const C3DVector*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the vector argument is copied to a frame local (y and z through the x87 stack, x as a dword) and passed by reference to the out-of-line update (@ 0x00C10F40)
// Leaf: ?SetPosition@CPhysicalPrimitiveMesh_00c110a0@@QAEXPBUC3DVector_00c110a0@@@Z
// Vendor naming V2 (plan phase 1, header-first): PKCProtectionInstance / PKCCriticalSection come from the shared libpk/PKCProtectionInstance.h (was a local copy).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C110A0, oracle length 120 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,14h ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov eax,[arg] ; fld dword ptr [eax+8] ; lea ecx,[esp+0Ch] ; fld dword ptr [eax+4] ; mov eax,[eax] ; fstp dword ptr [esp+10h] ; push ecx ; mov ecx,esi ; fstp dword ptr [esp+18h] ; mov dword ptr [esp+24h],0 ; mov [esp+10h],eax ; call Update ; lea ecx,[esp+4] ; mov dword ptr [esp+20h],-1 ; call ~PKCProtectionInstance ; ...
// The copy is a three-float constructor (`C3DVector(x, y, z)`) evaluated from the argument's
// components. The manifest label is propagated; the class is named by address.
#include "libpk/PKCProtectionInstance.h"
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14


struct C3DVector_00c110a0 {
    float x, y, z;
    C3DVector_00c110a0(float X, float Y, float Z) : x(X), y(Y), z(Z) { }
};

struct CPhysicalPrimitiveMesh_00c110a0 {
    void Update(const C3DVector_00c110a0& position);   // 0x00C10F40
    void SetPosition(const C3DVector_00c110a0* position);
};

void CPhysicalPrimitiveMesh_00c110a0::SetPosition(const C3DVector_00c110a0* position)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    C3DVector_00c110a0 local(position->x, position->y, position->z);
    Update(local);
}