#include <cstdio>

static int g_order[8];
static int g_n = 0;

class CCharString {
public:
    ~CCharString() { g_order[g_n++] = 1; }
    void* PStringData;
};

class CAFile;

class CBaseClassNonCopyable {
protected:
    CBaseClassNonCopyable() { }
    virtual ~CBaseClassNonCopyable() { g_order[g_n++] = 2; }
};

class CDataStream : public CBaseClassNonCopyable {
public:
    virtual ~CDataStream() { }
};

class __declspec(novtable) CDataOutputStream : public CDataStream {
public:
    CDataOutputStream() { NewLine.PStringData = 0; }
    virtual ~CDataOutputStream() { }
    CCharString NewLine;
};

class CFileDataOutputStream : public CDataOutputStream {
public:
    CFileDataOutputStream() : File(0) { }
    virtual ~CFileDataOutputStream();
    CAFile* File;
};

CFileDataOutputStream::~CFileDataOutputStream()
{
}

int main()
{
    CDataStream* s = new CFileDataOutputStream;
    delete s;
    int ok = g_n == 2 && g_order[0] == 1 && g_order[1] == 2 && sizeof(CFileDataOutputStream) == 12;
    std::printf(ok ? "00994430_TEST PASS\n" : "00994430_TEST FAIL\n");
    return ok ? 0 : 1;
}
