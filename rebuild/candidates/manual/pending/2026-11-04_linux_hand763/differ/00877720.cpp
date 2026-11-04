// CTools::GetStandardThings(list<const CThing*>* out, const C3DVector& pos, float radius) -- refill the list with the things of classes 3 and 5 inside the sphere, searched over the sphere's integer bounding square
// Grade: DIFFER(280v277) -- dl=24: the box corners are computed x-r, x+r, y-r, y+r in place (retail) where ours keeps the centre and orders the stores differently; the Bind2 filter temporaries pack 3 bytes larger
// Leaf: ?GetStandardThings@CTools@NCameraModes@@QBEXAAV?$list@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@ABVC3DVector@@M@Z
extern "C" void __cdecl free(void* block);        // 0x00BFEA14
class C3DVector { public: float X, Y, Z; };
class C2DCoordI { public: long X, Y; C2DCoordI(long x, long y) : X(x), Y(y) { } };
class C2DBoxI { public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};
class CThing;
namespace std {
template <class _Tp> class allocator { };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    _List_node_base* _M_node;
    void clear()
    {
        _List_node<_Tp>* __cur = (_List_node<_Tp>*)_M_node->_M_next;
        while (__cur != (_List_node<_Tp>*)_M_node) {
            _List_node<_Tp>* __tmp = __cur;
            __cur = (_List_node<_Tp>*)__cur->_M_next;
            free(__tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};
}
class CThingFilter_IsOfType { public:
    unsigned long Type;
    CThingFilter_IsOfType(unsigned long type) : Type(type) { }
};
class CThingFilter_OverlappingSphere { public:
    C3DVector SpherePos;
    float     Radius;
    CThingFilter_OverlappingSphere(const C3DVector& pos, float radius) : SpherePos(pos), Radius(radius) { }
};
template <class A, class B> struct CFilter_Bind2 {
    const A* F1;
    const B* F2;
    CFilter_Bind2(const A& a, const B& b) : F1(&a), F2(&b) { }
};
template <class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
class CThingSearchTools { public:
    template <class TThing, class TList, class TFilter> void GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter, TList& out) const;   // 0x00878430
};
class CWorld { public:
    unsigned char _pad[0x20];
    CThingSearchTools* PThingSearchTools;             // +0x20
};
class CGameCameraManager { public:
    unsigned char _pad[0x74];
    CWorld* PWorld;                                   // +0x74
};
namespace NCameraModes {
class CTools { public:
    CGameCameraManager* CameraManager;                // +0x00
    void GetStandardThings(std::list<const CThing*>& things, const C3DVector& pos, float radius) const;
};

void CTools::GetStandardThings(std::list<const CThing*>& things, const C3DVector& pos, float radius) const
{
    things.clear();
    C2DCoordI centre((long)pos.X, (long)pos.Y);
    long r = (long)radius;
    C2DBoxI area(centre.X - r, centre.Y - r, centre.X + r, centre.Y + r);
    CameraManager->PWorld->PThingSearchTools->GetAllThingsInRoughArea<const CThing>(area, CThingFilter_IsOfType(3) && CThingFilter_OverlappingSphere(pos, radius), things);
    CameraManager->PWorld->PThingSearchTools->GetAllThingsInRoughArea<const CThing>(area, CThingFilter_IsOfType(5) && CThingFilter_OverlappingSphere(pos, radius), things);
}
}
