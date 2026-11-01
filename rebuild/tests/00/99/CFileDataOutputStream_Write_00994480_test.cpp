#include "engine/CBaseClassNonCopyable.h"
#include <cstdio>
// 00994480 test (naming wave 27, family fdosw): CFileDataOutputStream::Write is the retail virtual (slot 4); called qualified.
class CAFile : public CBaseClassNonCopyable { public:
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual void v3() = 0;
    virtual void v4(const void* p, long n, int zero) = 0;
};
class CDataStream : public CBaseClassNonCopyable { public: virtual ~CDataStream(); };
class CDataOutputStream : public CDataStream { public:
    void* NewLine;
    virtual void SetPosition(unsigned long position) = 0;
    virtual unsigned long GetPosition() const = 0;
    virtual void Close() = 0;
    virtual void Write(const void* data, long length) = 0;
};
#define FABLE_RETAIL_FILE_DATA_OUTPUT_STREAM
#include "engine/CFileDataOutputStream.h"

static CAFile* g_self;
static long   g_n;
static const void* g_p;
static int    g_zero = 99;

struct SinkImpl : CAFile {
    void v1() {}
    void v2() {}
    void v3() {}
    void v4(const void* p, long n, int zero) { g_self = this; g_n = n; g_p = p; g_zero = zero; }
};
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}

int main()
{
    SinkImpl sink;
    struct { void* vptr; void* newLine; CAFile* file; } raw = { 0, 0, &sink };
    CFileDataOutputStream* s = reinterpret_cast<CFileDataOutputStream*>(&raw);
    char buf[4];
    s->CFileDataOutputStream::Write(&buf[0], 42);
    if (g_self == &sink && g_n == 42 && g_p == &buf[0] && g_zero == 0)
        printf("00994480_TEST PASS\n");
    else
        printf("FAIL self=%p n=%ld p=%p zero=%d\n", (void*)g_self, g_n, g_p, g_zero);
    return 0;
}
