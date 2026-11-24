// Leaf: ?Transfer@CHeroPostcardGeneratorDef@@UAEXAAVCPersistContext@@@Z
#pragma optimize("s",on)
class CPersistContext { public:
    void PutA(void* p);
    void PutB(void* p);
    template <class T> void Transfer(T& value);
};
class CHeroPostcardGeneratorDef { public:
    char pad[0x54];
    char base[4];
    virtual void Transfer(class CPersistContext&);
};

void CHeroPostcardGeneratorDef::Transfer(class CPersistContext& ctx)
{
    CHeroPostcardGeneratorDef* def = this;
    ctx.PutA((char*)def + 0x39);
    ctx.PutA((char*)def + 0x41);
    ctx.PutB((char*)def + 0x49);
    ctx.Transfer(*(float*)((char*)def + 0x58));
}
// Flatten route (flatten_pilot v4, NF324): CHeroPostcardGeneratorDef_Transfer -> CHeroPostcardGeneratorDef::Transfer, the RTTI-placed PDB member; class model: the row's local class model.
