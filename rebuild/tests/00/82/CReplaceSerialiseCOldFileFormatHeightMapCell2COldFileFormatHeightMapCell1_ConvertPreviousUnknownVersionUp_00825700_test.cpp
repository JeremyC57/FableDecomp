// Behaviour test: CReplaceSerialise<COldFileFormatHeightMapCell2,COldFileFormatHeightMapCell1>::ConvertPreviousUnknownVersionUp -- build a new version-2 cell from a version-1 record (converting an older record up first), default alpha 0x80
#include <cstdio>

class CCPPointerInfo { public:
    unsigned int refs;
    void (*destroy)(void*);
    void* object;
    CCPPointerInfo(void* p, void (*d)(void*)) : refs(1), destroy(d), object(p) { }
};
static int g_destroyed = 0;
template <class T> class CCountedPointer {
public:
    T*              object;
    CCPPointerInfo* control;
    static void DeleteData(void* data) { ++g_destroyed; delete (T*)data; }
    CCountedPointer(T* p) : object(p), control(p ? new CCPPointerInfo(p, DeleteData) : 0) { }
    CCountedPointer(const CCountedPointer& x) : object(x.object), control(x.control) { if (control) ++control->refs; }
    CCountedPointer& operator=(const CCountedPointer& x)
    {
        CCPPointerInfo* c = x.control;
        T* o = x.object;
        if (control != c) {
            if (control && --control->refs == 0) {
                control->destroy(control->object);
                delete control;
            }
            object = o;
            control = c;
            if (control)
                ++control->refs;
        }
        return *this;
    }
    ~CCountedPointer()
    {
        if (control) {
            --control->refs;
            if (control->refs == 0) {
                control->destroy(control->object);
                delete control;
            }
        }
    }
    T& operator*() const { return *object; }
};

#pragma pack(push, 1)
template <class T, long V> class CBaseSerialise {
public:
    unsigned int  ByteLength;
    unsigned char SerialiseVersionNumber;
    static unsigned short GetClassVersionNumber() { return (unsigned short)V; }
    static CCountedPointer<T> ConvertPreviousUnknownVersionUp(void* data);
};
template <class T, class TOld> class CReplaceSerialise {
public:
    unsigned int  ByteLength;
    unsigned char SerialiseVersionNumber;
    static unsigned short GetClassVersionNumber() { return T::VERSION; }
    static CCountedPointer<T> ConvertPreviousUnknownVersionUp(void* data);
};
class Cell1 : public CBaseSerialise<Cell1, 1> { public: float Height; };
class Cell2 : public CReplaceSerialise<Cell2, Cell1> {
public:
    enum { VERSION = 2 };
    float         Height;
    unsigned char TextureBlendAlpha;
    void ConvertFromOld(const Cell1* old) { Height = old->Height; TextureBlendAlpha = 0x80; }
};
#pragma pack(pop)

// stand-in for CBaseSerialise<Cell1,1>::ConvertPreviousUnknownVersionUp 0x00824D30: an unknown older record becomes height 7
template <> CCountedPointer<Cell1> CBaseSerialise<Cell1, 1>::ConvertPreviousUnknownVersionUp(void*)
{
    Cell1* c = new Cell1;
    c->SerialiseVersionNumber = 1;
    c->Height = 7.0f;
    return CCountedPointer<Cell1>(c);
}

template <class T, class TOld>
CCountedPointer<T> CReplaceSerialise<T, TOld>::ConvertPreviousUnknownVersionUp(void* data)
{
    CCountedPointer<T> result(new T);
    CCountedPointer<TOld> held((TOld*)0);
    TOld* old = (TOld*)data;
    if (((TOld*)data)->SerialiseVersionNumber != TOld::GetClassVersionNumber()) {
        held = TOld::ConvertPreviousUnknownVersionUp(data);
        old = held.object;
    }
    result.object->ConvertFromOld(old);
    return result;
}

int main()
{
    Cell1 rec;
    rec.ByteLength = sizeof(Cell1);
    rec.SerialiseVersionNumber = 1;
    rec.Height = 3.5f;
    {
        CCountedPointer<Cell2> up = Cell2::ConvertPreviousUnknownVersionUp(&rec);
        if (!up.object || up.object->Height != 3.5f || up.object->TextureBlendAlpha != 0x80 || up.control->refs != 1 || g_destroyed != 0)
            { std::printf("00825700_TEST FAIL\n"); return 1; }
    }
    if (g_destroyed != 1)                                  // the result released at scope end
        { std::printf("00825700_TEST FAIL\n"); return 1; }
    rec.SerialiseVersionNumber = 0;                        // older than version 1: converted up through the base first
    {
        CCountedPointer<Cell2> up = Cell2::ConvertPreviousUnknownVersionUp(&rec);
        if (up.object->Height != 7.0f || up.object->TextureBlendAlpha != 0x80 || g_destroyed != 2)   // the held version-1 copy is gone
            { std::printf("00825700_TEST FAIL\n"); return 1; }
    }
    std::printf("00825700_TEST PASS\n");
    return 0;
}