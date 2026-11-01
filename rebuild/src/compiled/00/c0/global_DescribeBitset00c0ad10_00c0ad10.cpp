// DescribeBitset_00c0ad10(names, count, bits) -- EH-framed `__fastcall` helper returning an MFC-style string by value (hidden result in ecx, the name table in edx; the manifest's "CShaderResource_ConstructFromBitset" is propagated): an empty bit set yields "[not set]"; otherwise each set bit of the 16-bit mask below the count appends its quoted name (operator<<(const CString&) @ 0x00C0DEA0), joined by " + " once the line is non-empty (lstrlenA through CString::SafeStrlen @ 0x00C0BCC0); the frame string (state 1) is copy-constructed into the result (@ 0x00C0DAB0) on both returns, the flag word guarding the result's destruction on unwind
// Leaf: ?DescribeBitset_00c0ad10@@YI?AVCString@@PBV1@KG@Z
// Naming (naming wave 23, DECISIONS_W22_RESOLVED item 3): on the library CString (was the stand-in CString_00c0ad10); int counts as the MFC interface and the bound 0x00C0E440 spelling.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0AD10, oracle length 245 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,0Ch ; push ebx ; push ebp ; push esi ; xor esi,esi ; mov [esp+10h],esi ; mov eax,[g_pchNil] ; mov ebp,ecx ; push edi ; mov [esp+18h],ebp ; mov [esp+10h],eax ; mov bx,[esp+30h] ; cmp bx,si ; mov edi,1 ; mov [esp+24h],edi ; jne scan ; mov edx,offset "[not set]" ; lea ecx,[esp+10h] ; call Append ; lea ecx,[esp+10h] ; push ecx ; mov ecx,ebp ; call CString::CString(const CString&) ; mov [esp+14h],edi ; jmp done ; scan: mov edi,edx ; next: cmp esi,[esp+2Ch] ; jae finish ; test bl,1 ; je skip ; mov edx,[esp+10h] ; push edx ; call StringLength ; test eax,eax ; je quote ; mov edx,offset " + " ; lea ecx,[esp+10h] ; call Append ; quote: mov edx,offset "'" ; lea ecx,[esp+10h] ; call Append ; mov edx,edi ; mov ecx,eax ; call Append(const CString&) ; mov edx,offset "'" ; mov ecx,eax ; call Append ; skip: shr bx,1 ; inc esi ; add edi,4 ; test bx,bx ; jne next ; finish: lea eax,[esp+10h] ; push eax ; mov ecx,ebp ; call CString::CString(const CString&) ; mov dword ptr [esp+14h],1 ; done: lea ecx,[esp+10h] ; mov byte ptr [esp+24h],0 ; call ~CString ; ... mov eax,ebp ; ... ret 8
// The function keeps its stand-in name (no evidence names it).
class CString {
public:
    char* m_pchData;                            // +0x00
    CString() { Init(); }
    CString(const CString& stringSrc);          // 0x00C0DAB0
    ~CString();                                 // 0x00C0BA40
    void Init();
    static int __stdcall SafeStrlen(const char* lpsz);   // 0x00C0BCC0
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CString& __fastcall operator<<(const CString& string);   // 0x00C0DEA0
};
extern CString _EmptyString;                    // 0x0139C900 (its m_pchData)

inline void CString::Init()
{
    m_pchData = _EmptyString.m_pchData;
}

CString __fastcall DescribeBitset_00c0ad10(const CString* names, unsigned long count, unsigned short bits)
{
    unsigned long i = 0;
    CString line;
    if (bits == 0) {
        line << "[not set]";
        return line;
    }
    const CString* name = names;
    while (bits != 0) {
        if (i >= count)
            break;
        if (bits & 1) {
            if (CString::SafeStrlen(line.m_pchData))
                line << " + ";
            line << "'" << *name << "'";
        }
        bits >>= 1;
        ++i;
        ++name;
    }
    return line;
}