// CEnginePrimitiveManagerSprites::CEnginePrimitiveManagerSprites(void*) -- base CEnginePrimitiveManager(argument) then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BACF70, oracle length 25 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push dword ptr [esp+8] ; mov esi,ecx ; call CEnginePrimitiveManager::CEnginePrimitiveManager ; mov dword ptr [esi],offset CEnginePrimitiveManagerSprites::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor of a polymorphic class that only forwards the argument to its base
// constructor @ 0x00BACDB0; the compiler then stores this class's vtable (0x012A55B4, named
// through rtti-vtables.tsv) and returns `this`. One of 12 bodies of this shape.

class CEnginePrimitiveManager { public:
    virtual void v00();
    CEnginePrimitiveManager(void* argument);            // 0x00BACDB0
};

class CEnginePrimitiveManagerSprites : public CEnginePrimitiveManager { public:
    virtual void v00();
    CEnginePrimitiveManagerSprites(void* argument);
};

CEnginePrimitiveManagerSprites::CEnginePrimitiveManagerSprites(void* argument)
    : CEnginePrimitiveManager(argument)
{
}