struct CTCPhysicsData { char pad_f4[0xf4]; float airResistance; };
class CTCPhysicsBase { public: char pad_e0[0xe0]; CTCPhysicsData* data; };
float __fastcall CTCPhysicsBase_GetAirResistance(const CTCPhysicsBase* self)
{
    return self->data->airResistance;
}