// CTCDoor::ScriptTriggerOpen  retail 0x007305d0
// void __fastcall CTCDoor::ScriptTriggerOpen(CTCDoor* this, bool open)

class CThing
{ public:
public:
    void SetAsFrameUpdated();   // 0x004C7C40
};

class CTCDoor
{ public:
    void*        m0;      // +0x00
    CThing*  ParentThing;      // +0x04
    char         pad8[8]; // +0x08 .. +0x0f
    bool         CurrentOpen;     // +0x10  current open state
    void ScriptTriggerOpen(bool open);
};

void CTCDoor::ScriptTriggerOpen(bool open)
{
    if (this->CurrentOpen != open)
    {
        this->ParentThing->SetAsFrameUpdated();
    }
    this->CurrentOpen = open;
}