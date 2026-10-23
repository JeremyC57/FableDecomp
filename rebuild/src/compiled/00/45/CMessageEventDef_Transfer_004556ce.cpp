#pragma optimize("s",on)
struct CPersistContext {
    void TransferA(void* p);
    void TransferB_00455712(void* p);
    void TransferC(void* p);
};
struct CMessageEventDef {
    char pad[0x3c];
    int f3c;
    int MaxSeenRadius;
    int MaxHeardRadius;
    int Lifespan;
    int ReactionSpeechType;
};

void __fastcall CMessageEventDef_Transfer(CMessageEventDef* message, void* edx, CPersistContext* ctx) {
    ctx->TransferA(&message->f3c);
    ctx->TransferA(&message->MaxSeenRadius);
    ctx->TransferA(&message->MaxHeardRadius);
    ctx->TransferB_00455712(&message->ReactionSpeechType);
    ctx->TransferC(&message->Lifespan);
}