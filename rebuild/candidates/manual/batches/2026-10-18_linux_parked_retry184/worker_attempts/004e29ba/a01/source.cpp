// CParticleAttachmentInfoVector::operator= -- assign the particle-attachment definition vector.
// Retail004e29ba length241. Caller004e299b CParticleAttacherDef::Copy accesses
// ParticlesToAttach+28; propagated CParticleMorphs identity rejected. Actual owner
// header included; native vector12 vs donor16. PDB element16 fields0/4/8/c.
// Callees004e2aab AllocateAndCopy and004e2aec deallocate/clear existing vector.
#pragma optimize("y",off)
#pragma optimize("s",on)
#include "engine/_quarantine/CParticleAttacherDef.h"
#include <new>
#ifdef _MSC_VER
typedef long S32;
#else
typedef int S32;
#endif
struct ParticleAttachmentInfo { S32 ParticleIndex; S32 AttachmentObjectName; float GenerationCutoffDistance; bool DummyObject; };
static inline ParticleAttachmentInfo* CopyRange(const ParticleAttachmentInfo* first,const ParticleAttachmentInfo* last,ParticleAttachmentInfo* out) {
    for(int n=int(last-first);n>0;--n,++first,++out) *out=*first;
    return out;
}
static inline ParticleAttachmentInfo* UninitializedCopy(const ParticleAttachmentInfo* first,const ParticleAttachmentInfo* last,ParticleAttachmentInfo* out) {
    for(;first!=last;++first,++out) new(out) ParticleAttachmentInfo(*first);
    return out;
}
struct CParticleAttachmentInfoVector {
    ParticleAttachmentInfo *Start,*Finish,*End;
    unsigned int size() const {return Finish-Start;}
    unsigned int capacity() const {return End-Start;}
    ParticleAttachmentInfo* AllocateAndCopy(unsigned int,const ParticleAttachmentInfo*,const ParticleAttachmentInfo*); //004e2aab
    void Clear(); //004e2aec
    CParticleAttachmentInfoVector& operator=(const CParticleAttachmentInfoVector&);
};
CParticleAttachmentInfoVector& CParticleAttachmentInfoVector::operator=(const CParticleAttachmentInfoVector& other) {
    if(&other!=this) {
        unsigned int n=other.size();
        if(n>capacity()) {
            ParticleAttachmentInfo* fresh=AllocateAndCopy(n,other.Start,other.Finish);
            Clear(); Start=fresh;End=Start+n;
        } else if(size()>=n) {
            CopyRange(other.Start,other.Finish,Start);
        } else {
            CopyRange(other.Start,other.Start+size(),Start);
            UninitializedCopy(other.Start+size(),other.Finish,Finish);
        }
        Finish=Start+n;
    }
    return *this;
}
