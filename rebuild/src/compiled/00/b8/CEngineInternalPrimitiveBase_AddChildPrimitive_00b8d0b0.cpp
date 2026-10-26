class CEnginePrimitive;
// Leaf: AddChildPrimitive
class CEngineInternalPrimitiveBase { public:
    unsigned long __fastcall AddChildPrimitive(int /*edx*/, CEnginePrimitive* prim, unsigned long flags);
};
unsigned long __fastcall CEngineInternalPrimitiveBase::AddChildPrimitive(int, CEnginePrimitive* prim, unsigned long flags) {
    (void)prim; (void)flags;
    return 0;
}