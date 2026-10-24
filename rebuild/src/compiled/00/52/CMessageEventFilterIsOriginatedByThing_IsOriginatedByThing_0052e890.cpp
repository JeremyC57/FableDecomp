struct ThingRef { unsigned int a; unsigned int b; };
// Leaf: ??0CMessageEventFilter_IsOriginatedByThing@
struct CMessageEventFilter { char pad[0x34]; unsigned int uid; unsigned int idx; };
ThingRef * __fastcall IsOriginatedByThing(CMessageEventFilter *self, int edxfill, ThingRef *out)
{
    (void)edxfill;
    out->a = self->uid;
    out->b = self->idx;
    return out;
}