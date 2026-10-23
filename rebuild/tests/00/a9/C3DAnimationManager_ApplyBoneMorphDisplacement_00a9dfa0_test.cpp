// C3DAnimationManager::ApplyBoneMorphDisplacement(C3DKeyframe*, const C3DVector*, const C3DMesh2&) const -- scale each bone keyframe's position by the morph vector its +4 index selects (W cleared)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9DFA0, length 565 bytes (`ret 0Ch`; no callees; Ego_r.pdb
//   ?ApplyBoneMorphDisplacement@C3DAnimationManager@@ABEXPAVC3DKeyframe@@PBVC3DVector@@ABVC3DMesh2@@@Z).
// Shape: the bone count is read once into a local (that is what lets VC unroll 4x); i is unsigned (`jae` remainder test);
//   the position goes through a C3DVector temporary with a user copy ctor times the morph, assigned through
//   C4DVector::operator=(const C3DVector&) that clears W.
// Layout: C3DKeyframe Rotation +0, Position C4DVector +0x10; C3DMesh2 BoneCount +0x98, Bones +0x9c (0x3c stride).
// Leaf: ApplyBoneMorphDisplacement
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator*(const C3DVector& o) const { return C3DVector(X * o.X, Y * o.Y, Z * o.Z); }
};
struct C4DVector {
    float X, Y, Z, W;
    C4DVector() {}
    C4DVector& operator=(const C3DVector& v) { X = v.X; Y = v.Y; Z = v.Z; W = 0.0f; return *this; }
};
struct CQuaternion { float X, Y, Z, W; };
struct C3DKeyframe {
    CQuaternion Rotation;                       // +0x00
    C4DVector Position;                         // +0x10
    C4DVector Scaling;                          // +0x20
};
struct C3DMeshBone {                            // C3DBone2, 0x3c bytes
    unsigned long NameCRC;
    long MorphIndex;                            // +0x04 (C3DBone2::Parent in the landed RemapBoneIndices model)
    unsigned char _pad08[0x34];
};
struct C3DMesh2 {
    unsigned char _pad00[0x98];
    unsigned long NumBones;                              // +0x98
    C3DMeshBone* Bones;                         // +0x9c
};
struct C3DAnimationManager {
    void ApplyBoneMorphDisplacement(C3DKeyframe* keyframes, const C3DVector* morphs, const C3DMesh2& mesh) const;
};

void C3DAnimationManager::ApplyBoneMorphDisplacement(C3DKeyframe* keyframes, const C3DVector* morphs, const C3DMesh2& mesh) const
{
    unsigned long numBones = mesh.NumBones;
    const C3DMesh2& m = mesh;
    for (unsigned long i = 0; i < numBones; ++i) {
        C3DKeyframe& keyframe = keyframes[i];
        long morph = m.Bones[i].MorphIndex;
        if (morph >= 0) {
            const C3DVector& scale = morphs[morph];
            keyframe.Position = C3DVector(keyframe.Position.X, keyframe.Position.Y, keyframe.Position.Z) * scale;
        }
    }
}

#include <stdio.h>
#include <string.h>
int main()
{
    static C3DMesh2 mesh; memset(&mesh,0,sizeof(mesh));
    C3DMeshBone bones[7]; memset(bones,0,sizeof(bones));
    long idx[7]={1,-1,0,2,-5,1,0};
    for(int k=0;k<7;++k) bones[k].MorphIndex=idx[k];
    mesh.NumBones=7; mesh.Bones=bones;
    C3DVector morphs[3]; morphs[0]=C3DVector(2,3,4); morphs[1]=C3DVector(0.5f,-1,10); morphs[2]=C3DVector(-2,0,1);
    C3DKeyframe kf[8];
    for(int k=0;k<8;++k){kf[k].Position.X=(float)k+1;kf[k].Position.Y=(float)k*2;kf[k].Position.Z=(float)-k;kf[k].Position.W=9;kf[k].Scaling.X=7;}
    C3DAnimationManager am; am.ApplyBoneMorphDisplacement(kf,morphs,mesh);
    int ok=1;
    for(int k=0;k<8;++k){
        float x=(float)k+1,y=(float)k*2,z=(float)-k,w=9;
        if(k<7&&idx[k]>=0){const C3DVector&s=morphs[idx[k]];x*=s.X;y*=s.Y;z*=s.Z;w=0;}
        if(kf[k].Position.X!=x||kf[k].Position.Y!=y||kf[k].Position.Z!=z||kf[k].Position.W!=w||kf[k].Scaling.X!=7){ok=0;printf("bad %d\n",k);}
    }
    if(ok) printf("00a9dfa0_TEST PASS\n");
    return ok?0:1;
}