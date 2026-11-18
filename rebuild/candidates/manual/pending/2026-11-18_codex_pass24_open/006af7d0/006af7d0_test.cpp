#include <math.h>
#include <stdio.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x = 0, float y = 0, float z = 0) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X-o.X,Y-o.Y,Z-o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X+o.X,Y+o.Y,Z+o.Z); }
    C3DVector& operator-=(const C3DVector& o) { X-=o.X; Y-=o.Y; Z-=o.Z; return *this; }
    C3DVector operator*(float k) const { return C3DVector(X*k,Y*k,Z*k); }
    C3DVector operator^(const C3DVector& o) const { return C3DVector(Y*o.Z-Z*o.Y,Z*o.X-X*o.Z,X*o.Y-Y*o.X); }
    float Dot(const C3DVector& o) const { return X*o.X+Y*o.Y+Z*o.Z; }
    float GetLengthSquared() const { return Dot(*this); }
    void AccurateNormalise()
    {
        float length = (float)sqrt(GetLengthSquared());
        X/=length; Y/=length; Z/=length;
    }
    void Normalise() { AccurateNormalise(); }
};

bool g_flipMeshCollideNormals_013b86ec = false;
struct CPhysicsCollideResponse {
    C3DVector CollideNormal;
};
struct CTCPhysicsBase {
    void ImpulseRespondToCollision(const CPhysicsCollideResponse&, C3DVector&) const;
};
static bool Equal(const C3DVector& v, float x, float y, float z)
{
    return fabs(v.X-x)<0.0001f && fabs(v.Y-y)<0.0001f && fabs(v.Z-z)<0.0001f;
}

void CTCPhysicsBase::ImpulseRespondToCollision(const CPhysicsCollideResponse& response_info, C3DVector& current_velocity) const
{
    if (g_flipMeshCollideNormals_013b86ec) {
        C3DVector ax_z = response_info.CollideNormal;
        C3DVector ax_x = ax_z ^ (ax_z.Y > ax_z.Z ? C3DVector(0.0f, 0.0f, 1.0f) : C3DVector(0.0f, 1.0f, 0.0f));
        ax_x.AccurateNormalise();
        C3DVector ax_y = ax_z ^ ax_x;
        ax_y.AccurateNormalise();
        C3DVector velocity_in_new_coords;
        velocity_in_new_coords.Z = ax_z.Dot(current_velocity);
        velocity_in_new_coords.X = ax_x.Dot(current_velocity);
        velocity_in_new_coords.Y = ax_y.Dot(current_velocity);
        if (velocity_in_new_coords.Z < 0.0f)
            velocity_in_new_coords.Z = 0.0f;
        current_velocity = ax_z * velocity_in_new_coords.Z + ax_x * velocity_in_new_coords.X + ax_y * velocity_in_new_coords.Y;
    } else {
        float dot = response_info.CollideNormal.Dot(current_velocity);
        current_velocity -= response_info.CollideNormal * dot;
    }
}

int main()
{
    CTCPhysicsBase physics;
    CPhysicsCollideResponse response;
    response.CollideNormal = C3DVector(0,0,1);
    C3DVector motion(3,4,5);
    physics.ImpulseRespondToCollision(response,motion);
    if (!Equal(motion,3,4,0))
        return 1;

    g_flipMeshCollideNormals_013b86ec = true;
    motion = C3DVector(3,4,5);
    physics.ImpulseRespondToCollision(response,motion);
    if (!Equal(motion,3,4,5))
        return 2;
    motion = C3DVector(3,4,-5);
    physics.ImpulseRespondToCollision(response,motion);
    if (!Equal(motion,3,4,0))
        return 3;
    response.CollideNormal = C3DVector(0,1,0);
    motion = C3DVector(3,-4,5);
    physics.ImpulseRespondToCollision(response,motion);
    if (!Equal(motion,3,0,5))
        return 4;
    puts("006af7d0_TEST PASS");
    return 0;
}
