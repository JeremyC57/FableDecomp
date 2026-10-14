#include <stdio.h>
#include <stddef.h>
static int g_assign, g_wdtors, g_resets; static void* g_resetArg; static void* g_assignedFrom;
struct CWideString {
    void* m_pStringData;
    CWideString() : m_pStringData(0) {}
    CWideString(const CWideString& o) : m_pStringData(o.m_pStringData) {}
    CWideString& operator=(const CWideString& other) { ++g_assign; g_assignedFrom = other.m_pStringData; m_pStringData = other.m_pStringData; return *this; }
    ~CWideString() { ++g_wdtors; }
};
struct CAFile {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {} virtual void v06() {}
    virtual long GetLength() { return 777; }
    virtual void v08() {} virtual void v09() {} virtual void v10() {}
    virtual CWideString GetPathname() { CWideString s; s.m_pStringData = (void*)0xABC; return s; }
    virtual void v12() {} virtual void v13() {}
    virtual CAFile* GetSharedFile() { return this; }
};
struct CCountedPointer_CAFile { CAFile* m_pObject; void* m_pCount; void Reset(CAFile* file) { ++g_resets; g_resetArg = file; m_pObject = file; } };
struct CCachedFile {
    void* _vptr; long Position; long BufferStartPosition; unsigned char DataBuffer[0x0c]; CWideString Pathname; CCountedPointer_CAFile PDiskFile; bool Opened; bool BufferValid;
    void OpenFromFile(CAFile* file, long mode);
};
void CCachedFile::OpenFromFile(CAFile* file, long mode)
{
    Pathname = file->GetPathname(); Position = file->GetLength(); Opened = true; BufferValid = false; PDiskFile.Reset(file->GetSharedFile());
}
int main()
{
    CAFile f; CCachedFile* c = new CCachedFile; c->Position = 0; c->Opened = false; c->BufferValid = true; c->PDiskFile.m_pObject = 0;
    int dtorsBefore = g_wdtors;
    c->OpenFromFile(&f, 3);
    bool ok = g_assign == 1 && g_assignedFrom == (void*)0xABC && c->Pathname.m_pStringData == (void*)0xABC && c->Position == 777 && c->Opened && !c->BufferValid && g_resets == 1 && g_resetArg == &f
        && g_wdtors - dtorsBefore >= 1 && offsetof(CCachedFile, PDiskFile) == 0x1c && offsetof(CCachedFile, Opened) == 0x24 && offsetof(CCachedFile, Pathname) == 0x18;
    if (!ok) { printf("00a5b210_TEST FAIL\n"); return 1; }
    printf("00a5b210_TEST PASS\n"); return 0;
}
