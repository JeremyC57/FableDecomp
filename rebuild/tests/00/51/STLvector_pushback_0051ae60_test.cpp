// Behaviour test for 0x0051AE60: vector<CLevelLoader::CObject::CLevelFiles>::push_back copy-constructs in place (three counted pointers add a reference) or calls _M_insert_overflow when full
#include <cstdio>
#include <new>

struct CCPPointerInfo {
    unsigned int RefCount;
    void* Data;
};

class CAFile { public: int Dummy; };

template <class T> class CCountedPointer {
public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};

class CLevelFiles {
public:
    long LevelID;
    CCountedPointer<CAFile> PTopologyFile;
    CCountedPointer<CAFile> PNavigationFile;
    CCountedPointer<CAFile> PThingFile;
};

class TestVector {
public:
    unsigned char storage[4 * sizeof(CLevelFiles)];
    CLevelFiles* _M_start;
    CLevelFiles* _M_finish;
    CLevelFiles* _M_end_of_storage;
    int overflowCalls;
    const CLevelFiles* overflowValue;
    TestVector() : overflowCalls(0), overflowValue(0)
    { _M_start = _M_finish = (CLevelFiles*)storage; _M_end_of_storage = _M_start + 2; }
    void _M_insert_overflow(CLevelFiles* pos, const CLevelFiles& x, unsigned int fill_len, bool atend)
    { ++overflowCalls; overflowValue = &x; (void)pos; (void)fill_len; (void)atend; }
    void push_back(const CLevelFiles& x)            // the reconstructed body
    {
        if (_M_finish != _M_end_of_storage) {
            new ((void*)_M_finish) CLevelFiles(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, 1, true);
    }
};

int main()
{
    static CAFile topology, navigation;
    CCPPointerInfo topoInfo = { 1, &topology };
    CCPPointerInfo navInfo = { 1, &navigation };
    CLevelFiles files;
    files.LevelID = 42;
    files.PTopologyFile.Data = &topology;   files.PTopologyFile.Info = &topoInfo;
    files.PNavigationFile.Data = &navigation; files.PNavigationFile.Info = &navInfo;
    // PThingFile stays null: its copy must skip the reference count
    TestVector v;
    v.push_back(files);
    v.push_back(files);
    if (v._M_finish - v._M_start != 2 || v._M_start[1].LevelID != 42 || v._M_start[0].PTopologyFile.Data != &topology
        || topoInfo.RefCount != 3 || navInfo.RefCount != 3 || v._M_start[1].PThingFile.Info != 0 || v.overflowCalls != 0) {
        std::printf("0051ae60_TEST FAIL\n");
        return 1;
    }
    v.push_back(files);                              // full: goes to _M_insert_overflow with the same value
    if (v.overflowCalls != 1 || v.overflowValue != &files || v._M_finish - v._M_start != 2 || topoInfo.RefCount != 3) {
        std::printf("0051ae60_TEST FAIL\n");
        return 1;
    }
    std::printf("0051ae60_TEST PASS\n");
    return 0;
}