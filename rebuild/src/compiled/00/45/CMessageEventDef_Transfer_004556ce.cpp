#pragma optimize("s",on)
class CPersistContext { public:
    template <class T> void Transfer(T& value);
    void TransferB_00455712(void* p);
    void TransferC(void* p);
};
class CMessageEventDef { public:
    char pad[0x3c];
    int f3c;
    int MaxSeenRadius;
    int MaxHeardRadius;
    int Lifespan;
    int ReactionSpeechType;
};

void __fastcall CMessageEventDef_Transfer(CMessageEventDef* message, void* edx, CPersistContext* ctx) {
    ctx->Transfer(*(long*)(&message->f3c));
    ctx->Transfer(*(long*)(&message->MaxSeenRadius));
    ctx->Transfer(*(long*)(&message->MaxHeardRadius));
    ctx->TransferB_00455712(&message->ReactionSpeechType);
    ctx->TransferC(&message->Lifespan);
}