// Behaviour test: CReplaceSerialise<CFileFormatBrush,COldFileFormatBrush>::ConvertPreviousUnknownVersionUp -- build a new version-2 brush record from a version-1 record (converting an older one up first)
#include <cstdio>
#include <cstring>

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

class C2DBoxI { public: int m_x0, m_y0, m_x1, m_y1; C2DBoxI() { } };
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
class COldFileFormatBrush : public CBaseSerialise<COldFileFormatBrush, 1> {
public:
    C2DBoxI GameArea;
    char    ScriptName[32];
    float   CreationTimeSeconds;
    int     CreationPattern;
};
class CFileFormatBrush : public CReplaceSerialise<CFileFormatBrush, COldFileFormatBrush> {
public:
    enum { VERSION = 2 };
    C2DBoxI GameArea;
    char    ScriptName[32];
    float   CreationTimeSeconds;
    int     CreationPattern;
    int     NumberOfThingsToMove;
    int     NumberOfThingsToDelete;
    int     NumberOfThingsToAdd;
    void ConvertFromOld(const COldFileFormatBrush* old)   // stand-in for 0x0081E4F0
    {
        GameArea = old->GameArea;
        std::memcpy(ScriptName, old->ScriptName, sizeof(ScriptName));
        CreationTimeSeconds = old->CreationTimeSeconds;
        CreationPattern = old->CreationPattern;
        NumberOfThingsToMove = NumberOfThingsToDelete = NumberOfThingsToAdd = 0;
    }
};
#pragma pack(pop)

template <> CCountedPointer<COldFileFormatBrush> CBaseSerialise<COldFileFormatBrush, 1>::ConvertPreviousUnknownVersionUp(void*)
{                                                          // stand-in for 0x00824C50
    COldFileFormatBrush* b = new COldFileFormatBrush;
    std::memset(b, 0, sizeof(*b));
    b->SerialiseVersionNumber = 1;
    b->CreationPattern = 42;
    return CCountedPointer<COldFileFormatBrush>(b);
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
    COldFileFormatBrush rec;
    std::memset(&rec, 0, sizeof(rec));
    rec.SerialiseVersionNumber = 1;
    rec.GameArea.m_x0 = 1; rec.GameArea.m_y0 = 2; rec.GameArea.m_x1 = 30; rec.GameArea.m_y1 = 40;
    std::strcpy(rec.ScriptName, "Bowerstone");
    rec.CreationTimeSeconds = 2.5f;
    rec.CreationPattern = 3;
    {
        CCountedPointer<CFileFormatBrush> up = CFileFormatBrush::ConvertPreviousUnknownVersionUp(&rec);
        if (up.object->GameArea.m_x1 != 30 || std::strcmp(up.object->ScriptName, "Bowerstone") != 0
            || up.object->CreationTimeSeconds != 2.5f || up.object->CreationPattern != 3 || up.object->NumberOfThingsToAdd != 0
            || up.control->refs != 1 || g_destroyed != 0)
            { std::printf("00825590_TEST FAIL\n"); return 1; }
    }
    rec.SerialiseVersionNumber = 0;
    {
        CCountedPointer<CFileFormatBrush> up = CFileFormatBrush::ConvertPreviousUnknownVersionUp(&rec);
        if (up.object->CreationPattern != 42 || g_destroyed != 2)
            { std::printf("00825590_TEST FAIL\n"); return 1; }
    }
    std::printf("00825590_TEST PASS\n");
    return 0;
}