// CTCGatherPointBase::vfunc_1(CPersistContext* context, long unused) -- persists MaxOccupation (+0x18, zero default), then hands the context to virtual slot 30
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FFE40, length 52 bytes (`ret 8`: two arguments, the second unread).
// Callees: CPersistContext::Transfer @ 0x00410BE0 (manifest "Transfer<signed_char>"; the default is a DWORD, so a 4-byte
// instance -- the landed Transfer<long> is 0x00410620, this is a sibling 4-byte instance), this class's virtual slot 30
// (+0x78) with the context.
// Layout from Ego_r.pdb (CTCGatherPointBase, 32 bytes; retail vector 12 bytes): CTCBase +0x00..+0x0b,
// Users (vector<CIntelligentPointer<CThing const> >) +0x0c..+0x17, MaxOccupation (long) +0x18.
// Shape: the zero default lives in the `push ecx` frame slot; the context is kept in edi for the virtual call.
class CPersistContext { public:
    void Transfer(const char* name, long& value, const long& defaultValue);   // 0x00410BE0
};

class CTCGatherPointBase { public:
    unsigned char _pad_0x04[0x14];                      // +0x04 ParentThing, flags, Users (vector)
    long          MaxOccupation;                        // +0x18
    virtual void v00();
    virtual void vfunc_1(CPersistContext* context, long unused);
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26();
    virtual void v27(); virtual void v28(); virtual void v29();
    virtual void TransferDerived(CPersistContext* context);   // slot 30 (+0x78)
};

void CTCGatherPointBase::vfunc_1(CPersistContext* context, long)
{
    long defaultValue = 0;
    context->Transfer("MaxOccupation", MaxOccupation, defaultValue);
    TransferDerived(context);
}