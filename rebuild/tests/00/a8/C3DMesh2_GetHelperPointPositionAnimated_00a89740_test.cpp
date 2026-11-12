// Behaviour test for 0x00A89740 C3DMesh2::GetHelperPointPositionAnimated (one blend state): a copy of the model and of the
// function's logic, checked against independently worked values (all exactly representable floats).
#include <cstdio>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    const C3DVector GetScaled(const C3DVector& scale) const { return C3DVector(X * scale.X, Y * scale.Y, Z * scale.Z); }
};
class CMatrix3x4 { public:
    float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
    void TransformPoint(C3DVector& point) const
    {
        const float xx = point.X;
        const float yy = point.Y;
        const float zz = point.Z;
        point.X = E31 * zz + E21 * yy + E11 * xx + E41;
        point.Y = E32 * zz + E22 * yy + E12 * xx + E42;
        point.Z = E33 * zz + E23 * yy + E13 * xx + E43;
    }
};
class C3DHelperPoint { public: C3DVector Position; long BoneIndex; };
class C3DBone2 { public: unsigned long NameCRC; long Parent; long OriginalNoChildren; CMatrix3x4 LocalizationMatrix; };
class C3DAnimationBlendState { public: int id; };
class C3DMesh2;
class C3DAnimationManager { public:
    int calls;
    const C3DAnimationBlendState* lastState;
    const C3DMesh2* lastMesh;
    unsigned long lastBone;
    void CalcBoneTransformation(const C3DAnimationBlendState& state, const C3DMesh2& mesh, unsigned long bone,
        CMatrix3x4& transform, C3DVector& scale)
    {
        ++calls; lastState = &state; lastMesh = &mesh; lastBone = bone;
        // a quarter turn about Z (x -> y, y -> -x) plus a translation, and a non-uniform scale
        transform.E11 = 0.0f;  transform.E12 = 1.0f; transform.E13 = 0.0f;
        transform.E21 = -1.0f; transform.E22 = 0.0f; transform.E23 = 0.0f;
        transform.E31 = 0.0f;  transform.E32 = 0.0f; transform.E33 = 1.0f;
        transform.E41 = 10.0f; transform.E42 = 20.0f; transform.E43 = 30.0f;
        scale = C3DVector(2.0f, 4.0f, 0.5f);
    }
};
class C3DMesh2 { public:
    C3DBone2* Bones;
    C3DVector GetHelperPointPositionAnimated(const C3DHelperPoint& helper, const C3DAnimationBlendState& state,
        C3DAnimationManager& manager) const
    {
        if (helper.BoneIndex != -1) {
            C3DVector pos(helper.Position);
            Bones[helper.BoneIndex].LocalizationMatrix.TransformPoint(pos);
            CMatrix3x4 transform;
            C3DVector scale;
            manager.CalcBoneTransformation(state, *this, helper.BoneIndex, transform, scale);
            pos = scale.GetScaled(pos);
            transform.TransformPoint(pos);
            return pos;
        }
        return helper.Position;
    }
};

static bool Same(const C3DVector& v, float x, float y, float z) { return v.X == x && v.Y == y && v.Z == z; }

int main()
{
    bool ok = true;
    C3DBone2 bones[3];
    for (int i = 0; i < 3; ++i) {
        CMatrix3x4& m = bones[i].LocalizationMatrix;
        m.E11 = 1.0f; m.E12 = 0.0f; m.E13 = 0.0f;
        m.E21 = 0.0f; m.E22 = 1.0f; m.E23 = 0.0f;
        m.E31 = 0.0f; m.E32 = 0.0f; m.E33 = 1.0f;
        m.E41 = 0.0f; m.E42 = 0.0f; m.E43 = 0.0f;
    }
    // bone 2: swap x and z, double y, translate by (1, 2, 3)
    CMatrix3x4& l = bones[2].LocalizationMatrix;
    l.E11 = 0.0f; l.E13 = 1.0f; l.E22 = 2.0f; l.E31 = 1.0f; l.E33 = 0.0f;
    l.E41 = 1.0f; l.E42 = 2.0f; l.E43 = 3.0f;
    C3DMesh2 mesh; mesh.Bones = bones;
    C3DAnimationBlendState state; state.id = 7;
    C3DAnimationManager manager; manager.calls = 0; manager.lastState = 0; manager.lastMesh = 0; manager.lastBone = 99;

    // 1. no bone: the stored position comes back untouched and the manager is not asked
    C3DHelperPoint loose; loose.Position = C3DVector(3.0f, -5.0f, 8.0f); loose.BoneIndex = -1;
    C3DVector r = mesh.GetHelperPointPositionAnimated(loose, state, manager);
    if (!Same(r, 3.0f, -5.0f, 8.0f) || manager.calls != 0) ok = false;

    // 2. bone 2: localise (3,-5,8) -> (8+1, -10+2, 3+3) = (9,-8,6); scale (2,4,.5) -> (18,-32,3);
    //    transform: X = -y + 10 = 42, Y = x + 20 = 38, Z = z + 30 = 33
    C3DHelperPoint bound; bound.Position = C3DVector(3.0f, -5.0f, 8.0f); bound.BoneIndex = 2;
    r = mesh.GetHelperPointPositionAnimated(bound, state, manager);
    if (!Same(r, 42.0f, 38.0f, 33.0f)) ok = false;
    if (manager.calls != 1 || manager.lastBone != 2 || manager.lastState != &state || manager.lastMesh != &mesh) ok = false;
    if (!Same(bound.Position, 3.0f, -5.0f, 8.0f)) ok = false;      // the helper itself is not modified

    // 3. bone 0 (identity localisation): (1,1,2) -> scale (2,4,1) -> X = -4 + 10 = 6, Y = 2 + 20 = 22, Z = 1 + 30 = 31
    C3DHelperPoint root; root.Position = C3DVector(1.0f, 1.0f, 2.0f); root.BoneIndex = 0;
    r = mesh.GetHelperPointPositionAnimated(root, state, manager);
    if (!Same(r, 6.0f, 22.0f, 31.0f) || manager.calls != 2 || manager.lastBone != 0) ok = false;

    if (!ok) { std::printf("00a89740_TEST FAIL\n"); return 1; }
    std::printf("00a89740_TEST PASS\n");
    return 0;
}