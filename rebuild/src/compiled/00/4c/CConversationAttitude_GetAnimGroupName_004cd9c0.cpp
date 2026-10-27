// CConversationAttitude::GetAnimGroupName(EConversationAttitude attitude) -- the animation group name of a conversation attitude (an index into the static name table)
// Leaf: ?GetAnimGroupName@CConversationAttitude@@SIABVCCharString@@W4EConversationAttitude@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CD9C0, 9 bytes: mov eax,[0x013B8A44] ; lea eax,[eax+ecx*4] ; ret.
// Identity: Ego_r.pdb ?GetAnimGroupName@CConversationAttitude@@SIABVCCharString@@W4EConversationAttitude@@@Z (static,
//   __fastcall: the attitude arrives in ecx and the result is a const CCharString&). The table is the static member
//   CConversationAttitude::ConversationAttitudeToAnimGroupName (Ego_r.pdb dynamic initializer / atexit names), an
//   STLport vector<CCharString> whose _M_start sits at 0x013B8A44 (4-byte CCharString elements: `*4`).
//   Callers: five sites in 0x008D9230..0x008D93A0.
class CCharString {
public:
    void* PStringData;                          // +0x00 (4 bytes)
};

enum EConversationAttitude { ECONVERSATION_ATTITUDE_NONE = 0 };

namespace std {
template <class _Tp> class allocator { public: };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef _Tp& reference;
    typedef const _Tp& const_reference;
    iterator begin() { return this->_M_start; }
    const_iterator begin() const { return this->_M_start; }
    reference operator[](unsigned int __n) { return *(begin() + __n); }
    const_reference operator[](unsigned int __n) const { return *(begin() + __n); }
};
} // namespace std

class CConversationAttitude {
public:
    static const CCharString& __fastcall GetAnimGroupName(EConversationAttitude attitude);
    static std::vector<CCharString> ConversationAttitudeToAnimGroupName;   // 0x013B8A44
};

const CCharString& __fastcall CConversationAttitude::GetAnimGroupName(EConversationAttitude attitude)
{
    return ConversationAttitudeToAnimGroupName[attitude];
}