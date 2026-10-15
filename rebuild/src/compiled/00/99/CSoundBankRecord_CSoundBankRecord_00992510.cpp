// CSoundBankRecord::~CSoundBankRecord() -- destructor of a 0x20-byte sound-bank record: four strings, then two counted pointers
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00992510, length 120 bytes.
// Callees: CCharString::~CCharString() @ 0x0099EAE0 (x4), operator delete @ 0x00BFE9BC (count blocks).
// Label correction: the manifest's "CEditFractal::~CEditFractal" is a propagated name. The record is the value half (+4) of the 0x24-byte
// elements of the vector at +0x30 of the sound manager whose destructor is 0x00991880 (vtable 0x0129A574, RTTI "CASoundManager"); the
// four strings are built by RegisterSoundBanks (0x009919C0, "Register Sound Bank 1/2"). The sound module is absent from the Ego PDB, so the
// class and member names are descriptive: two counted pointers at +0x00 / +0x08, four CCharStrings at +0x10..+0x1c.
// Shape: no vtable store; members destroyed in reverse order with one zero register (edi); each counted pointer is
// `if (block) { --count; if (count == 0) { deleter(object); delete block; } }` re-reading the block, then object = 0, block = 0.
// Leaf: ??1CSoundBankRecord
struct SCountBlock {                         // the shared count block of CCountedPointer: count, per-type deleter, object
    long  m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*);  // +4 (called with the object in ecx)
    void* m_pObject;                         // +8
};

template <class T>
struct CCountedPointer {                     // {object, count block}
    T*           m_pObject;
    SCountBlock* m_pCount;
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

class CCharString {
public:
    ~CCharString();                          // 0x0099EAE0
    struct CCharStringData* PStringData;
};

struct CSoundBank;
struct CSoundBankData;

class CSoundBankRecord {
public:
    CCountedPointer<CSoundBank>     m_pBank;         // +0x00
    CCountedPointer<CSoundBankData> m_pData;         // +0x08
    CCharString                     m_name;          // +0x10
    CCharString                     m_fileName;      // +0x14
    CCharString                     m_dataFileName;  // +0x18
    CCharString                     m_description;   // +0x1c
    ~CSoundBankRecord();
};

CSoundBankRecord::~CSoundBankRecord()
{
}