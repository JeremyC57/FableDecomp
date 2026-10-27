// C3DMeshVolume::operator=(const C3DMeshVolume& other) -- memberwise assignment: the leading part through its out-of-line operator=, then the counted pointer (release the old block, adopt, add-ref)
// Leaf: ??4C3DMeshVolume@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a32960, 80 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ??4C3DMeshVolume@@QAEAAV0@ABV0@@Z (bound in reloc_bindings); Ego_r.pdb members Name (CCharString) and ConvexVolume (CCountedPointer<CContainmentVolume>, +4); callers the pair<long, C3DMeshVolume> heap / insertion helpers 0x00A331B0 / 0x00A33250 / 0x00A333A0.
// Callees: CCharString::operator=(const CCharString&) 0x0099EFB0, operator delete 0x00BFE9BC (the old count block); CCountedPointer's operator= inline.
// Shape (family L088): the compiler-generated copy assignment, written out (same body) so the translation unit emits it.
class CCPPointerInfo {                                     // 12 bytes
public:
    unsigned long RefCount;                                // +0x00
    void (__fastcall* DeleteFunc)(void* data);             // +0x04
    void* Data;                                            // +0x08
};
template <class T> class CCountedPointer {                 // 8 bytes
public:
    T*              Data;                                  // +0x00
    CCPPointerInfo* Info;                                  // +0x04
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;                 // the source's fields are read first
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
class CCharString {
public:
    void* PStringData;
    CCharString& operator=(const CCharString& other);      // 0x0099EFB0
};
class CContainmentVolume;
class C3DMeshVolume {
public:
    CCharString                         Name;              // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;      // +0x04
    C3DMeshVolume& operator=(const C3DMeshVolume& other);
};
C3DMeshVolume& C3DMeshVolume::operator=(const C3DMeshVolume& other)
{
    Name = other.Name;
    ConvexVolume = other.ConvexVolume;
    return *this;
}