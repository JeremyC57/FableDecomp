// CModeSitTavernStool::CModeSitTavernStool(CThing*) -- base CModeSitBase(thing), own vtable, CAnimFilterOfGroup member at +0x2c built from CCharString(SIT_TAVERN_STOOL, -1): string assigned, CRC of the name stored
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094B530, oracle length 116 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[esp+4] ; push esi ; push edi ; push eax ; mov esi,ecx ; call CModeSitBase::CModeSitBase ; push -1 ; push offset "SIT_TAVERN_STOOL" ; lea ecx,[esp+14h] ; mov dword ptr [esi],offset CModeSitTavernStool::`vftable' ; call CCharString::CCharString(const char*, long) ; lea edi,[esi+0x30] ; mov ecx,edi ; mov dword ptr [esi+0x2c],offset CAnimFilterOfGroup::`vftable' ; call CCharString::CCharString() ; lea ecx,[esp+0Ch] ; push ecx ; mov ecx,edi ; call CCharString::operator= ; mov eax,[esp+0Ch] ; test eax,eax ; jne C ; xor ecx,ecx ; mov eax,offset "" ; jmp D ; C: mov ecx,[eax+4] ; mov eax,[eax] ; D: push ecx ; mov edx,eax ; xor ecx,ecx ; call Calc ; lea ecx,[esp+0Ch] ; mov [esi+0x34],eax ; call CCharString::~CCharString ; pop edi ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor of a CModeSitBase-derived class: the base constructor @ 0x008D7CA0
// runs with the owner, this class's vtable (0x01292FF8) is installed, then the CAnimFilterOfGroup member at
// +0x2c (vtable 0x01267568) is constructed from a CCharString temporary built from the
// literal: the member's own constructor is inlined -- default-construct its string
// (@ 0x0099E4B0), assign the name (@ 0x0099EFB0) and store the name's CRC
// (Calc @ 0x004014A0, `__fastcall(seed, data, length)`, run over the empty literal when the
// string has no data block). The temporary is destroyed (@ 0x0099EAE0) before returning
// `this`. Names come from rtti-vtables.tsv. One of 46 bodies of this shape.
struct CThing;

struct CCharStringData {
    const char* m_pData;                   // +0  (CBasicString<char>::PData)
    unsigned long m_length;                // +4  (StringLength)
};

struct CCharString {
    CCharStringData* m_pData;
    CCharString();                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0; the default argument makes the literal convert implicitly
    ~CCharString();                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC

struct CAnimFilterOfGroup {
    virtual void v00();
    CCharString m_string;                  // +4
    unsigned long m_hash;                  // +8
    CAnimFilterOfGroup(const CCharString& name)
        : m_string()
    {
        m_string = name;
        const char* data;
        unsigned long length;
        if (name.m_pData == 0) {
            length = 0;
            data = "";
        }
        else {
            length = name.m_pData->m_length;
            data = name.m_pData->m_pData;
        }
        m_hash = Calc_004014a0(0, data, length);
    }
};

struct CModeSitBase {
    virtual void v00();
    unsigned char _body[0x28];
    CModeSitBase(CThing* thing);              // 0x008D7CA0
};

struct CModeSitTavernStool : CModeSitBase {
    CAnimFilterOfGroup m_name;                         // +0x2c
    virtual void v00();
    CModeSitTavernStool(CThing* thing);
};

CModeSitTavernStool::CModeSitTavernStool(CThing* thing)
    : CModeSitBase(thing)
    , m_name("SIT_TAVERN_STOOL")                        // implicit CCharString conversion: the temporary is re-addressed with lea (an explicit CCharString("..", -1) would reuse the constructor's returned pointer)
{
}
