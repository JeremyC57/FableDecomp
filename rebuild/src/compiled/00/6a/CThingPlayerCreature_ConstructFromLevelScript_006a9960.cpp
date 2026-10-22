// CThingPlayerCreature::vfunc_4(const char*) -- base construct step, then MaxHealth from the creature def, Health filled when it is still 0, and the Drawable flag set
// Retail: Fable.exe 0x006A9960, 82 bytes (CThingPlayerCreature vtable 0x012457FC slot 4).
// Callees: CThingCreatureBase 0x00662930 (manifest "ConstructFromLevelScript", the direct base call);
// float pool 0x0122DEDC = 0.0f.
// Retail offsets: PDef (CDefPointer<CThingCreatureDef>) +0xe0, MaxHealth +0xb0, Health +0xb4, the CThing
// flag byte +0x92 (PDB +0xa2: DeletedOnOwnerLevelOutOfScope, Drawable, ...; bit 1 = Drawable). The def's
// maximum health is the float at +0x50. Local model (the generated headers keep the PDB offsets).
// Shape: body under `if (base call)` with the shared `xor al,al` tail; the def value is stored with `fst`
// and kept on the x87 stack for the conditional `Health = MaxHealth` (`fucompp` against the 0.0f pool
// = `Health == 0.0f`); the bitfield set is `or byte ptr [esi+92h],2`.
struct CThingCreatureDef {
    unsigned char _pad00[0x50];
    float         MaxHealth;                             // +0x50
};

template <class T> struct CDefPointer {
    T* m_pDef;
    T* operator->() const { return m_pDef; }
};

struct CThingCreatureBase {
    unsigned char                   _pad00[0x92];
    unsigned char                   DeletedOnOwnerLevelOutOfScope : 1;   // +0x92 bit 0
    unsigned char                   Drawable : 1;                        // +0x92 bit 1
    unsigned char                   _flags92_2 : 6;
    unsigned char                   _pad93[0x1d];
    float                           MaxHealth;           // +0xb0
    float                           Health;              // +0xb4
    unsigned char                   _padb8[0x28];
    CDefPointer<CThingCreatureDef>  PDef;                // +0xe0
    bool ConstructFromLevelScript(const char* script);   // 0x00662930
};

struct CThingPlayerCreature : CThingCreatureBase {
    bool vfunc_4(const char* script);
};

bool CThingPlayerCreature::vfunc_4(const char* script)
{
    if (CThingCreatureBase::ConstructFromLevelScript(script))
    {
        MaxHealth = PDef->MaxHealth;
        if (Health == 0.0f)
            Health = MaxHealth;
        Drawable = true;
        return true;
    }
    return false;
}