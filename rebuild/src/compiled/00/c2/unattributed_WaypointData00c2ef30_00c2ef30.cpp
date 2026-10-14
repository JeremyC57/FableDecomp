// WaypointData_00c2ef30::~WaypointData_00c2ef30() -- EH-framed destructor of the three 0x10-byte container members (destroyed in reverse: +0x20 @ 0x00C30730 at state 1, +0x10 @ 0x00C30640 at state 0, +0 @ 0x00C30550 at state -1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2EF30, oracle length 89 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; lea ecx,[esi+20h] ; mov dword ptr [esp+10h],1 ; call ~CThird ; lea ecx,[esi+10h] ; mov byte ptr [esp+10h],0 ; call ~CSecond ; mov ecx,esi ; mov dword ptr [esp+10h],-1 ; call ~CFirst ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The destructor counterpart of 0x00C2EED0: three members with out-of-line destructors (the
// typed-container destructors of `2026-10-14_linux_i1131`), destroyed last to first with the EH
// state counting down (the state-0 store narrowed to a byte). The manifest label is propagated;
// the class is named by address.
struct CFirst_00c2ef30 { unsigned char _body[0x10]; ~CFirst_00c2ef30(); };     // 0x00C30550
struct CSecond_00c2ef30 { unsigned char _body[0x10]; ~CSecond_00c2ef30(); };   // 0x00C30640
struct CThird_00c2ef30 { unsigned char _body[0x10]; ~CThird_00c2ef30(); };     // 0x00C30730

struct WaypointData_00c2ef30 {
    CFirst_00c2ef30 m_first;               // +0
    CSecond_00c2ef30 m_second;             // +0x10
    CThird_00c2ef30 m_third;               // +0x20
    ~WaypointData_00c2ef30();
};

WaypointData_00c2ef30::~WaypointData_00c2ef30()
{
}