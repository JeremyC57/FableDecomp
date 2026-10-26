// Behaviour test: CReplaceSerialise<CFileFormatHeightMapCell,COldFileFormatHeightMapCell6>::ConvertToThisVersion -- an older height-map cell is replaced in place by its up-converted copy
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

#pragma pack(push, 1)
template <class T, class TOld> class CReplaceSerialise {
public:
    unsigned int  ByteLength;
    unsigned char SerialiseVersionNumber;
    static unsigned short GetClassVersionNumber() { return T::VERSION; }
    static CCountedPointer<T> ConvertPreviousUnknownVersionUp(void* data);
    static void ConvertToThisVersion(void* data)
    {
        if (((CReplaceSerialise<T, TOld>*)data)->SerialiseVersionNumber != GetClassVersionNumber()) {
            *(T*)data = *ConvertPreviousUnknownVersionUp(data);
        }
    }
};
class COld;
class CFileFormatHeightMapCell : public CReplaceSerialise<CFileFormatHeightMapCell, COld> {
public:
    enum { VERSION = 7 };
    float Height;
    unsigned char TextureBlendAlpha;
    unsigned char EngineTheme[3];
    unsigned char EngineBlend[2];
    unsigned char Passable;
    unsigned char CameraPassable;
    signed char SoundLayerPaletteIndex;
    signed char MinimapZone;
    bool ShorePoint;
    bool NavPreferred;
};
#pragma pack(pop)

static int g_converted = 0;
template <> CCountedPointer<CFileFormatHeightMapCell> CReplaceSerialise<CFileFormatHeightMapCell, COld>::ConvertPreviousUnknownVersionUp(void*)
{                                                         // stand-in: the up-converted copy of the record
    ++g_converted;
    CFileFormatHeightMapCell* r = new CFileFormatHeightMapCell;
    std::memset(r, 0, sizeof(*r));
    r->ByteLength = sizeof(CFileFormatHeightMapCell);
    r->SerialiseVersionNumber = 7;
    r->Height = 12.5f;
    r->TextureBlendAlpha = 0x80;
    r->NavPreferred = true;
    return CCountedPointer<CFileFormatHeightMapCell>(r);
}

int main()
{
    CFileFormatHeightMapCell rec;
    std::memset(&rec, 0, sizeof(rec));
    rec.SerialiseVersionNumber = 7;
    CFileFormatHeightMapCell::ConvertToThisVersion(&rec);                     // current: untouched
    if (g_converted != 0 || rec.ByteLength != 0)
        { std::printf("008273d0_TEST FAIL\n"); return 1; }
    rec.SerialiseVersionNumber = 1;
    CFileFormatHeightMapCell::ConvertToThisVersion(&rec);                     // old: overwritten in place with the converted copy, which is released
    if (g_converted != 1 || g_destroyed != 1 || rec.SerialiseVersionNumber != 7 || rec.ByteLength != sizeof(CFileFormatHeightMapCell) || !(rec.Height == 12.5f && rec.TextureBlendAlpha == 0x80 && rec.NavPreferred))
        { std::printf("008273d0_TEST FAIL\n"); return 1; }
    std::printf("008273d0_TEST PASS\n");
    return 0;
}