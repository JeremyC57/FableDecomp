
struct CTCBData {
    char pad[0xC4];
    int lifeStage;
};
class CThingCreatureBase { public:
    char pad[0xE0];
    CTCBData* data;
};
bool __fastcall CThingCreatureBase_IsChild(const CThingCreatureBase* creature) {
    return creature->data->lifeStage == 1;
}