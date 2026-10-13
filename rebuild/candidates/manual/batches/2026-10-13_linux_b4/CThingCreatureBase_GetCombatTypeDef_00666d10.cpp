// CThingCreatureBase::GetCombatTypeDef()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00666D10, oracle length 47 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  push ecx ; push esi ; lea eax,[esp+4] ; push eax ; call CThingCreatureBase::GetPCombatTypeDef ; mov esi,[eax] ; mov eax,[esp+4] ; test eax,eax ; je R ; mov edx,[eax+4] ; mov ecx,eax ; add eax,4 ; dec edx ; test edx,edx ; mov [eax],edx ; jne R ; mov edx,[ecx] ; call [edx+4] ; R: mov eax,esi ; pop esi ; pop ecx ; ret
// Convenience over GetPCombatTypeDef @ 0x00664D40, which returns a CDefPointer<CCombatTypeDef>
// (ref-counted, via a hidden pointer): the raw def pointer is read out of the temporary, the
// temporary's destructor drops the reference (virtual slot 1 of the pointee frees it when the
// count hits zero), and the raw pointer is returned.

struct CDefPointeeBase {
    virtual void v00();
    virtual void Release();          // slot 1 (+0x04): called when the reference count reaches 0
    long RefCount;                   // +0x04
};

struct CDefPointer_CombatTypeDef {
    CDefPointeeBase* Object;
    ~CDefPointer_CombatTypeDef()
    {
        CDefPointeeBase* object = this->Object;
        if (object) {
            if (--object->RefCount == 0)
                object->Release();
        }
    }
};

struct CThingCreatureBase_Methods {
    CDefPointer_CombatTypeDef GetPCombatTypeDef();      // 0x00664D40
    CDefPointeeBase* GetCombatTypeDef();
};

CDefPointeeBase* CThingCreatureBase_Methods::GetCombatTypeDef()
{
    return GetPCombatTypeDef().Object;
}
