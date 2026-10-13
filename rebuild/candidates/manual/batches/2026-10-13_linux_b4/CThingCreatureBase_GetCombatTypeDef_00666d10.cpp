// CThingCreatureBase::GetCombatTypeDef()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00666D10, oracle length 47 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push ecx ; push esi ; lea eax,[esp+4] ; push eax ; call CThingCreatureBase::GetPCombatTypeDef ; mov esi,[eax] ; mov eax,[esp+4] ; test eax,eax ; je R ; mov edx,[eax+4] ; mov ecx,eax ; add eax,4 ; dec edx ; test edx,edx ; mov [eax],edx ; jne R ; mov edx,[ecx] ; call [edx+4] ; R: mov eax,esi ; pop esi ; pop ecx ; ret
// Convenience over GetPCombatTypeDef @ 0x00664D40, which returns a CDefPointer<CCombatTypeDef>
// (ref-counted, via a hidden pointer): the raw def pointer is read out of the temporary, the
// temporary's destructor drops the reference (virtual slot 1 of the pointee frees it when the
// count hits zero), and the raw pointer is returned.
//
// Lane feedback: byte-exact when the release is an inline pointee method (`mov ecx,eax` forms inside the
// null test); the open-coded `--RefCount` in the smart-pointer dtor differs. The behaviour test
// needed a ref-counting copy constructor (VC7.1 copies the by-value return). RELOCATION_MATCH.
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();          // slot 1 (+0x04): called when the reference count reaches 0
    long RefCount;                   // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }   // inline: `this` is formed inside the test
};

struct CDefPointer_CombatTypeDef {
    CDefPointeeBase* Object;
    ~CDefPointer_CombatTypeDef() { if (Object) Object->Release(); }
};

struct CThingCreatureBase_Methods {
    CDefPointer_CombatTypeDef GetPCombatTypeDef();      // 0x00664D40
    CDefPointeeBase* GetCombatTypeDef();
};

CDefPointeeBase* CThingCreatureBase_Methods::GetCombatTypeDef()
{
    // Raw pointer out of the counted def pointer; the temporary's destructor releases the ref.
    return GetPCombatTypeDef().Object;
}
