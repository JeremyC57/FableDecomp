// C3DMeshStats::GetAllDummyObjectNames(std::vector<CCharString>& names, bool flag) -- UnpackNames(names, true, CCharString(), flag)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2F0F0, oracle length 45 bytes.
// Callees: CCharString::CCharString @ 0x0099E4B0 (landed default constructor), C3DMeshStats::UnpackNames(vector<CCharString>&,
//          bool, const CCharString&, bool) @ 0x00A2ED10 (manifest), CCharString::~CCharString @ 0x0099EAE0 (the manifest's
//          `_Cons_val<...>` label on it is propagated; it is the string destructor called on the temporary).
//
// Retail bytes:  mov eax,[esp+8] ; push esi ; mov esi,ecx ; push eax ; lea ecx,[esp+10h] ; call CCharString::CCharString ; mov ecx,[esp+0Ch] ;
//                push eax ; push 1 ; push ecx ; mov ecx,esi ; call UnpackNames ; lea ecx,[esp+0Ch] ; call CCharString::~CCharString ; pop esi ; ret 8
// Shape: the second argument is forwarded as the last UnpackNames argument (loaded as a dword before the
// temporary is built), the empty CCharString temporary is constructed in the dead second argument's home
// slot (esp+0x10 after the two pushes = the `flag` slot) and destroyed after the call; `true` selects the
// dummy-object name table (the sibling @ 0x00A2F120 passes false for the helper points).
struct CCharString {
    char* m_pString;
    CCharString();                         // 0x0099E4B0
    ~CCharString();                        // 0x0099EAE0
};

struct CCharStringVector;                  // std::vector<CCharString>

struct C3DMeshStats {
    virtual void v00();
    void GetAllDummyObjectNames(CCharStringVector& names, bool flag);
    void UnpackNames(CCharStringVector& names, bool dummyObjects, const CCharString& prefix, bool flag);   // 0x00A2ED10
};

void C3DMeshStats::GetAllDummyObjectNames(CCharStringVector& names, bool flag)
{
    UnpackNames(names, true, CCharString(), flag);
}
