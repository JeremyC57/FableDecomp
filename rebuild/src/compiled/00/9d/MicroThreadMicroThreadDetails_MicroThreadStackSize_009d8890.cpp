// MicroThread::MicroThreadDetails::MicroThreadStackSize() const -- bytes of saved stack this micro thread holds (0 while it runs)
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D8890, 18 bytes, plain `ret` (__fastcall, no arguments).
// Name: vtable 0x0129B938 slot 1; FableWin.pdb lists ?MicroThreadStackSize@MicroThreadDetails@MicroThread@@UBEIXZ.
// The vtable: slot 0 MarkAsDead (inherited from MicroThreadInfo, 0x009D8630 `mov byte [ecx+4],1`), slot 1 this,
// slot 2 the scalar deleting dtor 0x009D88B0. Ego_r.pdb: MicroThreadInfo { vptr; bool Dead +4 },
// MicroThreadDetails : MicroThreadInfo { vector<unsigned char> Stack +8 } (retail STLport vector, 12 bytes).
// Globals: the active micro thread 0x013CA894 (GetActiveMicroThread 0x009D8620 returns it; the switch code at
// 0x009D8818 stores it). The active thread's stack is the live one, so it holds no saved copy (`xor eax,eax`);
// otherwise Stack.size() = _M_finish - _M_start (+0x0c minus +0x08).
namespace MicroThread {
struct MicroThreadInfo {
    virtual void MarkAsDead();
    virtual unsigned int MicroThreadStackSize() const;
    virtual ~MicroThreadInfo();
    bool Dead;                                          // +0x04
};
extern MicroThreadInfo* g_pActiveMicroThread;          // 0x013CA894
struct ByteVector {                                     // STLport vector<unsigned char>
    unsigned char* _M_start;                            // +0x00
    unsigned char* _M_finish;                           // +0x04
    unsigned char* _M_end_of_storage;                   // +0x08
    unsigned int size() const { return _M_finish - _M_start; }
};
struct MicroThreadDetails : MicroThreadInfo {
    ByteVector Stack;                                   // +0x08
    virtual unsigned int MicroThreadStackSize() const;
};
unsigned int MicroThreadDetails::MicroThreadStackSize() const
{
    if (g_pActiveMicroThread == this)
        return 0;
    return Stack.size();
}
}