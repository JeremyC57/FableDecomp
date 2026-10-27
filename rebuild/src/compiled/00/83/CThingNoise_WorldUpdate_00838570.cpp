// CThingNoise::WorldUpdate() -- run the base CThing world update, then report "nothing more to do" (false)
// Leaf: ?WorldUpdate@CThingNoise@@UAE_NXZ
// Retail: Fable.exe @ 0x00838570, 8 bytes, thiscall plain `ret`, bool in al.
// Identity (proven): vtable slot 19 (+0x4C) of CThingNoise 0x01272EE4. The sixteen classes that override slot 19 in
// rtti-vtables.tsv are exactly the sixteen classes Ego_r.pdb / FableWin.pdb give ?WorldUpdate@<Class>@@UAE_NXZ
// (CThing, CThingNoise, CThingSoundEmitter, CThingCreatureBase, ...), and CThing's slot 19 is 0x004C8050, the landed
// CThing::WorldUpdate. Ego_r.pdb has ?WorldUpdate@CThingNoise@@UAE_NXZ.
// Label correction: catalog "CThingNoise::vfunc_19".
// Callees: 0x004C8050 ?WorldUpdate@CThing@@UAE_NXZ (qualified, non-virtual call; reloc_bindings currently binds it as the
// stand-in ?GetState@CThingSoundEmitter@@QAEEXZ from landed 0x00836780).
// Shape: the base result is discarded and the constant false comes back as `xor al,al`.
class CThing {
public:
    virtual ~CThing();
    virtual bool WorldUpdate();
};

class CThingNoise : public CThing {
public:
    virtual bool WorldUpdate();
};

bool CThingNoise::WorldUpdate()
{
    CThing::WorldUpdate();
    return false;
}