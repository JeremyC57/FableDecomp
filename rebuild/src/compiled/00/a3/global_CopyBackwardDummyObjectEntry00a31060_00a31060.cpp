// _global::CopyBackward_DummyObjectEntry_00a31060(SDummyObjectEntry*, SDummyObjectEntry*, SDummyObjectEntry*, const random_access_iterator_tag&, int*) -- SGI __copy_backward over 0x40-byte (dummy object, name, index, flag) records
// Leaf: CopyBackward_DummyObjectEntry_00a31060
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A31060, length 103 bytes.
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the tag reference + the distance pointer on the stack, `ret 0Ch`); the count is
// (last - first) / sizeof(T), both pointers step back one element and the moved-back result is returned.
// Element: 0x40 bytes = a 0x34-byte POD block (13-dword `rep movsd`; Ego_r.pdb C3DDummyObject is 0x34: CMatrix3x4
// Transform + long BoneIndex), a CCharString +0x34 (CCharString::operator= @ 0x0099EFB0), a long +0x38 and a byte
// +0x3C. The TU is the mesh dummy-object one (0x00A314E0 / 0x00A33800 sort pair<unsigned long, C3DDummyObject>); the
// manifest's `pair<CCharString,C3DDummyObject>` label is propagated (that pair is 0x38 bytes), so the record stays
// descriptive. The count is kept in the `push ecx` frame word.
struct CCharString {                         // one-word handle
    void* m_pData;                           // +0x00
    CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};

struct CMatrix3x4 { float M[12]; };
struct C3DDummyObject {                      // Ego_r.pdb, 0x34 bytes
    CMatrix3x4 Transform;                    // +0x00
    long       BoneIndex;                    // +0x30
};

struct SDummyObjectEntry {                   // 0x40 bytes
    C3DDummyObject Object;                   // +0x00
    CCharString    Name;                     // +0x34
    long           Index;                    // +0x38
    bool           Flag;                     // +0x3C
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SDummyObjectEntry* __fastcall CopyBackward_DummyObjectEntry_00a31060(SDummyObjectEntry* first, SDummyObjectEntry* last, SDummyObjectEntry* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}