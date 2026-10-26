// CThingPhysicalSwitch::SerialiseGameState(CPersistContext*) -- base serialise, clears Triggered unless the context mode is 1 or 3, transfers TriggerableObjectScriptName
// Retail: Fable.exe 0x0083D940, 71 bytes.
// Callees: CThingPhysical::SerialiseGameState 0x006A5AF0 (direct base call), CPersistContext::Transfer
// <CCharString>(name, CCharString*) 0x00411010 (landed), literal "TriggerableObjectScriptName" 0x01273078.
// Ego_r.pdb CThingPhysicalSwitch: Triggered +0xe8, TriggerableObjectScriptName +0xec; retail sits 0x18
// lower (+0xd0 / +0xd4, the CThing/CThingPhysical shrink, see 0x006A5260). CPersistContext::Mode +0x18.
// Shape: the body under `if (base call)` with `return false` after it (an early `return false` reuses
// the call's al and drops retail's trailing `xor al,al`), two-value mode test as an && of inequalities, the
// member address formed by advancing `this` (`add edi,0D4h`) as its last use, bool true/false returns.
class CCharString { public: void* m_pData; };

class CPersistContext { public:
    unsigned char _pad00[0x18];
    long          Mode;                                  // +0x18 EMode
    void Transfer(const char* name, CCharString* value); // 0x00411010
};

class CThingPhysical { public:
    unsigned char _pad00[0xd0];
    bool SerialiseGameState(CPersistContext* context);   // 0x006A5AF0
};

class CThingPhysicalSwitch : public CThingPhysical { public:
    bool          Triggered;                             // +0xd0
    unsigned char _pad_d1[3];
    CCharString   TriggerableObjectScriptName;           // +0xd4
    bool SerialiseGameState(CPersistContext* context);
};

bool CThingPhysicalSwitch::SerialiseGameState(CPersistContext* context)
{
    if (CThingPhysical::SerialiseGameState(context))
    {
        if (context->Mode != 1 && context->Mode != 3)
            Triggered = false;
        context->Transfer("TriggerableObjectScriptName", &TriggerableObjectScriptName);
        return true;
    }
    return false;
}