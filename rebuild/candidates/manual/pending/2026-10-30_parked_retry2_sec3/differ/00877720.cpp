// Grade: DIFFER(280v277) -- 39 difflines (was 250v277, 108): the sphere filter is block-local to each search (retail rebuilds it per class) and the class word lands in the dead out-argument slot as retail; C2DBoxI with its Ego_r member names and a four-argument ctor (same bytes). Left: the box corners (retail grows x/y in place in ebx/ebp: add ebx,eax) and the this reload into ebx scheduled earlier; in-place locals and the ctor did not move it
// CTools::GetStandardThings(list<const CThing*>* out, const C3DVector& pos, float radius) -- refill the list with the things of classes 3 and 5 inside the sphere, searched over the sphere's integer bounding square
// Leaf: GetStandardThings
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00877720, length 278 bytes (+ int3 padding), `ret 0Ch`.
// Callees: free @ 0x00BFEA14 (the old list nodes), __ftol2 @ 0x00BFEA70 (x3), the thing-search template @ 0x00878430
// (catalog "GetPBestThingInRoughArea<...>" is a propagated twin; here (const C2DBoxI&, filter&, list*)) on the world's
// search tools ([this]+0x74 -> +0x20), twice.
// Shape: STLport list clear() as the node walk + sentinel relink; the square from the truncated centre and radius; two
// combined sphere + thing-class filters (class 3, then 5) whose class word is homed in a dead argument slot.
extern "C" void __cdecl free(void* block);                     // 0x00BFEA14
struct C3DVector { float X, Y, Z; };
class C2DBoxI { public: long TLX, TLY, BRX, BRY; C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { } };   // Ego_r.pdb
class CThing;
struct SThingListNode { SThingListNode* _M_next; SThingListNode* _M_prev; const CThing* _M_data; };
struct CThingList {                                            // STLport list<const CThing*>: the sentinel pointer
    SThingListNode* _M_node;
    void clear()
    {
        SThingListNode* cur = _M_node->_M_next;
        while (cur != _M_node) {
            SThingListNode* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};
struct CThingFilter_InSphere {
    C3DVector Centre;
    float     Radius;
};
struct CThingFilter_SphereAndClass {
    const CThingFilter_InSphere* Sphere;
    const long*                  ThingClass;
};
class CThingSearchTools {
public:
    void GetThingsInRoughArea(const C2DBoxI& area, CThingFilter_SphereAndClass& filter, CThingList* out);   // 0x00878430
};
struct CWorldSearch { unsigned char _pad[0x20]; CThingSearchTools* SearchTools; };
struct CToolsOwner { unsigned char _pad[0x74]; CWorldSearch* World; };
class CTools {
public:
    CToolsOwner* Owner;                                        // +0x00
    void GetStandardThings(CThingList* out, const C3DVector& pos, float radius);
};

void CTools::GetStandardThings(CThingList* out, const C3DVector& pos, float radius)
{
    out->clear();
    long x = (long)pos.X;
    long y = (long)pos.Y;
    long r = (long)radius;
    C2DBoxI area(x - r, y - r, x + r, y + r);
    {
        CThingFilter_InSphere sphere;
        sphere.Centre = pos;
        sphere.Radius = radius;
        long thingClass = 3;
        CThingFilter_SphereAndClass filter;
        filter.Sphere = &sphere;
        filter.ThingClass = &thingClass;
        Owner->World->SearchTools->GetThingsInRoughArea(area, filter, out);
    }
    {
        CThingFilter_InSphere sphere;
        sphere.Centre = pos;
        sphere.Radius = radius;
        long thingClass = 5;
        CThingFilter_SphereAndClass filter;
        filter.Sphere = &sphere;
        filter.ThingClass = &thingClass;
        Owner->World->SearchTools->GetThingsInRoughArea(area, filter, out);
    }
}
