// NCameraModes::CPCMouseLook::CPCMouseLook(CGameCameraManager&, const CDefPointer<const CCameraModeDef>&) -- PC mouse-look camera mode: base mode, empty mouse-input list, current-state recentre timer -1
// Leaf: ??0CPCMouseLook@NCameraModes@@QAE@
// Retail 0x00874E00, 0x49 bytes (+ int3 padding), thiscall `ret 8`. Manifest label "CActionParam_Constructor" is a propagated
// auto label; the only caller is NCameraModes::CPCMouseLook::CPCMouseLookInit::Alloc 0x00874770 (landed: `new(0x10C)` then a
// sibling jmp here) and FableWin.pdb names ??0CPCMouseLook@NCameraModes@@QAE@AAVCGameCameraManager@@ABV?$CDefPointer@$$CBVCCameraModeDef@@@@@Z.
// Callees: NCameraModes::CBase::CBase(CGameCameraManager&, const CDefPointer<const CCameraModeDef>&) @ 0x0084E420 (also
// labelled "CActionParam_Constructor" by propagation), malloc @ 0x00BFEA0E (STLport list sentinel). Vtable 0x01277784
// (rtti-vtables: NCameraModes::CPCMouseLook).
// Layout (Ego_r CPCMouseLook less the 36 bytes the retail CBase saves -- 0xA4 vs 0xC8 -- with STLport's 4-byte list):
// MouseInputs list<C2DVector> +0xA4 (0x10-byte nodes), AxisX/Y/Z +0xA8/+0xB4/+0xC0, StateCurrent +0xCC (its RecentreTimer
// +0xDC), StateLast +0xEC; sizeof 0x10C (Alloc's new).
// Shape: base constructor, vtable store, the inline STLport list constructor (zero, malloc, self-link via a local), the
// C3DVector and state-packet members have empty inline constructors, then the body's `StateCurrent.RecentreTimer = -1`.
extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E

class CGameCameraManager;
class CCameraModeDef;
template <class T> class CDefPointer;

class C2DVector { public: float X, Y; };
class C3DVector { public: float X, Y, Z; C3DVector() { } };

template <class T> struct SListNode { SListNode* _M_next; SListNode* _M_prev; T _M_data; };
template <class T> class CRetailList                 // STLport list: the sentinel pointer only
{
public:
    SListNode<T>* _M_node;
    CRetailList() : _M_node(0)
    {
        SListNode<T>* node = (SListNode<T>*)malloc(sizeof(SListNode<T>));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
};

namespace NCameraModes
{
    class CBase
    {
    public:
        CBase(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def);   // 0x0084E420
        virtual void v00();
        unsigned char _body[0xa4 - 4];
    };

    class CPCMouseLook : public CBase                // vtable 0x01277784
    {
    public:
        class CRandomPacketOfVariablesDefiningTheCameraState
        {
        public:
            float     AngleXY;                       // +0x00
            float     AngleUpDown;                   // +0x04
            float     DollyLength;                   // +0x08
            float     DeltaAngleXY;                  // +0x0C
            long      RecentreTimer;                 // +0x10
            C3DVector ViewPosOffset;                 // +0x14
            CRandomPacketOfVariablesDefiningTheCameraState() { }
        };

        CRetailList<C2DVector> MouseInputs;          // +0xA4
        C3DVector AxisX;                             // +0xA8
        C3DVector AxisY;                             // +0xB4
        C3DVector AxisZ;                             // +0xC0
        CRandomPacketOfVariablesDefiningTheCameraState StateCurrent;   // +0xCC
        CRandomPacketOfVariablesDefiningTheCameraState StateLast;      // +0xEC

        CPCMouseLook(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def);
        virtual void v00();
    };
}

NCameraModes::CPCMouseLook::CPCMouseLook(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def)
    : CBase(manager, def)
{
    StateCurrent.RecentreTimer = -1;
}