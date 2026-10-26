// _global::ForEach_AddDummyObjectName_00a72f40(const SDummyObject* first, const SDummyObject* last, CAddName f) -- for_each over CPhysicsMesh dummy objects: push_back each record's name into the functor's CCharString vector, return the functor
// Leaf: ForEach_AddDummyObjectName_00a72f40
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A72F40, length 96 bytes (`ret 8`).
// Identity: catalog `_Move<std::_List_const_iterator<...CAnimSet...>*, std::_Temp_iterator<...> >` is propagated. The landed caller
//   CPhysicsMesh::GetAllDummyObjectNames 0x00A6D090 declares this SGI `for_each<const SDummyObject*, CAddName>` (`__fastcall`: the functor
//   result's hidden pointer in ecx, first in edx, last and the by-value functor on the stack); the records are
//   56-byte pair<CCharString, C3DDummyObject> entries (0x38 stride).
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30, STLport vector<CCharString>::_M_insert_overflow 0x00433530.
// Shape: the functor body is the inlined STLport `push_back`: placement copy at the finish pointer when there is room
//   (null-tested), else `_M_insert_overflow(finish, x, __false_type(), 1, true)` whose empty tag temporary is homed in
//   the dead functor argument slot (`lea eax,[esp+20h]`); the functor's vector pointer stays in edi and is stored
//   through the hidden result pointer on both exits.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag (its temporary is homed in a dead argument slot)

class CCharString { public:                            // 4-byte counted string handle
    void* m_pData;
    CCharString(const CCharString& other);      // 0x0099EC30
};

struct SDummyObject {                           // pair<CCharString, C3DDummyObject>, 56 bytes
    CCharString   m_name;                       // +0x00
    unsigned char m_object[0x34];               // C3DDummyObject
};

struct CCharStringVector {                      // STLport vector<CCharString>, 12 bytes
    CCharString* m_pFirst;
    CCharString* m_pLast;
    CCharString* m_pEnd;
    void _M_insert_overflow(CCharString* position, const CCharString& x, const __false_type&, unsigned int fillLen, bool atEnd);   // 0x00433530
    void push_back(const CCharString& x)
    {
        if (m_pLast != m_pEnd) {
            new (m_pLast) CCharString(x);
            ++m_pLast;
        }
        else
            _M_insert_overflow(m_pLast, x, __false_type(), 1, true);
    }
};

struct CAddName {                               // CPhysicsMesh's name-collecting functor (landed callers' model)
    CCharStringVector* m_pNames;
    CAddName(CCharStringVector& names) : m_pNames(&names) { }
    void operator()(const SDummyObject& object) { m_pNames->push_back(object.m_name); }
};

CAddName __fastcall ForEach_AddDummyObjectName_00a72f40(const SDummyObject* first, const SDummyObject* last, CAddName f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}