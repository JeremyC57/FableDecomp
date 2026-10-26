// CThingNoiseDef::Transfer(CPersistContext& context) -- persist the CThingBaseDef part, then the "Noise" sub-component
// Leaf: ?Transfer@CThingNoiseDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00455F1A, virtual (Ego_r.pdb ?Transfer@CThingNoiseDef@@UAEXAAVCPersistContext@@@Z), `ret 4`.
// Callees: CThingBaseDef::Transfer 0x00676BE0; CPersistContext::TransferSubComponent<CTCNoiseDef>(name, value) 0x00455F3C
//   (Ego_r ??$TransferSubComponent@VCTCNoiseDef@@@CPersistContext@@QAEXPBDAAVCTCNoiseDef@@@Z; the name is "Noise" @ 0x012338F0).
// Layout: Ego_r.pdb CThingNoiseDef (0xa8) less 4: Noise (CTCNoiseDef) +0x70.
// namepass DEF1: the stand-in archive class and base were CPersistContext / CThingBaseDef (one name per callee).
#pragma optimize("s",on)
class CTCNoiseDef { public: unsigned char _body[0x34]; };
class CPersistContext { public:
    template <class T> void TransferSubComponent(const char* name, T& value);
};
class CThingBaseDef { public:
    virtual void Transfer(CPersistContext& context);   // 0x00676BE0
    unsigned char _body04[0x70 - 4];                   // retail def base (Ego_r.pdb CThingBaseDef 0x74 less 4)
};
class CThingNoiseDef : public CThingBaseDef { public:
    CTCNoiseDef Noise;                 // +0x70
    virtual void Transfer(CPersistContext& context);
};

void CThingNoiseDef::Transfer(CPersistContext& context)
{
    CThingBaseDef::Transfer(context);
    context.TransferSubComponent("Noise", Noise);
}
