// Behaviour test for 0x0083FBC0 LHNetworkLib::LHHttpHeaders::Reset: empties the header string and erases every stored header line
#include <cstdio>
#include <cstring>
static int g_erases = 0;
struct CRetailString {
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    char m_storage[32];
    CRetailString& Assign(const char* first, const char* last)
    {
        std::memcpy(m_storage, first, last - first);
        _M_start = m_storage; _M_finish = m_storage + (last - first); *_M_finish = 0;
        return *this;
    }
    CRetailString& operator=(const char* text) { return Assign(text, text + std::strlen(text)); }
};
struct CRetailStringVector {
    CRetailString* _M_start;
    CRetailString* _M_finish;
    CRetailString* _M_end_of_storage;
    CRetailString* begin() { return _M_start; }
    CRetailString* end() { return _M_finish; }
    CRetailString* erase(CRetailString* first, CRetailString* last)
    {
        ++g_erases;
        CRetailString* out = first;
        CRetailString* in;
        for (in = last; in != _M_finish; ++in, ++out)
            *out = *in;
        _M_finish = out;
        return first;
    }
    void clear() { erase(begin(), end()); }
};
struct LHHttpHeaders {
    CRetailStringVector HdrDocumentHeader;
    CRetailString       HeaderString;
    void Reset();
};
void LHHttpHeaders::Reset()
{
    HeaderString = "";
    HdrDocumentHeader.clear();
}
int main()
{
    CRetailString lines[3];
    lines[0] = "Host: a"; lines[1] = "Accept: b";
    LHHttpHeaders h;
    h.HdrDocumentHeader._M_start = lines; h.HdrDocumentHeader._M_finish = lines + 2; h.HdrDocumentHeader._M_end_of_storage = lines + 3;
    h.HeaderString = "GET / HTTP/1.0";
    h.Reset();
    if (h.HeaderString._M_finish != h.HeaderString._M_start || h.HeaderString.m_storage[0] != 0 ||
        h.HdrDocumentHeader._M_finish != lines || g_erases != 1) { std::printf("0083fbc0_TEST FAIL\n"); return 1; }
    std::printf("0083fbc0_TEST PASS\n");
    return 0;
}