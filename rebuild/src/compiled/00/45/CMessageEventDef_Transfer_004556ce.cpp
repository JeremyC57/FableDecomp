// Leaf: ?Transfer@CMessageEventDef@@UAEXAAVCPersistContext@@@Z
#pragma optimize("s",on)
class CPersistContext { public:
    template <class T> void Transfer(T& value);
    void TransferB_00455712(void* p);
    void TransferC(void* p);
};
class CMessageEventDef { public:
    char pad[0x38];
    int f3c;
    int MaxSeenRadius;
    int MaxHeardRadius;
    int Lifespan;
    int ReactionSpeechType;
    virtual void Transfer(class CPersistContext&);
};

void CMessageEventDef::Transfer(class CPersistContext& ctx)
{
    CMessageEventDef* message = this;
    ctx.Transfer(*(long*)(&message->f3c));
    ctx.Transfer(*(long*)(&message->MaxSeenRadius));
    ctx.Transfer(*(long*)(&message->MaxHeardRadius));
    ctx.TransferB_00455712(&message->ReactionSpeechType);
    ctx.TransferC(&message->Lifespan);
}
// Flatten route (flatten_pilot v4, NF324): CMessageEventDef_Transfer -> CMessageEventDef::Transfer, the RTTI-placed PDB member; class model: the row's local class model.
