// CThingManager::AddToRenderList(CThing* thing) -- appends the thing to the intrusive render list unless it is already linked (has a next/prev link or is the head)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051DF80, length 81 bytes.
// No callees. Layout: CThing's render-list links sit at +0x7c (prev) and +0x80 (next); the
// manager's RenderList (ThingLList {head, tail}) at +0x84/+0x88 in retail (PDB +0x9c: the retail
// manager is 0x18 smaller ahead of it -- 0xc base + four 4-byte container words).
// Shape: three early-out tests sharing the `ret 4`; the tail is re-read after the store through
// the old tail (it may alias), next cleared, tail set last.
struct CThing {
    unsigned char _pad00[0x7c];
    CThing*       PRenderPrev;                 // +0x7c
    CThing*       PRenderNext;                 // +0x80
};
struct ThingLList {
    CThing* PHead;
    CThing* PTail;
};
struct CThingManager {
    unsigned char _pad00[0x84];
    ThingLList    RenderList;                  // +0x84
    void AddToRenderList(CThing* thing);
};

void CThingManager::AddToRenderList(CThing* thing)
{
    if (thing->PRenderNext == 0 && thing->PRenderPrev == 0 && RenderList.PHead != thing) {
        if (RenderList.PTail != 0)
            RenderList.PTail->PRenderNext = thing;
        else
            RenderList.PHead = thing;
        thing->PRenderPrev = RenderList.PTail;
        thing->PRenderNext = 0;
        RenderList.PTail = thing;
    }
}