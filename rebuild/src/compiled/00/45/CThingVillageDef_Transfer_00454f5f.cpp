#pragma optimize("s",on)
struct CTransferArc {
    void TransferField(int* field);
};
class CThingVillageDef { public:
    void BaseTransfer(CTransferArc* arc);
    char pad[0x70];
    int member70;
};

void __fastcall CThingVillageDef_Transfer(CThingVillageDef* self, void* edx, CTransferArc* arc)
{
    self->BaseTransfer(arc);
    arc->TransferField(&self->member70);
}