// CTCEntityEvents::FrameUpdate @ 0x00811450

struct CBaseEntityEvents {
    void baseUpdate(); // __fastcall, 0x00686a70
};

struct CSubObject {
    void consume(int x); // __fastcall, 0x00812030
};

class CWorld { public: static long __fastcall GetFrame(); }; // cdecl global, 0x0049d870

class CTCEntityEvents { public:
    // base subobject at offset 0
    CBaseEntityEvents base;   // +0x00
    char pad[0x0c - sizeof(CBaseEntityEvents)]; // pad to +0x0c
    CSubObject sub;           // +0x0c
};

void __fastcall CTCEntityEvents_FrameUpdate(CTCEntityEvents* events)
{
    events->base.baseUpdate();
    int x = CWorld::GetFrame();
    events->sub.consume(x);
}