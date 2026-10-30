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
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
// def vtable 0x01232D1C: slot 0 0x004545EE (implicit inline dtor); real overrides from the def vtable scan
class CDegradableDef : public CDefClassBase {
public:
    void Copy(const CDefClassBase* other);
    bool                      Degradable;                        // +0x25
    EEngineGraphicType        GraphicType;                       // +0x28
    bool                      InitiallyBlocksNavigation;         // +0x2c
    std::vector<CDegradableInfo> Degradations;                      // +0x30
    virtual void Transfer(CPersistContext& context);         // slot 18 0x0067E290
    virtual unsigned long GetSizeofClass();                  // slot 20 0x00453884
};
void CDegradableDef::Copy(const CDefClassBase* other)
{
    *this = *(const CDegradableDef*)other;
}