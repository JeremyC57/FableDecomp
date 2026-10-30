// CFrontEndDef::`scalar deleting destructor'(unsigned int) -- reset the vptr, destroy the attract-mode movie list and the parent-def base, free when bit 0 is set
// Leaf: ??_GCFrontEndDef@@UAEPAXI@Z
// Retail: Fable.exe @ 0x00431E40, 46 bytes, `ret 4`; slot 0 of vtable 0x01230FBC, whose COL names .?AVCFrontEndDef@@
// (: CParentDefClassBase : CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase); Ego_r.pdb has
// ??_GCFrontEndDef@@UAEPAXI@Z, FableWin.pdb ??1CFrontEndDef@@UAE@XZ. Catalog label CThingTrackNode::`vector deleting
// destructor' is wrong (no array path). Body: the inline ~CFrontEndDef stores the CFrontEndDef vptr, calls
// ~vector<CCharString> (0x00414EA0) on vAttractModeMovie (+0x3c) and the out-of-line ~CParentDefClassBase (0x00430300:
// frees the SubDefInfoMap storage at +0x29, CDefPointeeBase vptr, ~CResource), then operator delete (0x00BFE9BC) when
// (flags & 1); returns this.
// Layout: retail CFrontEndDef -- CParentDefClassBase is 0x39 bytes (as 0x00431ED0), so vAttractModeMovie (a 12-byte STLport
// vector) is at +0x3c (Ego_r.pdb +0x40). The base destructor is spelled as a declared out-of-line one (retail's is the
// compiler-generated one, emitted once at 0x00430300).
// Shape: /O2; the user destructor is inline and empty; a placement-new helper makes VC7.1 emit the vtable and ??_G.
#include <new>
class CCharString {
public:
    void* PStringData;
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector();                                           // 0x00414EA0 for CCharString
};
}
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
class CFrontEndDef : public CParentDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);              // ?Transfer@CFrontEndDef@@UAEXAAVCPersistContext@@@Z
    virtual void Copy(const CDefClassBase* other);                // ?Copy@CFrontEndDef@@UAEXPBVCDefClassBase@@@Z
    virtual unsigned long GetSizeofClass();                       // ?GetSizeofClass@CFrontEndDef@@UAEKXZ
    std::vector<CCharString> vAttractModeMovie;          // +0x3c
    long ErrorMessageBackgroundGraphic;                  // +0x48
    long ButtonABigGraphic;                              // +0x4c
    long ButtonBBigGraphic;                              // +0x50
    virtual ~CFrontEndDef() {}
};
void ConstructCFrontEndDef(void* p)                      // emission helper, not a retail function: the vtable (and its ??_G) is emitted
{
    new (p) CFrontEndDef;
}