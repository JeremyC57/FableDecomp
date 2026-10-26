// CPersistTraits<std::list<CReactionMatch*> >::TransferBinaryOut(CDataOutputStream&, const std::list<CReactionMatch*>&) -- binary persist write of a reaction-match list: the count, then each match's type id and its own binary data
// Leaf: TransferBinaryOut
// Retail: Fable.exe (TLC, VC7.1) @ 0x00678250, 88 bytes (+8 int3 padding), `__fastcall` (the traits object in ecx is never
//   read), `ret 8`. Callees: CDataOutputStream::WriteLong @ 0x00993EB0 (x2) and two CReactionMatch virtuals: slot 2
//   GetMatchType (+8, e.g. CReactionMatch_Sphere 0x0067C320) and slot 4 TransferBinaryOut(CDataOutputStream&) const (+0x10,
//   e.g. CReactionMatch_MRPyramid 0x006FECA0) -- vtable 0x0125B730 / 0x0125B7C4 slots in rtti-vtables.tsv.
// Shape: list::size() is the inline node walk (STLport `distance(begin(), end())`, count in edx); the write loop reloads
//   the sentinel through the list each iteration (`cmp esi,[ebp]`) because the virtual calls may modify it, and the match
//   pointer is held in edi across both virtual calls.
// The stream uses engine/CDataOutputStream.h with its out-of-line WriteLong declared in a methods view.
#define CDataOutputStream CDataOutputStreamLayout
#include "engine/CDataOutputStream.h"
#undef CDataOutputStream

class CDataOutputStream : public CDataOutputStreamLayout
{
public:
    void WriteSLONG(long);   // 0x00993EB0
};

struct CDataOutputStream_Methods : CDataOutputStream {
};

enum EReactionMatchType { REACTION_MATCH_TYPE_FORCE_DWORD = 0x7fffffff };

class CReactionMatch {
public:
    virtual ~CReactionMatch();
    virtual float Match(const void* opinionSet);                        // slot 1
    virtual EReactionMatchType GetMatchType() const;                    // slot 2 (+0x08)
    virtual void TransferBinaryIn(void* stream);                        // slot 3
    virtual void TransferBinaryOut(CDataOutputStream& out) const;       // slot 4 (+0x10)
};

namespace std {
template <class T> struct _List_node {
    _List_node* _M_next;                                                // +0x00
    _List_node* _M_prev;                                                // +0x04
    T           _M_data;                                                // +0x08
};
template <class T> struct _List_iterator {
    _List_node<T>* _M_node;
    _List_iterator(_List_node<T>* node) : _M_node(node) { }
    bool operator!=(const _List_iterator& other) const { return _M_node != other._M_node; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    T& operator*() const { return _M_node->_M_data; }
};
template <class T> class list {
public:
    _List_node<T>* _M_node;                                             // the sentinel
    _List_iterator<T> begin() const { return _List_iterator<T>(_M_node->_M_next); }
    _List_iterator<T> end() const { return _List_iterator<T>(_M_node); }
    unsigned long size() const
    {
        unsigned long n = 0;
        for (_List_iterator<T> it = begin(); it != end(); ++it)
            ++n;
        return n;
    }
};
}

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferBinaryOut(CDataOutputStream& out, const T& value) const;
};

template <> void CPersistTraits<std::list<CReactionMatch*> >::TransferBinaryOut(CDataOutputStream& out, const std::list<CReactionMatch*>& value) const
{
    CDataOutputStream_Methods& stream = static_cast<CDataOutputStream_Methods&>(out);
    stream.WriteSLONG(value.size());
    for (std::_List_iterator<CReactionMatch*> it = value.begin(); it != value.end(); ++it) {
        CReactionMatch* match = *it;
        stream.WriteSLONG(match->GetMatchType());
        match->TransferBinaryOut(out);
    }
}