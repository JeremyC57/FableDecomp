// CParticleProgram::ApplyTransform(const CMatrix3x4&) -- transforms initial particles and scales distance constraints.
// Leaf: ?ApplyTransform@CParticleProgram@@QAEXABVCMatrix3x4@@@Z
#define FABLE_RETAIL_VECTOR_SPLINE_CTOR
#define FABLE_RETAIL_VECTOR_SPLINE_COPY
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_PARTICLE_PROGRAM_TRANSFORM
#include <math.h>
#pragma intrinsic(sqrt)
#include "engine/CParticleProgram.h"
#include "engine/CMatrix3x4.h"
#include "engine/CParticleDistanceConstraint.h"
#include "engine/CParticleMinMaxDistanceConstraint.h"
#include "engine/TConstraintType.h"

void CParticleProgram::ApplyTransform(const CMatrix3x4& transform)
{
    const CParticleSimulation& sim=PeekInitialSimulation();
    char* data=(char*)Constraints.PeekData();
    unsigned long total_length=Constraints.GetLength();
    unsigned long pos=0;
    while(pos<total_length) {
        const unsigned long* header=(const unsigned long*)(data+pos);
        unsigned long type=header[0];
        unsigned long count=header[1];
        unsigned long length=header[2];
        pos+=3*sizeof(unsigned long);
        if(type==CONSTRAINT_DISTANCE) {
            CParticleDistanceConstraint* constraints=(CParticleDistanceConstraint*)(data+pos);
            for(unsigned long c1=0;c1<count;++c1) {
                CParticleDistanceConstraint& constraint=constraints[c1];
                C3DVector pos1=sim.GetParticlePosition(constraint.Index1);
                C3DVector pos2=sim.GetParticlePosition(constraint.Index2);
                float old_dist=(pos2-pos1).GetMagnitude();
                transform.TransformPoint(pos1);
                transform.TransformPoint(pos2);
                float new_dist=(pos2-pos1).GetMagnitude();
                if(old_dist>0.0f) {
                    float scale=new_dist/old_dist;
                    constraint.Distance*=scale;
                }
            }
        } else if(type==CONSTRAINT_MIN_MAX_DISTANCE) {
            CParticleMinMaxDistanceConstraint* constraints=(CParticleMinMaxDistanceConstraint*)(data+pos);
            for(unsigned long c1=0;c1<count;++c1) {
                CParticleMinMaxDistanceConstraint& constraint=constraints[c1];
                C3DVector pos1=sim.GetParticlePosition(constraint.Index1);
                C3DVector pos2=sim.GetParticlePosition(constraint.Index2);
                float old_dist=(pos2-pos1).GetMagnitude();
                transform.TransformPoint(pos1);
                transform.TransformPoint(pos2);
                float new_dist=(pos2-pos1).GetMagnitude();
                if(old_dist>0.0f) {
                    float scale=new_dist/old_dist;
                    constraint.MinDistance*=scale;
                    constraint.MaxDistance*=scale;
                }
            }
        }
        pos+=length;
    }
    for(unsigned long c1=0;c1<InitialSimulation.GetParticleCount();++c1) {
        C3DVector pos;
        transform.TransformPoint(InitialSimulation.GetParticlePosition(c1),pos);
        InitialSimulation.SetParticlePosition(c1,pos);
    }
}
