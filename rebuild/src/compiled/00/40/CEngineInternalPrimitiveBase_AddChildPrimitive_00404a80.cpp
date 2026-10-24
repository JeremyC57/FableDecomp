// CEngineInternalPrimitiveBase::AddChildPrimitive() -- accessor returning the address of the engine root object at 0x013B7CD8
// Leaf: AddChildPrimitive
// Retail: Fable.exe (TLC, VC7.1) @ 0x00404A80, 6 bytes. Callees: none.
//
// `mov eax, offset g_engineRoot; ret`. This is the "get the engine" accessor that GFInitialise and the
// shutdown path use to reach the engine root object (the one holding the display, primitive, texture and
// resource managers). The AddChildPrimitive label on this address is a propagated name and has nothing
// to do with what the function does; it is a global-object getter.
//
// Matching: returns the address of a global (absolute relocation), not its contents.
extern unsigned char g_primitiveVTableOrData[0x54];   // the engine root object at 0x013B7CD8

unsigned long CEngineInternalPrimitiveBase_AddChildPrimitive()
{
    return (unsigned long)(&g_primitiveVTableOrData[0]);
}
