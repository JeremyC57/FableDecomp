// CTCWallMount::GetClassTypeName  @ 0x007df2c0
// Returns a CCharString constructed from a string literal.
// Disasm:
//   push esi; mov esi,[esp+8]      ; esi = hidden return-buffer ptr
//   push -1; push OFFSET literal   ; args (const char*, int len=-1)
//   mov ecx,esi; call CCharString::ctor
//   mov eax,esi; pop esi; ret 4    ; return the buffer ptr

class CCharString {
public:
    CCharString(const char* s, long len);
private:
    char* m_pData;
    int   m_len;
};

// The literal the ctor is fed (address 0x123d724 in retail).
const char g_actionName[] = "DoCreatureAction";

class CTCWallMount {
public:
    CCharString GetClassTypeName();
};

CCharString CTCWallMount::GetClassTypeName()
{
    return CCharString(g_actionName, -1);
}