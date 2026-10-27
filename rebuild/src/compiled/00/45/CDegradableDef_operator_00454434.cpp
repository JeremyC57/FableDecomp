// CDegradableDef::operator=(const CDegradableDef& other) -- the implicit copy assignment: CDefClassBase's, then every member in order
// Leaf: ??4CDegradableDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00454434, 51 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): its only reference is the tail jump of CDegradableDef::Copy 0x0045442F; the 4 member operations read from
//   these bytes (byte/dword copies and the vector operator= call) are Ego_r.pdb CDegradableDef's members in order (names and types are Ego_r.pdb's).
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10, vector<CDegradableInfo>::operator= 0x00454467.
// Shape (family M037, representative 0x00454434): compiler-generated `operator=`, emitted through Copy's
//   `*this = *(const CDegradableDef*)other;`; `optimize("s", on)` keeps every scalar copy in eax/al.
// Header debt: the def base is modelled by its retail size only (its operator= is out of line).
#pragma optimize("s", on)
enum EEngineGraphicType { EENGINEGRAPHICTYPE_NONE = 0 };
class CDegradableInfo { public: unsigned long Members[3]; };   // element (opaque here)
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    vector& operator=(const vector& __x);                 // out of line
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}
class CDefClassBase {
public:
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
    unsigned char _base[0x25];
};
class CDegradableDef : public CDefClassBase {
public:
    void Copy(const CDefClassBase* other);
    bool                      Degradable;                        // +0x25
    EEngineGraphicType        GraphicType;                       // +0x28
    bool                      InitiallyBlocksNavigation;         // +0x2c
    std::vector<CDegradableInfo> Degradations;                      // +0x30
};
void CDegradableDef::Copy(const CDefClassBase* other)
{
    *this = *(const CDegradableDef*)other;
}