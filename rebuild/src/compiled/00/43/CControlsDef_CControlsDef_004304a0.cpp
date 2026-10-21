// CControlsDef::~CControlsDef() -- free the Controls vector storage, then the parent-def base: the SubDefInfoMap storage, the CDefPointeeBase vptr, ~CResource
// Leaf: ??1CControlsDef@@UAE@XZ
// Retail: Fable.exe @ 0x004304A0, 49 bytes; its only caller is 0x00430480 (slot 0 of vtable 0x01230E84, whose COL names
// .?AVCControlsDef@@ : CParentDefClassBase : CDefClassBase : CDefPointeeBase : CResource; CControlsDef::GetSizeofClass
// 0x00430470 returns 0x50). Catalog label CCompressedMemoryDataInputStream::~CCompressedMemoryDataInputStream is wrong.
// Body: the implicit destructor -- STLport ~vector<CActionInputControl> on Controls (+0x3c: `if (start) free(start)`
// through 0x00BFEA14), the inlined implicit ~CParentDefClassBase (SubDefInfoMap's vector storage at +0x29), the inline
// ~CDefPointeeBase storing its vptr (0x01230BA0), and a tail jump to ~CResource (0x009FC550).
// Layout: retail CControlsDef (0x50 bytes): CParentDefClassBase 0x39 (as 0x00431ED0), Controls (12-byte STLport vector)
// +0x3c (Ego_r.pdb +0x40), six bools +0x48..+0x4d.
// Shape: /O2; every destructor below CDefPointeeBase is implicit; DestroyCControlsDef's qualified call emits it out of line.
#include <stdlib.h>
class CResourceList;
class CResource {
public:
    unsigned long  IVRefCount;                           // +0x04
    CResourceList* ResourceList;                         // +0x08
    CResource*     PrevResource;                         // +0x0c
    CResource*     NextResource;                         // +0x10
    unsigned long  ResourceSize;                         // +0x14
    unsigned long  LastUsedFrame;                        // +0x18
    virtual ~CResource();                                // 0x009FC550
};
class CDefPointeeBase : public CResource {
public:
    virtual ~CDefPointeeBase() {}
};
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {
public:
    struct CSubDefInfo {
        long DefIndex;
        long OriginalParentDefIndex;
    };
    CDefinitionManager* PDefManager;                     // +0x1c
    unsigned long       GlobalIndex;                     // +0x20
    unsigned char       Setup : 1;                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
#pragma pack(pop)
class CDefString {
public:
    long TablePos;
};
namespace std {
template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~vector() { if (_M_start) free(_M_start); }
};
}
template <class K, class V> struct CKeyPairCompareLess {};
template <class K, class V, class C> class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    C    Compare;                                        // +0x0c
    bool Dirty;                                          // +0x0d
};
#pragma pack(push, 1)
class CParentDefClassBase : public CDefClassBase {
public:
    CDefString InstantiationName;                        // +0x25
    CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> > SubDefInfoMap;   // +0x29
};
#pragma pack(pop)
class CActionInputControl {                              // element type does not reach the bytes; members descriptive
public:
    long Action;
    long Control;
};
class CControlsDef : public CParentDefClassBase {
public:
    std::vector<CActionInputControl> Controls;           // +0x3c
    bool ToggleZTarget;                                  // +0x48
    bool ToggleSpells;                                   // +0x49
    bool ToggleSneak;                                    // +0x4a
    bool ToggleExpressionMenu;                           // +0x4b
    bool ToggleExpressionShift;                          // +0x4c
    bool FlourishNeedsAttackButtonHeld;                  // +0x4d
};
void DestroyCControlsDef(CControlsDef* p)                // emission helper, not a retail function
{
    p->CControlsDef::~CControlsDef();
}