// Member getter at this+0xac. __fastcall this=ecx. pack(1) keeps the field at
// the retail byte offset (natural alignment would shift an odd one).
#pragma pack(push,1)
struct CScriptedControl {
    char pad[0xac];
    float field;
    float GetObservedThingCurrentMeshHeight();
};
#pragma pack(pop)
float CScriptedControl::GetObservedThingCurrentMeshHeight() { return this->field; }