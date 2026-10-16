// CThingSwitch::SerialiseGameState(CPersistContext* context) -- base serialise, clears Triggered unless the context mode is 1 or 3, then transfers TriggerRadius, TimeToChangeEnvironmentDef, TriggerableObjectScriptName and EnvironmentDef
// Leaf: ?SerialiseGameState@CThingSwitch
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083C230, length 156 bytes (`ret 4`, + int3 padding). CThingSwitch vtable 0x012453FC slot 3
// (the catalog files it under CThingPhysicalSwitch; that class's SerialiseGameState is the landed 0x0083D940).
// Callees: CThingPhysical::SerialiseGameState @ 0x006A5AF0, CPersistContext::Transfer<float>(name, float*, const float& default)
// @ 0x00410620 (manifest "Transfer<signed char>" is a propagated label), CPersistContext::Transfer<CCharString>(name, CCharString*) @ 0x00411010.
// Strings: "TriggerRadius" 0x012696E4, "TimeToChangeEnvironmentDef" 0x01273094, "TriggerableObjectScriptName" 0x01273078, "EnvironmentDef" 0x01244D10.
// Layout: Ego_r.pdb CThingSwitch: TriggerRadius +0xe8, EnvironmentDef +0xec, TimeToChangeEnvironmentDef +0xf0, Triggered +0xf4,
// TriggerableObjectScriptName +0xf8; retail sits 0x18 lower (+0xd0 / +0xd4 / +0xd8 / +0xdc / +0xe0). CPersistContext::Mode +0x18.
// Shape: as the landed CThingPhysicalSwitch sibling; each float default is a `0.0f` temporary built in the dead context
// argument slot (zero stored after the pushes); the EnvironmentDef address is formed by advancing `this` as its last use.
struct CCharString { void* m_pData; };
struct CPersistContext {
    unsigned char _pad00[0x18];
    long          Mode;                                  // +0x18 EMode
    void Transfer(const char* name, float* value, const float& defaultValue);   // 0x00410620
    void Transfer(const char* name, CCharString* value);                        // 0x00411010
};
struct CThingPhysical {
    unsigned char _pad00[0xd0];
    bool SerialiseGameState(CPersistContext* context);   // 0x006A5AF0
};
struct CThingSwitch : CThingPhysical {
    float         TriggerRadius;                         // +0xd0
    CCharString   EnvironmentDef;                        // +0xd4
    float         TimeToChangeEnvironmentDef;            // +0xd8
    bool          Triggered;                             // +0xdc
    unsigned char _pad_dd[3];
    CCharString   TriggerableObjectScriptName;           // +0xe0
    bool SerialiseGameState(CPersistContext* context);
};

bool CThingSwitch::SerialiseGameState(CPersistContext* context)
{
    if (CThingPhysical::SerialiseGameState(context))
    {
        if (context->Mode != 1 && context->Mode != 3)
            Triggered = false;
        context->Transfer("TriggerRadius", &TriggerRadius, 0.0f);
        context->Transfer("TimeToChangeEnvironmentDef", &TimeToChangeEnvironmentDef, 0.0f);
        context->Transfer("TriggerableObjectScriptName", &TriggerableObjectScriptName);
        context->Transfer("EnvironmentDef", &EnvironmentDef);
        return true;
    }
    return false;
}
