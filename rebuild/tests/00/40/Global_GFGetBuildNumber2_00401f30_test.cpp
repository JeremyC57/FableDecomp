#include <stdio.h>
#include <stddef.h>
// Behaviour spies for the callees GFGetBuildNumber2 (0x00401F30) binds: the two string classes' ctors and
// destructors, CWideString::PathExists, the CDiskFileWin32 ctor/Close and the base destructor.
class CAFile;
class CWideString
{
public:
    CWideString(const wchar_t* text);
    ~CWideString();
    bool PathExists() const;
    void* PStringData;
};
class CCharString
{
public:
    CCharString(CAFile* file, long length);
    ~CCharString();
    const CCharString& operator=(const CCharString& other);
    void* PStringData;
};
class CBaseClassNonCopyable
{
public:
    ~CBaseClassNonCopyable();
    void** Vftable;
};
class CAFile : public CBaseClassNonCopyable
{
};
#pragma pack(push, 1)
class CDiskFileWin32 : public CAFile
{
public:
    CDiskFileWin32(const CWideString& path, int accessMode, int shareMode);
    void Close();
    unsigned char Unknown04[0x14];
    CWideString Pathname;
};
#pragma pack(pop)
void __cdecl GFGetBuildNumber2();
enum BuildEvent
{
    EVENT_ASSIGN_WIDE,
    EVENT_PATH_EXISTS,
    EVENT_DESTROY,
    EVENT_DISK_CONSTRUCT,
    EVENT_FILE_CONSTRUCT,
    EVENT_ASSIGN_STRING,
    EVENT_DISK_CLOSE,
    EVENT_DISK_BASE_CLEANUP
};

struct BuildCall
{
    BuildEvent Event;
    const void* Self;
    const void* Argument;
    int ValueA;
    int ValueB;
};

CCharString FableFullBuildVersion(0, 0);
void* FableDiskFileWin32Vftable[1];
static BuildCall gCalls[20];
static int gCallCount;
static bool gPathExists;

static void Record(BuildEvent event, const void* self, const void* argument, int valueA, int valueB)
{
    if (gCallCount >= 20) return;
    BuildCall& call = gCalls[gCallCount++];
    call.Event = event;
    call.Self = self;
    call.Argument = argument;
    call.ValueA = valueA;
    call.ValueB = valueB;
}

CWideString::CWideString(const wchar_t* text)
{
    if (text == 0) return;          // the spy file's own Pathname member below
    Record(EVENT_ASSIGN_WIDE, this, text, 0, 0);
}

bool CWideString::PathExists() const
{
    Record(EVENT_PATH_EXISTS, this, 0, 0, 0);
    return gPathExists;
}

CWideString::~CWideString()
{
    Record(EVENT_DESTROY, this, 0, 0, 0);
}

CCharString::~CCharString()
{
    Record(EVENT_DESTROY, this, 0, 0, 0);
}

CDiskFileWin32::CDiskFileWin32(const CWideString& path, int accessMode, int shareMode) : Pathname(0)
{
    Record(EVENT_DISK_CONSTRUCT, this, &path, accessMode, shareMode);
}

CCharString::CCharString(CAFile* file, long length)
{
    Record(EVENT_FILE_CONSTRUCT, this, file, length, 0);
}

const CCharString& CCharString::operator=(const CCharString& other)
{
    Record(EVENT_ASSIGN_STRING, this, &other, 0, 0);
    return *this;
}

void CDiskFileWin32::Close()
{
    Record(EVENT_DISK_CLOSE, this, Vftable, 0, 0);
}

CBaseClassNonCopyable::~CBaseClassNonCopyable()
{
    Record(EVENT_DISK_BASE_CLEANUP, this, 0, 0, 0);
}

int main()
{
    gCallCount = 0;
    gPathExists = false;
    GFGetBuildNumber2();
    if (gCallCount != 3) return 1;
    if (gCalls[0].Event != EVENT_ASSIGN_WIDE) return 2;
    if (gCalls[1].Event != EVENT_PATH_EXISTS) return 3;
    if (gCalls[2].Event != EVENT_DESTROY) return 4;

    gCallCount = 0;
    gPathExists = true;
    GFGetBuildNumber2();
    if (gCallCount != 12) return 5;
    if (gCalls[0].Event != EVENT_ASSIGN_WIDE ||
        gCalls[1].Event != EVENT_PATH_EXISTS ||
        gCalls[2].Event != EVENT_DESTROY) return 6;
    if (gCalls[3].Event != EVENT_ASSIGN_WIDE) return 7;
    if (gCalls[4].Event != EVENT_DISK_CONSTRUCT ||
        gCalls[4].ValueA != 1 || gCalls[4].ValueB != 2) return 8;
    if (gCalls[5].Event != EVENT_DESTROY) return 9;
    if (gCalls[6].Event != EVENT_FILE_CONSTRUCT || gCalls[6].ValueA != -1) return 10;
    if (gCalls[7].Event != EVENT_ASSIGN_STRING ||
        gCalls[7].Self != &FableFullBuildVersion ||
        gCalls[7].Argument != gCalls[6].Self) return 11;
    if (gCalls[8].Event != EVENT_DESTROY || gCalls[8].Self != gCalls[6].Self) return 12;
    if (gCalls[9].Event != EVENT_DISK_CLOSE ||
        gCalls[9].Argument != FableDiskFileWin32Vftable) return 13;
    if (gCalls[10].Event != EVENT_DESTROY) return 14;
    if (gCalls[11].Event != EVENT_DISK_BASE_CLEANUP ||
        gCalls[11].Self != gCalls[9].Self) return 15;

    printf("GF_GET_BUILD_NUMBER_2_TEST PASS\n");
    return 0;
}
