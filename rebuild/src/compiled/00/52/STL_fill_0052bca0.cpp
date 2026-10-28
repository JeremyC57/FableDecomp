// std::fill(NThingManager::CEntityRuntimePersistence::CEntityScriptInfo* first, NThingManager::CEntityRuntimePersistence::CEntityScriptInfo* last, const NThingManager::CEntityRuntimePersistence::CEntityScriptInfo& value) -- assign one entity-script save record to every element of a range
// Leaf: ??$fill@PAVCEntityScriptInfo@CEntityRuntimePersistence@NThingManager@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052BCA0, 62 bytes, `__fastcall` (first ecx, last edx, value pointer
//   on the stack), `ret 4`.
// Identity: 24-byte stride = {CCharString +0 (0x0099EFB0), 12-byte member +4 assigned through 0x005295B0 (a vector
//   operator= over 1-byte elements: malloc of the byte count), long +0x10, bool +0x14}: Ego_r.pdb
//   NThingManager::CEntityRuntimePersistence::CEntityScriptInfo {CCharString Script; CArray<unsigned char> SaveData;
//   long UncompressedSize; bool Valid} with the retail 12-byte CArray; the neighbouring _Cons_val<...CEntityScriptInfo>
//   0x0052B4E0 shares the family. The manifest's "CSymbolMap::CSymbolInfo" is propagated (CSymbolInfo is 12 bytes).
//   One caller, 0x0052C410.
// Callees: CCharString::operator= @ 0x0099EFB0, CArray<unsigned char>::operator= @ 0x005295B0.
// Shape: STLport fill with CEntityScriptInfo's implicit operator= inline, member by member; &val.SaveData hoisted.

class CCharString {
public:
    void* PStringData;
    const CCharString& operator=(const CCharString& other);    // 0x0099EFB0
};
template <class T> class CArray {                        // STLport vector, 12 bytes
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CArray& operator=(const CArray& other);              // 0x005295B0 for unsigned char
};

namespace NThingManager {
class CEntityRuntimePersistence {
public:
    class CEntityScriptInfo {                            // 24 bytes
    public:
        CCharString           Script;                    // +0x00
        CArray<unsigned char> SaveData;                  // +0x04
        long                  UncompressedSize;          // +0x10
        bool                  Valid;                     // +0x14
    };
};
}
typedef NThingManager::CEntityRuntimePersistence::CEntityScriptInfo CEntityScriptInfo;

namespace std {
// STLport 4.6 fill; the algorithm templates are __fastcall here (first/last in ecx/edx, callee pops the value pointer).
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
}

template void __fastcall std::fill<CEntityScriptInfo*, CEntityScriptInfo>(CEntityScriptInfo*, CEntityScriptInfo*, const CEntityScriptInfo&);