struct ThingRef { unsigned int a; unsigned int b; };
// Leaf: ?IsOriginatedByThing@@YIPAUThingRef@@PAUCMessageEventFilter@@HPAU1@@Z
struct CMessageEventFilter { char pad[0x34]; unsigned int uid; unsigned int idx; };
ThingRef * __fastcall IsOriginatedByThing(CMessageEventFilter *self, int edxfill, ThingRef *out)
{
    (void)edxfill;
    out->a = self->uid;
    out->b = self->idx;
    return out;
}