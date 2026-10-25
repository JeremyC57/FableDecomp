// CAudioObject::StopVoice(unsigned __int64 time) -- forward a stop request with its 64-bit time to the audio object's voice (+0x50, virtual slot 12): the stop path the stop API 0x00C35C50 takes when the object carries a loop-point (+0x2c) or volume-ramp (+0x30) record
// Leaf: ?StopVoice@CAudioObject@@QAEX_K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C43370, 21 bytes (+ int3 padding), __thiscall `ret 8`.
// Retail bytes:  mov edx,[esp+8] ; mov ecx,[ecx+50h] ; mov eax,[ecx] ; push edx ; mov edx,[esp+8] ; push edx ; call [eax+30h] ; ret 8
// Reconstruction (sweep1, 2026-10-25; FINDINGS chunk 439: re-author the landed inline-assembly body as C++): the landed row
//   was a bare-function inline-assembly thunk because VC7.1 turns a forward of two 4-byte arguments into `jmp [eax+30h]`. The
//   re-push plus `call ; ret 8` is what VC7.1 emits when the forwarded argument is ONE 8-byte by-value value (an
//   `unsigned __int64` or an 8-byte struct): it does not sibling-call with by-value 8-byte arguments (checked on the lane:
//   `long`/`bool`/`float` pairs all become `jmp`, a by-value `unsigned __int64` or 8-byte struct gives these exact bytes).
//   The audio object's times are 64-bit (CircularQueue m_times, CAudioObject::PlaySource's start time), so the argument is a
//   64-bit time; both API callers (0x00C27A03 through 0x00C35C50, and 0x00C35C30's sibling) pass `push 0 ; push 0`.
// Identity: `this` is the libpk audio object (agent439's CAudioObject: voice +0x50). Its only caller is the `__stdcall` stop
//   API 0x00C35C50, which calls CAudioObject::Stop 0x00C43980 (voice slot 11) when the loop (+0x2c) and volume (+0x30) records
//   are both clear and this function (voice slot 12) otherwise. Descriptive name (no PDB covers libpk); the landed
//   CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation is a propagated manifest label.
class CVoice {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();                             // slot 11: Stop (two words, bool; agent439's 0x00C43980)
    virtual void StopAt(unsigned __int64 time);     // slot 12 (+0x30)
};

class CAudioObject {
public:
    void*         m_vptr;                           // +0x00 (vtable 0x012B5A88)
    unsigned char m_pad04[0x50 - 4];
    CVoice*       m_pVoice;                         // +0x50
    void StopVoice(unsigned __int64 time);
};

void CAudioObject::StopVoice(unsigned __int64 time)
{
    m_pVoice->StopAt(time);
}
