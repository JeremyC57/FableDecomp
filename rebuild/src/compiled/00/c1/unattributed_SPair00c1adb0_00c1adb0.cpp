// SPair_00c1adb0::~SPair_00c1adb0() -- EH-framed destructor of a pair<CCharString, CScriptInfo>-like node value: the second (+0x1c, @ 0x00C29FB0) then the first (+0, @ 0x00C1C820)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1ADB0, oracle length 76 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; lea ecx,[esi+1Ch] ; mov dword ptr [esp+10h],0 ; call ~CSecond ; mov ecx,esi ; mov dword ptr [esp+10h],-1 ; call ~CFirst ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The manifest label is Dinkumware's `_Dest_val<allocator<pair<CCharString,CScriptInfo>>>`, i.e. the
// out-of-line destructor of the map node value. Members are destroyed in reverse order: the second
// at +0x1c (EH state 0 while the first is still alive), then the first on `this` (state -1).
// Both member destructors are out of line; the user-declared pair destructor is the landed unit.
struct CFirst_00c1adb0 {                   // 0x1c bytes (a CCharString)
    unsigned char _body[0x1c];
    ~CFirst_00c1adb0();                    // 0x00C1C820
};

struct CSecond_00c1adb0 {                  // the mapped value (a CScriptInfo)
    unsigned char _body[4];
    ~CSecond_00c1adb0();                   // 0x00C29FB0
};

struct SPair_00c1adb0 {
    CFirst_00c1adb0 first;                 // +0
    CSecond_00c1adb0 second;               // +0x1c
    ~SPair_00c1adb0();
};

SPair_00c1adb0::~SPair_00c1adb0()
{
}