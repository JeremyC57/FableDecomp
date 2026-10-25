// CVsyncCallbackTimer::CVsyncCallbackTimer() -- construct the vsync timer: build the empty FreeQueoryObjects and PendingQueories query lists, zero the shared CPU/GPU frame stamps, PendingFrame = 1, FinishedFrame = 0
// Leaf: ??0CVsyncCallbackTimer@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F70E0, 354 bytes, thiscall, returns this. Caller: 0x009C0EC8 (`new` 0x14 bytes,
//   stored at +0x294 of the display device owner). The chunk label "Init_ListHeads" is descriptive.
// Layout (Ego_r.pdb CVsyncCallbackTimer, retail 0x14 with STLport's 4-byte list): Device +0 (left alone), FreeQueoryObjects +4,
//   PendingQueories +8 (list<CPendingQuery>, 0x18-byte nodes), PendingFrame +0xC, FinishedFrame +0x10.
// Globals: two 0x18-byte CPU frame stamps at 0x013CB4B0 {TimeStamp, PrevTimeStamp, FrameCount, Readers (the
//   InterlockedIncrement/Decrement count GetInfo 0x009F7050 brackets its read with)}, GPU stamps 0x013CB4E0 / 0x013CB4E8,
//   GPU frame count 0x013CB4F0, current CPU stamp index 0x013CB4F4.
// Callees: 0x00BFEA0E malloc (STLport allocate(1) through __malloc_alloc::allocate, not folded: its __result local is
//   the extra frame temporary).
// Shape: an UNOPTIMISED TU (`#pragma optimize("", off)`, as the whole 0x009F7020..0x009F7ECx timer TU): `this` spilled to
//   [ebp-0x28], the inlined list constructors keep their `this`, malloc and ternary temporaries in frame slots, stores in
//   source order with every 0.0 loaded from the double pool 0x0122ED70.
#pragma optimize("", off)
extern "C" void* __cdecl malloc(unsigned int size);                 // 0x00BFEA0E

namespace std {

class __malloc_alloc {
public:
    static void* allocate(unsigned int __n)
    {
        void* __result = malloc(__n);
        return __result;
    }
};

template <class _Tp> class allocator {
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)__malloc_alloc::allocate(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _List_base(const _Alloc&) : _M_node(allocator<_Node>(), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    explicit list(const _Alloc& __a = _Alloc()) : _List_base<_Tp, _Alloc>(__a) {}
};

}

struct IDirect3DDevice9;
struct IDirect3DQuery9;

struct CVsyncFrameStamp {                           // 0x18 bytes
    double TimeStamp;                               // +0x00
    double PrevTimeStamp;                           // +0x08
    long FrameCount;                                // +0x10
    long Readers;                                   // +0x14
};

extern CVsyncFrameStamp g_CPUFrameStamps[2];        // 0x013CB4B0
extern double g_GPUTimeStamp;                       // 0x013CB4E0
extern double g_GPUPrevTimeStamp;                   // 0x013CB4E8
extern long g_GPUFrameCount;                        // 0x013CB4F0
extern long g_CurrentCPUFrameStamp;                 // 0x013CB4F4

class CVsyncCallbackTimer {
public:
    struct CPendingQuery {                          // 0x10 bytes
        IDirect3DQuery9* Query;                     // +0x00
        long Frame;                                 // +0x04
        double TimeStamp;                           // +0x08
    };

    CVsyncCallbackTimer();

    IDirect3DDevice9* Device;                       // +0x00
    std::list<CPendingQuery> FreeQueoryObjects;     // +0x04
    std::list<CPendingQuery> PendingQueories;       // +0x08
    long PendingFrame;                              // +0x0C
    long FinishedFrame;                             // +0x10
};

CVsyncCallbackTimer::CVsyncCallbackTimer()
{
    g_GPUFrameCount = 0;
    g_CurrentCPUFrameStamp = 0;
    g_CPUFrameStamps[0].TimeStamp = 0.0;
    g_CPUFrameStamps[0].PrevTimeStamp = 0.0;
    g_CPUFrameStamps[1].TimeStamp = 0.0;
    g_CPUFrameStamps[1].PrevTimeStamp = 0.0;
    g_GPUTimeStamp = 0.0;
    g_GPUPrevTimeStamp = 0.0;
    g_CPUFrameStamps[0].FrameCount = 0;
    g_CPUFrameStamps[0].Readers = 0;
    g_CPUFrameStamps[1].FrameCount = 0;
    g_CPUFrameStamps[1].Readers = 0;
    PendingFrame = 1;
    FinishedFrame = 0;
}