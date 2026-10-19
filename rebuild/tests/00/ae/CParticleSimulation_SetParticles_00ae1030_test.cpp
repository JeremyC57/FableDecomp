#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __stdcall
#define __declspec(x)
#define __int64 long long
#endif

// CParticleSimulation::SetParticles -- allocate and initialize simulation arrays.
// Retail00ae1030 length286, AllocateArrays00adf2a0. Native PDB Size+18,
// Positions/Last/Saved/Accelerations+1c/20/24/28, Alphas+2c. No generated
// CParticleSimulation, C3DVector or C4DVector headers exist. Donor layout agrees.
struct C3DVector { float X,Y,Z; };
struct C4DVector {
    float X,Y,Z,W;
    C4DVector() { X=0.0f; Y=0.0f; Z=0.0f; W=0.0f; }
    C4DVector(const C3DVector& v,float w=0.0f) { X=v.X; Y=v.Y; Z=v.Z; W=w; }
};
template<class T> struct CArray { T* Start; T* Finish; T* End; };
struct CMovableResourceMemoryPool;
struct CParticleSimulation {
    void* VTable; void* RenderObject; void* RenderCounter;
    float Timestep; bool TimestepChanged; float TimestepMultiplier;
    unsigned int Size;
    C4DVector* Positions; C4DVector* LastPositions; C4DVector* SavedPositions;
    C4DVector* Accelerations; float* SimulationAlphas;
    bool AllocateArrays(CMovableResourceMemoryPool&); // 0x00adf2a0
    void SetParticles(const CArray<C3DVector>&,const CArray<float>&,CMovableResourceMemoryPool&);
};
void CParticleSimulation::SetParticles(const CArray<C3DVector>& positions,const CArray<float>& alphas,CMovableResourceMemoryPool& pool) {
    Size=positions.Finish-positions.Start;
    AllocateArrays(pool);
    for(unsigned int i=0;i<Size;++i) {
        C4DVector v(positions.Start[i]);
        Positions[i]=v;
        LastPositions[i]=v;
        SavedPositions[i]=v;
        Accelerations[i]=C4DVector();
        SimulationAlphas[i]=alphas.Start[i];
    }
}



#include <stdio.h>
#include <string.h>
struct CMovableResourceMemoryPool {
    C4DVector positions[6], last[6], saved[6], accelerations[6];
    float alphas[6]; unsigned int calls, expectedSize, overrideSize;
    bool success; C3DVector* mutate;
};
bool CParticleSimulation::AllocateArrays(CMovableResourceMemoryPool& pool) {
    CMovableResourceMemoryPool* p=&pool;
    ++p->calls;
    if(Size!=p->expectedSize) { p->calls=999; return false; }
    Positions=p->positions; LastPositions=p->last; SavedPositions=p->saved;
    Accelerations=p->accelerations; SimulationAlphas=p->alphas;
    if(p->overrideSize!=999) Size=p->overrideSize;
    if(p->mutate) p->mutate[0].Y=123.25f;
    return p->success;
}
static bool same(const C4DVector& v,const C3DVector& p) {
    return v.X==p.X&&v.Y==p.Y&&v.Z==p.Z&&v.W==0.0f;
}
int main() {
    CParticleSimulation a;
    CMovableResourceMemoryPool pool;
    C3DVector pos[4]; float alpha[4];
    for(unsigned int i=0;i<4;++i) { pos[i].X=float(i)-2;pos[i].Y=float(i)*3.25f;pos[i].Z=-float(i)*8;alpha[i]=float(i)*0.25f; }
    CArray<C3DVector> p; p.Start=pos;p.Finish=pos+4;p.End=pos+4;
    CArray<float> al;al.Start=alpha;al.Finish=alpha+4;al.End=alpha+4;
    for(unsigned int pass=0;pass<3;++pass) {
        pool.calls=0;pool.expectedSize=4;pool.overrideSize=pass==1?2:999;
        pool.success=pass!=2;pool.mutate=pass==2?pos:0;
        for(unsigned int j=0;j<6;++j) {
            pool.positions[j].W=99;pool.last[j].W=99;pool.saved[j].W=99;
            pool.accelerations[j].X=99;pool.accelerations[j].Y=99;
            pool.accelerations[j].Z=99;pool.accelerations[j].W=99;pool.alphas[j]=99;
        }
        a.SetParticles(p,al,pool);
        unsigned int n=pass==1?2:4;
        if(pool.calls!=1||a.Size!=n) return 1;
        for(unsigned int k=0;k<n;++k) {
            if(!same(pool.positions[k],pos[k])||!same(pool.last[k],pos[k])||!same(pool.saved[k],pos[k])) return 2;
            C4DVector& z=pool.accelerations[k];if(z.X||z.Y||z.Z||z.W||pool.alphas[k]!=alpha[k]) return 3;
        }
        if(pool.positions[n].W!=99||pool.last[n].W!=99||pool.saved[n].W!=99||pool.accelerations[n].W!=99||pool.alphas[n]!=99) return 4;
    }
    p.Finish=p.Start;pool.calls=0;pool.expectedSize=0;pool.overrideSize=999;pool.mutate=0;
    a.SetParticles(p,al,pool);if(pool.calls!=1||a.Size) return 5;
    puts("00ae1030_TEST PASS");return 0;
}