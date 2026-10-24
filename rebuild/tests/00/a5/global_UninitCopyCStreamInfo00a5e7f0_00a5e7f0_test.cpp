// Behaviour test for 0x00A5E7F0 UninitCopy_CStreamInfo_00a5e7f0: placement copy of CVertexShaderInput::CStreamInfo.
#include <cstdio>
#include <new>

struct __false_type { __false_type() { } };
class CBaseClass {
public:
    CBaseClass() { }
    virtual ~CBaseClass() { }
    virtual int Kind() const { return 1; }
};
static int g_arrayCopies = 0;
template <class T> struct CArray {
    T* start; T* finish; T* end;
    CArray() : start(0), finish(0), end(0) { }
    CArray(const CArray& other) : start(other.start), finish(other.finish), end(other.end) { ++g_arrayCopies; }
};
class CVertexStreamFormat : public CBaseClass {
public:
    struct CDataElement { unsigned int Register; int DataType; };
    CArray<CDataElement> DataElements;
    CVertexStreamFormat() { }
    CVertexStreamFormat(const CVertexStreamFormat& o) : CBaseClass(), DataElements(o.DataElements) { }
    virtual ~CVertexStreamFormat() { }
    virtual int Kind() const { return 2; }
};
struct CStreamInfo {
    unsigned int        StreamIndex;
    CVertexStreamFormat StreamFormat;
    CStreamInfo() : StreamIndex(0) { }
    CStreamInfo(const CStreamInfo& o) : StreamIndex(o.StreamIndex), StreamFormat(o.StreamFormat) { }
};

static CStreamInfo* UninitCopy(const CStreamInfo* first, const CStreamInfo* last, CStreamInfo* result, const __false_type&)
{
    CStreamInfo* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) CStreamInfo(*first);
    return cur;
}

int main()
{
    static CVertexStreamFormat::CDataElement elements[3];
    CStreamInfo src[3];
    for (int i = 0; i < 3; ++i) { src[i].StreamIndex = 4u + i; src[i].StreamFormat.DataElements.start = &elements[i]; }
    static double storage[3 * sizeof(CStreamInfo) / sizeof(double) + 1];
    CStreamInfo* dst = (CStreamInfo*)storage;
    __false_type tag;
    CStreamInfo* r = UninitCopy(src, src + 3, dst, tag);
    bool ok = r == dst + 3 && g_arrayCopies == 3;
    for (int k = 0; k < 3; ++k)
        if (dst[k].StreamIndex != 4u + k || dst[k].StreamFormat.DataElements.start != &elements[k] || dst[k].StreamFormat.Kind() != 2) ok = false;
    if (UninitCopy(src, src, dst, tag) != dst || g_arrayCopies != 3) ok = false;
    if (!ok) { std::printf("00a5e7f0_TEST FAIL\n"); return 1; }
    std::printf("00a5e7f0_TEST PASS\n");
    return 0;
}