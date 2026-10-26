// _global::FUN_006d4d00() -- CIntelligentPointer<CThing> raw-pointer accessor: forwards to CBaseIntelligentPointer::GetPItem
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D4D00, 5 bytes: a lone `jmp 0x00A01B50` (tail call, `this` passed through).
//   Unreferenced out-of-line copy of an inline typed accessor (the next function, 0x006D4D10, is the
//   CIntelligentPointer<const CThing> constructor).
class CThing;
class CBaseObject;

struct CBaseIntelligentPointer {
    void*        __vftable;                      // +0x00
    void*        PData;                          // +0x04
    CBaseObject* GetPItem() const;               // 0x00A01B50
};

struct CIntelligentPointer_CThing : CBaseIntelligentPointer {
    CThing* FUN_006d4d00() const;
};

CThing* CIntelligentPointer_CThing::FUN_006d4d00() const
{
    return (CThing*)GetPItem();
}