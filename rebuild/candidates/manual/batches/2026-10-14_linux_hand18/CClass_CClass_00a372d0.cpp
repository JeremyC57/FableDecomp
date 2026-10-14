// CClass::CClass(CClassFactory*, unsigned long, bool) -- base CBaseIntelligentPointer(), own vtable, Type = (unsigned char)type, then factory->RegisterAllocatedClass(this, type, local)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A372D0, length 43 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x0099A380 (vtable 0x0129A7D8, null PData),
// CClassFactory::RegisterAllocatedClass(CClass*, unsigned long, bool) @ 0x00A371F0 (manifest
// auto-name "CClass_AddAllocation": links the class into the factory's ClassList and into
// ClassTypeInfo[type].TypeList and records both list positions in the class). Vtable 0x0129CF60 = CClass.
// Layout = engine/CClass.h (ClassListPos +8, TypeListPos +0xc, Type +0x10, 0x11 bytes) with the
// flattened +4 word being CBaseIntelligentPointer::PData; the header is a flat POD, hence the
// `_Calls` model carrying the real base and the constructor. The two list positions are left to the
// register call. The vtable and Type stores are scheduled between the argument pushes.
struct CClassFactory_Calls;

struct CBaseIntelligentPointer {
    virtual void v00();
    void* PData;                                            // +4
    CBaseIntelligentPointer();                              // 0x0099A380
};

struct CClass_Calls : CBaseIntelligentPointer {
    void*         ClassListPos;                             // +0x08 list<CClass*>::iterator (node pointer)
    void*         TypeListPos;                              // +0x0c
    unsigned char Type;                                     // +0x10
    virtual void v00();                                     // own vtable 0x0129CF60
    CClass_Calls(CClassFactory_Calls* factory, unsigned long type, bool local);
};

struct CClassFactory_Calls {
    void RegisterAllocatedClass(CClass_Calls* cls, unsigned long type, bool local);   // 0x00A371F0
};

CClass_Calls::CClass_Calls(CClassFactory_Calls* factory, unsigned long type, bool local)
    : CBaseIntelligentPointer()
    , Type((unsigned char)type)
{
    factory->RegisterAllocatedClass(this, type, local);
}
