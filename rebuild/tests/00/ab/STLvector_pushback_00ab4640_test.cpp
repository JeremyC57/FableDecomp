#include <cstdio>
#include <cstdlib>
#include <new>

// Host model of _STL::vector<CVertexShaderInput::CStreamInfo>::push_back: copy-construct {StreamIndex, CVertexStreamFormat}
// in place when there is room, else grow through _M_insert_overflow (old + max(old, 1), copy, construct, destroy, free).
static int g_live = 0;

struct CDataElement { unsigned int Register; int DataType; };
struct CDataElementArray {                         // CArray<CDataElement>: deep-copied vector
    CDataElement* Start; CDataElement* Finish; CDataElement* End;
    CDataElementArray() : Start(0), Finish(0), End(0) { }
    CDataElementArray(const CDataElementArray& o) : Start(0), Finish(0), End(0)
    {
        unsigned int n = (unsigned int)(o.Finish - o.Start);
        if (n) { Start = (CDataElement*)std::malloc(n * sizeof(CDataElement)); Finish = End = Start + n; }
        for (unsigned int i = 0; i < n; ++i) Start[i] = o.Start[i];
    }
    ~CDataElementArray() { if (Start) std::free(Start); }
    void add(unsigned int reg, int type)
    {
        unsigned int n = (unsigned int)(Finish - Start);
        CDataElement* s = (CDataElement*)std::malloc((n + 1) * sizeof(CDataElement));
        for (unsigned int i = 0; i < n; ++i) s[i] = Start[i];
        s[n].Register = reg; s[n].DataType = type;
        if (Start) std::free(Start);
        Start = s; Finish = End = s + n + 1;
    }
private:
    CDataElementArray& operator=(const CDataElementArray&);
};
struct CVertexStreamFormat {
    CDataElementArray DataElements;
    CVertexStreamFormat() { ++g_live; }
    CVertexStreamFormat(const CVertexStreamFormat& o) : DataElements(o.DataElements) { ++g_live; }
    virtual ~CVertexStreamFormat() { --g_live; }
};
struct CStreamInfo {
    unsigned int        StreamIndex;
    CVertexStreamFormat StreamFormat;
};

struct CStreamVector {
    CStreamInfo* Start; CStreamInfo* Finish; CStreamInfo* End;
    CStreamVector() : Start(0), Finish(0), End(0) { }
    ~CStreamVector() { for (CStreamInfo* p = Start; p != Finish; ++p) p->~CStreamInfo(); if (Start) std::free(Start); }
    void insert_overflow(CStreamInfo* pos, const CStreamInfo& x, unsigned int fill)
    {
        unsigned int old = (unsigned int)(Finish - Start);
        unsigned int len = old + (old > fill ? old : fill);
        CStreamInfo* ns = (CStreamInfo*)std::malloc(len * sizeof(CStreamInfo));
        CStreamInfo* nf = ns;
        for (CStreamInfo* p = Start; p != pos; ++p, ++nf) new (nf) CStreamInfo(*p);
        new (nf) CStreamInfo(x); ++nf;
        for (CStreamInfo* p = Start; p != Finish; ++p) p->~CStreamInfo();
        if (Start) std::free(Start);
        Start = ns; Finish = nf; End = ns + len;
    }
    void push_back(const CStreamInfo& x)
    {
        if (Finish != End) {
            if (Finish) new (Finish) CStreamInfo(x);
            ++Finish;
        }
        else
            insert_overflow(Finish, x, 1);
    }
};

int main()
{
    {
        CStreamVector v;
        for (unsigned int i = 0; i < 5; ++i) {
            CStreamInfo s;
            s.StreamIndex = i;
            for (unsigned int k = 0; k <= i; ++k) s.StreamFormat.DataElements.add(k, (int)(i * 10 + k));
            v.push_back(s);
        }
        if (v.Finish - v.Start != 5 || v.End - v.Start != 8) { std::printf("00ab4640_TEST FAIL\n"); return 1; }
        for (unsigned int i = 0; i < 5; ++i) {
            const CStreamInfo& s = v.Start[i];
            if (s.StreamIndex != i || s.StreamFormat.DataElements.Finish - s.StreamFormat.DataElements.Start != (int)(i + 1)) {
                std::printf("00ab4640_TEST FAIL\n");
                return 1;
            }
            if (s.StreamFormat.DataElements.Start[i].DataType != (int)(i * 11)) { std::printf("00ab4640_TEST FAIL\n"); return 1; }
        }
        if (g_live != 5) { std::printf("00ab4640_TEST FAIL\n"); return 1; }   // one live format per stored stream
    }
    if (g_live != 0) { std::printf("00ab4640_TEST FAIL\n"); return 1; }
    std::printf("00ab4640_TEST PASS\n");
    return 0;
}