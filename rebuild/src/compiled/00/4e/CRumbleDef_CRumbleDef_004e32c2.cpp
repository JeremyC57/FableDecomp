// CRumbleDef::~CRumbleDef() -- the compiler-generated destructor of CRumbleDef: QuakeDurations (+0x34) then QuakeIntensities (+0x28) (reverse declaration order), then the inline CDefPointeeBase::~CDefPointeeBase() (its vptr store) and a tail jump to CResource::~CResource()
// Leaf: ??1CRumbleDef@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E32C2, 33 bytes, thiscall, tail `jmp`.
// Identity: CRumbleDef -- proven; evidence: its only caller is the scalar deleting destructor 0x004E32A6 in slot 0 of vtable 0x0124273C
//   (RTTI .?AVCRumbleDef@@ : CSubDefClassBase, CDefClassBase, CDefPointeeBase, CResource, CIVCountedPointeeBase); FableWin.pdb names ??1CRumbleDef@@UAE@XZ; members per Ego_r.pdb CRumbleDef (Ego_r.pdb +0x28 / +0x38, retail +0x28 / +0x34 with 12-byte STLport maps).
// Label: the manifest's "~CAIStateGroup_HoverHornetQueen" is propagated across byte-identical bodies (no CAIStateGroup_HoverHornetQueen is involved
//   except at 0x00903383).
// Callees: map<EQuakeLength, float>::~map @ 0x004DD5EE, map<EQuakeStrength, float>::~map @ 0x004DD5AD, CResource::~CResource @ 0x009FC550 (tail jmp).
// Shape: no vptr store of CRumbleDef's own (the destructor is implicit: VC7.1 stores the vptr only in user-written destructors);
//   the members' destructors run in reverse order, then CDefPointeeBase's inline user-written destructor stores vtable 0x01230BA0
//   (RTTI .?AVCDefPointeeBase@@) and the call to CResource::~CResource() becomes the tail `jmp`. A size-optimised TU (odd addresses, no
//   16-byte alignment): `#pragma optimize("s", on)`. The class is constructed once (MakeOne) so its vtable, scalar
//   deleting destructor and this destructor are emitted.
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> struct less { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport: 12 bytes
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector();
    ~vector();
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {                                              // STLport: the 12-byte _Rb_tree (header proxy, node count, compare)
public:
    void*        _M_header;
    unsigned int _M_node_count;
    _Compare     _M_key_compare;
    map();
    ~map();
};
}
class CResourceList;
class CResource {                                        // vtable 0x012354A4
public:
    unsigned long  IVRefCount;                           // +0x04
    CResourceList* ResourceList;                         // +0x08
    CResource*     PrevResource;                         // +0x0c
    CResource*     NextResource;                         // +0x10
    unsigned long  ResourceSize;                         // +0x14
    unsigned long  LastUsedFrame;                        // +0x18
    virtual ~CResource();                                // 0x009FC550
};
class CDefPointeeBase : public CResource {               // vtable 0x01230BA0
public:
    virtual ~CDefPointeeBase() { }
};
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {           // 0x25 bytes
public:
    CDefinitionManager* PDefManager;                     // +0x1c
    unsigned long       GlobalIndex;                     // +0x20
    unsigned char       Setup : 1;                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
class CSubDefClassBase : public CDefClassBase { };
#pragma pack(pop)
enum EQuakeStrength { QUAKE_STRENGTH_NONE = 0 };
enum EQuakeLength { QUAKE_LENGTH_NONE = 0 };
class CRumbleDef : public CSubDefClassBase {
public:
    std::map<EQuakeStrength, float> QuakeIntensities;   // +0x28 (~map 0x004DD5AD)
    std::map<EQuakeLength, float> QuakeDurations;       // +0x34 (~map 0x004DD5EE)
};
void* MakeOne() { return new CRumbleDef; }