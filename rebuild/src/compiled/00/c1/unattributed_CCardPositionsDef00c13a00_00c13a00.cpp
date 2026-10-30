// CCardPositionsDef_00c13a00::CCardPositionsDef_00c13a00(const CCardPositionsDef_00c13a00&) -- EH-framed copy constructor: the imported base copied, own vtable (0x012B263C), the imported member at +0xc copied from the other's
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C13A00, oracle length 89 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; push edi ; mov edi,[other] ; mov esi,ecx ; push edi ; mov [esp+0Ch],esi ; call [__imp_CBase::CBase(const CBase&)] ; add edi,0Ch ; push edi ; lea ecx,[esi+0Ch] ; mov dword ptr [esp+18h],0 ; mov dword ptr [esi],offset vtable ; call [__imp_CMember::CMember(const CMember&)] ; mov ecx,[esp+0Ch] ; pop edi ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 4
// The copy-constructor counterpart of 0x00C108E0 (same vtable): the `exception` base (MSVCR71) and the
// std::string member (MSVCP71) are copied through import-address-table slots. `other` is kept in edi
// and advanced to its member. The class is named by address.
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

struct CCardPositionsDef_00c13a00 : exception {   // vtable 0x012B263C
    std::basic_string<char, std::char_traits<char>, std::allocator<char> > m_positions;   // +0xc
    CCardPositionsDef_00c13a00(const CCardPositionsDef_00c13a00& other);
};

CCardPositionsDef_00c13a00::CCardPositionsDef_00c13a00(const CCardPositionsDef_00c13a00& other)
    : exception(other), m_positions(other.m_positions)
{
}