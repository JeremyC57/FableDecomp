// CCreatureAction_SheatheTrophy::Clone() -- new CCreatureAction_SheatheTrophy(*this) with the implicit copy constructor inlined
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DFCE0, oracle length 47 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; push 0xb0 ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call CScriptGameResourceObjectScriptedThingBase::CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase&) ; pop edi ; mov dword ptr [esi],offset CCreatureAction_SheatheTrophy::`vftable' ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone of a class that adds no data to its base: the implicit copy constructor is
// inlined as the base copy constructor @ 0x006A0C50 plus this class's vtable store
// (0x0125FB14, named through rtti-vtables.tsv). sizeof = 0xb0. One of 28 bodies of this shape.
struct CScriptGameResourceObjectScriptedThingBase {
    virtual void v00();
    unsigned char _body[0xb0 - 4];    // sizeof(CScriptGameResourceObjectScriptedThingBase) = sizeof(CCreatureAction_SheatheTrophy) = 0xb0
    CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase& other);     // 0x006A0C50
};

struct CCreatureAction_SheatheTrophy : CScriptGameResourceObjectScriptedThingBase {
    virtual void v00();                    // own vtable; copy constructor implicit
    CScriptGameResourceObjectScriptedThingBase* Clone();
};

CScriptGameResourceObjectScriptedThingBase* CCreatureAction_SheatheTrophy::Clone()
{
    return new CCreatureAction_SheatheTrophy(*this);
}