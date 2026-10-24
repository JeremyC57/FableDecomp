// MicroThread::CreateMicroThread(void (*)(void*), void*) -- allocate a MicroThreadDetails whose saved stack image is the initial frame MicroThreadSleep will pop: SEH chain head, four zero registers, the entry point as return address, MicroThreadReturn as the entry's return address, then the entry argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D8710, length 125 bytes.
// PDB (FableWin): ?CreateMicroThread@MicroThread@@YIPAVMicroThreadInfo@1@Q6AXPAX@ZQAX@Z -- __fastcall, entry in ecx,
// argument in edx, returns the new MicroThreadInfo*.
// Classes (PDB): MicroThreadInfo = {vptr, bool Dead}; MicroThreadDetails : MicroThreadInfo (vtable 0x0129B938) adds
// the STLport vector<unsigned char> Stack at +8; 0x14 bytes, scalar operator new 0x00BFEA1A.
// Callee: STLport vector<unsigned char>::_M_range_insert(iterator, const unsigned char*, const unsigned char*,
// const forward_iterator_tag&) (0x009B3630, out of line), reached through the inline insert / _M_insert_dispatch.
// Globals: MicroThread::DummyExceptionBlock (0x0139650C, a {-1, 0} SEH record) and MicroThreadReturn (0x009D8690,
// marks the active thread dead and sleeps forever).
// Shape: Dead is stored before the derived vtable (the base constructor's own vtable store is dead), the vector's
// three zero pointers after it; the iterator-category tag temporary lives in the high byte of the frame word
// below the 0x20-byte image (`lea eax,[esp+0Fh]`, pushed first); the image's zero words reuse the null-test zero.
extern void* __cdecl operator new(unsigned int size);   // 0x00BFEA1A

namespace _STL
{
    struct input_iterator_tag { input_iterator_tag() {} };
    struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() {} };
    struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() {} };
    struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() {} };
    struct __false_type { __false_type() {} };

    template <class _Tp> class allocator { };

    template <class _Tp, class _Alloc = allocator<_Tp> >
    class vector
    {
    public:
        typedef _Tp* iterator;
        _Tp* _M_start;
        _Tp* _M_finish;
        _Tp* _M_end_of_storage;

        vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
        iterator begin() { return _M_start; }

        template <class _InputIterator>
        void insert(iterator __pos, _InputIterator __first, _InputIterator __last)
        {
            _M_insert_dispatch(__pos, __first, __last, __false_type());
        }
        template <class _InputIterator>
        void _M_insert_dispatch(iterator __pos, _InputIterator __first, _InputIterator __last, const __false_type&)
        {
            _M_range_insert(__pos, __first, __last, random_access_iterator_tag());
        }
        template <class _ForwardIterator>
        void _M_range_insert(iterator __pos, _ForwardIterator __first, _ForwardIterator __last,
                             const forward_iterator_tag&);                       // 0x009B3630
    };
}

namespace MicroThread
{
    class MicroThreadInfo
    {
    public:
        MicroThreadInfo() : Dead(false) { }
        virtual void MarkAsDead();                          // slot 0, 0x009D8630
        virtual unsigned int MicroThreadStackSize() const = 0;
    protected:
        virtual ~MicroThreadInfo();
    public:
        bool Dead;                                          // +0x04
    };

    class MicroThreadDetails : public MicroThreadInfo
    {
    public:
        MicroThreadDetails() { }
        virtual unsigned int MicroThreadStackSize() const;   // 0x009D8890
        virtual ~MicroThreadDetails();
        _STL::vector<unsigned char> Stack;                  // +0x08
    };

    struct CDummyExceptionBlock { void* Next; void* Handler; };
    extern CDummyExceptionBlock DummyExceptionBlock;        // 0x0139650C
    void __fastcall MicroThreadReturn();                    // 0x009D8690

    // The register image MicroThreadSleep pops on the first switch into the thread.
    struct SInitialStackFrame
    {
        CDummyExceptionBlock* ExceptionChain;   // popped into fs:[0]
        unsigned long Edi, Esi, Ebp, Ebx;
        void (*Entry)(void*);                   // MicroThreadSleep's `ret` lands here
        void (__fastcall *Return)();            // the entry's return address
        void* Argument;                         // the entry's argument
    };

    MicroThreadInfo* __fastcall CreateMicroThread(void (*entry)(void*), void* argument)
    {
        MicroThreadDetails* details = new MicroThreadDetails;
        SInitialStackFrame frame;
        frame.ExceptionChain = &DummyExceptionBlock;
        frame.Edi = 0;
        frame.Esi = 0;
        frame.Ebp = 0;
        frame.Ebx = 0;
        frame.Entry = entry;
        frame.Return = MicroThreadReturn;
        frame.Argument = argument;
        details->Stack.insert(details->Stack.begin(), (unsigned char*)&frame, (unsigned char*)(&frame + 1));
        return details;
    }
}