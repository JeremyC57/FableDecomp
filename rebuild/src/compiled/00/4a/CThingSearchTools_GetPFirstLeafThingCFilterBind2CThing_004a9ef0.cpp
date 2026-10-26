// CThingSearchTools::GetPFirstLeafThing<CFilter_Bind2<CThingFilter_HasTC,CThingFilter_InRegion>,CThing> @ 0x004A9EF0
// (retail): walk the type-0xf thing list (0x0049C780) and return the first live leaf thing that passes both bound
// filters -- CThingFilter_HasTC::operator() @ 0x006A80A0, then CThingFilter_InRegion::operator() @ 0x006A7960.

struct Thing { char pad[0x91]; unsigned char flags; };
struct Node { Node* next; void* pad4; Thing* thing; };
class CThingFilter_HasTC { public: bool operator()(Thing* t) const; };       // 0x006A80A0 (the interface-bitset test)
class CThingFilter_InRegion { public: bool operator()(Thing* t) const; };    // 0x006A7960
struct Filter { CThingFilter_HasTC* f0; CThingFilter_InRegion* f1; };  // CFilter_Bind2<HasTC, InRegion>
struct List { Node* sentinel; };

extern List* __stdcall GetThingList(int kind);

bool __fastcall CThingSearchTools_GetPFirstLeafThing(void* tools, int edx, Filter* filter, Thing** out)
{
    List* lst = GetThingList(0xf);
    Node* n = lst->sentinel->next;
    if (n != lst->sentinel) {
        do {
            Thing* t = n->thing;
            unsigned char al = t->flags;
            unsigned char cl = ~al;
            if ((al & 4) && (al & 8) && (cl & 1) && !(al & 0x20)) {
                if ((*filter->f0)(t)) {
                    if ((*filter->f1)(t)) {
                        *out = t;
                        return true;
                    }
                }
            }
            n = n->next;
        } while (n != lst->sentinel);
    }
    return false;
}