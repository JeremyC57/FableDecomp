// _global::Copy_ToNameInserter4_008ed510(const SDefEntry4*, const SDefEntry4*, CDefNameInserter4) -- std::copy of 4-byte def references into an output iterator that appends each one's name to a CCharString vector
// Leaf: Copy_ToNameInserter4_008ed510
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008ED510, length 61 bytes.
// Shape: __fastcall with a hidden return pointer -- the output iterator is a non-POD class (a user constructor), so the
// caller (0x008ED32A, in the persist container transfer 0x008ED2B0) passes the return slot in ecx, first in edx, and last
// + the iterator by value on the stack (`ret 8`). A pointer-compare loop calls the iterator's out-of-line
// operator=(const T&) @ 0x008ED440 on the by-value argument slot (`lea ecx,[esp+18h]`); its operator* and operator++ are
// inline no-ops. The iterator word is copied to the return slot on both exits. The loop is written directly in this
// function: an inlined SGI `__copy(..., input_iterator_tag, Distance*)` wrapper copies the by-value iterator into a
// second slot and merges the exits.
// Identity: the manifest's `_Copy_impl<CArray<CRandomAppearanceMorph::CBodyPartMesh>*>` label is propagated. The
// iterator's operator= (catalogued std_vector_push_back<..>) turns the element's def index into its name through
// CGameDefinitionManager::Get @ 0x0044C6B0 / the definition manager and push_backs a CCharString; the element is the
// 4-byte def index of the container being written, kept descriptive.
struct SDefEntry4 {                          // 4 bytes
    long          DefIndex;                  // +0x00
};

struct CCharStringVector;

struct CDefNameInserter4 {                   // output iterator over a vector<CCharString>, 4 bytes
    CCharStringVector* Container;            // +0x00
    explicit CDefNameInserter4(CCharStringVector& container) : Container(&container) { }
    CDefNameInserter4& operator=(const SDefEntry4& entry);     // 0x008ED440
    CDefNameInserter4& operator*() { return *this; }
    CDefNameInserter4& operator++() { return *this; }
};

CDefNameInserter4 __fastcall Copy_ToNameInserter4_008ed510(const SDefEntry4* first, const SDefEntry4* last, CDefNameInserter4 result)
{
    for (; first != last; ++result, ++first)
        *result = *first;
    return result;
}