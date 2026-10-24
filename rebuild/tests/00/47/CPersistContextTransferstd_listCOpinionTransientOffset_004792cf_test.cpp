// Behaviour test for 0x004792CF CPersistContext::Transfer<std::list<COpinionTransientOffset> >: CheckCRC(""), then a
// CPersistTraits<list> local (default value = an empty list) loads (binary in) or saves (binary out); traits destroyed on every path.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int g_crc = 0, g_in = 0, g_out = 0, g_dtor = 0; static const void* g_stream = 0; static const void* g_value = 0;


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

template <class _Tp> list<_Tp>::list(const allocator<_Tp>&) : _M_node(0)
{
    _List_node_base* __node = (_List_node_base*)malloc(sizeof(_List_node<_Tp>));
    __node->_M_next = __node;
    __node->_M_prev = __node;
    _M_node = __node;
}
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
namespace std {
template <> list<COpinionTransientOffset>::~list() { ++g_dtor; if (_M_node && _M_node->_M_next == _M_node) free(_M_node); else g_dtor += 100; }
}
void CPersistContext::CheckCRC(const char* name) { if (name && name[0] == 0) ++g_crc; }
template <> void CPersistTraits<std::list<COpinionTransientOffset> >::TransferBinaryIn(CMemoryDataInputStream& stream, std::list<COpinionTransientOffset>& value) const
{ ++g_in; g_stream = &stream; g_value = &value; if (DefaultVal._M_node->_M_prev != DefaultVal._M_node) g_in += 100; }
template <> void CPersistTraits<std::list<COpinionTransientOffset> >::TransferBinaryOut(CDataOutputStream& stream, const std::list<COpinionTransientOffset>& value) const
{ ++g_out; g_stream = &stream; g_value = &value; }

int main()
{
    CPersistContext ctx; memset(&ctx, 0, sizeof(ctx));
    int inTag = 0, outTag = 0;
    ctx.PLoadStream = (CMemoryDataInputStream*)&inTag; ctx.PSaveStream = (CDataOutputStream*)&outTag;
    std::list<COpinionTransientOffset>* v = (std::list<COpinionTransientOffset>*)malloc(sizeof(std::list<COpinionTransientOffset>));
    int ok = 1;
    ctx.Mode = CPersistContext::MODE_BINARY_IN; ctx.Transfer(*v);
    if (g_crc != 1 || g_in != 1 || g_out != 0 || g_stream != &inTag || g_value != v || g_dtor != 1) ok = 0;
    ctx.Mode = CPersistContext::MODE_BINARY_OUT; ctx.Transfer(*v);
    if (g_crc != 2 || g_in != 1 || g_out != 1 || g_stream != &outTag || g_value != v || g_dtor != 2) ok = 0;
    ctx.Mode = CPersistContext::MODE_TEXT_IN; ctx.Transfer(*v);
    if (g_crc != 3 || g_in != 1 || g_out != 1 || g_dtor != 3) ok = 0;
    free(v);
    printf(ok ? "004792cf_TEST PASS\n" : "004792cf_TEST FAIL\n");
    return ok ? 0 : 1;
}