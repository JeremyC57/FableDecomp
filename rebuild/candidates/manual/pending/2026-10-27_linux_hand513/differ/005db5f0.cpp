// CAnimComponentStrikeResponseAnim::TransferBinaryOut(CDataOutputStream& out) const -- write the response animation name: its length (never negative) and, when not empty, the string itself
// Grade: DIFFER(52v56) -- retail keeps the null-string diamond of the inline length (`jne ; xor esi,esi ; jmp ; mov esi,[eax+4]`) and then re-tests the clamp (`test esi,esi ; jge`); VC7.1 here threads the null path straight onto the clamp's zero (4 bytes shorter). Tried (4): plain `if (length < 0) length = 0`, both ternary spellings, std::max(0L, length) (adds a frame), a by-reference clamp helper. Next: an if/else GetLength body or a length type other than long.
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
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->Length; }
};
inline long ClampToZero(long& value)
{
    if (value < 0)
        value = 0;
    return value;
}
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
    ClampToZero(length);
    out.WriteULONG(length);
    if ((unsigned long)length > 0)
        out.WriteString(ResponseAnimName);
}
