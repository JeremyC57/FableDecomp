// CMouseDX::ConvertMouseEventToInputEvent(const CMouseEvent*, CInputEvent*) -- translate one DirectInput mouse event into an input event at the current cursor position: button presses, releases (start time taken from the matching maintained held event), extra buttons and the wheel; returns whether the event was converted
// Leaf: ?ConvertMouseEventToInputEvent@CMouseDX@@IAE_NPBVCMouseEvent@@PAVCInputEvent@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB5420, 740 bytes including its three jump tables (0x00AB5678 / 0x00AB56A4 / 0x00AB56C4 and
//   the byte index tables 0x00AB568C / 0x00AB56AC / 0x00AB56EC; the row ends at 0x00AB5703), thiscall `ret 8`, aligned ebp frame.
// Identity: Ego_r.pdb ?ConvertMouseEventToInputEvent@CMouseDX@@IAE_NPBVCMouseEvent@@PAVCInputEvent@@@Z (protected; donor order next to
//   GetDeviceMouseEvent / Update); its one caller 0x00AB59B7 is in CMouseDX (vtable 0x0129EA14: CMouseDX : CMouse : CBaseClassNonCopyable).
//   The callees' order and argument shapes match the Ego_r CInputEvent setters (Left/Middle/Right/Extra Pressed (pos, time),
//   Released (pos, start, time), wheel (float, double)).
// Callees: CInputEvent::GetType 0x00A03B40, SetAsLeftMouseButtonPressed 0x00A03C80 / Released 0x00A03D60, Middle 0x00A03D90 / 0x00A03E10,
//   Right 0x00A03E40 / 0x00A03EC0, Extra 0x00A03EF0 / 0x00A03F70, SetAsMouseWheelMovement 0x00A04000.
// Layout: engine/CMouse.h (rename pattern; MaintainedEventsList +0x340C viewed as the one-word STLport list<CMaintainedEvent>, node data
//   = CMaintainedEvent {vptr, CInputEvent Event}), CMouseDX::Pos +0x3414 (Ego_r +0x341C less the Dinkumware list's extra 8 bytes).
// Shape: three switches on the mouse event type; case bodies in retail's source order (Left, Middle, Right, Extra; the jump tables
//   were checked entry by entry: release type 4/6/5 -> held input type 5/8/0xB, 20..24 -> type+7).
#include "engine/CBaseClass.h"
class C2DVector { public: float X, Y; };
enum EInputEventType { };
enum EMouseEventType { };
class CInputEvent {
public:
    unsigned char Data[0x20];                                   // +0x00 UInputEventData
    long          DeviceType;                                   // +0x20
    long          DeviceNumber;                                 // +0x24
    long          Type;                                         // +0x28
    float         CurrentTime;                                  // +0x2c
    float         StartTime;                                    // +0x30
    EInputEventType GetType() const;                                                              // 0x00A03B40
    void SetAsLeftMouseButtonPressed(const C2DVector& pos, double time);                         // 0x00A03C80
    void SetAsLeftMouseButtonReleased(const C2DVector& pos, double startTime, double time);      // 0x00A03D60
    void SetAsMiddleMouseButtonPressed(const C2DVector& pos, double time);                       // 0x00A03D90
    void SetAsMiddleMouseButtonReleased(const C2DVector& pos, double startTime, double time);    // 0x00A03E10
    void SetAsRightMouseButtonPressed(const C2DVector& pos, double time);                        // 0x00A03E40
    void SetAsRightMouseButtonReleased(const C2DVector& pos, double startTime, double time);     // 0x00A03EC0
    void SetAsExtraMouseButtonPressed(const C2DVector& pos, double time, long button);           // 0x00A03EF0
    void SetAsExtraMouseButtonReleased(const C2DVector& pos, double startTime, double time, long button);   // 0x00A03F70
    void SetAsMouseWheelMovement(float movement, double time);                                   // 0x00A04000
};
class CMaintainedEvent : public CBaseClass {
public:
    CInputEvent Event;                                          // +0x04
};
class CMouseEvent {
public:
    double          Time;                                       // +0x00
    EMouseEventType Type;                                       // +0x08
    float           Movement;                                   // +0x0c
    float           PosX;                                       // +0x10
    float           PosY;                                       // +0x14
};
namespace std {
template <class _Tp> class allocator { public: };
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) {}
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _Tp* operator->() const { return &(operator*()); }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_iterator<_Tp> iterator;
    _List_node<_Tp>* _M_node;
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
};
}
#define CMouse CMouseLayout
#include "engine/CMouse.h"
#undef CMouse
// Layout: engine/CMouse.h flattens the vptr and keeps MaintainedEventsList as opaque bytes (+0x340c, the one-word STLport list);
//   CMouseDX adds Pos at +0x3414 (Ego_r.pdb 0x341c less the Dinkumware list's 8 bytes).
class CMouseDX : public CMouseLayout {
public:
    C2DVector Pos;                                              // +0x3414
protected:
    bool ConvertMouseEventToInputEvent(const CMouseEvent* event, CInputEvent* inputEvent);
    std::list<CMaintainedEvent>& MaintainedEvents() { return *reinterpret_cast<std::list<CMaintainedEvent>*>(MaintainedEventsList); }
};
bool CMouseDX::ConvertMouseEventToInputEvent(const CMouseEvent* event, CInputEvent* inputEvent)
{
    float startTime = (float)event->Time;
    long heldType = 0;
    bool converted = false;
    C2DVector pos = Pos;
    switch (event->Type) {
    case 4: heldType = 5; break;
    case 6: heldType = 8; break;
    case 5: heldType = 11; break;
    case 20: case 21: case 22: case 23: case 24: heldType = event->Type + 7; break;
    }
    switch (event->Type) {
    case 4: case 5: case 6: case 20: case 21: case 22: case 23: case 24:
        for (std::list<CMaintainedEvent>::iterator it = MaintainedEvents().begin(); it != MaintainedEvents().end(); ++it) {
            if (it->Event.GetType() == heldType) {
                startTime = it->Event.StartTime;
                break;
            }
        }
        break;
    }
    switch (event->Type) {
    case 1: inputEvent->SetAsLeftMouseButtonPressed(pos, event->Time); converted = true; break;
    case 3: inputEvent->SetAsMiddleMouseButtonPressed(pos, event->Time); converted = true; break;
    case 2: inputEvent->SetAsRightMouseButtonPressed(pos, event->Time); converted = true; break;
    case 15: case 16: case 17: case 18: case 19:
        inputEvent->SetAsExtraMouseButtonPressed(pos, event->Time, event->Type - 15); converted = true; break;
    case 4: inputEvent->SetAsLeftMouseButtonReleased(pos, startTime, event->Time); converted = true; break;
    case 6: inputEvent->SetAsMiddleMouseButtonReleased(pos, startTime, event->Time); converted = true; break;
    case 5: inputEvent->SetAsRightMouseButtonReleased(pos, startTime, event->Time); converted = true; break;
    case 20: case 21: case 22: case 23: case 24:
        inputEvent->SetAsExtraMouseButtonReleased(pos, startTime, event->Time, event->Type - 20); converted = true; break;
    case 10: inputEvent->SetAsMouseWheelMovement(event->Movement, event->Time); converted = true; break;
    }
    return converted;
}