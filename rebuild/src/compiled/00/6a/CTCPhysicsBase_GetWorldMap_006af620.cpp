class CDisplayEngine;

struct CDisplayEngineVtbl
{
    void* slot00; void* slot04; void* slot08; void* slot0C;
    void* slot10; void* slot14; void* slot18; void* slot1C;
    void* slot20; void* slot24; void* slot28; void* slot2C;
    bool (__fastcall *DrawIsDrawable)(CDisplayEngine*);
};

class CDisplayEngine
{ public:
    CDisplayEngineVtbl* vtbl;
};

struct CViewBase {};
CDisplayEngine* __fastcall GetDisplayEngine(CViewBase* self);

class CThingTrackNode : public CViewBase
{ public:
    bool __fastcall DrawIsDrawable();
};

bool __fastcall CThingTrackNode::DrawIsDrawable()
{
    CDisplayEngine* eng = GetDisplayEngine(this);
    return eng->vtbl->DrawIsDrawable(eng);
}