// NThingShot::CTCShotDefinitionDrainLife::~CTCShotDefinitionDrainLife() -- kills the soul effect (+0xac) if it is still alive, then the member pointer and the CTCShotDefinitionBase base are destroyed
// Leaf: ??1CTCShotDefinitionDrainLife
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BD80A, length 59 bytes; called from the scalar deleting destructor at
// 0x004DB1A9.
// Callees: IS_THING_ALIVE<CIntelligentPointer<...>>(const CIntelligentPointer&) @ 0x004CC340 (__fastcall: non-null item
// whose +0x91 bit 0 is clear), CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill(bool) @ 0x004C9B80,
// ~CBaseIntelligentPointer @ 0x00A01C10, NThingShot::CTCShotDefinitionBase::~CTCShotDefinitionBase @ 0x007BF2B0 (tail
// jump). Vtable 0x012403F4 = NThingShot::CTCShotDefinitionDrainLife.
// Layout: PSoulEffect at +0xac (PDB +0xb0; the retail base is 0x9c bytes, as in the landed constructor 0x004DB15E).
// Shape: the member's address is formed once (`lea edi,[esi+0ACh]`) before the vtable store; the CIntelligentPointer
// member has a compiler-generated destructor (no vtable reset, just the base destructor call); the base destructor
// becomes the tail jump.
class CBaseObject;

class CThing { public:
    void Kill(bool immediately);             // 0x004C9B80
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                    // +4
    ~CBaseIntelligentPointer();              // 0x00A01C10
    CBaseObject* GetPItem() const;           // 0x00A01B50
};

struct CIntelligentPointer_CThing : CBaseIntelligentPointer {
    virtual void v00();
    CThing* operator->() const { return (CThing*)GetPItem(); }
};

bool __fastcall IS_THING_ALIVE(const CIntelligentPointer_CThing& thing);   // 0x004CC340

namespace NThingShot {

class CTCShotDefinitionBase { public:
    virtual ~CTCShotDefinitionBase();        // 0x007BF2B0
    unsigned char _body[0x9c - 4];           // retail base: 0x9c bytes
};

class CTCShotDefinitionDrainLife : public CTCShotDefinitionBase { public:
    float                      MaxCosAngleToTurn;   // +0x9c
    float                      MaxAngleToTurn;      // +0xa0
    float                      AmountToHeal;        // +0xa4
    float                      Speed;               // +0xa8
    CIntelligentPointer_CThing PSoulEffect;         // +0xac
    virtual ~CTCShotDefinitionDrainLife();
};

CTCShotDefinitionDrainLife::~CTCShotDefinitionDrainLife()
{
    if (IS_THING_ALIVE(PSoulEffect))
        PSoulEffect->Kill(true);
}

}