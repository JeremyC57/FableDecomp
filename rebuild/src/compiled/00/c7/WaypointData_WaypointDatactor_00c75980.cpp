// WaypointData::WaypointData() -- EH-framed constructor: out-of-line chunk base constructor (@ 0x00C74420), own vtable (0x012BF080), the three 0x14-byte elements at +4 built through `__ehvec_ctor` (@ 0x00C69660 / @ 0x00C69680, state 1 after them), the two 0x28-byte elements at +0x40 built through `__ehvec_ctor` (@ 0x00C75530 / @ 0x00C69990), then the dword at +0x90 zeroed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C75980, oracle length 127 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; call CChunkBase::CChunkBase ; push offset ~CSmall ; push offset CSmall::CSmall ; push 3 ; push 14h ; lea eax,[esi+4] ; push eax ; mov dword ptr [esp+24h],0 ; mov dword ptr [esi],offset vtable ; call __ehvec_ctor ; push offset ~CLarge ; push offset CLarge::CLarge ; push 2 ; push 28h ; lea ecx,[esi+40h] ; push ecx ; mov byte ptr [esp+24h],1 ; call __ehvec_ctor ; mov ecx,[esp+8] ; mov dword ptr [esi+90h],0 ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The constructor counterpart of the destructor landed at 0x00C6A340 (same arrays, same base).
// Class arrays with constructors and destructors go through the `__ehvec_ctor` helper.
struct CChunkBase_00c75980 {
    virtual void v00();
    CChunkBase_00c75980();                 // 0x00C74420
    ~CChunkBase_00c75980();                // 0x00C74450
};

struct CSmall_00c75980 {                   // 0x14 bytes
    unsigned char _body[0x14];
    CSmall_00c75980();                     // 0x00C69660
    ~CSmall_00c75980();                    // 0x00C69680
};

struct CLarge_00c75980 {                   // 0x28 bytes
    unsigned char _body[0x28];
    CLarge_00c75980();                     // 0x00C75530
    ~CLarge_00c75980();                    // 0x00C69990
};

struct WaypointData : CChunkBase_00c75980 {   // vtable 0x012BF080
    CSmall_00c75980 m_small[3];            // +4
    CLarge_00c75980 m_large[2];            // +0x40
    long m_count;                          // +0x90
    WaypointData();
};

WaypointData::WaypointData()
    : m_count(0)
{
}