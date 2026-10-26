#include <cstdio>
// NParticleEngine::CPSCSplineBase::CSplineRandomOffsetData::GetPointAtPos(out,position,time) -- evaluate current point or previous-frame point, synchronizing changed spline counts
// Leaf: GetPointAtPos
// Retail00B20FD0,177 bytes through ret12 at00B2107E;15 trailing int3 bytes excluded.
// Same PDB/native20-byte CCardinalSpline model as canonical Update00B21160. No shared
// spline/vector headers exist. Array assignment0075B8D0; spline evaluator00A567A0.
// Stable-count path evaluates current into a discarded local before evaluating previous toout;
// changed count snapshots current and evaluates current. Third float parameter is unused.
class C3DVector { public: float X, Y, Z; };
template<class T> class CArray { public:
    T* m_begin; T* m_end; T* m_capacity;
    CArray& operator=(const CArray& other);
    long Size() const { return (long)(m_end-m_begin); }
};
class CCardinalSpline { public:
    CArray<C3DVector> ControlPoints;
    float Tension;
    bool EndPointsConnected;
    void GetPoint(C3DVector*,float); //00A567A0
    CCardinalSpline& operator=(const CCardinalSpline& other) {
        ControlPoints=other.ControlPoints; Tension=other.Tension; EndPointsConnected=other.EndPointsConnected; return *this;
    }
};
struct CSplineRandomOffsetData {
    CCardinalSpline* InitialSpline;
    CCardinalSpline ThisSpline;
    CCardinalSpline PreviousSpline;
    bool OffsetOnlyAtStartup;
    bool ResetEachFrame;
    CArray<C3DVector> SplinePosOffsetLimits;
    CArray<C3DVector> SplinePosOffsetSpeed;
    void GetPointAtPos(C3DVector* out,float position,float time);
};
void CSplineRandomOffsetData::GetPointAtPos(C3DVector* out,float position,float time) {
    if(OffsetOnlyAtStartup) {ThisSpline.GetPoint(out,position);return;}
    if(PreviousSpline.ControlPoints.Size()!=ThisSpline.ControlPoints.Size()) {PreviousSpline=ThisSpline;ThisSpline.GetPoint(out,position);}
    else {C3DVector current;ThisSpline.GetPoint(&current,position);PreviousSpline.GetPoint(out,position);}
}

static CSplineRandomOffsetData* active;
static C3DVector* destination;
static C3DVector copied[5];
static int bad,copies,evaluations;
static CCardinalSpline* evaluated[2];
static C3DVector* targets[2];
static float wantedPosition;
template<class T> CArray<T>& CArray<T>::operator=(const CArray<T>& other) {
    ++copies;
    if(this!=&active->PreviousSpline.ControlPoints || &other!=&active->ThisSpline.ControlPoints || evaluations) bad=1;
    long count=other.Size();
    for(long i=0;i<count;++i) copied[i]=other.m_begin[i];
    m_begin=copied;m_end=copied+count;m_capacity=copied+5;return *this;
}
void CCardinalSpline::GetPoint(C3DVector* out,float position) {
    if(evaluations>=2 || position!=wantedPosition || (this!=&active->ThisSpline && this!=&active->PreviousSpline)) {bad=1;return;}
    evaluated[evaluations]=this;targets[evaluations]=out;++evaluations;
    if(this==&active->PreviousSpline && copies) bad=1;
    out->X=Tension+position;out->Y=(float)ControlPoints.Size();out->Z=EndPointsConnected?1.0f:0.0f;
}
static int run(int onlyStart,int currentCount,int previousCount,float position,float unusedTime) {
    CSplineRandomOffsetData data;active=&data;
    C3DVector current[4],previous[4],out;
    for(int i=0;i<4;++i) {current[i].X=(float)i+1;current[i].Y=(float)i+2;current[i].Z=(float)i+3;previous[i].X=-11;previous[i].Y=-13;previous[i].Z=-17;}
    data.ThisSpline.ControlPoints.m_begin=current;data.ThisSpline.ControlPoints.m_end=current+currentCount;data.ThisSpline.ControlPoints.m_capacity=current+4;
    data.PreviousSpline.ControlPoints.m_begin=previous;data.PreviousSpline.ControlPoints.m_end=previous+previousCount;data.PreviousSpline.ControlPoints.m_capacity=previous+4;
    data.ThisSpline.Tension=3.5f;data.ThisSpline.EndPointsConnected=true;
    data.PreviousSpline.Tension=-7.25f;data.PreviousSpline.EndPointsConnected=false;
    data.OffsetOnlyAtStartup=onlyStart!=0;data.ResetEachFrame=true;data.InitialSpline=&data.ThisSpline;
    data.SplinePosOffsetLimits.m_begin=current;data.SplinePosOffsetLimits.m_end=current+1;data.SplinePosOffsetLimits.m_capacity=current+4;
    data.SplinePosOffsetSpeed.m_begin=previous;data.SplinePosOffsetSpeed.m_end=previous+2;data.SplinePosOffsetSpeed.m_capacity=previous+4;
    bad=copies=evaluations=0;destination=&out;wantedPosition=position;
    data.GetPointAtPos(&out,position,unusedTime);
    int changed=!onlyStart && currentCount!=previousCount;
    int stable=!onlyStart && !changed;
    if(bad || copies!=changed || evaluations!=(stable?2:1) || evaluated[0]!=&data.ThisSpline ||
       (stable && (evaluated[1]!=&data.PreviousSpline || targets[0]==&out || targets[1]!=&out)) ||
       (!stable && targets[0]!=&out)) return 0;
    float expectedX=(stable?-7.25f:3.5f)+position;
    if(out.X!=expectedX || out.Y!=(float)currentCount || out.Z!=(stable?0.0f:1.0f)) return 0;
    if(changed) {
        if(data.PreviousSpline.Tension!=3.5f || !data.PreviousSpline.EndPointsConnected || data.PreviousSpline.ControlPoints.m_begin!=copied || data.PreviousSpline.ControlPoints.Size()!=currentCount) return 0;
        for(int j=0;j<currentCount;++j) if(copied[j].X!=current[j].X || copied[j].Y!=current[j].Y || copied[j].Z!=current[j].Z) return 0;
    } else if(data.PreviousSpline.Tension!=-7.25f || data.PreviousSpline.EndPointsConnected || data.PreviousSpline.ControlPoints.m_begin!=previous || data.PreviousSpline.ControlPoints.Size()!=previousCount) return 0;
    return data.ResetEachFrame && data.OffsetOnlyAtStartup==(onlyStart!=0) && data.InitialSpline==&data.ThisSpline &&
       data.SplinePosOffsetLimits.m_begin==current && data.SplinePosOffsetSpeed.m_begin==previous;
}
int main() {
    if(!run(1,3,1,0.5f,7) || !run(0,3,1,-2.0f,13) || !run(0,1,3,1.25f,-29) ||
      !run(0,2,2,0.75f,0) || !run(0,2,2,0.75f,999)) {printf("00b20fd0_TEST FAIL\n");return 1;}
    printf("00b20fd0_TEST PASS\n");return 0;
}