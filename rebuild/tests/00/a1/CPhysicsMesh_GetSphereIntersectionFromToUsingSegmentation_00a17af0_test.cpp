#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <vector>

struct FixtureVector {
    float x,y,z;
    FixtureVector() {}
    FixtureVector(float a,float b,float c) : x(a),y(b),z(c) {}
    FixtureVector operator-(const FixtureVector& v) const { return FixtureVector(x-v.x,y-v.y,z-v.z); }
    float GetSquaredMagnitude() const { return x*x+y*y+z*z; }
    float GetMagnitudeAndNormalise()
    {
        float magnitude=static_cast<float>(sqrt(GetSquaredMagnitude()));
        x/=magnitude;
        y/=magnitude;
        z/=magnitude;
        return magnitude;
    }
};
static bool SameVector(const FixtureVector& a,const FixtureVector& b)
{
    return a.x==b.x && a.y==b.y && a.z==b.z;
}
struct FixtureTriangle {
    FixtureTriangle(bool occurs,const FixtureVector& hit_pos,const FixtureVector& hit_normal)
        : hit(occurs),position(hit_pos),normal(hit_normal) {}
    bool hit;
    FixtureVector position,normal;
};
struct FixtureIntersectionData {
    FixtureVector IntersectionPosition,Normal;
    float DistanceSquared;
};
class FixtureMesh;
struct FixtureSegment {
    std::vector<long> candidates;
    mutable unsigned calls;
    FixtureSegment() : calls(0) {}
    void GetCollidedWithTriangles(const FixtureMesh*,float,const FixtureVector&,
                                  const FixtureVector&,std::vector<long>&) const;
};

class FixtureMesh {
public:
    std::vector<FixtureTriangle> TotalFaceIndices;
    const FixtureSegment* RootSegment;
    mutable std::vector<long> working_set;
    mutable unsigned visited;
    FixtureMesh(const FixtureSegment& segment) : RootSegment(&segment),visited(0) {}

    std::vector<long>& GetSingleInstanceCollisionSet() const { return working_set; }

    bool HasCollidedWithTriangleAt(const FixtureTriangle& tri,float radius,const FixtureVector& from,
                                   const FixtureVector& direction,float speed,
                                   FixtureVector& position,FixtureVector& normal) const
    {
        assert(radius==1.0f && speed==4.0f);
        assert(SameVector(from,FixtureVector(3,2,1)));
        assert(SameVector(direction,FixtureVector(0,0,1)));
        assert(visited<RootSegment->candidates.size());
        assert(&tri==&TotalFaceIndices[RootSegment->candidates[visited]]);
        ++visited;
        if (!tri.hit)
            return false;
        position=tri.position;
        normal=tri.normal;
        return true;
    }

    bool GetSphereIntersectionFromToUsingSegmentation(float,const FixtureVector&,const FixtureVector&,
                                                      FixtureIntersectionData&) const;
};

void FixtureSegment::GetCollidedWithTriangles(const FixtureMesh* mesh,float radius,
    const FixtureVector& from,const FixtureVector& to,std::vector<long>& affected) const
{
    assert(radius==1.0f);
    assert(SameVector(from,FixtureVector(3,2,1)));
    assert(SameVector(to,FixtureVector(3,2,5)));
    assert(mesh->RootSegment==this);
    assert(&affected==&mesh->working_set && affected.size()==1 && affected[0]==-1);
    ++calls;
    affected=candidates;
}

bool FixtureMesh::GetSphereIntersectionFromToUsingSegmentation(
    float sphere_radius,const FixtureVector& from,const FixtureVector& to,FixtureIntersectionData& data) const
{
    std::vector<long>& possible_set = GetSingleInstanceCollisionSet();
    RootSegment->GetCollidedWithTriangles(this,sphere_radius,from,to,possible_set);
    std::vector<long>::const_iterator index;
    FixtureVector intersection_pos;
    FixtureVector intersection_normal;
    float best_intersection_distance = 1.0e10f;
    bool found = false;
    FixtureVector normalised_velocity = to - from;
    float speed = normalised_velocity.GetMagnitudeAndNormalise();
    for (index = possible_set.begin(); index != possible_set.end(); ++index) {
        const FixtureTriangle& test_triangle = TotalFaceIndices[*index];
        if (HasCollidedWithTriangleAt(test_triangle,sphere_radius,from,normalised_velocity,speed,
                                     intersection_pos,intersection_normal)) {
            float intersection_distance = (intersection_pos - from).GetSquaredMagnitude();
            if (intersection_distance < best_intersection_distance) {
                data.DistanceSquared = intersection_distance;
                data.IntersectionPosition = intersection_pos;
                data.Normal = intersection_normal;
                best_intersection_distance = intersection_distance;
                found = true;
            }
        }
    }
    return found;
}

static void Check(FixtureMesh& mesh,FixtureSegment& segment,const long* indices,unsigned count,
                  bool result,const FixtureVector& position,const FixtureVector& normal,float distance)
{
    FixtureIntersectionData data={FixtureVector(10,12,14),FixtureVector(9,8,7),-5.0f};
    segment.candidates.clear();
    if (count)
        segment.candidates.assign(indices,indices+count);
    mesh.working_set.assign(1,-1);
    mesh.visited=0;
    const unsigned previous=segment.calls;
    assert(mesh.GetSphereIntersectionFromToUsingSegmentation(1.0f,FixtureVector(3,2,1),FixtureVector(3,2,5),data)==result);
    assert(segment.calls==previous+1 && mesh.visited==count);
    assert(data.DistanceSquared==distance);
    assert(SameVector(data.Normal,normal));
    assert(SameVector(data.IntersectionPosition,position));
}

int main()
{
    FixtureSegment segment;
    FixtureMesh mesh(segment);
    const FixtureVector untouched(10,12,14);
    const FixtureVector untouched_normal(9,8,7);
    const FixtureVector far(3,2,4);
    const FixtureVector near(3,2,2);
    const FixtureVector alternate_near(3,1,1);
    const FixtureVector normal_a(0,0,1);
    const FixtureVector normal_b(0,1,0);
    mesh.TotalFaceIndices.push_back(FixtureTriangle(true,far,normal_a));
    mesh.TotalFaceIndices.push_back(FixtureTriangle(true,near,normal_b));
    mesh.TotalFaceIndices.push_back(FixtureTriangle(true,alternate_near,normal_a));
    mesh.TotalFaceIndices.push_back(FixtureTriangle(false,far,normal_a));
    mesh.TotalFaceIndices.push_back(FixtureTriangle(true,FixtureVector(100003,2,1),normal_a));
    mesh.TotalFaceIndices.push_back(FixtureTriangle(true,FixtureVector(100004,2,1),normal_b));
    Check(mesh,segment,0,0,false,untouched,untouched_normal,-5);
    const long only_far[]={0};
    Check(mesh,segment,only_far,1,true,far,normal_a,9);
    const long missed[]={3,3};
    Check(mesh,segment,missed,2,false,untouched,untouched_normal,-5);
    const long later_nearer[]={0,3,1,2,1};
    Check(mesh,segment,later_nearer,5,true,near,normal_b,1);
    const long reverse_tie[]={2,1,0};
    Check(mesh,segment,reverse_tie,3,true,alternate_near,normal_a,1);
    const long at_sentinel[]={4};
    Check(mesh,segment,at_sentinel,1,false,untouched,untouched_normal,-5);
    const long past_sentinel[]={5,3};
    Check(mesh,segment,past_sentinel,2,false,untouched,untouched_normal,-5);
    puts("00a17af0_TEST PASS");
}