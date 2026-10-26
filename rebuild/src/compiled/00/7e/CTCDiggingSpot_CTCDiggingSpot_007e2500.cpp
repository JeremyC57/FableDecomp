// CTCDiggingSpot::CTCDiggingSpot(CThing*) -- thing component of a digging spot: CTCBase(parent), own vtable, the "can be dug" flag (+0xc) set and the "dug" flag (+0xd) cleared
// Leaf: ??0CTCDiggingSpot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E2500, length 33 bytes (+15 int3 padding).
// Callee: CTCBase::CTCBase(CThing*) @ 0x00686800 (landed in 0x007509A0). Vtable 0x0127017C = CTCDiggingSpot.
// Label correction: the manifest "CTCShotDefinitionNymphFun::CTCShotDefinitionNymphFun" is propagated; the vtable is
// CTCDiggingSpot's.
// Shape: speed TU (`mov eax,[esp+4] ; push eax`); the two byte initialisers follow the vtable store in declaration order.
class CThing;

class CTCBase { public:
    virtual void v00();
    unsigned char _body[0x8];                // ParentThing +4, flags +8/+9
    CTCBase(CThing* parent);                 // 0x00686800
};

class CTCDiggingSpot : public CTCBase { public:
    bool m_canBeDug;                         // +0x0c
    bool m_dug;                              // +0x0d
    virtual void v00();
    CTCDiggingSpot(CThing* parent);
};

CTCDiggingSpot::CTCDiggingSpot(CThing* parent)
    : CTCBase(parent)
    , m_canBeDug(true)
    , m_dug(false)
{
}