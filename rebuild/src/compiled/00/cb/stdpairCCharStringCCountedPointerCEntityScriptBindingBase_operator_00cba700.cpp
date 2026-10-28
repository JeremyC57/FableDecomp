// std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >::operator=(const std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >& other) -- memberwise assignment: the leading part through its out-of-line operator=, then the counted pointer (release the old block, adopt, add-ref)
// Leaf: ??4?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@std@@QAEAAU01@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00cba700, 80 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ??4?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@std@@QAEAAU01@ABU01@@Z (bound in reloc_bindings); callers the swap 0x00CBA8E0 and the __adjust_heap 0x00CBB4D0 of the entity-script binding pairs.
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
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0
};
class CEntityScriptBindingBase;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                             // +0x00
    _T2 second;                                            // +0x04
    pair& operator=(const pair& __o) { first = __o.first; second = __o.second; return *this; }   // the implicit memberwise assignment, spelled out to emit it
};
template struct pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >;
}