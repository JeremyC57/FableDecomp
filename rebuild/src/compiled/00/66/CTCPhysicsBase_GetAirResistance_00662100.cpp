struct CPhysData { char pad[0xac]; float airResistance; };
class CTCPhysicsBase { public: char pad[0xe0]; CPhysData* data; };

float __fastcall CTCPhysicsBase_GetAirResistance(const CTCPhysicsBase* self)
{
    return self->data->airResistance;
}