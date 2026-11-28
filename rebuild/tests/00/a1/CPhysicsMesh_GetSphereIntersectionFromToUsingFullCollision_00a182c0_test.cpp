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
class FixtureMesh {
public:
    std::vector<FixtureTriangle> TotalFaceIndices;
    mutable unsigned visited;
    FixtureMesh() : visited(0) {}

    bool HasCollidedWithTriangleAt(const FixtureTriangle& tri,float radius,const FixtureVector& from,
                                   const FixtureVector& direction,float speed,
                                   FixtureVector& position,FixtureVector& normal) const
    {
        assert(radius==1.0f && speed==4.0f);
        assert(SameVector(from,FixtureVector(3,2,1)));
        assert(SameVector(direction,FixtureVector(0,0,1)));
        assert(&tri==&TotalFaceIndices[visited]);
        ++visited;
        if (!tri.hit)
            return false;
        position=tri.position;
        normal=tri.normal;
        return true;
    }

    bool GetSphereIntersectionFromToUsingFullCollision(float,const FixtureVector&,const FixtureVector&,
                                                       FixtureIntersectionData&) const;
};

bool FixtureMesh::GetSphereIntersectionFromToUsingFullCollision(
    float sphere_radius, const FixtureVector& from, const FixtureVector& to, FixtureIntersectionData& data) const
{
    FixtureVector intersection_pos;
    FixtureVector intersection_normal;
    float best_intersection_distance = 1.0e10f;
    long total_faces = TotalFaceIndices.size();
    bool found = false;
    FixtureVector normalised_velocity = to - from;
    float speed = normalised_velocity.GetMagnitudeAndNormalise();
    for (long index = 0; index < total_faces; ++index) {
        const FixtureTriangle& test_triangle = TotalFaceIndices[index];
        if (HasCollidedWithTriangleAt(test_triangle, sphere_radius, from, normalised_velocity, speed,
                                      intersection_pos, intersection_normal)) {
            float intersection_distance = (intersection_pos - from).GetSquaredMagnitude();
            if (intersection_distance < best_intersection_distance) {
                found = true;
                data.Normal = intersection_normal;
                data.DistanceSquared = intersection_distance;
                data.IntersectionPosition = intersection_pos;
                best_intersection_distance = intersection_distance;
            }
        }
    }
    return found;
}

static void Check(FixtureMesh& mesh,bool result,const FixtureVector& position,
                  const FixtureVector& normal,float distance)
{
    FixtureIntersectionData data={FixtureVector(10,12,14),FixtureVector(9,8,7),-5.0f};
    mesh.visited=0;
    assert(mesh.GetSphereIntersectionFromToUsingFullCollision(1.0f,FixtureVector(3,2,1),FixtureVector(3,2,5),data)==result);
    assert(mesh.visited==mesh.TotalFaceIndices.size());
    assert(data.DistanceSquared==distance);
    assert(SameVector(data.Normal,normal));
    assert(SameVector(data.IntersectionPosition,position));
}

int main()
{
    FixtureMesh mesh;
    const FixtureVector untouched(10,12,14);
    const FixtureVector untouched_normal(9,8,7);
    const FixtureVector far(3,2,4);
    const FixtureVector near(3,2,2);
    const FixtureVector alternate_near(3,1,1);
    const FixtureVector normal_a(0,0,1);
    const FixtureVector normal_b(0,1,0);
    Check(mesh,false,untouched,untouched_normal,-5);
    mesh.TotalFaceIndices.push_back(FixtureTriangle(false,far,normal_a));
    Check(mesh,false,untouched,untouched_normal,-5);
    mesh.TotalFaceIndices.push_back(FixtureTriangle(true,far,normal_a));
    Check(mesh,true,far,normal_a,9);
    mesh.TotalFaceIndices.push_back(FixtureTriangle(true,near,normal_b));
    Check(mesh,true,near,normal_b,1);
    mesh.TotalFaceIndices.push_back(FixtureTriangle(true,alternate_near,normal_a));
    mesh.TotalFaceIndices.push_back(FixtureTriangle(true,far,normal_a));
    Check(mesh,true,near,normal_b,1);
    mesh.TotalFaceIndices.clear();
    mesh.TotalFaceIndices.push_back(FixtureTriangle(true,FixtureVector(100003,2,1),normal_a));
    Check(mesh,false,untouched,untouched_normal,-5);
    puts("00a182c0_TEST PASS");
}