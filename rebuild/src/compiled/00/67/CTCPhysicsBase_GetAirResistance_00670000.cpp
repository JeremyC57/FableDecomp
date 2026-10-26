struct CTCPhysicsData { char pad[0x38]; float airResistance; };
class CTCPhysicsBase { public: char pad[0x28]; CTCPhysicsData* data; };

float __fastcall CTCPhysicsBase_GetAirResistance(const CTCPhysicsBase* self)
{
    return self->data->airResistance;
}