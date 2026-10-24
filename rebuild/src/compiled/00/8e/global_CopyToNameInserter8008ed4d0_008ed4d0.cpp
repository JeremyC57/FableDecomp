// _global::Copy_ToNameInserter8_008ed4d0(const SDefEntry8*, const SDefEntry8*, CDefNameInserter8) -- std::copy of 8-byte def references into an output iterator that appends each one's name to a CCharString vector
// Leaf: Copy_ToNameInserter8_008ed4d0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008ED4D0, length 61 bytes (+ int3 padding).
// Shape: __fastcall with a hidden return pointer -- the output iterator is a non-POD class (a user constructor), so the
// caller (0x008ED2F0, in the persist container transfer 0x008ED2B0) passes the return slot in ecx, first in edx, and last
// + the iterator by value on the stack (`ret 8`). A pointer-compare loop calls the iterator's out-of-line
// operator=(const T&) @ 0x008ED3B0 on the by-value argument slot (`lea ecx,[esp+18h]`); its operator* and operator++ are
// inline no-ops. The iterator word is copied to the return slot on both exits. The loop is written directly in this
// function: an inlined SGI `__copy(..., input_iterator_tag, Distance*)` wrapper copies the by-value iterator into a
// second slot and merges the exits.
// Identity: the manifest's `_Copy_impl<CArray<CRandomAppearanceMorph::CBodyPartMesh>*>` label is propagated. The
// iterator's operator= (catalogued std_vector_push_back<..>) turns the element's def index into its name through
// CGameDefinitionManager::Get @ 0x0044C6B0 / the definition manager and push_backs a CCharString; the element is the
// 8-byte def reference of the container being written, kept descriptive.
struct SDefEntry8 {                          // 8 bytes
    long          DefIndex;                  // +0x00 (read by the inserter)
    unsigned long Extra;                     // +0x04
};

struct CCharStringVector;

struct CDefNameInserter8 {                   // output iterator over a vector<CCharString>, 4 bytes
    CCharStringVector* Container;            // +0x00
    explicit CDefNameInserter8(CCharStringVector& container) : Container(&container) { }
    CDefNameInserter8& operator=(const SDefEntry8& entry);     // 0x008ED3B0
    CDefNameInserter8& operator*() { return *this; }
    CDefNameInserter8& operator++() { return *this; }
};

CDefNameInserter8 __fastcall Copy_ToNameInserter8_008ed4d0(const SDefEntry8* first, const SDefEntry8* last, CDefNameInserter8 result)
{
    for (; first != last; ++result, ++first)
        *result = *first;
    return result;
}