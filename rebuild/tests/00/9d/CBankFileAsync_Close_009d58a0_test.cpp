// Behaviour test for 0x009D58A0 CBankFileAsync::Close(): the threaded file is closed only when it exists and the bank is not
// in retail mode; the CBankFile base Close always runs.
#include <cstdio>

static int g_threadClosed = 0;
static int g_baseClosed = 0;

class CThreadedFile {
public:
    void Close() { ++g_threadClosed; }
};
template <class T> class CCountedPointer {
public:
    CCountedPointer() : Data(0) { }
    T* operator->() const { return Data; }
    bool IsNull() const { return Data == 0; }
    T* Data;
};
class CBankFile {
public:
    virtual ~CBankFile() { }
    virtual void Close() { ++g_baseClosed; }
    bool RetailMode;
};
class CBankFileAsync : public CBankFile {
public:
    virtual void Close()
    {
        if (!ThreadedFile.IsNull() && !RetailMode)
            ThreadedFile->Close();
        CBankFile::Close();
    }
    CCountedPointer<CThreadedFile> ThreadedFile;
};

int main()
{
    CThreadedFile file;
    CBankFileAsync bank;
    CBankFile* base = &bank;
    bank.RetailMode = false;
    base->Close();                                  // no threaded file
    bank.ThreadedFile.Data = &file;
    bank.RetailMode = true;
    base->Close();                                  // retail mode: skip
    bank.RetailMode = false;
    base->Close();                                  // closes the threaded file
    if (g_threadClosed != 1 || g_baseClosed != 3) {
        std::printf("009d58a0_TEST FAIL\n");
        return 1;
    }
    std::printf("009d58a0_TEST PASS\n");
    return 0;
}