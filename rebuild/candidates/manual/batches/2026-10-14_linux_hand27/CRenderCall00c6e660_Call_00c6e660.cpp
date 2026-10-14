// CRenderCall_00c6e660::Call() -- call virtual slot 4 on `this` with four zero arguments, the address of a zeroed two-word record, a zero and a one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6E660, length 45 bytes.
// Callees: virtual slot 4 of `this` (seven arguments; the vtable is read before the pushes).
// Shape: `sub esp,8` is the 8-byte local record, its address is pushed as the fifth argument (`lea edx,[esp+8]` after
// three pushes) and its two words are zeroed only after all seven pushes (the scheduler sinks the stores next to
// the call); the trailing `add esp,8` frees the local -- the callee is __thiscall/__stdcall and pops its arguments.
// The manifest label "CDataOutputStream::WriteCBYTE" is propagated from a same-shape body (WriteCBYTE takes a
// byte argument; this body takes none), so the class and record are named by address and shape.
struct SZeroPair_00c6e660 {
    long m_first;
    long m_second;
};

struct CRenderCall_00c6e660 {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Submit(long a, long b, long c, long d, SZeroPair_00c6e660* record, long e, long f);   // slot 4
    void Call();
};

void CRenderCall_00c6e660::Call()
{
    SZeroPair_00c6e660 record;
    record.m_first = 0;
    record.m_second = 0;
    Submit(0, 0, 0, 0, &record, 0, 1);
}
