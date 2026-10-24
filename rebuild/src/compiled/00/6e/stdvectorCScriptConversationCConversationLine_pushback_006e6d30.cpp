// std::vector<CScriptConversation::CConversationLine>::push_back(const CConversationLine& line) -- STLport push_back: copy-construct the line in place (null-checked placement new) and advance _M_finish, else the reallocating _M_insert_overflow
// Leaf: ?push_back@?$vector@VCConversationLine@CScriptConversation@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E6D30, 86 bytes, thiscall, `ret 4`.
// Identity: the only caller is CScriptConversation::AddLineToConversation 0x006E58A0 (this chunk), which pushes its
//   16-byte CConversationLine temporary on `lea ecx,[esi+14h]` (the retail STLport Conversation vector). The catalog's
//   "ConversationLine_PushToVector" is descriptive.
// Callees: CCharString copy ctor @ 0x0099EC30 (the implicit line copy ctor: two longs, the string, the bool),
//   _M_insert_overflow(pos, x, const __false_type&, 1, true) @ 0x006E6BC0 (the tag temporary homed in the dead x slot).
// Layout: CConversationLine (Ego_r.pdb, 16 bytes): PersonSpeaking +0, PersonSpokenTo +4, LineToSay +8, ShowSubtitle +0xC.
#include <new>

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
};

class CScriptConversation {
public:
    class CConversationLine {
    public:
        long        PersonSpeaking;                         // +0x00
        long        PersonSpokenTo;                         // +0x04
        CCharString LineToSay;                              // +0x08
        bool        ShowSubtitle;                           // +0x0C
    };
};

namespace std {
struct __false_type {
    __false_type() { }
};

template <class _Tp> class allocator { };

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend);              // 0x006E6BC0
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(__x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1, true);
    }
};

template class vector<CScriptConversation::CConversationLine>;
}