// CTCCameraPointScriptedSpline::UpdateSplines() -- rebuild the camera splines from the scripted points: the time spline (reused, emptied front-first) gets one key per point at evenly spaced times, and fresh position, target, up and FOV splines are built from the points' fields; then reset the playback state
// Grade: DIFFER(1242v1146) -- first full draft (time-spline reuse with front erase, five STLport push_back sites, four new splines, playback reset); retail zeroes a fifth key vector up front and never destroys it (ours frees the empty temporary passed to the new time spline), keeps this in esi (ours ebx) and is 96 bytes shorter
// Leaf: UpdateSplines
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075DDF0, 1152 bytes, `ret`.
// Callees: operator new 0x00BFEA1A, free 0x00BFEA14, CSpline ctor(const vector<C3DVector>&) 0x006F0810, the spline holder assign
//   0x0075FA30, STLport vector<C3DVector>::_M_insert_overflow 0x0066E370, __copy (random access) 0x0074CA20 (__fastcall),
//   ResetPlayback 0x0075CF00. 1.0f 0x0122DED8, 2^32 0x0122DCB4.
extern "C" void __cdecl free(void*);                    // 0x00BFEA14
void* operator new(unsigned int size);                  // 0x00BFEA1A
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct __true_type { };
struct random_access_iterator_tag { };
C3DVector* __fastcall __copy(const C3DVector* first, const C3DVector* last, C3DVector* result, const random_access_iterator_tag&, long*);   // 0x0074CA20
struct CVec3Vector {
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    CVec3Vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CVec3Vector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    bool empty() const { return _M_start == _M_finish; }
    void _M_insert_overflow(C3DVector* pos, const C3DVector& x, const __true_type&, unsigned long n, bool atend);   // 0x0066E370
    __forceinline void push_back(const C3DVector& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
    __forceinline void erase(C3DVector* pos)
    {
        if (pos + 1 != _M_finish)
            __copy(pos + 1, _M_finish, pos, random_access_iterator_tag(), (long*)0);
        --_M_finish;
    }
    __forceinline void pop_front()
    {
        if (size() != 0)
            erase(_M_start);
    }
};
struct CSpline {
    CVec3Vector Keys;                                   // +0x00
    float Tension;                                      // +0x0c
    bool Looped;                                        // +0x10
    CSpline(const CVec3Vector& keys);                   // 0x006F0810
};
struct CSplineHolder {
    CSpline* P;
    void* Aux;
    void Set(CSpline* spline);                          // 0x0075FA30
};
struct SScriptedPoint {
    long Flags;                                         // +0x00
    C3DVector Position;                                 // +0x04
    C3DVector Target;                                   // +0x10
    float UpX, UpY, UpZ;                                // +0x1c
    float Speed;                                        // +0x28
    long _2c, _30;
    float FOV;                                          // +0x34
};
struct CPointVector {
    SScriptedPoint* _M_start;
    SScriptedPoint* _M_finish;
    SScriptedPoint* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
};

class CTCCameraPointScriptedSpline {
public:
    unsigned char _pad00[0x60];
    CSplineHolder TimeSpline;                           // +0x60
    CPointVector Points;                                // +0x68
    unsigned char _pad74[0x78 - 0x74];
    CSplineHolder PositionSpline;                       // +0x78
    CSplineHolder TargetSpline;                         // +0x80
    CSplineHolder UpSpline;                             // +0x88
    CSplineHolder FOVSpline;                            // +0x90
    unsigned char _pad98[0xa4 - 0x98];
    float TimeTension;                                  // +0xa4
    long PlayTime;                                      // +0xa8
    unsigned char _padac[0xb4 - 0xac];
    long CurrentTime;                                   // +0xb4
    long LastPoint;                                     // +0xb8
    long CurrentPoint;                                  // +0xbc
    void ResetPlayback();                               // 0x0075CF00
    void UpdateSplines();
};

static __forceinline CSpline* NewSpline(const CVec3Vector& keys)
{
    CSpline* spline = new CSpline(keys);
    if (spline) {
        spline->Tension = 0.5f;
        spline->Looped = false;
    }
    return spline;
}

void CTCCameraPointScriptedSpline::UpdateSplines()
{
    if (Points.size() <= 1)
        return;
    unsigned long segments = Points.size() - 1;
    CVec3Vector positions;
    CVec3Vector targets;
    CVec3Vector ups;
    CVec3Vector fovs;
    float step = 1.0f / (float)segments;
    if (TimeSpline.P == 0)
        TimeSpline.Set(NewSpline(CVec3Vector()));
    else {
        for (unsigned long n = TimeSpline.P->Keys.size(); n != 0; --n)
            TimeSpline.P->Keys.pop_front();
    }
    float time = 0.0f;
    for (SScriptedPoint* point = Points._M_start; point != Points._M_finish; ++point) {
        positions.push_back(point->Position);
        targets.push_back(point->Target);
        ups.push_back(C3DVector(point->UpX, point->UpY, point->UpZ));
        fovs.push_back(C3DVector(point->FOV, 0.0f, 0.0f));
        TimeSpline.P->Keys.push_back(C3DVector(time, point->Speed, 0.0f));
        time += step;
    }
    TimeSpline.P->Tension = TimeTension;
    PositionSpline.Set(NewSpline(positions));
    TargetSpline.Set(NewSpline(targets));
    UpSpline.Set(NewSpline(ups));
    FOVSpline.Set(NewSpline(fovs));
    ResetPlayback();
    LastPoint = -1;
    CurrentPoint = 0;
    PlayTime = 0;
    CurrentTime = 0;
    LastPoint = Points.size() - 1;
}
