// Behaviour test for 0x00826380 CCachedFile::~CCachedFile: the destructor closes the file first, then releases PDiskFile (count
// decrement; at zero the deleter runs and the block is deleted; both fields nulled), destroys Pathname, frees DataBuffer and runs
// the base destructor -- in that order.
#include <cstdio>
#include <cstdlib>
static int g_order[8]; static int g_n = 0;
static void Note(int what) { if (g_n < 8) g_order[g_n++] = what; }
enum { CLOSE = 1, DELETER = 2, STRING = 3, FREE = 4, BASE = 5 };
class CCPPointerInfo { public:
    unsigned long RefCount;
    void (*DeleteFunc)(void*);
    void* Data;
};
static void Deleter(void*) { Note(DELETER); }
template <class T> class CCountedPointer { public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CWideString { public: void* PStringData; CWideString() : PStringData(0) { } ~CWideString() { Note(STRING); } };
template <class T> class CArray { public:
    T* Start; T* Finish; T* EndOfStorage;
    CArray() : Start(0), Finish(0), EndOfStorage(0) { }
    ~CArray() { if (Start) { Note(FREE); std::free(Start); } }
};
class CBaseClassNonCopyable { public: virtual ~CBaseClassNonCopyable() { Note(BASE); } };
class CAFile : public CBaseClassNonCopyable { public: virtual void Close() = 0; };
class CCachedFile : public CAFile { public:
    long                    Position;
    long                    BufferStartPosition;
    CArray<unsigned char>   DataBuffer;
    CWideString             Pathname;
    CCountedPointer<CAFile> PDiskFile;
    bool                    Opened;
    bool                    BufferValid;
    CCachedFile() : Position(0), BufferStartPosition(0), Opened(true), BufferValid(false) { }
    virtual void Close() { Note(CLOSE); Opened = false; }
    virtual ~CCachedFile() { Close(); }
};
int main()
{
    CCPPointerInfo* info = new CCPPointerInfo;
    info->RefCount = 1; info->DeleteFunc = Deleter; info->Data = 0;
    {
        CCachedFile file;
        file.PDiskFile.Info = info;
        file.DataBuffer.Start = (unsigned char*)std::malloc(16);
    }
    static const int want[] = { CLOSE, DELETER, STRING, FREE, BASE };
    int ok = (g_n == 5);
    for (int i = 0; ok && i < 5; ++i) if (g_order[i] != want[i]) ok = 0;
    g_n = 0;
    { CCachedFile empty; }                                   // nothing to release or free
    static const int want2[] = { CLOSE, STRING, BASE };
    if (g_n != 3) ok = 0;
    for (int i = 0; ok && i < 3; ++i) if (g_order[i] != want2[i]) ok = 0;
    if (!ok) { std::printf("00826380_TEST FAIL\n"); return 1; }
    std::printf("00826380_TEST PASS\n");
    return 0;
}