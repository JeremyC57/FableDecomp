// CInputManager::FlagEventAsProcessed(const CInputEvent& event) -- append the CInputEvent to the ProcessedEvents vector
// Leaf: ?FlagEventAsProcessed@CInputManager@@QAEXABVCInputEvent@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F55C0, 62 bytes, thiscall, `ret 4`.
// Identity (proven): its callers (CGamePlayerInterface::GetInputProcessEvent 0x00446330, twice, after an event is handled) pass g_pInputManager 0x013B8388; it appends a CInputEvent (copy constructor 0x009E4850, 0x34 bytes) to the vector<CInputEvent> at +4 = Ego_r.pdb CInputManager::ProcessedEvents; Ego_r.pdb ?FlagEventAsProcessed@CInputManager@@QAEXABVCInputEvent@@@Z is the void method taking const CInputEvent& that fills it.
// Callees: CInputEvent::CInputEvent(const CInputEvent&) 0x009E4850, vector<CInputEvent>::_M_insert_overflow 0x009F6AC0.
// Shape (family M082, representative 0x004B2850): `ProcessedEvents.push_back(event);` -- STLport push_back inline (null-checked
//   placement copy, or _M_insert_overflow(finish, x, __false_type(), 1, true) with the tag homed in the argument slot).
// Header debt: engine/CInputManager.h cannot carry the STLport vector member with its methods; the class is modelled locally by
//   the member's retail offset.
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
class CInputEvent {                                              // 0x34 bytes
public:
    CInputEvent(const CInputEvent& other);                            // 0x009E4850
    unsigned long Members[13];
};
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);   // 0x009F6AC0
};
}
class CInputManager {
public:
    void FlagEventAsProcessed(const CInputEvent& event);
    unsigned char _pad[0x4];                         // +0x0 .. members before ProcessedEvents
    std::vector<CInputEvent> ProcessedEvents;    // +0x4
};
void CInputManager::FlagEventAsProcessed(const CInputEvent& event)
{
    ProcessedEvents.push_back(event);
}