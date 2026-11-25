// CPersistContext::Transfer<CDefString>(CDefString&) -- one-argument transfer helper (member template; see the landed 0x00431061 Transfer<float>):
//   CheckCRC("") (0x00404500, the empty key 0x0122D70E), then the binary load (mode 2) / save (mode 3) switch.
//   namepass: one name for 0x0045228F -- its callers call CPersistContext::Transfer<T>; the catalog label
//   "CEngineLightingManager::UpdateShadowScene" / Transfer*_xxx was propagated or descriptive.
#pragma optimize("s",on)
// Leaf: ??$Transfer@VCDefString@@@CPersistContext@@QAEXAAVCDefString@@@Z
// Literal (NF415, 2026-11-25): was the bare integer 0x122d70e (the retail VA of the pooled ""); now the literal is spelled, cast to the
//   stand-in's existing int parameter so no callee or table line changes (the callee port is T3). Retail 0x0122D70E: 00 (retail's one pooled "", NF388 / NF413 b2 / NF414 b1).
class CDefString;
class CPersistContext { public:
    char pad[0x18];
    int state;      /* +0x18 */
    char pad2[0x24-0x1c];
    int field24;    /* +0x24 */
    void* field28;  /* +0x28 */
    template <class T> void Transfer(T& value);
};

class CDataOutputStream { public: public:
    void WriteSLONG(long);   // 0x00993EB0
};
struct CShadowSceneB { void Put(int v); };

extern void __stdcall PrepShadow(int tag);

template <> void CPersistContext::Transfer<CDefString>(CDefString& value_)
{
    CPersistContext* self = this;
    int* p = (int*)&value_;
    PrepShadow((int)"");
    switch (self->state) {
    case 2:
        ((CShadowSceneB*)p)->Put(self->field24);
        break;
    case 3:
        ((CDataOutputStream*)self->field28)->WriteSLONG(*p);
        break;
    }
}