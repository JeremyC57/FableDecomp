// Behaviour test for retail 0x005467f0: candidate source embedded verbatim, then stubs and a main.
#include <stdio.h>
#include <math.h>
#ifndef _MSC_VER
#define __fastcall
#endif
// NUISystem::CZoomingComponent::ProcessEvent(EEvent event) -- once the input delay has passed, zoom in or out by the scroll step (clamped to MinZoom..MaxZoom), keep the zoom centre fixed by moving the component, and record the input time; any other event ends zooming
// Leaf: ProcessEvent
// Retail: Fable.exe (TLC, VC7.1) @ 0x005467F0, 486 bytes (+ int3 padding), `ret 4`. Slot 1 of vtable 0x01247DA0, the
//   CZoomingComponent vtable of the event-observer base at +4 (rtti-vtables.tsv), so `this` arrives adjusted (esi =
//   object + 4; the primary vtable is reached at [esi-4]).
// Callees: NUISystem::CFrontEndManager::GetInstance 0x0041E5F2 (`__fastcall`, no arguments) and the manager's virtual
//   slot 39 (+0x9C, a float returned by reference: the per-input scroll step); the component's primary virtual slots 31
//   ChangePosition(const C2DVector&, float, bool) (+0x7C, 0x0052E9C0) and 35 ChangeZoom (+0x8C); constants 0.4 (double
//   @ 0x01248028) and 0.2f (immediate).
// Layout: retail CZoomingComponent = Ego_r.pdb minus 0x30 (LastInputTime +0x15C, ZoomCenter +0x160, MinZoom +0x168,
//   MaxZoom +0x170, Panning +0x178, Zooming +0x179, InputDelay +0x17C, ScrollingSpeed +0x180), bases as in the landed
//   constructor 0x00546B00 / ChangeState 0x005469E0; CComponent Position +0x3C and Zoom +0x64 (as Ego_r.pdb), the UI time
//   read at +0x30.
// Shape: the cases are written 13 then 12 (VC7.1 lays case bodies out in source order; the default arm sits inline);
//   `Panning == true` keeps the `mov al,1 ; cmp cl,al` constant that the Zooming store reuses; each zoom axis is
//   `Zoom + step * ScrollingSpeed (* 0.4)` with the step re-fetched per axis; the clamps are four plain ifs.
//   The whole tail (clamps, ChangeZoom, position, ChangePosition, LastInputTime) is written out in BOTH case arms, each
//   ending in `return;`: VC7.1 tail-merges the copies and so places `push edi` at the switch head as retail does (one
//   shared tail after the switch sinks the push to the last block). C2DVector has a user memberwise operator= (oldZoom
//   loads after the Zooming store) and position is built per member in its own {} block (found by the Fable advisor).
namespace NUISystem {

enum EEvent { EVENT_ZOOM_OUT_SLOW = 12, EVENT_ZOOM_IN = 13 };

struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector& operator=(const C2DVector& o) { X = o.X; Y = o.Y; return *this; }
};

struct CFrontEndManager_Calls {                      // vtable-slot view of NUISystem::CFrontEndManager
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038();
    virtual const float& GetScrollStep();            // slot 39 (+0x9C)
    static CFrontEndManager_Calls* __fastcall GetInstance();   // 0x0041E5F2
};

struct CComponentInterface {                         // primary vtable at +0
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030();
    virtual void ChangePosition(const C2DVector& position, float time, bool immediate);   // slot 31 (+0x7C)
    virtual void v032(); virtual void v033(); virtual void v034();
    virtual void ChangeZoom(const C2DVector& zoom, float time, bool immediate);           // slot 35 (+0x8C)
};

struct CObserver_Retail {                            // vtable at +4, then its 0x10 bytes of state
    virtual void v0();
    virtual void ProcessEvent(EEvent event);         // slot 1
    unsigned char _body[0x10];
};

struct CEventSinkInterface {                         // vtable at +0x18
    virtual void w0();
};

struct CChangingStateComponent : CComponentInterface, CObserver_Retail, CEventSinkInterface {   // retail 0x15C bytes
    unsigned char _pad1c[0x14];                      // +0x1c
    float Time;                                      // +0x30
    unsigned char _pad34[0x8];                       // +0x34
    C2DVector Position;                              // +0x3c
    unsigned char _pad44[0x20];                      // +0x44
    C2DVector Zoom;                                  // +0x64
    unsigned char _pad6c[0x15c - 0x6c];              // +0x6c
};

struct CZoomingComponent : CChangingStateComponent {   // vtables 0x01247DC4 / 0x01247DA0 / 0x01247D98
    float     LastInputTime;                         // +0x15C
    C2DVector ZoomCenter;                            // +0x160
    C2DVector MinZoom;                               // +0x168
    C2DVector MaxZoom;                               // +0x170
    bool      Panning;                               // +0x178
    bool      Zooming;                               // +0x179
    float     InputDelay;                            // +0x17C
    float     ScrollingSpeed;                        // +0x180
    virtual void ProcessEvent(EEvent event);
};


void CZoomingComponent::ProcessEvent(EEvent event)
{
    if (Time - LastInputTime < InputDelay)
        return;
    C2DVector zoom;
    C2DVector oldZoom;
    switch (event) {
    case EVENT_ZOOM_IN:
        if (Panning == true)
            return;
        Zooming = true;
        oldZoom = Zoom;
        zoom.X = Zoom.X + CFrontEndManager_Calls::GetInstance()->GetScrollStep() * ScrollingSpeed;
        zoom.Y = Zoom.Y + CFrontEndManager_Calls::GetInstance()->GetScrollStep() * ScrollingSpeed;
        if (zoom.X < MinZoom.X)
            zoom.X = MinZoom.X;
        if (zoom.X > MaxZoom.X)
            zoom.X = MaxZoom.X;
        if (zoom.Y < MinZoom.Y)
            zoom.Y = MinZoom.Y;
        if (zoom.Y > MaxZoom.Y)
            zoom.Y = MaxZoom.Y;
        ChangeZoom(zoom, 0.2f, false);
        {
        C2DVector position;
        position.X = ZoomCenter.X - (ZoomCenter.X - Position.X) * (Zoom.X / oldZoom.X);
        position.Y = ZoomCenter.Y - (ZoomCenter.Y - Position.Y) * (Zoom.Y / oldZoom.Y);
        ChangePosition(position, 0.2f, false);
        }
        LastInputTime = Time;
        return;
    case EVENT_ZOOM_OUT_SLOW:
        if (Panning == true)
            return;
        Zooming = true;
        oldZoom = Zoom;
        zoom.X = (float)(Zoom.X + CFrontEndManager_Calls::GetInstance()->GetScrollStep() * ScrollingSpeed * 0.4);
        zoom.Y = (float)(Zoom.Y + CFrontEndManager_Calls::GetInstance()->GetScrollStep() * ScrollingSpeed * 0.4);
        if (zoom.X < MinZoom.X)
            zoom.X = MinZoom.X;
        if (zoom.X > MaxZoom.X)
            zoom.X = MaxZoom.X;
        if (zoom.Y < MinZoom.Y)
            zoom.Y = MinZoom.Y;
        if (zoom.Y > MaxZoom.Y)
            zoom.Y = MaxZoom.Y;
        ChangeZoom(zoom, 0.2f, false);
        {
        C2DVector position;
        position.X = ZoomCenter.X - (ZoomCenter.X - Position.X) * (Zoom.X / oldZoom.X);
        position.Y = ZoomCenter.Y - (ZoomCenter.Y - Position.Y) * (Zoom.Y / oldZoom.Y);
        ChangePosition(position, 0.2f, false);
        }
        LastInputTime = Time;
        return;
    default:
        Zooming = false;
        return;
    }
}

}

namespace NUISystem {
void CFrontEndManager_Calls::v000() { }
void CFrontEndManager_Calls::v001() { }
void CFrontEndManager_Calls::v002() { }
void CFrontEndManager_Calls::v003() { }
void CFrontEndManager_Calls::v004() { }
void CFrontEndManager_Calls::v005() { }
void CFrontEndManager_Calls::v006() { }
void CFrontEndManager_Calls::v007() { }
void CFrontEndManager_Calls::v008() { }
void CFrontEndManager_Calls::v009() { }
void CFrontEndManager_Calls::v010() { }
void CFrontEndManager_Calls::v011() { }
void CFrontEndManager_Calls::v012() { }
void CFrontEndManager_Calls::v013() { }
void CFrontEndManager_Calls::v014() { }
void CFrontEndManager_Calls::v015() { }
void CFrontEndManager_Calls::v016() { }
void CFrontEndManager_Calls::v017() { }
void CFrontEndManager_Calls::v018() { }
void CFrontEndManager_Calls::v019() { }
void CFrontEndManager_Calls::v020() { }
void CFrontEndManager_Calls::v021() { }
void CFrontEndManager_Calls::v022() { }
void CFrontEndManager_Calls::v023() { }
void CFrontEndManager_Calls::v024() { }
void CFrontEndManager_Calls::v025() { }
void CFrontEndManager_Calls::v026() { }
void CFrontEndManager_Calls::v027() { }
void CFrontEndManager_Calls::v028() { }
void CFrontEndManager_Calls::v029() { }
void CFrontEndManager_Calls::v030() { }
void CFrontEndManager_Calls::v031() { }
void CFrontEndManager_Calls::v032() { }
void CFrontEndManager_Calls::v033() { }
void CFrontEndManager_Calls::v034() { }
void CFrontEndManager_Calls::v035() { }
void CFrontEndManager_Calls::v036() { }
void CFrontEndManager_Calls::v037() { }
void CFrontEndManager_Calls::v038() { }
void CComponentInterface::v000() { }
void CComponentInterface::v001() { }
void CComponentInterface::v002() { }
void CComponentInterface::v003() { }
void CComponentInterface::v004() { }
void CComponentInterface::v005() { }
void CComponentInterface::v006() { }
void CComponentInterface::v007() { }
void CComponentInterface::v008() { }
void CComponentInterface::v009() { }
void CComponentInterface::v010() { }
void CComponentInterface::v011() { }
void CComponentInterface::v012() { }
void CComponentInterface::v013() { }
void CComponentInterface::v014() { }
void CComponentInterface::v015() { }
void CComponentInterface::v016() { }
void CComponentInterface::v017() { }
void CComponentInterface::v018() { }
void CComponentInterface::v019() { }
void CComponentInterface::v020() { }
void CComponentInterface::v021() { }
void CComponentInterface::v022() { }
void CComponentInterface::v023() { }
void CComponentInterface::v024() { }
void CComponentInterface::v025() { }
void CComponentInterface::v026() { }
void CComponentInterface::v027() { }
void CComponentInterface::v028() { }
void CComponentInterface::v029() { }
void CComponentInterface::v030() { }
void CComponentInterface::v032() { }
void CComponentInterface::v033() { }
void CComponentInterface::v034() { }
void CObserver_Retail::v0() { }
void CEventSinkInterface::w0() { }
static float g_step = 0.5f;
static int g_zoomCalls = 0, g_posCalls = 0;
static C2DVector g_lastZoom, g_lastPos;
static float g_lastZoomTime = 0, g_lastPosTime = 0;
static bool g_lastZoomImm = true, g_lastPosImm = true;
const float& CFrontEndManager_Calls::GetScrollStep() { return g_step; }
static CFrontEndManager_Calls g_manager;
CFrontEndManager_Calls* __fastcall CFrontEndManager_Calls::GetInstance() { return &g_manager; }
void CComponentInterface::ChangePosition(const C2DVector& position, float time, bool immediate)
{
    ++g_posCalls; g_lastPos = position; g_lastPosTime = time; g_lastPosImm = immediate;
}
void CComponentInterface::ChangeZoom(const C2DVector& zoom, float time, bool immediate)
{
    ++g_zoomCalls; g_lastZoom = zoom; g_lastZoomTime = time; g_lastZoomImm = immediate;
    static_cast<CChangingStateComponent*>(this)->Zoom = zoom;   // the component takes the new zoom at once
}
void CObserver_Retail::ProcessEvent(EEvent) { }
}

using namespace NUISystem;

static bool Near(float a, float b) { return fabs(a - b) < 1e-4f; }

int main()
{
    CZoomingComponent c;
    c.Time = 10.0f; c.LastInputTime = 9.9f; c.InputDelay = 0.5f;
    c.Zoom = C2DVector(2.0f, 4.0f); c.Position = C2DVector(4.0f, 4.0f);
    c.ZoomCenter = C2DVector(10.0f, 20.0f); c.MinZoom = C2DVector(0.0f, 0.0f); c.MaxZoom = C2DVector(10.0f, 4.5f);
    c.Panning = false; c.Zooming = false; c.ScrollingSpeed = 2.0f;
    bool ok = true;

    c.ProcessEvent(EVENT_ZOOM_IN);                     // inside the input delay: nothing happens
    if (g_zoomCalls != 0 || g_posCalls != 0 || c.Zooming || !Near(c.LastInputTime, 9.9f)) ok = false;

    c.LastInputTime = 0.0f;
    c.ProcessEvent(EVENT_ZOOM_IN);                     // (2,4) + 1 -> (3,5) clamped to (3,4.5)
    if (g_zoomCalls != 1 || !Near(g_lastZoom.X, 3.0f) || !Near(g_lastZoom.Y, 4.5f) || !Near(g_lastZoomTime, 0.2f) || g_lastZoomImm) ok = false;
    if (g_posCalls != 1 || !Near(g_lastPos.X, 1.0f) || !Near(g_lastPos.Y, 2.0f) || !Near(g_lastPosTime, 0.2f) || g_lastPosImm) ok = false;
    if (!c.Zooming || !Near(c.LastInputTime, 10.0f)) ok = false;

    c.Time = 20.0f; c.Panning = true;
    c.ProcessEvent(EVENT_ZOOM_OUT_SLOW);              // panning blocks zooming
    if (g_zoomCalls != 1 || !Near(c.LastInputTime, 10.0f)) ok = false;

    c.ProcessEvent((EEvent)5);                         // any other event ends zooming
    if (c.Zooming || g_zoomCalls != 1) ok = false;

    c.Panning = false; c.Position = C2DVector(2.0f, 2.0f);
    c.MaxZoom = C2DVector(3.2f, 10.0f);
    c.ProcessEvent(EVENT_ZOOM_OUT_SLOW);              // (3,4.5) + 0.4 -> (3.4,4.9), X clamped to 3.2
    if (g_zoomCalls != 2 || !Near(g_lastZoom.X, 3.2f) || !Near(g_lastZoom.Y, 4.9f)) ok = false;
    if (!Near(g_lastPos.X, 10.0f - 8.0f * (3.2f / 3.0f)) || !Near(g_lastPos.Y, 20.0f - 18.0f * (4.9f / 4.5f))) ok = false;
    if (!c.Zooming || !Near(c.LastInputTime, 20.0f)) ok = false;

    if (!ok) {
        printf("005467f0_TEST FAIL\n");
        return 1;
    }
    printf("005467f0_TEST PASS\n");
    return 0;
}