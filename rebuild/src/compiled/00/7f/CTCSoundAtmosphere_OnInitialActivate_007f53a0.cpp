// CTCSoundAtmosphere::OnInitialActivate() -- take the sound-atmospheres def from the parent thing's def into PDef
// Leaf: ?OnInitialActivate@CTCSoundAtmosphere@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F53A0, 16 bytes, plain `ret`:
//   lea eax,[ecx+10h] ; mov ecx,[ecx+4] ; mov ecx,[ecx+70h] ; push eax ; call 0x007F5560 ; ret
// Identity: CTCSoundAtmosphere vtable 0x01271DFC slot 6 = 0x007F53A0 (rtti-vtables.tsv); slot 6 is the CTCBase
//   OnInitialActivate slot (55 landed OnInitialActivate overrides sit there, e.g. CTCDRumble 0x007F1930); Ego_r/FableWin.pdb
//   ?OnInitialActivate@CTCSoundAtmosphere@@UAEXXZ. Was the descriptive "<unattributed>::ForwardMember10ToSubobject"
//   (catalog CTCSoundAtmosphere::vfunc_6, manifest CTCInventoryAbilities::AddLearnedExpression propagated).
// Layout: CTCBase {vptr, ParentThing +4, flags +8}; Ego_r.pdb CTCSoundAtmosphere (20 bytes): TimeUntilNextUpdate +0xC,
//   PDef CDefPointer<const CSoundAtmospheresDef> +0x10. CThing PDef +0x70 (retail, as in the landed CTCDRumble model).
// Callee: the def's GetSubDef<const CSoundAtmospheresDef> @ 0x007F5560 (landed; looks up the 'CSoundAtmospheresDef' sub-def
//   index and assigns the def to the out pointer).
class CSoundAtmospheresDef;

template <class T> class CDefPointer {
public:
    T* Data;
};

class CThingBaseDef {
public:
    bool GetSubDef(CDefPointer<const CSoundAtmospheresDef>* out) const;   // 0x007F5560
};

class CThing {
public:
    unsigned char  _pad00[0x70];
    CThingBaseDef* PDef;                                // +0x70
};

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
};

class CTCSoundAtmosphere : public CTCBase {
public:
    float TimeUntilNextUpdate;                          // +0x0c
    CDefPointer<const CSoundAtmospheresDef> PDef;       // +0x10
    virtual void OnInitialActivate();
};

void CTCSoundAtmosphere::OnInitialActivate()
{
    ParentThing->PDef->GetSubDef(&PDef);
}
