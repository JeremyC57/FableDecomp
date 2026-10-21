#include <cstdio>
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                                       // 0x0099E4B0
    ~CCharString();                                      // 0x0099EAE0
};
class CDataOutputStream {
public:
    void WriteLong(long value);                          // 0x00993EB0
    void WriteNullTerminatedString(const CCharString& s);   // 0x00995AE0
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
template <class T> class CPersistTraits;
template <> class CPersistTraits<CCharString> {
public:
    CCharString DefaultValue;
    void TransferBinaryOut(CDataOutputStream& stream, const CCharString& value) { stream.WriteNullTerminatedString(value); }
};
template <class T> void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStream& stream, std::vector<T>& v)
{
    CPersistTraits<T> traits;
    const long count = v.size();
    stream.WriteLong(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryOut(stream, v[i]);
}
template void __fastcall GFSerialiseVectorBinaryOut<CCharString>(CDataOutputStream&, std::vector<CCharString>&);


static int g_ctor, g_dtor, g_long = -1, g_writes; static const CCharString* g_last;
CCharString::CCharString() : PStringData(0) { ++g_ctor; }
CCharString::~CCharString() { ++g_dtor; }
void CDataOutputStream::WriteLong(long value) { g_long = value; }
void CDataOutputStream::WriteNullTerminatedString(const CCharString& s) { ++g_writes; g_last = &s; }
int main()
{
    CCharString items[3];
    g_ctor = 0; g_dtor = 0;
    std::vector<CCharString> v; v._M_start = items; v._M_finish = items + 3;
    CDataOutputStream out;
    GFSerialiseVectorBinaryOut(out, v);
    bool ok = g_long == 3 && g_writes == 3 && g_last == &items[2] && g_ctor == 1 && g_dtor == 1;
    v._M_finish = items; GFSerialiseVectorBinaryOut(out, v);
    ok = ok && g_long == 0 && g_writes == 3;
    if (ok) std::printf("00433227_TEST PASS\n");
    return 0;
}