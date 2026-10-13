// CNamedQuery_00dd6a00::Query__00dd6a00() -- asks virtual slot 18 of the embedded object at +0x8 about ""; on true sets the byte at +0x68 of the object at +0x14
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DD6A00, oracle length 61 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ecx ; push ebx ; push esi ; push -1 ; mov esi,ecx ; push offset "" ; lea ecx,[esp+10h] ; call CCharString::CCharString ; mov eax,[esi+0x8] ; lea ecx,[esi+0x8] ; lea edx,[esp+8] ; push edx ; call [eax+0x48] ; lea ecx,[esp+8] ; mov bl,al ; call CCharString::~CCharString ; test bl,bl ; je E ; mov eax,[esi+0x14] ; mov byte ptr [eax+0x68],1 ; E: pop esi ; pop ebx ; pop ecx ; ret
// Builds a scoped CCharString from the literal (constructor @ 0x0099EBF0, destructor
// @ 0x0099EAE0, labels propagated), calls virtual slot 18 (vtable offset 0x48) of the
// polymorphic member embedded at +0x8 with it, keeps the bool result in bl across the
// string's destructor, and on success sets the byte flag at +0x68 of the object at
// +0x14. Unnamed in the manifest (gap scan). One of 12 bodies of this shape.
struct CCharString {                     // one pointer; the default length makes the literal convert implicitly (re-addressed with lea, GOTCHAS)
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                        // 0x0099EAE0
};

struct CQueryTarget_00dd6a00 {
    virtual void _slot000();
    virtual void _slot001();
    virtual void _slot002();
    virtual void _slot003();
    virtual void _slot004();
    virtual void _slot005();
    virtual void _slot006();
    virtual void _slot007();
    virtual void _slot008();
    virtual void _slot009();
    virtual void _slot010();
    virtual void _slot011();
    virtual void _slot012();
    virtual void _slot013();
    virtual void _slot014();
    virtual void _slot015();
    virtual void _slot016();
    virtual void _slot017();
    virtual bool Query(const CCharString& name);   // slot 18
};

struct SFlagOwner_00dd6a00 {
    unsigned char _pad[0x68];
    unsigned char m_68;                 // +0x68
};

struct CNamedQuery_00dd6a00 {
    unsigned char _pad[0x8];
    CQueryTarget_00dd6a00 m_08;            // +0x8 embedded (vptr first)
    unsigned char _pad2[0x8];
    SFlagOwner_00dd6a00* m_14;             // +0x14
    void Query__00dd6a00();
};

void CNamedQuery_00dd6a00::Query__00dd6a00()
{
    bool found = m_08.Query("");   // the temporary dies after the call; the result rides in bl
    if (found)
        m_14->m_68 = 1;
}
