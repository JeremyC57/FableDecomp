// CCardPositionsDef::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCCardPositionsDef@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C109B0, 30 bytes, thiscall, `ret 4`; calls 00c10940=_unattributed::~CCardPositionsDef_00c10940 ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Naming wave 33 (family ctorB): slot 0 of vtable 0x012B263C (no RTTI), the vtable the landed constructor 0x00C108E0 stores last
//   (??0CCardPositionsDef@@QAE@ABV0@@Z (the ctor row is catalogued as CVtable_012b263c::CVtable_012b263c; its Leaf is the PDB symbol), Ego_r.pdb / FableWin.pdb); FableWin.pdb ??_GCCardPositionsDef@@UAEPAXI@Z; FableWin.pdb ??1CCardPositionsDef@@UAE@XZ.
//   Was a free __fastcall stand-in (catalog CVtable_012b263c::`scalar deleting destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the constructor's TU: the class model and the PDB constructor of
//   the landed row 0x00C108E0 (rebuild/src/compiled/00/c1/CCardPositionsDef_CCardPositionsDef_00c108e0.cpp), its own source unchanged. The constructor body is not part of this graded body.
class __declspec(dllimport) exception {
public:
    exception();                           // IAT 0x0143FFF8 = MSVCR71.dll!??0exception@@QAE@XZ
    exception(const exception&);           // IAT 0x0143FFFC = MSVCR71.dll!??0exception@@QAE@ABV0@@Z
    virtual ~exception();                  // IAT 0x01440028 = MSVCR71.dll!??1exception@@UAE@XZ
private:
    const char* _m_what;
    int _m_doFree;
};
namespace std {
template<class _Elem> struct char_traits;
template<class _Ty> class allocator;
template<class _Elem, class _Traits, class _Ax> class basic_string;
template<> class __declspec(dllimport) basic_string<char, char_traits<char>, allocator<char> > {
public:
    basic_string(const basic_string&);     // IAT 0x0143FFA0 = MSVCP71.dll!??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@ABV01@@Z
    ~basic_string();                       // IAT 0x0143FF9C = MSVCP71.dll!??1?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@XZ
private:
    unsigned char _body[28];
};
}

class CCardPositionsDef : public exception { public:   // vtable 0x012B263C
    std::basic_string<char, std::char_traits<char>, std::allocator<char> > m_positions;   // +0xc
    CCardPositionsDef(const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& source);
};

CCardPositionsDef::CCardPositionsDef(const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& source)
    : m_positions(source)
{
}