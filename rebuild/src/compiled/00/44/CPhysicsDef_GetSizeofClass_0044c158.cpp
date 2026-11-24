// Leaf: ?GetSizeofClass@CPhysicsDef@@UAEKXZ
#pragma optimize("s",on)
class CPhysicsDef { public:
    virtual unsigned long GetSizeofClass();
};

unsigned long CPhysicsDef::GetSizeofClass()
{
    return 0x44;
}
// Flatten route (flatten_pilot v4, NF324): CPhysicsDef_GetSizeofClass -> CPhysicsDef::GetSizeofClass, the RTTI-placed PDB member; class model: the row's local class model.
