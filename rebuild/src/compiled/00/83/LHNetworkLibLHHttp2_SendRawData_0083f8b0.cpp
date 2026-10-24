// LHNetworkLib::LHHttp2::SendRawData(char* data, unsigned long size, unsigned long* bytesSent, bool) -- sends a raw block over the connection socket: 3 when there is no socket, else copy the block into a byte vector, send it with default (unused) addresses, report the socket's last sent count, 0
// Leaf: ?SendRawData@LHHttp2@LHNetworkLib@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083F8B0, 199 bytes, __fastcall, `ret 10h`, LH_RETURN in eax; the bool is unused.
// Identity: Ego_r.pdb ?SendRawData@LHHttp2@LHNetworkLib@@QAE?AW4LH_RETURN@2@PADKPAK_N@Z. Parameter names are descriptive.
//   The manifest label "LHHttp2_SendDataPacket" is descriptive.
// Callees: STLport vector<char>::_M_fill_insert(pos, n, const char&) 0x004110D0 (landed; the inline resize's insert),
//   LAddress::LAddress() 0x00841E90 (landed default constructor, twice), LSocket::Send(vector<char>&, const LAddress&,
//   const LAddress&) 0x008F8F50 (Ego_r ?Send@LSocket@LHNetworkLib@@QAE_NAAV?$vector@D...@ABULAddress@2@1@Z, bool result
//   ignored), free 0x00BFEA14 (the two address names, the buffer).
// Layout: LHHttp2 Host +0 (landed ResetRequest 0x0083F980); LSocket NrLastDataSent +0x90 (the Ego_r LSocket order with
//   TLC's 0x1C-byte LAddress and 12-byte vectors: RemoteAddress 0, LocalAddress 0x1C, Socket 0x38, .., SendOutputQueue
//   0x64, InternalPacketBuffer 0x70, .., NrTotalDataSent 0x88, NrTotalDataReceived 0x8C, NrLastDataSent 0x90).
// Shape: `if (!Host) return 3; vector<char> buffer; buffer.resize(size); memcpy(&buffer[0], data, size);
//   Host->Send(buffer, LAddress(), LAddress()); *bytesSent = Host->NrLastDataSent; return 0;` -- STLport resize inline on
//   the empty vector (the size test folds away), memcpy as the intrinsic `rep movsd / rep movsb`, the address temporaries
//   built right to left and freed before the count is read. resize(n) passes `_Tp()` to a `const _Tp&` resize, so the
//   zero char is a temporary homed at byte 3 of its slot (`lea eax,[esp+13h]`); a by-value `_Tp __x` parameter sits at
//   byte 0 (`[esp+10h]`).
#include <string.h>
#pragma intrinsic(memcpy)
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

class basic_string {                               // STLport basic_string<char>, 12 bytes
public:
    char* _M_start;
    char* _M_finish;
    _STLP_alloc_proxy<char*, char, allocator<char> > _M_end_of_storage;
    ~basic_string() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* iterator;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { }                                  // char: _Destroy is trivial
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    void erase(iterator __first, iterator __last);
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);                          // 0x004110D0
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};

}

namespace LHNetworkLib {

struct LAddress {                                  // retail 0x1C bytes (landed 0x00841E90 / 0x00841F60)
    std::basic_string ServerName;                  // +0x00
    unsigned long     ServerIP;                    // +0x0c
    unsigned short    ServerPort;                  // +0x10
    unsigned long     TransportProtocol;           // +0x14
    unsigned long     TransportFlags;              // +0x18
    LAddress();                                    // 0x00841E90
};

class LSocket {                                    // retail offsets (Ego_r.pdb names)
public:
    unsigned char _pad_0x00[0x90];
    unsigned long NrLastDataSent;                  // +0x90
    bool Send(std::vector<char>& data, const LAddress& to, const LAddress& from);              // 0x008F8F50
};

enum LH_RETURN { };

class LHHttp2 {
public:
    LSocket* Host;                                 // +0x000
    LH_RETURN SendRawData(char* data, unsigned long size, unsigned long* bytesSent, bool flag);
};

LH_RETURN LHHttp2::SendRawData(char* data, unsigned long size, unsigned long* bytesSent, bool flag)
{
    if (!Host)
        return (LH_RETURN)3;                       // no connection socket
    std::vector<char> buffer;
    buffer.resize(size);
    memcpy(&buffer[0], data, size);
    Host->Send(buffer, LAddress(), LAddress());
    *bytesSent = Host->NrLastDataSent;
    return (LH_RETURN)0;
}
}