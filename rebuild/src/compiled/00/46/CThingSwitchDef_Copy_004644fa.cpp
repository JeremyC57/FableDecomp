// Leaf: ?Copy@CThingSwitchDef@@UAEXPBVCDefClassBase@@@Z
class CDefClassBase;

class CThingSwitchDef { public:
    char pad[0x6c];
    int f70;
    // base-class copy at 0x463950; member => thiscall(this, src)
    void base_clone(const CDefClassBase* src);
    virtual void Copy(const class CDefClassBase*);
};

void CThingSwitchDef::Copy(const class CDefClassBase* src)
{
    CThingSwitchDef* self = this;
    self->base_clone(src);
    self->f70 = ((const CThingSwitchDef*)src)->f70;
}
// Flatten route (flatten_pilot v6, NF335): CThingSwitchDef_Copy -> CThingSwitchDef::Copy, the RTTI-placed PDB member; class model: the row's local class model.
