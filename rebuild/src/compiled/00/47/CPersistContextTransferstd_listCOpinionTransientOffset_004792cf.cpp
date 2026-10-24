// CPersistContext::Transfer<std::list<COpinionTransientOffset> >(std::list<COpinionTransientOffset>& value) -- binary persist of an opinion-offset list: CRC check, then the list traits read it from the load stream (mode 2) or write it to the save stream (mode 3)
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- the switch arms return instead of break (landed 0x004E9550 shape): the traits this is formed once above the first je and the default arm falls into the destructor call
// Retail: Fable.exe (TLC, VC7.1) @ 0x004792CF, 79 bytes, `ret 4`.
// Callers: COpinionDeedEffectsDef::Transfer @ 0x00478EE2 (landed: "Transfer(list<COpinionTransientOffset>&)
// 0x004792CF", the Effects list at +0x4C).
// Callees: CPersistContext::CheckCRC(const char*) @ 0x00404500 ("" @ 0x0122D70E), list(const allocator_type&) @
// 0x00450D90 / ~list() @ 0x00450DAB (the traits' DefaultVal), CPersistTraits<list<COpinionTransientOffset> >::
// TransferBinaryIn @ 0x00479369 and TransferBinaryOut @ 0x0047931E (both landed; traits in ecx, `ret 8`).
// Layout: Ego_r.pdb CPersistContext (Mode +0x18, PLoadStream +0x24, PSaveStream +0x28); no engine header.
// Shape: the traits object is a frame local whose only member is the default value list (built with an
// `allocator_type()` temporary homed in the argument area, destroyed on every path); the two-way switch is the
// size TU's `dec ; dec ; je ; dec ; jne`. Def TU settings: `optimize("y",off)` + `("s",on)`.
// Label correction: catalog "COpinionDeedEffectsDef_Transfer" names the caller; this is the context's typed transfer.
// Leaf: ??$Transfer@V?$list@VCOpinionTransientOffset@@@std@@@CPersistContext@@QAEXAAV?$list@VCOpinionTransientOffset@@@std@@@Z
// Fix (agent901): the switch arms RETURN instead of break (the landed 0x004E9550 TransferHelper shape): VC7.1 then
//   forms the traits `this` once above the first `je` and the default arm falls straight into the destructor call.
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E

class CMemoryDataInputStream;
class CDataOutputStream;

class COpinionTransientOffset {                             // 0x18 bytes
public:
    unsigned char m_data[0x18];
};

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp> class list {
public:
    _List_node_base* _M_node;                               // +0x00 the sentinel
    explicit list(const allocator<_Tp>& __a = allocator<_Tp>());   // 0x00450D90
    ~list();                                                // 0x00450DAB
};

// 0x00450D90 (its folded copy), defined here and kept out of line: the call site then knows the allocator argument is
// never touched and homes the empty `allocator_type()` temporary in the argument area ([ebp+0Bh]), as retail does.
#pragma auto_inline(off)
template <class _Tp> list<_Tp>::list(const allocator<_Tp>&) : _M_node(0)
{
    _List_node_base* __node = (_List_node_base*)malloc(sizeof(_List_node<_Tp>));
    __node->_M_next = __node;
    __node->_M_prev = __node;
    _M_node = __node;
}
#pragma auto_inline(on)
}

template <class T> class CPersistTraits {
public:
    T DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream& stream, T& value) const;    // 0x00479369
    void TransferBinaryOut(CDataOutputStream& stream, const T& value) const;  // 0x0047931E
};

class CPersistContext {
public:
    enum EMode { MODE_NONE, MODE_TEXT_IN, MODE_BINARY_IN, MODE_BINARY_OUT };
    unsigned char           _pad00[0x18];
    EMode                   Mode;                           // +0x18
    bool                    Binary;                         // +0x1c
    void*                   PStringToSaveTo;                // +0x20
    CMemoryDataInputStream* PLoadStream;                    // +0x24
    CDataOutputStream*      PSaveStream;                    // +0x28
    void CheckCRC(const char* name);                        // 0x00404500
    template <class T> void Transfer(T& value);
};

template <class T> void CPersistContext::Transfer(T& value)
{
    CheckCRC("");
    CPersistTraits<T> traits;
    switch (Mode) {
    case MODE_BINARY_IN:
        traits.TransferBinaryIn(*PLoadStream, value);
        return;
    case MODE_BINARY_OUT:
        traits.TransferBinaryOut(*PSaveStream, value);
        return;
    }
}

template void CPersistContext::Transfer<std::list<COpinionTransientOffset> >(std::list<COpinionTransientOffset>&);