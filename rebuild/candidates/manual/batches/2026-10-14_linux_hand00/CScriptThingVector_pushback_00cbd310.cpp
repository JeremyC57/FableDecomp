// std::vector<CScriptThing>::push_back(const CScriptThing& thing) -- STLport push_back with the overflow path out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBD310, length 96 bytes.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, _M_insert_overflow @ 0x00CBC940
//          (manifest label "CScriptThing_VectorReallocate").
// The manifest's "C3DAnimFile::AddSkeletonMorph" label is propagated noise. Vector layout {start +0,
// finish +4, end_of_storage +8}; element = CScriptThing {CCharString Name +0, CCountedPointer PImp +4}
// (PDB: 12 bytes, PImp at +4). With room left, placement-new copies the element at finish (VC's null
// test on the placement pointer, the string copy out of line, the counted pointer inline with its count
// bump) and finish is re-read and advanced by 12. Otherwise the STLport overflow helper gets
// (finish, &thing, a __false_type tag temporary homed in the dead argument slot, 1, true).
struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4
    void* m_pObject;                        // +8
};

struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);  // 0x0099EC30
};

struct CCountedPointer {
    void* m_pObject;                        // +0
    SCountBlock* m_pCount;                  // +4
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
};

struct CScriptThing {
    CCharString Name;                       // +0
    CCountedPointer PImp;                   // +4
};

struct __false_type { __false_type() {} };  // empty tag with a user constructor (no byte store)

inline void* operator new(unsigned int, void* where) { return where; }

struct CScriptThingVector {
    CScriptThing* m_start;                  // +0
    CScriptThing* m_finish;                 // +4
    CScriptThing* m_end_of_storage;         // +8
    void _M_insert_overflow(CScriptThing* position, const CScriptThing& x, const __false_type&,
                            unsigned long fill_len, bool atend);        // 0x00CBC940
    void push_back(const CScriptThing& x);
};

void CScriptThingVector::push_back(const CScriptThing& x)
{
    if (m_finish != m_end_of_storage) {
        new (m_finish) CScriptThing(x);
        ++m_finish;
    }
    else
        _M_insert_overflow(m_finish, x, __false_type(), 1, true);
}
