// Leaf: ?GetSizeofClass@CExpressionDef@@UAEKXZ
class CExpressionDef { public:
    virtual unsigned long GetSizeofClass();
};
unsigned long CExpressionDef::GetSizeofClass()
{
    return 0x90;
}
// Flatten route (flatten_pilot v4, NF324): CExpressionDef_GetSizeofClass -> CExpressionDef::GetSizeofClass, the RTTI-placed PDB member; class model: the row's local class model.
