#include <cstdio>
#include <list>
#include <vector>
struct V { float X, Y, Z; };
struct M { float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43; };
struct Entry { V Position; float Width; Entry(const V& p, float w) : Position(p), Width(w) {} };
struct Sys { bool StayWithEmitter; M m;
  void TransformPosIfWorld(V& p) const { if (!StayWithEmitter) { float xx=p.X, yy=p.Y, zz=p.Z;
    p.X=m.E31*zz+m.E21*yy+m.E11*xx+m.E41; p.Y=m.E32*zz+m.E22*yy+m.E12*xx+m.E42; p.Z=m.E33*zz+m.E23*yy+m.E13*xx+m.E43; } } };
struct Sprite {
  std::vector<V> pts; long MaxTrailLength; bool UseSplinePoints; std::list<Entry> Trail;
  long n() const { return (long)pts.size(); }
  void GetPointAtIndex(V& out, long i, float t) { out = pts[i]; out.Z += t; }
  void GetPointAtPos(V& out, float pos, float t) { out.X = pos; out.Y = 2*pos; out.Z = t; }
  void Set(Sys& system, float width, float time) {
    Trail.clear();
    if (UseSplinePoints) {
      long count = MaxTrailLength < n() ? MaxTrailLength : n();
      for (long i = 0; i < count; ++i) { V point; GetPointAtIndex(point, i, time); V p = point; system.TransformPosIfWorld(p); Trail.push_back(Entry(p, width)); }
    } else {
      float step = 1.0f / (float)MaxTrailLength;
      for (float pos = 0.0f; pos <= 1.0f; pos += step) { V point; GetPointAtPos(point, pos, time); V p = point; system.TransformPosIfWorld(p); Trail.push_back(Entry(p, width)); }
    }
  }
};
int main() {
  Sys s; s.StayWithEmitter = false; M id = {1,0,0, 0,1,0, 0,0,1, 10,20,30}; s.m = id;
  Sprite sp; sp.MaxTrailLength = 2; sp.UseSplinePoints = true;
  V a = {1,2,3}, b = {4,5,6}, c = {7,8,9}; sp.pts.push_back(a); sp.pts.push_back(b); sp.pts.push_back(c);
  sp.Trail.push_back(Entry(a, 9.0f));
  sp.Set(s, 0.5f, 1.0f);
  if (sp.Trail.size() != 2 || sp.Trail.front().Position.X != 11 || sp.Trail.front().Position.Z != 34 || sp.Trail.back().Width != 0.5f) { std::printf("00b08250_TEST FAIL\n"); return 1; }
  s.StayWithEmitter = true; sp.MaxTrailLength = 5; sp.Set(s, 1.0f, 0.0f);
  if (sp.Trail.size() != 3) { std::printf("00b08250_TEST FAIL\n"); return 1; }
  sp.UseSplinePoints = false; sp.MaxTrailLength = 4; sp.Set(s, 2.0f, 3.0f);
  if (sp.Trail.size() != 5 || sp.Trail.back().Position.X != 1.0f || sp.Trail.back().Position.Z != 3.0f) { std::printf("00b08250_TEST FAIL\n"); return 1; }
  std::printf("00b08250_TEST PASS\n"); return 0;
}