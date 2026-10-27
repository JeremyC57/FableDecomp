// CQuestManager::RegisterQuest(const CCharString& name) -- append the CCharString to the RegisteredQuestNames vector
// Leaf: ?RegisterQuest@CQuestManager@@QAEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B2850, 62 bytes, thiscall, `ret 4`.
// Identity (proven): its caller CWorld::LoadQuestsFromFile 0x004A0D90 passes the quest manager singleton 0x013B89FC; it appends to the vector<CCharString> at +0x2C = RegisteredQuestNames (the landed CQuestManager destructor layout); Ego_r.pdb ?RegisterQuest@CQuestManager@@QAEXABVCCharString@@@Z.
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30, vector<CCharString>::_M_insert_overflow 0x00433530.
// Shape (family M082, representative 0x004B2850): `RegisteredQuestNames.push_back(name);` -- STLport push_back inline (null-checked
//   placement copy, or _M_insert_overflow(finish, x, __false_type(), 1, true) with the tag homed in the argument slot).
// Header debt: engine/CQuestManager.h cannot carry the STLport vector member with its methods; the class is modelled locally by
//   the member's retail offset.
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
class CCharString {                                              // 0x4 bytes
public:
    CCharString(const CCharString& other);                            // 0x0099EC30
    unsigned long Members[1];
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
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);   // 0x00433530
};
}
class CQuestManager {
public:
    void RegisterQuest(const CCharString& name);
    unsigned char _pad[0x2c];                         // +0x0 .. members before RegisteredQuestNames
    std::vector<CCharString> RegisteredQuestNames; // +0x2c
};
void CQuestManager::RegisterQuest(const CCharString& name)
{
    RegisteredQuestNames.push_back(name);
}