// SetScriptActiveStatus(const CCharString& name, EScriptActiveStatus status) -- ScriptActiveStatuses[name] = status
// Leaf: ?SetScriptActiveStatus@@YIXABVCCharString@@W4EScriptActiveStatus@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBFAB8, 18 bytes, __fastcall (name in ecx, status in edx).
// Identity (proven, chunk c674): writes ScriptActiveStatuses[name] = status over the global map 0x0143E910 (calls the STLport
//   map<CCharString, EScriptActiveStatus>::operator[] 0x00CD39B1, kept out of line); Ego_r pairs it with GetScriptActiveStatus
//   0x00CBF9CF in the same TU.
// Callees: std::map<CCharString, EScriptActiveStatus>::operator[] 0x00CD39B1 (`mov ecx, offset ScriptActiveStatuses`).
// Naming wave 16: landed as the free __fastcall fn_00cbfab8(void*, int) calling Glob::h on an immediate address (catalog
//   CRegion::SetMiniMapRegionExitTextOffsetX); respelled on the PDB function and data symbol (name-only: bytes unchanged).
class CCharString;
enum EScriptActiveStatus { };
namespace std {
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { };
template <class _Tp> class allocator { };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    _Tp& operator[](const _Key& __k);              // 0x00CD39B1 (out of line)
};
}
extern std::map<CCharString, EScriptActiveStatus> ScriptActiveStatuses;   // 0x0143E910

void __fastcall SetScriptActiveStatus(const CCharString& name, EScriptActiveStatus status)
{
    ScriptActiveStatuses[name] = status;
}
