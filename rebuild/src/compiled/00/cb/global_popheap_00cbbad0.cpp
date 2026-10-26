// _global::__pop_heap(SBindingPair* first, SBindingPair* last, SBindingPair* result, SBindingPair value, CCompareString comp, long*) -- SGI pop_heap core for a heap of (name, CCountedPointer<CEntityScriptBindingBase>) pairs: *result = *first, then sift `value` down from the root
// Leaf: __pop_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBBAD0, length 153 bytes (`ret 18h`: result, the 0x0c-byte value, the comparator and
// the distance pointer on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Label correction: the manifest's "CGameCameraManager::PushCameraMode" is propagated and wrong; the callee 0x00CBB4D0 carries the manifest name
// `_Adjust_heap<std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase> >*,int,...,CCompareString<CCountedPointer<CEntityScriptBindingBase> >>`,
// so this is the matching __pop_heap of that instantiation.
// Callees: the pair's assignment @ 0x00CBA700 (out of line; label "CScriptGameResourceObjectScriptedThingBase::operator=" is propagated:
// CCharString::operator= then the counted-pointer assignment), CCharString::CCharString(const CCharString&) @ 0x0099EC30, __adjust_heap
// @ 0x00CBB4D0, the count block's destroy function, operator delete @ 0x00BFE9BC, CCharString::~CCharString @ 0x0099EAE0.
// Shape: the pair's assignment is NOT inlined (one call); its copy constructor is (string out of line, counted pointer inline); the
// count is the reciprocal multiply by 0x2AAAAAAB with `sar edx,1` (divide by 12); the by-value `value` is destroyed by this callee
// (counted-pointer release with the block re-read, object and block zero stores, then the string).
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

struct SCountBlock {
    long m_references;                                  // +0
    void (__fastcall* m_pfnDestroy)(void*);             // +4 per-type deleter, object in ecx
    void* m_pObject;                                    // +8
};

template <class T>
class CCountedPointer { public:
    T* m_pObject;                                       // +0
    SCountBlock* m_pCount;                              // +4
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount) ++m_pCount->m_references;
    }
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;                        // operator delete @ 0x00BFE9BC
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

class CEntityScriptBindingBase;

struct SBindingPair {                                   // std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >, 12 bytes
    CCharString                               first;    // +0x00
    CCountedPointer<CEntityScriptBindingBase> second;   // +0x04
    SBindingPair& operator=(const SBindingPair& other);   // 0x00CBA700 (out of line)
};

struct CCompareString { };                              // CCompareString<CCountedPointer<CEntityScriptBindingBase> >, empty, by value

void __fastcall __adjust_heap(SBindingPair* first, long holeIndex, long len, SBindingPair value,
                              CCompareString comp);     // 0x00CBB4D0

void __fastcall __pop_heap(SBindingPair* first, SBindingPair* last, SBindingPair* result, SBindingPair value,
                           CCompareString comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}