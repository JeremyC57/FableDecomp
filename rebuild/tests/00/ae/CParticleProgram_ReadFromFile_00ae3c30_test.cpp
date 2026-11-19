// Behaviour test for CParticleProgram::ReadFromFile (0x00AE3C30): a fixture models the stream (ULONG words, raw reads recorded by
// size and destination), the arrays and the simulation, and runs the row's verbatim body over 66 cases: every array count present
// or empty (empty arrays read into the array object's own address), constraints, exports, and the two early `return false` exits.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/C3DTriangle2.h"
#include "engine/C3DQuad2.h"
static void check(bool ok){if(!ok)abort();}
class C3DClothRenderVertex { public:unsigned short position,texcoord;};
class C2DVector {public: float x,y;};
template<class T> class TestArray { public:
    T elements[4];unsigned int count;TestArray():count(0){}
    void resize(unsigned int n){check(n<=4);count=n;}
    unsigned int size()const{return count;}
    T* begin(){return elements;}
    T* GetAsCArray(){return count?elements:reinterpret_cast<T*>(this);}
    unsigned int GetByteSize()const{return count*sizeof(T);}
};
class ConstraintBuffer {public:
    int clears; unsigned int count; unsigned char first;
    ConstraintBuffer():clears(0),count(7),first(0){}
    void Clear(){++clears;count=0;}
};
class CCharString {public: char id; explicit CCharString(char c):id(c){} };
class ExportMap {public:
    int inserts,updates;unsigned int capacity; char names[4];unsigned long ids[4];
    ExportMap():inserts(0),updates(0),capacity(0){}
    void resize(unsigned long n){capacity=n;}
    void Insert(const CCharString& name,unsigned long id){check((unsigned int)inserts<capacity);names[inserts]=name.id;ids[inserts++]=id;}
    void Update(){++updates;}
};
class CDataInputStream {public:
    unsigned long words[24];unsigned int n,next,raw_reads,string_reads,float_reads,bool_reads;
    unsigned int read_sizes[6];void* destinations[6];bool enabled;
    CDataInputStream():n(0),next(0),raw_reads(0),string_reads(0),float_reads(0),bool_reads(0),enabled(false){}
    void word(unsigned long value){words[n++]=value;}
    unsigned long ReadULONG(){check(next<n);return words[next++];}
    void CopyTo(ConstraintBuffer& b,unsigned long len){check(b.clears==1&&b.count==0);b.count=len;b.first=len?99:0;}
    void Read(void* p,unsigned long len){check(p && raw_reads<6);read_sizes[raw_reads]=len;destinations[raw_reads]=p;memset(p,4+raw_reads,len);++raw_reads;}
    float ReadFloat(){++float_reads;return 1.5f;}
    bool ReadEBOOL(){++bool_reads;return enabled;}
    CCharString ReadNullTerminatedString(){return CCharString((char)('A'+string_reads++));}
};
class CMovableResourceMemoryPool {};
class CParticleSimulation {public:
    bool ok;int reads;unsigned int particles;
    CParticleSimulation():ok(true),reads(0),particles(3){}
    bool ReadFromFile(CDataInputStream&,CMovableResourceMemoryPool&){++reads;return ok;}
    unsigned int GetParticleCount()const{return particles;}
    static void ReadVectorArray(unsigned int& dst,unsigned long n,CDataInputStream&,float zero){check(zero==0.0f);dst=n;}
};
class CParticleProgram {public:
    ConstraintBuffer Constraints; CParticleSimulation InitialSimulation;
    TestArray<C3DTriangle2> RenderTris; TestArray<C3DQuad2> RenderQuads; TestArray<C3DClothRenderVertex> RenderVertices;
    unsigned int NonSimCount, NonSimPositions, allocated;bool alloc_ok;
    TestArray<C2DVector> IndexedTextureCoords;TestArray<int> ParticleIndices,VertexIndices;
    float AveragePatchSize;bool BezierEnable;ExportMap ExportParticles;int SimGroups,NonSimGroups,group_calls;
    CParticleProgram():NonSimCount(0),NonSimPositions(100),allocated(0),alloc_ok(true),AveragePatchSize(0),BezierEnable(false),SimGroups(0),NonSimGroups(0),group_calls(0){}
    bool AllocateArrays(unsigned int n,CMovableResourceMemoryPool&){allocated=n;return alloc_ok;}
    unsigned long GetVertexPositionCount()const{return NonSimCount+InitialSimulation.GetParticleCount();}
    void ReadGroups(CDataInputStream&,int& g){check(&g==(group_calls?&NonSimGroups:&SimGroups));g=++group_calls;}
    bool ReadFromFile(CDataInputStream&,CMovableResourceMemoryPool&);
};
bool CParticleProgram::ReadFromFile(CDataInputStream& is, CMovableResourceMemoryPool& alloc)
{
    unsigned long version = is.ReadULONG();
    Constraints.Clear();
    unsigned long size = is.ReadULONG();
    is.CopyTo(Constraints, size);
    if (!InitialSimulation.ReadFromFile(is, alloc))
        return false;
    unsigned long no_tris = is.ReadULONG();
    RenderTris.resize(no_tris);
    is.Read(RenderTris.GetAsCArray(), RenderTris.GetByteSize());
    unsigned long no_quads = is.ReadULONG();
    RenderQuads.resize(no_quads);
    is.Read(RenderQuads.GetAsCArray(), RenderQuads.GetByteSize());
    unsigned long no_render_verts = is.ReadULONG();
    RenderVertices.resize(no_render_verts);
    is.Read(RenderVertices.GetAsCArray(), RenderVertices.GetByteSize());
    NonSimCount = is.ReadULONG();
    if (!AllocateArrays(GetVertexPositionCount(), alloc))
        return false;
    CParticleSimulation::ReadVectorArray(NonSimPositions, NonSimCount, is, 0.0f);
    unsigned long no_texcoord = is.ReadULONG();
    IndexedTextureCoords.resize(no_texcoord);
    is.Read(IndexedTextureCoords.GetAsCArray(), IndexedTextureCoords.GetByteSize());
    unsigned long no_indices = is.ReadULONG();
    ParticleIndices.resize(no_indices);
    is.Read(ParticleIndices.GetAsCArray(), ParticleIndices.GetByteSize());
    no_indices = is.ReadULONG();
    VertexIndices.resize(no_indices);
    is.Read(VertexIndices.GetAsCArray(), VertexIndices.GetByteSize());
    AveragePatchSize = is.ReadFloat();
    BezierEnable = is.ReadEBOOL();
    unsigned long no_exp = is.ReadULONG();
    ExportParticles.resize(no_exp);
    for (unsigned long c1 = 0; c1 < no_exp; ++c1) {
        CCharString name = is.ReadNullTerminatedString();
        unsigned long index = is.ReadULONG();
        ExportParticles.Insert(name, index);
    }
    ExportParticles.Update();
    ReadGroups(is, SimGroups);
    ReadGroups(is, NonSimGroups);
    return true;
}

int main(){
    for(int c=0;c<66;++c){
        CDataInputStream is;CParticleProgram prog;CMovableResourceMemoryPool pool;
        int mode=c<64?0:c-63;
        unsigned int counts[6];for(int i=0;i<6;++i)counts[i]=((c>>i)&1)?1+(i%3):0;
        unsigned int ns=c%3,names=c%4,len=c%2?3:0;
        is.word(82);is.word(len);
        for(int i=0;i<3;++i)is.word(counts[i]);
        is.word(ns);
        for(int i=3;i<6;++i)is.word(counts[i]);
        is.word(names);for(unsigned int i=0;i<names;++i)is.word(20+i);
        is.enabled=c%2!=0;prog.InitialSimulation.ok=mode!=1;prog.alloc_ok=mode!=2;
        check(prog.ReadFromFile(is,pool)==(mode==0));check(prog.InitialSimulation.reads==1);
        check(prog.Constraints.clears==1 && prog.Constraints.count==len && prog.Constraints.first==(len?99:0));
        check(prog.allocated==(mode==1?0:3+ns));check(prog.NonSimPositions==(mode?100:ns));
        const unsigned int sizes[]={6,8,4,8,4,4};check(is.raw_reads==(mode==1?0u:mode==2?3u:6u));
        for(unsigned int i=0;i<is.raw_reads;++i)check(is.read_sizes[i]==sizes[i]*counts[i]);
        if(!mode) {
            check(is.next==is.n && prog.AveragePatchSize==1.5f && prog.BezierEnable==is.enabled);
            check(prog.ExportParticles.capacity==names && (unsigned int)prog.ExportParticles.inserts==names && prog.ExportParticles.updates==1);
            for(unsigned int n=0;n<names;++n)check(prog.ExportParticles.names[n]=='A'+(int)n && prog.ExportParticles.ids[n]==20+n);
            check(is.destinations[3]==(counts[3]?(void*)prog.IndexedTextureCoords.begin():(void*)&prog.IndexedTextureCoords));
            check(is.bool_reads==1 && is.float_reads==1 && prog.SimGroups==1 && prog.NonSimGroups==2);
        } else {
            check(is.next==(mode==1?2u:6u) && prog.ExportParticles.updates==0 && prog.group_calls==0 && is.bool_reads==0 && is.float_reads==0);
        }
    }
    puts("00ae3c30_TEST PASS");return 0;
}