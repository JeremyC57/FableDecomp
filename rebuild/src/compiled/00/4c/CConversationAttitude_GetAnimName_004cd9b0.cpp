// CConversationAttitude::GetAnimName(EConversationAttitude attitude) -- the talk animation name of a conversation attitude (an index into the static name table)
// Leaf: ?GetAnimName@CConversationAttitude@@SIABVCCharString@@W4EConversationAttitude@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CD9B0, 9 bytes: mov eax,[0x013B8A38] ; lea eax,[eax+ecx*4] ; ret.
// Identity (two sources): (1) the table it reads, 0x013B8A38, is CConversationAttitude::ConversationAttitudeToAnimName (chunk c674:
//   Ego_r.pdb dynamic initializer / atexit names, the landed initializer 0x0121A4F0); (2) Ego_r.pdb's donor run
//   GetAnimName(EAttitude) 0x004EB240 / GetAnimName(EConversationAttitude) 0x004EB250 / GetAnimGroupName(EConversationAttitude)
//   0x004EB260 = retail ... / 0x004CD9B0 / 0x004CD9C0 (the landed GetAnimGroupName twin reads 0x013B8A44). Static __fastcall:
//   the attitude arrives in ecx and the result is a const CCharString&.
// Naming wave 16: landed as the free __fastcall GetAnimGroupName(int) over `void** g_AnimGroupNameTable` (catalog
//   CConversationAttitude::GetAnimGroupName -- the 0x004CD9C0 name, wrong here); re-authored as the 0x004CD9C0 twin
//   (name-only: the bytes are unchanged).
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
    static const CCharString& __fastcall GetAnimName(EConversationAttitude attitude);
    static std::vector<CCharString> ConversationAttitudeToAnimName;        // 0x013B8A38
};

const CCharString& __fastcall CConversationAttitude::GetAnimName(EConversationAttitude attitude)
{
    return ConversationAttitudeToAnimName[attitude];
}