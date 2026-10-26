// CAnimComponentStrikeResponseAnim::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated destructor (ResponseAnimName destroyed, then the inline CIVCountedPointeeBase destructor's vtable store) and the flag-tested operator delete
// Leaf: ??_GCAnimComponentStrikeResponseAnim@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E0030, 39 bytes (+ int3 padding), thiscall `ret 4`, returns this.
// Identity (proven): CAnimComponentStrikeResponseAnim::`scalar deleting destructor'; evidence: it is slot 0 of vtable 0x01255828 (RTTI .?AVCAnimComponentStrikeResponseAnim@@,
//   bases .?AVCAnimComponentBase@@(+0) .?AVCIVCountedPointeeBase@@(+0)), it destroys ResponseAnimName at +0x8 (Ego_r.pdb CCharString ResponseAnimName +0x8) and stores the
//   CIVCountedPointeeBase vtable 0x012354D0 (RTTI .?AVCIVCountedPointeeBase@@).
// Label: the manifest's "`scalar_deleting_destructor'" is propagated (family H085: six deleting destructors of classes whose only non-trivial
//   member is one object at +8 / +0x94; retail has no identical-code folding, so each address is its own class's ??_G).
// Callees: CCharString::~CCharString 0x0099EAE0 on ResponseAnimName, operator delete 0x00BFE9BC.
// Shape: CAnimComponentStrikeResponseAnim has no user-declared destructor (no own vtable store); CIVCountedPointeeBase's `virtual ~CIVCountedPointeeBase() { }` is inline
//   and stores its vtable; the vtable (and so this ??_G) is emitted by the construction helper below. Local classes, Review at landing.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
class CCharString {                                        // 4 bytes: shared string block
public:
    void* PStringData;
    CCharString();
    ~CCharString();                                         // 0x0099EAE0
};
class CIVCountedPointeeBase {                               // vtable 0x012354D0
public:
    virtual ~CIVCountedPointeeBase() { }
    long RefCount;                                          // +0x04
};
class CAnimComponentBase : public CIVCountedPointeeBase {
};
class CAnimComponentStrikeResponseAnim : public CAnimComponentBase {
public:
    CCharString ResponseAnimName;                           // +0x08
};

// VC7.1 emits a class's vtable (and the deleting destructor it references) in every translation
// unit that constructs the object; this helper is that construction, nothing more.
void* MakeOne_005e0030()
{
    return new CAnimComponentStrikeResponseAnim;
}