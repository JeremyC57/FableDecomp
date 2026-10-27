// std::swap(pair<long, CCompositeSurfaceInfo>& a, pair<long, CCompositeSurfaceInfo>& b) -- STLport swap through a temporary: copy a (add-ref the surface cache), assign b to a and the temporary to b (counted-pointer release / add-ref), then destroy the temporary
// Flags: /Gr
// Leaf: ??$swap@U?$pair@JVCCompositeSurfaceInfo@@@std@@@std@@YIXAAU?$pair@JVCCompositeSurfaceInfo@@@0@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BED690, 250 bytes, __fastcall (a ecx, b edx), plain `ret`.
// Identity (probable): the element is the CVectorMap<long, CCompositeSurfaceInfo> entry of the landed sort TU (vptr
//   0x012A8154 = RTTI .?AVCCompositeSurfaceInfo@@ stored into the temporary; the 16-byte pair layout of landed
//   __adjust_heap 0x00BEDD70); the body is STLport 4.6 _algobase.h swap verbatim. Catalog "swap<CCompositeSurfaceInfo>"
//   names the second member, not the swapped type.
// Callees: operator delete 0x00BFE9BC (count blocks), ~CBaseClass 0x0099A2E0 (the temporary's base).
#include <new>

// CBaseClass: the polymorphic engine root (out-of-line virtual destructor 0x0099A2E0, vtable 0x01231710).
class CBaseClass {
public:
    virtual ~CBaseClass();                          // 0x0099A2E0
};

class CCPPointerInfo {                              // the shared count block of a CCountedPointer
public:
    unsigned int RefCount;                          // +0x00
    void (__fastcall* DeleteFunction)(void* data);  // +0x04
    void* Data;                                     // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                           // +0x00
    CCPPointerInfo* Info;                           // +0x04
    __forceinline CCountedPointer() : Data(0), Info(0) { }
    __forceinline CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    __forceinline ~CCountedPointer() { DeleteData(); Data = 0; Info = 0; }
    __forceinline CCPPointerInfo* GetInfo() const { return Info; }
    __forceinline void DeleteData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunction(Info->Data);
                ::operator delete(GetInfo());
            }
        }
    }
    __forceinline CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            DeleteData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};

class CCompositeSurfaceCache;
// Ego_r.pdb CCompositeSurfaceInfo (vtable 0x012A8154, 12 bytes): implicit copy, assignment and (virtual) destructor.
class CCompositeSurfaceInfo : public CBaseClass {
public:
    CCountedPointer<CCompositeSurfaceCache> Cache;  // +0x04
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;                                       // +0x00
    T2 second;                                      // +0x04
};
}   // namespace std

typedef std::pair<long, CCompositeSurfaceInfo> CSurfaceEntry;   // 0x10 bytes

namespace std {
template <class _Tp>
inline void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}
}   // namespace std

template void __fastcall std::swap(CSurfaceEntry&, CSurfaceEntry&);