// DescribeBitset_00c0ad10(names, count, bits) -- EH-framed `__fastcall` helper returning an MFC-style string by value (hidden result in ecx, the name table in edx; the manifest's "CShaderResource_ConstructFromBitset" is propagated): an empty bit set yields "[not set]"; otherwise each set bit of the 16-bit mask below the count appends its quoted name (Append(const CString&) @ 0x00C0DEA0), joined by " + " once the line is non-empty (lstrlenA through the null-safe wrapper @ 0x00C0BCC0); the frame string (state 1) is copy-constructed into the result (@ 0x00C0DAB0) on both returns, the flag word guarding the result's destruction on unwind
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0AD10, oracle length 245 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,0Ch ; push ebx ; push ebp ; push esi ; xor esi,esi ; mov [esp+10h],esi ; mov eax,[g_pchNil] ; mov ebp,ecx ; push edi ; mov [esp+18h],ebp ; mov [esp+10h],eax ; mov bx,[esp+30h] ; cmp bx,si ; mov edi,1 ; mov [esp+24h],edi ; jne scan ; mov edx,offset "[not set]" ; lea ecx,[esp+10h] ; call Append ; lea ecx,[esp+10h] ; push ecx ; mov ecx,ebp ; call CString::CString(const CString&) ; mov [esp+14h],edi ; jmp done ; scan: mov edi,edx ; next: cmp esi,[esp+2Ch] ; jae finish ; test bl,1 ; je skip ; mov edx,[esp+10h] ; push edx ; call StringLength ; test eax,eax ; je quote ; mov edx,offset " + " ; lea ecx,[esp+10h] ; call Append ; quote: mov edx,offset "'" ; lea ecx,[esp+10h] ; call Append ; mov edx,edi ; mov ecx,eax ; call Append(const CString&) ; mov edx,offset "'" ; mov ecx,eax ; call Append ; skip: shr bx,1 ; inc esi ; add edi,4 ; test bx,bx ; jne next ; finish: lea eax,[esp+10h] ; push eax ; mov ecx,ebp ; call CString::CString(const CString&) ; mov dword ptr [esp+14h],1 ; done: lea ecx,[esp+10h] ; mov byte ptr [esp+24h],0 ; call ~CString ; ... mov eax,ebp ; ... ret 8
// The classes are named by address.
extern char* g_pchNil_0139c900;            // 0x0139C900
unsigned long __stdcall StringLength_00c0ad10(const char* text);   // 0x00C0BCC0: lstrlenA, null-safe

struct CString_00c0ad10 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c0ad10() : m_pchData(g_pchNil_0139c900) { }
    CString_00c0ad10(const CString_00c0ad10& other);   // 0x00C0DAB0
    ~CString_00c0ad10();                       // 0x00C0BA40
    CString_00c0ad10* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c0ad10* __fastcall Append(const CString_00c0ad10& text);   // 0x00C0DEA0
};

CString_00c0ad10 __fastcall DescribeBitset_00c0ad10(const CString_00c0ad10* names, unsigned long count, unsigned short bits)
{
    unsigned long i = 0;
    CString_00c0ad10 line;
    if (bits == 0) {
        line.Append("[not set]");
        return line;
    }
    const CString_00c0ad10* name = names;
    while (bits != 0) {
        if (i >= count)
            break;
        if (bits & 1) {
            if (StringLength_00c0ad10(line.m_pchData))
                line.Append(" + ");
            line.Append("'")->Append(*name)->Append("'");
        }
        bits >>= 1;
        ++i;
        ++name;
    }
    return line;
}
