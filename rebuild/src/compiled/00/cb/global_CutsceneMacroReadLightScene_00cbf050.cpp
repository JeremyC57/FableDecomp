// CutsceneMacro_ReadLightScene(CCharString& line, CCharString& name, unsigned char* red, unsigned char* green, unsigned char* blue) -- split a "name:r,g,b" cutscene light line: the text before the colon becomes the name, each comma stores the parsed token through the current colour pointer
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: ?CutsceneMacro_ReadLightScene@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBF050, 236 bytes, `__fastcall` global (line in ecx, name in edx, the
//   three colour pointers on the stack: `ret 0Ch`) (PDB ?CutsceneMacro_ReadLightScene@@YIXAAVCCharString@@0PAE11@Z).
// Callees: CCharString::FindCharPos(char) @ 0x0099E5A0, GetSubString(start, count) @ 0x0099EC70 (hidden result),
//   operator= @ 0x0099EFB0, ~CCharString @ 0x0099EAE0 (x2), CCharString() @ 0x0099E4B0, operator const char*() @
//   0x0099E4C0 (x3), GFCharStringToInt(const CCharString&) @ 0x0099E7F0 (__fastcall), Append(char c, unsigned long
//   count) @ 0x0099F3B0 (PDB ?Append@CCharString@@QAEXDK@Z: appends `count` copies of c; catalogued as the propagated
//   CThingMapManager::ResizeGameMap).
// Shape: frame-pointer size TU. GetLength() is the inline `data == 0 ? 0 : data->length` (signed long: `jge`; the
//   `== 0` form reuses the null register as the zero, the empty-line guard folds into two `je`s, the bound is re-read
//   every pass). The scan starts AT the colon with its own `for` index (reusing the colon variable as the index
//   rotates the line/name/index registers ebx/edi/esi). The colour state is an unsigned char homed in the frame
//   ([ebp-1], movzx'd for the `sub eax,0 ; je ; dec eax ; je ; dec eax ; jne` switch whose case 2 and default share
//   the `state = 0` store hoisted above the last branch). Retail quirks kept: case 0 re-selects the red pointer, the
//   token is never cleared, and the append passes the token's own length as the repeat count.
class CCharString {
public:
    struct CStringData {                                        // CBasicString<char>
        char*         m_pChars;                                 // +0x00
        unsigned long m_length;                                 // +0x04
    };
    CStringData* m_pData;
    CCharString();                                              // 0x0099E4B0
    ~CCharString();                                             // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);           // 0x0099EFB0
    long FindCharPos(char c) const;                             // 0x0099E5A0
    CCharString GetSubString(unsigned long start, unsigned long count) const;   // 0x0099EC70
    operator const char*() const;                               // 0x0099E4C0
    void Append(char c, unsigned long count);                   // 0x0099F3B0
    long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
};
long __fastcall GFCharStringToInt(const CCharString& text);    // 0x0099E7F0

void __fastcall CutsceneMacro_ReadLightScene(CCharString& line, CCharString& name, unsigned char* red, unsigned char* green, unsigned char* blue)
{
    if (line.GetLength() == 0)
        return;
    long colon = line.FindCharPos(':');
    if (colon == -1)
        return;
    name = line.GetSubString(0, colon);
    CCharString token;
    unsigned char* dest = red;
    unsigned char state = 0;
    for (long pos = colon; pos < line.GetLength(); ++pos) {
        if (((const char*)line)[pos] == ',') {
            *dest = (unsigned char)GFCharStringToInt(token);
            switch (state) {
            case 0: dest = red;   state = 1; break;
            case 1: dest = green; state = 2; break;
            case 2: dest = blue;  state = 0; break;
            default: state = 0; break;
            }
        } else if (((const char*)line)[pos] != ' ') {
            token.Append(((const char*)line)[pos], token.GetLength());
        }
    }
}