// CAnimComponentStrikeResponseAnim::TransferBinaryOut(CDataOutputStream& out) const -- write the response animation name: its length (never negative) and, when not empty, the string itself
// Grade: RELOCATION_MATCH (retry wave 2) -- the inline GetLength written as two returns (`if (PStringData == 0) return 0; return PStringData->Length;`) keeps retail's
//   null diamond (`jne ; xor esi,esi ; jmp`); the ternary let VC7.1 thread the null path onto the clamp's zero (52v56). The inline returns the
//   unsigned length: Ego_r declares ?GetLength@CCharString@@QBEJXZ (long), but a long-returning body is threaded again (52v56); the inline
//   emits no symbol, so the bytes decide (Review at landing).
// Leaf: ?TransferBinaryOut@CAnimComponentStrikeResponseAnim@@UBEXAAVCDataOutputStream@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DB5F0, 56 bytes, virtual thiscall `ret 4`.
// Identity (proven): slot 7 of CAnimComponentStrikeResponseAnim's vtable 0x01255828 (rtti-vtables); FableWin.pdb
//   ?TransferBinaryOut@CAnimComponentStrikeResponseAnim@@UBEXAAVCDataOutputStream@@@Z; the class's only member is
//   Ego_r.pdb ResponseAnimName (CCharString) +8.
// Callees: CDataOutputStream::WriteULONG(unsigned long) 0x00993EA0, CDataOutputStream::WriteString(const CCharString&) 0x009959F0.
// Shape: the inline length (0 for a null block) is clamped at zero in a signed local, written, then tested unsigned (`jbe`).
class CCharStringData { public:
    const char*   PData;                                // +0
    unsigned long Length;                               // +4
};
class CCharString { public:
    CCharStringData* PStringData;
    unsigned long GetLength() const { if (PStringData == 0) return 0; return PStringData->Length; }   // inline (Ego_r ?GetLength@CCharString@@QBEJXZ)
};
class CDataOutputStream { public:
    void WriteULONG(unsigned long value);               // 0x00993EA0
    void WriteString(const CCharString& s);             // 0x009959F0
};
class CPersistContext;
class CMemoryDataInputStream;
class CAnimComponentBase { public:
    virtual ~CAnimComponentBase();
    virtual void _v001(); virtual void _v002();
    virtual bool Initialise(CPersistContext& context, const CCharString& value);   // slot 3
    virtual void _v004();
    virtual void TransferBinaryIn(CMemoryDataInputStream& in);                     // slot 5
    virtual void _v006();
    virtual void TransferBinaryOut(CDataOutputStream& out) const;                  // slot 7
    virtual void SetDefaultValue();                     // slot 8
    long _base04;                                       // +4
};
class CAnimComponentStrikeResponseAnim : public CAnimComponentBase { public:
    CCharString ResponseAnimName;                       // +8
    void TransferBinaryOut(CDataOutputStream& out) const;
};

void CAnimComponentStrikeResponseAnim::TransferBinaryOut(CDataOutputStream& out) const
{
    long length = ResponseAnimName.GetLength();
    if (length < 0)
        length = 0;
    out.WriteULONG(length);
    if ((unsigned long)length > 0)
        out.WriteString(ResponseAnimName);
}