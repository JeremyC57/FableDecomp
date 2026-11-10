// CThingNoise::OnCreate() -- returns the base CThing::OnCreate() result as a normalised bool
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00838550, oracle length 11 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  call OnCreate ; test al,al ; setne al ; ret
// Leaf: ?OnCreate@CThingNoise@@UAE_NXZ
// Identity: CThingNoise's OnCreate override (Ego_r/FableWin ?OnCreate@CThingNoise@@UAE_NXZ) calls the base CThing::OnCreate
// 0x004C9560 non-virtually on the same object (ecx passes through untouched) and normalises its bool result.
class CThing { public:
    virtual bool OnCreate();                 // 0x004C9560 (Ego_r/FableWin ?OnCreate@CThing@@UAE_NXZ; called non-virtually)
};
class CThingNoise : public CThing { public:
    virtual bool OnCreate();                 // Ego_r/FableWin ?OnCreate@CThingNoise@@UAE_NXZ
};

bool CThingNoise::OnCreate()
{
    return CThing::OnCreate() ? true : false;
}