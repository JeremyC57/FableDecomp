// CCombatActionBase::Clone() -- EH-framed `new CCombatActionBase(*this)` (0x44 bytes through the global operator new @ 0x00BFEA1A, copy constructor @ 0x00C2CBD0); null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C14C40, oracle length 93 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; push 44h ; mov esi,ecx ; call operator new ; add esp,4 ; mov [esp+4],eax ; test eax,eax ; mov dword ptr [esp+10h],0 ; je null ; push esi ; mov ecx,eax ; call CCombatActionBase::CCombatActionBase(const CCombatActionBase&) ; pop esi ; mov ecx,[esp+4] ; mov fs:[0],ecx ; add esp,10h ; ret ; null: mov ecx,[esp+8] ; xor eax,eax ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The block is kept in the frame because the copy constructor may throw (EH state 0; the funclet
// frees it through operator delete); the null path has its own epilogue. The class comes from the
// manifest label; the copy constructor is out of line.
struct CCombatActionBase {
    unsigned char _body[0x44];
    CCombatActionBase(const CCombatActionBase& other);   // 0x00C2CBD0
    CCombatActionBase* Clone();
};

CCombatActionBase* CCombatActionBase::Clone()
{
    return new CCombatActionBase(*this);
}