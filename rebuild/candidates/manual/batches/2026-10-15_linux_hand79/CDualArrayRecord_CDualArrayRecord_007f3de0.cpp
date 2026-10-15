// CDualArrayRecord::~CDualArrayRecord() -- free the second array's buffer, then the first's
// Leaf: ??1CDualArrayRecord
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F3DE0, length 36 bytes (+ int3 padding).
// Callee: free @ 0x00BFEA14 (twice).
// Identity: the manifest label "CSystemRegistry::~CSystemRegistry" is propagated over byte-identical
// bodies and is wrong (CSystemRegistry owns registry handles, not heap arrays). This copy sits
// among the CAICreatureWillPowerIndicator / CCombatActionBase bodies (0x007F3C20..0x007F3F00) and has no direct caller in the image, so the owning class is not
// established: it is modelled as a descriptive 0x18-byte record of two retail CArrays -- the same body
// as CIDrawEnvironmentTextureSet::~CIDrawEnvironmentTextureSet (0x006B5B00).
//
// Shape: two member objects with the inline CArray destructor (`if (first) free(first)`), destroyed in
// reverse declaration order (+0x0c, then +0x00 -- the last one through `mov esi,[esi]`). The retail
// CArray is three pointers (0x0c bytes).
extern "C" void __cdecl free(void* block);       // 0x00BFEA14

template<class T>
struct CArray {
    T* m_first;
    T* m_last;
    T* m_end;
    ~CArray() { if (m_first) free(m_first); }
};

class CDualArrayRecord {
public:
    ~CDualArrayRecord();
    CArray<long> m_first;                        // +0x00
    CArray<long> m_second;                       // +0x0c
};

CDualArrayRecord::~CDualArrayRecord()
{
}
