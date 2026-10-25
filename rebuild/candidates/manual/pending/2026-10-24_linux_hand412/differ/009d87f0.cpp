// MicroThread::SwitchToMicroThread(MicroThreadInfo*) -- run a micro-thread until it yields: copy its saved stack image under the shared stack top, make it active, MicroThreadSleep into it, then save the (possibly grown) image back; true while the thread is still alive
// Grade: DIFFER(124v124) -- scheduling around the first memmove: retail loads start/finish once (`mov ecx,[esi] ; mov edx,[esi+4] ; sub edx,ecx`) and sinks the active-thread store below the three pushes, ours stores the active thread first and re-reads start for the size (`sub edx,[esi]`)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D87F0, length 124 bytes.
// PDB (FableWin): ?SwitchToMicroThread@MicroThread@@YI_NPAVMicroThreadInfo@1@@Z -- __fastcall, info in ecx.
// Classes (PDB): MicroThreadInfo = {vptr, bool Dead (+4)}; MicroThreadDetails : MicroThreadInfo adds the STLport
// vector<unsigned char> Stack at +8 (12 bytes in retail; the Ego_r layout is 16). Every thread is created as a
// MicroThreadDetails (CreateMicroThread 0x009D8710), so the info pointer is used as one.
// Globals (MicroThread module): active thread 0x013CA894, parked stack pointer 0x013CA898, shared stack top 0x013CA8A0.
// Callees: MSVCR71 memmove (0x00BFEAE6, twice), MicroThreadSleep (0x009D8650, the stack switch),
// STLport vector<unsigned char>::resize(size_type, const unsigned char&) (0x00486189, out of line).
// Shape: `resize(n)` is STLport's inline one-argument overload forwarding `unsigned char()`: the zero temporary is the
// high byte of the `push ecx` frame slot (`lea eax,[esp+0Fh] ; ... ; mov byte ptr [esp+17h],0`); the parked pointer
// is computed as `top - size()` (start - finish + top) and handed to the first memmove from the register; the second
// memmove re-reads it; `return !info->IsDead()` gives `xor eax,eax ; test cl,cl ; sete al`.
// Tried: memmove before the active-thread store (store lands after the call), the store first with begin()/size()
// inline (126 bytes), a named image pointer (this source, 124 bytes), image + size locals before the store.
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

    extern MicroThreadInfo* g_pActiveThread;               // 0x013CA894
    extern unsigned char*   g_pSavedStack;                 // 0x013CA898
    extern unsigned char*   g_pStackTop;                   // 0x013CA8A0

    void __fastcall MicroThreadSleep();                    // 0x009D8650

    bool __fastcall SwitchToMicroThread(MicroThreadInfo* info)
    {
        MicroThreadDetails* details = static_cast<MicroThreadDetails*>(info);
        g_pSavedStack = g_pStackTop - details->Stack.size();
        unsigned char* image = details->Stack.begin();
        g_pActiveThread = info;
        memmove(g_pSavedStack, image, details->Stack.size());
        MicroThreadSleep();
        unsigned int used = g_pStackTop - g_pSavedStack;
        details->Stack.resize(used);
        memmove(details->Stack.begin(), g_pSavedStack, used);
        return !info->IsDead();
    }
}
