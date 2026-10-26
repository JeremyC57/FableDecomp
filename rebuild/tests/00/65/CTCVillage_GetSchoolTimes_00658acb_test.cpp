#include <cstdio>
struct CLongVector { long* _M_start; long* _M_finish; long* _M_end_of_storage; };
static int g_destroyed = 0;
class CVillageDef { public:
    long RefCount; CLongVector SchoolStartTimes; long SchoolDuration;
    virtual ~CVillageDef() {}
    virtual void Destroy() { ++g_destroyed; }
};
struct CDefPointerVillageDef {
    CVillageDef* Object;
    CDefPointerVillageDef(CVillageDef* p) : Object(p) { if (Object) ++Object->RefCount; }
    CDefPointerVillageDef(const CDefPointerVillageDef& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointerVillageDef() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    const CVillageDef* operator->() const { return Object; }
};
static long g_maxRef = 0;
class CTCVillage { public:
    CDefPointerVillageDef PDef;
    CTCVillage(CVillageDef* d) : PDef(d) { }
    CDefPointerVillageDef GetPDef() const { return PDef; }
    bool GetTimes(const CLongVector& v, long duration, float& start, float& end) const
    {
        if (PDef.Object->RefCount > g_maxRef) g_maxRef = PDef.Object->RefCount;
        if (v._M_start == v._M_finish) return false;
        start = (float)*v._M_start; end = start + (float)duration; return true;
    }
    bool GetSchoolTimes(float& start, float& end) const
    {
        return GetTimes(GetPDef()->SchoolStartTimes, GetPDef()->SchoolDuration, start, end);
    }
};
int main()
{
    long times[2] = { 9, 13 };
    CVillageDef def; def.RefCount = 0; def.SchoolStartTimes._M_start = times; def.SchoolStartTimes._M_finish = times + 2; def.SchoolDuration = 3;
    {
        CTCVillage v(&def);
        float s = 0.0f, e = 0.0f;
        if (!v.GetSchoolTimes(s, e) || s != 9.0f || e != 12.0f || g_maxRef != 3 || def.RefCount != 1 || g_destroyed != 0) { std::printf("00658acb_TEST FAIL\n"); return 1; }
        def.SchoolStartTimes._M_finish = times;
        if (v.GetSchoolTimes(s, e) || def.RefCount != 1) { std::printf("00658acb_TEST FAIL\n"); return 1; }
    }
    if (g_destroyed != 1) { std::printf("00658acb_TEST FAIL\n"); return 1; }
    std::printf("00658acb_TEST PASS\n");
    return 0;
}