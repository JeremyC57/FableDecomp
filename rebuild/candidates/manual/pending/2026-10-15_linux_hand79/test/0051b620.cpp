// Behaviour test for CLevelFilesVector::PushBackEmpty @ 0x0051B620: one record is appended whose three
// counted file pointers are empty; the temporary is destroyed after the copy.
#include <cstdio>

struct CAFile { int unused; };
static int g_destroyed = 0;

template<class T>
struct CCountedPointer {
    T*    m_pObject;
    void* m_pBlock;
    CCountedPointer() : m_pObject(0), m_pBlock(0) { }
};

struct CLevelFiles {
    long                    LevelID;
    CCountedPointer<CAFile> PTopologyFile;
    CCountedPointer<CAFile> PNavigationFile;
    CCountedPointer<CAFile> PThingFile;
    ~CLevelFiles() { ++g_destroyed; }
};

struct CLevelFilesVector {
    CLevelFiles m_items[4];
    int m_count;
    void push_back(const CLevelFiles& files)
    {
        CLevelFiles& d = m_items[m_count++];
        d.LevelID = 42;                         // marker: the copy happened
        d.PTopologyFile = files.PTopologyFile;
        d.PNavigationFile = files.PNavigationFile;
        d.PThingFile = files.PThingFile;
    }
    void PushBackEmpty() { push_back(CLevelFiles()); }
};

int main()
{
    CLevelFilesVector v;
    v.m_count = 0;
    static CAFile f;
    for (int i = 0; i < 4; ++i) {
        v.m_items[i].PTopologyFile.m_pObject = &f;
        v.m_items[i].PNavigationFile.m_pObject = &f;
        v.m_items[i].PThingFile.m_pObject = &f;
    }
    int before = g_destroyed;
    v.PushBackEmpty();
    v.PushBackEmpty();
    bool ok = v.m_count == 2 && g_destroyed - before == 2;
    for (int i = 0; i < 2; ++i)
        if (v.m_items[i].LevelID != 42 || v.m_items[i].PTopologyFile.m_pObject != 0
            || v.m_items[i].PNavigationFile.m_pObject != 0 || v.m_items[i].PThingFile.m_pObject != 0
            || v.m_items[i].PThingFile.m_pBlock != 0) ok = false;
    if (v.m_items[2].PThingFile.m_pObject != &f) ok = false;
    if (!ok) { std::printf("0051b620_TEST FAIL\n"); return 1; }
    std::printf("0051b620_TEST PASS\n");
    return 0;
}
