// WaypointData_00c2eed0::WaypointData_00c2eed0() -- EH-framed constructor of three 0x10-byte container members built out of line (@ 0x00C304F0 at +0, 0x00C305E0 at +0x10, 0x00C306D0 at +0x20; EH states 0 and 1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2EED0, oracle length 81 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; call CFirst::CFirst ; lea ecx,[esi+10h] ; mov dword ptr [esp+10h],0 ; call CSecond::CSecond ; lea ecx,[esi+20h] ; mov byte ptr [esp+10h],1 ; call CThird::CThird ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// Three members with out-of-line constructors and destructors: the first on `this` (state 0 once
// it exists), the second at +0x10 (state 1, stored as a byte because the slot's upper bytes are
// known to be zero), the third at +0x20. The three constructors are the typed-container
// constructors of `2026-10-14_linux_i1130`. The manifest label is propagated; the class is named by address.
struct CFirst_00c2eed0 { unsigned char _body[0x10]; CFirst_00c2eed0(); ~CFirst_00c2eed0(); };     // 0x00C304F0
struct CSecond_00c2eed0 { unsigned char _body[0x10]; CSecond_00c2eed0(); ~CSecond_00c2eed0(); };  // 0x00C305E0
struct CThird_00c2eed0 { unsigned char _body[0x10]; CThird_00c2eed0(); ~CThird_00c2eed0(); };     // 0x00C306D0

struct WaypointData_00c2eed0 {
    CFirst_00c2eed0 m_first;               // +0
    CSecond_00c2eed0 m_second;             // +0x10
    CThird_00c2eed0 m_third;               // +0x20
    WaypointData_00c2eed0();
};

WaypointData_00c2eed0::WaypointData_00c2eed0()
    : m_first(), m_second(), m_third()
{
}