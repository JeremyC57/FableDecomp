// Leaf: ?CThing_SetAsFrameUpdated@@YIXPAVCThing@@@Z
class CThing;
struct GlobalMgr {
    void Notify(CThing* t);
};
extern GlobalMgr* g_mgr;
class CThing { public:
    unsigned char pad[0x91];
    unsigned char flags;
};
void __fastcall CThing_SetAsFrameUpdated(CThing* thing) {
    if (!(thing->flags & 1))
        g_mgr->Notify(thing);
}