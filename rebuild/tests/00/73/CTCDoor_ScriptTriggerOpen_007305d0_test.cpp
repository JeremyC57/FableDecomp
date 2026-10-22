#include <stdio.h>

static int g_toggleCount = 0;

struct CDoorInner
{
    void Toggle();
};
void CDoorInner::Toggle()
{
    g_toggleCount++;
}

struct CTCDoor
{
    void*        m0;
    CDoorInner*  ParentThing;
    char         pad8[8];
    bool         CurrentOpen;
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

int main()
{
    CDoorInner inner;
    CTCDoor d;
    d.m0 = 0;
    d.ParentThing = &inner;
    d.CurrentOpen = false;

    // state changes false->true: should toggle once
    d.ScriptTriggerOpen(true);
    // no change true->true: no toggle
    d.ScriptTriggerOpen(true);
    // change true->false: toggle again
    d.ScriptTriggerOpen(false);
    // no change false->false
    d.ScriptTriggerOpen(false);

    if (g_toggleCount == 2 && d.CurrentOpen == false)
    {
        printf("SCRIPTTRIGGEROPEN_OK\n");
        return 0;
    }
    printf("FAIL count=%d state=%d\n", g_toggleCount, (int)d.CurrentOpen);
    return 1;
}