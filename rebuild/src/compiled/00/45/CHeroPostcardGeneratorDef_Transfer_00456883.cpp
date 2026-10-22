#pragma optimize("s",on)
struct CPersistContext {
    void PutA(void* p);
    void PutB(void* p);
    void PutC(void* p);
};
struct CHeroPostcardGeneratorDef {
    char pad[0x58];
    char base[4];
};

void __fastcall CHeroPostcardGeneratorDef_Transfer(CHeroPostcardGeneratorDef* def, int edx, CPersistContext* ctx)
{
    ctx->PutA((char*)def + 0x39);
    ctx->PutA((char*)def + 0x41);
    ctx->PutB((char*)def + 0x49);
    ctx->PutC((char*)def + 0x58);
}