// CNoRttiObject_012C2748::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CNoRttiObject_012C2748 inlined (destroying the vector<CCharString> at +0x134, the CCharString at +0x54, then ~CBaseClassNonCopyable), and operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCNoRttiObject_012C2748@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CDBD70, 48 bytes, thiscall, `ret 4`; reached only through vtable slot 0.
// Identity: CNoRttiObject_012C2748 -- descriptive; evidence: DESCRIPTIVE class name: the only reference is vtable 0x012C2748 slot 0, a vtable with no RTTI locator; the class is built only by the unreferenced factory 0x00CDBD20 (`new` 0x144 bytes: CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0 (stores vtable 0x0129A7C4), the vtable, a CCharString at +0x54, a zeroed vector at +0x134) and the constructor 0x00CDB9E0 (catalogued CIcicleData::CIcicleData, a propagated label); the destroyed members are that vector<CCharString> (+0x134, ~vector 0x00414EA0, which the catalog already binds by its STLport name) and the CCharString (+0x54).
// Label: the manifest's "`vector_deleting_destructor'" is propagated across byte-identical bodies.
// Callees: vector<CCharString>::~vector 0x00414EA0, CCharString::~CCharString 0x0099EAE0, CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300, operator delete 0x00BFE9BC.
// Shape: no vtable store -- VC7.1 resets the vptr only in user-written destructors, and every destructor between this class
//   and CBaseClassNonCopyable is implicit (CBaseClassNonCopyable's own one stores its vtable); member destructors run in reverse declaration order.
//   The deleting destructor is emitted with the vtable, i.e. in a TU that constructs the class: MakeOne is that
//   construction, nothing more.
// Model: local -- no engine header for this class; the bases are sized to the retail layout.
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> struct less { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector();
    ~vector();
};
}
class CBaseClass {                                // RTTI root, vtable 0x01231710
public:
    virtual ~CBaseClass();                        // 0x0099A2E0 (FableWin.pdb ??1CBaseClass@@UAE@XZ)
};
class CBaseClassNonCopyable : public CBaseClass { // vtable 0x0129A7C4
protected:
    CBaseClassNonCopyable();                      // 0x0099A2F0 (FableWin.pdb ??0CBaseClassNonCopyable@@IAE@XZ)
    virtual ~CBaseClassNonCopyable();             // 0x0099A300 (FableWin.pdb ??1CBaseClassNonCopyable@@MAE@XZ)
};
class CCharString {
public:
    CCharString();                                // 0x0099E4B0
    ~CCharString();                               // 0x0099EAE0
    void* PStringData;
};
class CNoRttiObject_012C2748 : public CBaseClassNonCopyable {   // 0x144 bytes
public:
    unsigned char _unknown_0x04[0x50];
    CCharString String_0x54;                      // +0x54
    unsigned char _unknown_0x58[0xDC];
    std::vector<CCharString> Strings_0x134;       // +0x134
    unsigned char _unknown_0x140[4];
};
void* MakeOne()
{
    return new CNoRttiObject_012C2748;
}