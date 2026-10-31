// MicroThread::SwitchToMicroThread(MicroThreadInfo*) -- run a micro-thread until it yields: make it active, copy its saved stack image under the shared stack top (RestoreStack), MicroThreadSleep into it, then save the (possibly grown) image back (PreserveStack); true while the thread is still alive
// Leaf: ?SwitchToMicroThread@MicroThread@@YI_NPAVMicroThreadInfo@1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D87F0, 124 bytes, __fastcall (info in ecx), plain ret.
// PDB: Ego_r + FableWin ?SwitchToMicroThread@MicroThread@@YI_NPAVMicroThreadInfo@1@@Z. Both PDBs also name the TU-static
//   (unmangled) helpers MicroThread::RestoreStack and MicroThread::PreserveStack (Ego_r 0x00B2BA10 / 0x00B2BA40, 0x30 bytes
//   each); retail inlines both here and keeps an out-of-line PreserveStack at 0x009D87B0 (vector in edi, the same bytes as
//   this function's second half).
// Classes (PDB): MicroThreadInfo = {vptr, bool Dead (+4)}; MicroThreadDetails : MicroThreadInfo adds the STLport
//   vector<unsigned char> Stack at +8 (12 bytes in retail; the Ego_r layout is 16). Every thread is created as a
//   MicroThreadDetails (CreateMicroThread 0x009D8710), so the info pointer is used as one.
// Globals (MicroThread TU, descriptive names -- data symbols are not in the name sets): active thread 0x013CA894 (4 refs,
//   all in this TU: GetActiveMicroThread reads it), saved stack pointer 0x013CA898, shared stack top 0x013CA8A0.
// Callees: MSVCR71 memmove (import thunk 0x00BFEAE6, twice), MicroThreadSleep() 0x009D8650 (the stack switch),
//   STLport vector<unsigned char>::resize(size_type, const unsigned char&) 0x00486189 (out of line).
// Shape: the active thread is a TU-STATIC global whose address is never taken: VC7.1 then knows its store cannot alias the
//   vector reads, so "ActiveThread = info;" written FIRST keeps start/finish in registers ("mov ecx,[esi] ; mov edx,[esi+4]
//   ; sub edx,ecx") and the store sinks below the three memmove pushes. With an extern global the store kills the reads
//   ("sub edx,[esi]", store before the pushes) -- the old 124v124 residue. The saved-stack store does kill them (retail
//   re-reads [esi] after it), so that global stays extern. resize(n) is STLport's one-argument overload forwarding
//   unsigned char() (zero byte in the high byte of the "push ecx" slot); "return !info->IsDead()" gives "sete al".
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

namespace std
{
    template <class _Tp> class allocator { };

    template <class _Tp, class _Alloc = allocator<_Tp> >
    class vector
    {
    public:
        typedef unsigned int size_type;
        _Tp* _M_start;
        _Tp* _M_finish;
        _Tp* _M_end_of_storage;

        _Tp* begin() { return _M_start; }
        size_type size() const { return size_type(_M_finish - _M_start); }
        void resize(size_type __new_size, const _Tp& __x);                // 0x00486189
        void resize(size_type __new_size) { resize(__new_size, _Tp()); }
    };
}

namespace MicroThread
{
    class MicroThreadInfo
    {
    public:
        virtual void MarkAsDead();                         // slot 0, 0x009D8630
        virtual unsigned int MicroThreadStackSize() const = 0;
        bool IsDead() const { return Dead; }
    protected:
        virtual ~MicroThreadInfo();
    public:
        bool Dead;                                         // +0x04
    };

    class MicroThreadDetails : public MicroThreadInfo
    {
    public:
        virtual unsigned int MicroThreadStackSize() const;  // 0x009D8890
        virtual ~MicroThreadDetails();
        std::vector<unsigned char> Stack;                 // +0x08
    };

    static MicroThreadInfo* ActiveThread;                 // 0x013CA894 (TU-static: only this TU touches it)
    extern unsigned char*   SavedStackPointer;            // 0x013CA898
    extern unsigned char*   StackTop;                     // 0x013CA8A0

    void __fastcall MicroThreadSleep();                   // 0x009D8650

    static inline void RestoreStack(std::vector<unsigned char>& stack)
    {
        SavedStackPointer = StackTop - stack.size();
        memmove(SavedStackPointer, stack.begin(), stack.size());
    }

    static inline void PreserveStack(std::vector<unsigned char>& stack)
    {
        unsigned int size = StackTop - SavedStackPointer;
        stack.resize(size);
        memmove(stack.begin(), SavedStackPointer, size);
    }

    bool __fastcall SwitchToMicroThread(MicroThreadInfo* info)
    {
        MicroThreadDetails* details = static_cast<MicroThreadDetails*>(info);
        ActiveThread = info;
        RestoreStack(details->Stack);
        MicroThreadSleep();
        PreserveStack(details->Stack);
        return !info->IsDead();
    }
}