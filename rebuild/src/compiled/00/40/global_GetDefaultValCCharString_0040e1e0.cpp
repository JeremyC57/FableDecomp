// _global::GetDefaultVal<CCharString>() -- default value of a CCharString setting: default-construct a local string and return a copy of it
// Leaf: ??$GetDefaultVal@VCCharString@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040E1E0, 39 bytes. Callees: CCharString::CCharString()
// 0x0099E4B0, CCharString::CCharString(const CCharString&) 0x0099EC30, CCharString::~CCharString
// 0x0099EAE0 (manifest label `_Cons_val<...CParticleMorphs::CEntry...>` is propagated).
//
// Retail bytes:  push ecx ; push esi ; mov esi,ecx ; lea ecx,[local] ; call CCharString() ;
//   lea eax,[local] ; push eax ; mov ecx,esi ; call CCharString(const CCharString&) ;
//   lea ecx,[local] ; call ~CCharString ; mov eax,esi ; pop esi ; pop ecx ; ret
// The generic `template <class T> T GetDefaultVal() { T value; return value; }`: VC7.1 has no
// named-return-value optimisation, so the named local is copy-constructed into the hidden result
// (which a no-argument __fastcall receives in ecx) and then destroyed. 16 call sites
// (0x00CDC225 .. 0x00F2FC90).
class CCharString {
public:
    CCharString();                                // 0x0099E4B0
    CCharString(const CCharString& other);        // 0x0099EC30
    ~CCharString();                               // 0x0099EAE0
private:
    void* m_pData;
};

template <class T>
T __fastcall GetDefaultVal()
{
    T value;
    return value;
}

template CCharString __fastcall GetDefaultVal<CCharString>();