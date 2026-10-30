// CCardPositionsDef::CCardPositionsDef(void*) -- EH-framed constructor: imported base constructor, own vtable (0x012B263C), the imported member at +0xc built from the argument
// Leaf: ??0CCardPositionsDef@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C108E0, oracle length 83 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; call [__imp_CBase::CBase] ; mov eax,[argument] ; push eax ; lea ecx,[esi+0Ch] ; mov dword ptr [esp+14h],0 ; mov dword ptr [esi],offset vtable ; call [__imp_CMember::CMember] ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 4
// The base is MSVCR71's `exception` and the member at +0xc a std::string (MSVCP71), both called through
// import-address-table slots: the base is constructed on `this` (EH state 0 once it exists), the derived
// vtable is stored and the string member is copy-constructed from the argument. The class comes from the
// manifest label.
// MSVCR71.dll exports `exception` (VC7.1 <exception>: `class _CRTIMP exception`, global namespace) and MSVCP71.dll the
// std::string members; both are called through their IAT slots. Bodies are opaque (only the member offsets matter).
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