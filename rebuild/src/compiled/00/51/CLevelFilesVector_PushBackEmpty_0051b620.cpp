// CLevelFilesVector::PushBackEmpty() -- append a default-constructed CLevelFiles record to the vector
// Leaf: PushBackEmpty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051B620, length 50 bytes (+ int3 padding).
// Callees: vector<CLevelLoader::CObject::CLevelFiles>::push_back(const CLevelFiles&) @ 0x0051AE60 (it copies
// the record into `last` when there is room: LevelID, then the three counted pointers with their add-refs),
// CLevelLoader::CObject::CLevelFiles::~CLevelFiles @ 0x0050F980.
// Identity: the manifest label "CTCActionUseBuyHouse::OnCreate" is propagated and wrong. `this` is handed to
// push_back untouched (ecx is never reloaded), so the function is a member of the retail vector of
// CLevelFiles itself (first/last/end -- the CLevelLoader::CObject::RegionLevelFiles element type); it has no
// direct caller in the image. The class and method names are descriptive.
//
// Layout = Ego_r.pdb CLevelFiles (0x1c bytes): LevelID +0x00, then three CCountedPointer<CAFile>
// {object, count block} at +0x04 / +0x0c / +0x14 (PTopologyFile, PNavigationFile, PThingFile).
// Shape: the temporary is built in the 0x1c-byte frame: the implicit constructor leaves LevelID
// uninitialised and the three counted pointers' inline constructors zero their six words from one
// register; it is passed by reference and destroyed through the out-of-line destructor.
struct CAFile;

template<class T>
struct CCountedPointer {
    T*    m_pObject;
    void* m_pBlock;
    CCountedPointer() : m_pObject(0), m_pBlock(0) { }
};

struct CLevelFiles {
    long                    LevelID;            // +0x00
    CCountedPointer<CAFile> PTopologyFile;      // +0x04
    CCountedPointer<CAFile> PNavigationFile;    // +0x0c
    CCountedPointer<CAFile> PThingFile;         // +0x14
    ~CLevelFiles();                             // 0x0050F980
};

struct CLevelFilesVector {                      // retail std::vector<CLevelFiles>
    CLevelFiles* m_first;
    CLevelFiles* m_last;
    CLevelFiles* m_end;
    void push_back(const CLevelFiles& files);   // 0x0051AE60
    void PushBackEmpty();
};

void CLevelFilesVector::PushBackEmpty()
{
    push_back(CLevelFiles());
}