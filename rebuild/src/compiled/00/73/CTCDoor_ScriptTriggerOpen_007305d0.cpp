// CTCDoor::ScriptTriggerOpen  retail 0x007305d0
// void __fastcall CTCDoor::ScriptTriggerOpen(CTCDoor* this, bool open)

struct CDoorInner
{
    void Toggle();   // direct call 0x4c7c40 (defined in another TU)
};

struct CTCDoor
{
    void*        m0;      // +0x00
    CDoorInner*  ParentThing;      // +0x04
    char         pad8[8]; // +0x08 .. +0x0f
    bool         CurrentOpen;     // +0x10  current open state
    void ScriptTriggerOpen(bool open);
};

void CTCDoor::ScriptTriggerOpen(bool open)
{
    if (this->CurrentOpen != open)
    {
        this->ParentThing->Toggle();
    }
    this->CurrentOpen = open;
}