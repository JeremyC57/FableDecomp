// Behaviour test for CDataStream::~CDataStream (0x00993B10): destroying a stream runs the CDataStream destructor and
// then the CBaseClassNonCopyable one, in that order.
#include <cstdio>

static int g_order = 0, g_streamAt = 0, g_baseAt = 0;

class CBaseClass {
public:
    virtual ~CBaseClass() { }
};

class CBaseClassNonCopyable : public CBaseClass {
protected:
    virtual ~CBaseClassNonCopyable() { g_baseAt = ++g_order; }
};

class CDataStream : public CBaseClassNonCopyable {
public:
    virtual ~CDataStream();
    virtual void Slot1() = 0;
    virtual void Slot2() = 0;
    virtual void Slot3() = 0;
};

CDataStream::~CDataStream()
{
    g_streamAt = ++g_order;
}

class CTestStream : public CDataStream {
public:
    void Slot1() { }
    void Slot2() { }
    void Slot3() { }
};

int main()
{
    CDataStream* s = new CTestStream;
    delete s;
    if (g_streamAt == 1 && g_baseAt == 2) {
        std::printf("marker_dtor_ok\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}
