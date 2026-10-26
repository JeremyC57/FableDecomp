// CInputManager::LoadAllEventPackages(const CVersion& version, CAFile& file) -- read a recorded-input file: skip its 12-byte version header, then pull event packages out of it one after another until the file position reaches its length, appending each to LoadedEventPackages; a package that fails to load stops the loop; finally rewind playback to the first package
// Leaf: LoadAllEventPackages
// where `this` is homed to [ebp-14h]: retail stores it right after `mov edi,ecx` at entry, the lane schedules the
// store after the header Read call (just before the loop). Tried: per-iteration try with `break` (this form), the
// try wrapping the loop (hoists the state-0 store to the loop pre-header, no per-iteration -1), the file calls
// declared throw(), and the Read as the for-loop init statement. Lane gap: home-store scheduling across a
// pre-loop call (QFE 4035 vs retail RTM?).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F5D70, length 156 bytes including the catch funclet @ 0x009F5DE9
// (`mov eax,009F5DEFh ; ret`); the catalogue row 009f5d70 (121 bytes) stopped at the funclet and was split via the
// skipped-row ledger, chunk 908. Caller: CInputManager::StreamInputsFromFile @ 0x009F5E10 (landed).
// Callees: CAFile virtual slots 3 Read(void*, long, CASuspendableProcess*) (+0x0c), 7 GetPosition (+0x1c), 9 GetLength
// (+0x24) (CDiskFileWin32 vtable 0x0122D06C: 0x0099A960 / 0x0099ABD0 / 0x00401670), CInputEventPackage::CInputEventPackage(CAFile*)
// @ 0x009F5690 (manifest label "Save", propagated), vector<CInputEventPackage>::push_back @ 0x009F6FE0,
// ~CInputEventPackage @ 0x009F6A70 (manifest label "CCombatWheel::ResetRings", propagated).
// Layout: retail CInputManager (0x80 bytes, 12-byte vectors; GOTCHAS wave 24m): LoadedEventPackages +0x4c,
// CurrentFilePackageIndex +0x58. CInputEventPackage is 48 bytes (Events +0, TimeStamp +0xc, Checksum +0x10,
// MousePos +0x20, JoystickPos +0x28). The `version` argument is never read.
// Shape: the module is built WITHOUT /EHsc (the try/catch compiles with C4530, no unwind semantics -- under /EHsc the
// lane schedules the `this` home store after the header Read and the state-0 store after `push esi`); EH frame (state
// -1 at entry); the try block starts at function entry, so `this` is spilled to [ebp-14h]
// right after the prologue for the catch continuation; GetLength is called before GetPosition (right operand
// first) and kept in ebx. Read, GetPosition, GetLength and push_back are declared non-throwing, so the try state 0
// is first stored just before the package constructor (the only call that can throw), the package needs no state
// of its own, and -1 is restored right after its destructor. The catch body is empty: its continuation
// (0x009F5DEF) reloads `this` and falls into the index reset after the loop.
class CVersion { public:                                  // 12 bytes, read straight from the file
    long Major;
    long Minor;
    long Build;
};
class CASuspendableProcess;
class CAFile { public:                                    // abstract file (vtable 0x0122ECCC is all __purecall)
    virtual ~CAFile();                                                                   // slot 0
    virtual void Open(const void* pathname, long access, unsigned long flags);          // slot 1
    virtual void Close();                                                                // slot 2
    virtual void Read(void* buffer, long size, CASuspendableProcess* process) throw();  // slot 3 (+0x0c)
    virtual void Write(const void* buffer, long size, CASuspendableProcess* process);   // slot 4
    virtual void SetPosition(long position);                                            // slot 5
    virtual void MovePosition(long delta);                                              // slot 6
    virtual long GetPosition() throw();                                                 // slot 7 (+0x1c)
    virtual void SetLength(long length);                                                // slot 8
    virtual long GetLength() throw();                                                   // slot 9 (+0x24)
};
class CInputEventPackage { public:                        // 48 bytes: one frame's worth of recorded input
    unsigned char _events[0x0c];                   // +0x00 Events (vector<CInputEvent>)
    long          TimeStamp;                       // +0x0c
    unsigned char _rest[0x20];                     // +0x10 Checksum, MousePos, JoystickPos
    CInputEventPackage(CAFile* file);              // 0x009F5690: zero the vector, then read the package from the file
    ~CInputEventPackage();                         // 0x009F6A70
};
struct CInputEventPackageVector {                  // vector<CInputEventPackage> (12 bytes)
    CInputEventPackage* m_start;
    CInputEventPackage* m_finish;
    CInputEventPackage* m_end_of_storage;
    void push_back(const CInputEventPackage& package) throw();   // 0x009F6FE0 (grow-and-copy, treated as non-throwing)
};

class CInputManager { public:
    virtual void v00();
    unsigned char            _pad04[0x48];         // +0x04 vectors, counters, devices, flags, Checksum
    CInputEventPackageVector LoadedEventPackages;  // +0x4c
    long                     CurrentFilePackageIndex;   // +0x58 next package to play back
    void LoadAllEventPackages(const CVersion& version, CAFile& file);
};

void CInputManager::LoadAllEventPackages(const CVersion& version, CAFile& file)
{
    CVersion fileVersion;
    file.Read(&fileVersion, sizeof(CVersion), 0);
    while (file.GetPosition() < file.GetLength()) {
        try {
            CInputEventPackage package(&file);
            LoadedEventPackages.push_back(package);
        }
        catch (...) {
            break;                                 // a bad package ends the load
        }
    }
    CurrentFilePackageIndex = 0;
}

// ---- test harness ----
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static long g_pos = 0, g_len = 0, g_readSize = 0;
static int g_built = 0, g_pushed = 0, g_dtor = 0, g_throwAt = -1;
CAFile::~CAFile() { }
void CAFile::Open(const void*, long, unsigned long) { }
void CAFile::Close() { }
void CAFile::Read(void*, long size, CASuspendableProcess*) throw() { g_readSize = size; g_pos += size; }
void CAFile::Write(const void*, long, CASuspendableProcess*) { }
void CAFile::SetPosition(long position) { g_pos = position; }
void CAFile::MovePosition(long delta) { g_pos += delta; }
long CAFile::GetPosition() throw() { return g_pos; }
void CAFile::SetLength(long length) { g_len = length; }
long CAFile::GetLength() throw() { return g_len; }
CInputEventPackage::CInputEventPackage(CAFile* file)
{
    if (g_built++ == g_throwAt) throw 1;
    TimeStamp = g_built;
    file->MovePosition(48);
}
CInputEventPackage::~CInputEventPackage() { ++g_dtor; }
void CInputEventPackageVector::push_back(const CInputEventPackage& package) throw() { ++g_pushed; }
void CInputManager::v00() { }

static void reset(int throwAt) { g_pos = 0; g_len = 12 + 48 * 3; g_built = g_pushed = g_dtor = 0; g_throwAt = throwAt; }

int main()
{
    static CAFile file;
    static CInputManager m;
    CVersion v = { 1, 2, 3 };
    reset(-1);
    m.CurrentFilePackageIndex = 5;
    m.LoadAllEventPackages(v, file);
    CHECK(g_readSize == 12 && g_built == 3 && g_pushed == 3 && g_dtor == 3 && m.CurrentFilePackageIndex == 0);
    CHECK(g_pos == g_len);
    reset(1);                                        // the second package fails: the load stops there
    m.CurrentFilePackageIndex = 5;
    m.LoadAllEventPackages(v, file);
    CHECK(g_built == 2 && g_pushed == 1 && g_dtor == 1 && m.CurrentFilePackageIndex == 0);
    reset(-1);
    g_len = 12;                                      // header only
    m.CurrentFilePackageIndex = 2;
    m.LoadAllEventPackages(v, file);
    CHECK(g_built == 0 && g_pushed == 0 && m.CurrentFilePackageIndex == 0);
    if (fails == 0) printf("009f5d70_TEST PASS\n");
    return fails;
}