#pragma optimize("s",on)
struct CInner {
    virtual void v0();
};

class CScriptThing { public:
    void* pad0;
    CInner* inner;
};

typedef void (__fastcall *SlotFn)(CInner*, int, int);

void __fastcall CScriptThing_SetAsUsable(CScriptThing* scriptThing, int edx, int isUsable)
{
    CInner* p = scriptThing->inner;
    if (p) {
        void** vt = *(void***)p;
        ((SlotFn)vt[66])(p, edx, isUsable);
    }
}