// std::pair<const unsigned __int64, std::pair<CCharString, CWideString> >::`scalar deleting destructor'(unsigned int flags) -- destroy the value's string pair and free it when bit 0 is set
// Leaf: ??_G?$pair@$$CB_KU?$pair@VCCharString@@VCWideString@@@std@@@std@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00526EF0, 45 bytes, thiscall, `ret 4`.
// Identity (probable): unreferenced COMDAT; an 8-byte scalar then a {CCharString, CWideString} pair at +8 is exactly the value
//   type of CThingManager::NPCNameMap, map<unsigned __int64, pair<CCharString, CWideString> > (Ego_r.pdb, the only container of
//   that pair), and it sits in the CThingManager code (0x00526EF0, beside the UIDToThingMap tree rows of family M118); STLport's
//   _Destroy(p) on the map node's value calls this ??_G with flags 0 under /Os (family M097). Destructors ~CWideString 0x0099B510
//   (+0xC) then ~CCharString 0x0099EAE0 (+8).
// Callees: ~CWideString 0x0099B510, ~CCharString 0x0099EAE0, operator delete 0x00BFE9BC.
// Shape (family M117, representative 0x00526EF0): the compiler-generated non-virtual `??_G` of the pair -- the inner pair's
//   implicit destructor inline (its `this` in edi, members destroyed in reverse), then the flag test; the delete-expression in
//   DeleteOne forces it into the object (not part of the retail body).
void __cdecl operator delete(void* p);                      // 0x00BFE9BC
class CCharString { public: ~CCharString(); void* PStringData; };          // ~ 0x0099EAE0
class CWideString { public: ~CWideString(); void* PStringData; };          // ~ 0x0099B510
namespace std { template <class _T1, class _T2> struct pair { _T1 first; _T2 second; }; }
typedef std::pair<const unsigned __int64, std::pair<CCharString, CWideString> > CNPCNameValue;
void DeleteOne(CNPCNameValue* object)   // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}