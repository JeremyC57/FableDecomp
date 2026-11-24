// Run the section builder against in-memory buffers. The real shared vector, packed-normal and rounding helpers are used.
#include <cassert>
#include <cstdio>
#include <cstring>
#define FABLE_PORTABLE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_BACKGROUND_VERTEX_HELPERS_PASS28
#define FABLE_RETAIL_CGENERICVERTEX_INLINE_SETTERS
#include "engine/C3DPackedNormal.h"
#include "engine/CGenericVertex.h"
#include "engine/GFFloatToLongNear.h"
enum {VERTEX_NULL,IB_FORMAT_16,PT_TRIANGLE_LIST};
struct VertexBase {
    unsigned short X,Y;float Z;float NormalX,NormalY,NormalZ;unsigned char U1,V1;
};
struct CTVertexLandscapeBackground : CGenericVertex<VertexBase> {};
class CLandscapeBackgroundPatch;
struct CPatchTesselationEdgeStrip {
    struct CVertex {float WorldZ;unsigned long Normal;unsigned short WorldX,WorldY,VertexBufferIndex,IndexBufferIndex,FanBaseIndex;unsigned char U,V;};
    typedef CVertex CFanBaseVertex;
    CVertex vertices[2];CFanBaseVertex fan;long start,length;bool vertical;int resized;
    void Reset(long s,long l,bool v,CLandscapeBackgroundPatch*){start=s;length=l;vertical=v;resized=0;}
    CFanBaseVertex& GetFanBaseVert(int i){assert(i==0);return fan;}
    CVertex& GetVert(int i){assert(i>=0 && i<2);return vertices[i];}
    void ResizeVertexCount(int a,int b,int c,int d,CLandscapeBackgroundPatch*){assert(a==2 && b==1 && c==0 && d==0);++resized;}
};
struct VertexBufferModel {
    CTVertexLandscapeBackground data[5];unsigned locks,unlocks;
    unsigned GetSize()const{return sizeof(data);}
    void* Lock(unsigned a,unsigned b,unsigned size){assert(a==0 && b==0 && size==sizeof(data));++locks;return data;}
    void Unlock(){++unlocks;}
};
struct IndexBufferModel {
    unsigned short data[12];unsigned locks,unlocks;
    void* Lock(unsigned a,unsigned b,unsigned c){assert(!a && !b && !c);++locks;return data;}
    void Unlock(){++unlocks;}
};
struct CEngineLandscapeMemoryManager {
    VertexBufferModel vb;IndexBufferModel ib;
    VertexBufferModel* AllocateVertexBuffer(CLandscapeBackgroundPatch*,int t,int n,unsigned stride,int usage){
        assert(t==VERTEX_NULL && n==5 && stride==sizeof(CTVertexLandscapeBackground) && usage==3);return &vb;}
    IndexBufferModel* AllocateIndexBuffer(CLandscapeBackgroundPatch*,int n,int usage,int f,int t){
        assert(n==24 && usage==7 && f==IB_FORMAT_16 && t==PT_TRIANGLE_LIST);return &ib;}
};
struct Resources {int GetVertexBufferUsage(){return 3;}int GetIndexBufferUsage(){return 7;}};
struct EngineLandscapeRenderer {CEngineLandscapeMemoryManager* MemoryManager;};
namespace NEngineInternal {
    static Resources resources; static Resources* GPResourceManager=&resources;
    static CEngineLandscapeMemoryManager memory;
    static EngineLandscapeRenderer renderer={&memory};static EngineLandscapeRenderer* GPLandscapeRenderer=&renderer;
}
struct Def {float SeaHeight;};
static Def def={-4.0f};namespace NEngineInternal { static const Def* GPEngineDef=&def; }
struct MapModel {
    C3DVector world;
    const C3DVector& PeekWorldPosition() const {return world;}
    long PeekWorldX() const{return (long)world.X;}long PeekWorldY() const{return (long)world.Y;}
};
struct CEngineLandscapeMap {MapModel* Map; C3DVector PeekWorldPosition() const;};
struct Node {CEngineLandscapeMap* Map;CEngineLandscapeMap* PeekMap() const{return Map;}};
class CLandscapeBackgroundPatch {
public:
    Node* Parent;unsigned short MapX,MapY,Width,Height,PolyCountIncDegenerates,VertexCount;
    VertexBufferModel* VertexBuffer;IndexBufferModel* IndexBuffer;bool SharedIndexBuffer,IsDefaultSection;
    CPatchTesselationEdgeStrip EdgeStrips[4];
    void BuildDefaultSection();
    long PeekWorldX() const;long PeekWorldY() const;
    static long PackUVForVertexBuffer(float);
    unsigned long PackVertexNormal(const C3DVector&);
    static void FABLE_FASTCALL SetTargetVertexNormal(CTVertexLandscapeBackground&,unsigned long);
};
inline long CLandscapeBackgroundPatch::PackUVForVertexBuffer(float uv)
{
    return GFFloatToLongNear(static_cast<float>(uv * 255.0f));
}

inline void C3DPackedNormal::Initialise(const C3DVector& normal)
{
    VectorBits = ((static_cast<unsigned long>(GFFloatToLongNear(static_cast<float>(normal.Z * 511.0f))) & 0x3ff) << 22)
               | ((static_cast<unsigned long>(GFFloatToLongNear(static_cast<float>(normal.Y * 1023.0f))) & 0x7ff) << 11)
               | (static_cast<unsigned long>(GFFloatToLongNear(static_cast<float>(normal.X * 1023.0f))) & 0x7ff);
}

inline unsigned long CLandscapeBackgroundPatch::PackVertexNormal(const C3DVector& normal)
{
    C3DPackedNormal packed_normal(normal);
    return packed_normal.GetAsPackedInt();
}

// FableWin 02CB2210 returns this vector by value. The inner CEngineMap getter returns a reference.
inline C3DVector CEngineLandscapeMap::PeekWorldPosition() const
{
    return Map->PeekWorldPosition();
}
inline long CLandscapeBackgroundPatch::PeekWorldX() const
{
    return MapX + Parent->PeekMap()->Map->PeekWorldX();
}
inline long CLandscapeBackgroundPatch::PeekWorldY() const
{
    return MapY + Parent->PeekMap()->Map->PeekWorldY();
}

void FABLE_FASTCALL CLandscapeBackgroundPatch::SetTargetVertexNormal(CTVertexLandscapeBackground& vertex, unsigned long packedNormal)
{
    long x = packedNormal & 0x7ff;
    long y = (packedNormal >> 11) & 0x7ff;
    long z = packedNormal >> 22;
    x |= (x & 0x400) ? ~0x7ffL : 0;
    y |= (y & 0x400) ? ~0x7ffL : 0;
    z |= (z & 0x200) ? ~0x3ffL : 0;
    C3DVector normal(x * (1.0f / 1023.0f), y * (1.0f / 1023.0f), z * (1.0f / 511.0f));
    vertex.NormalX = normal.X;
    vertex.NormalY = normal.Y;
    vertex.NormalZ = normal.Z;
}

void CLandscapeBackgroundPatch::BuildDefaultSection()
{
    PolyCountIncDegenerates = 4;
    VertexCount = 5;
    CEngineLandscapeMemoryManager* mem_manager = NEngineInternal::GPLandscapeRenderer->MemoryManager;
    VertexBuffer = mem_manager->AllocateVertexBuffer(this, VERTEX_NULL, VertexCount,
        sizeof(CTVertexLandscapeBackground), NEngineInternal::GPResourceManager->GetVertexBufferUsage());
    CTVertexLandscapeBackground* verts = reinterpret_cast<CTVertexLandscapeBackground*>(
        VertexBuffer->Lock(0, 0, VertexBuffer->GetSize()));
    const float sea_height = NEngineInternal::GPEngineDef->SeaHeight;
    const C3DVector vt[5] = {
        C3DVector(MapX, MapY, sea_height),
        C3DVector(MapX + Width, MapY, sea_height),
        C3DVector(MapX + Width, MapY + Height, sea_height),
        C3DVector(MapX, MapY + Height, sea_height),
        C3DVector(MapX + (Width / 2), MapY + (Height / 2), sea_height)
    };
    const float vert_uvs_u[5] = {0.0f, 1.0f, 1.0f, 0.0f, 0.5f};
    const float vert_uvs_v[5] = {0.0f, 0.0f, 1.0f, 1.0f, 0.5f};
    for (long n = 0; n < 5; ++n) {
        verts[n].SetPosition(vt[n] + Parent->PeekMap()->PeekWorldPosition());
        verts[n].SetU1(PackUVForVertexBuffer(vert_uvs_u[n]));
        verts[n].SetV1(PackUVForVertexBuffer(vert_uvs_v[n]));
        SetTargetVertexNormal(verts[n],
            PackVertexNormal(C3DVector(0.0f, 0.0f, 1.0f)));
    }
    VertexBuffer->Unlock();

    const long world_x = PeekWorldX();
    const long world_y = PeekWorldY();
    EdgeStrips[2].Reset(world_x, Width, false, this);
    EdgeStrips[3].Reset(world_x, Width, false, this);
    EdgeStrips[0].Reset(world_y, Height, true, this);
    EdgeStrips[1].Reset(world_y, Height, true, this);
    const long corner_vertex_table[2][2] = {{0, 3}, {1, 2}};
    for (long side = 0; side < 4; ++side) {
        CPatchTesselationEdgeStrip::CFanBaseVertex& fan_vert = EdgeStrips[side].GetFanBaseVert(0);
        fan_vert.Normal = PackVertexNormal(C3DVector(0.0f, 0.0f, 1.0f));
        fan_vert.U = static_cast<unsigned char>(PackUVForVertexBuffer(0.5f));
        fan_vert.V = static_cast<unsigned char>(PackUVForVertexBuffer(0.5f));
        fan_vert.WorldX = static_cast<unsigned short>(world_x + (Width / 2));
        fan_vert.WorldY = static_cast<unsigned short>(world_y + (Height / 2));
        fan_vert.WorldZ = Parent->PeekMap()->PeekWorldPosition().Z + sea_height;
        fan_vert.VertexBufferIndex = 4;
        for (long vt = 0; vt < 2; ++vt) {
            CPatchTesselationEdgeStrip::CVertex& tvert = EdgeStrips[side].GetVert(vt);
            long grid_x = 0;
            long grid_y = 0;
            if (side == 2) {
                grid_x += vt;
            } else if (side == 3) {
                grid_x += vt;
                ++grid_y;
            } else if (side == 0) {
                grid_y += vt;
            } else {
                ++grid_x;
                grid_y += vt;
            }
            const long map_pos_x = MapX + grid_x * Width;
            const long map_pos_y = MapY + grid_y * Height;
            tvert.WorldX = static_cast<unsigned short>(world_x + grid_x * Width);
            tvert.WorldY = static_cast<unsigned short>(world_y + grid_y * Height);
            tvert.WorldZ = Parent->PeekMap()->PeekWorldPosition().Z + sea_height;
            tvert.Normal = PackVertexNormal(C3DVector(0.0f, 0.0f, 1.0f));
            tvert.U = static_cast<unsigned char>(PackUVForVertexBuffer(static_cast<float>(grid_x)));
            tvert.V = static_cast<unsigned char>(PackUVForVertexBuffer(static_cast<float>(grid_y)));
            tvert.FanBaseIndex = 0;
            tvert.IndexBufferIndex = static_cast<unsigned short>(side * 3);
            tvert.VertexBufferIndex = static_cast<unsigned short>(corner_vertex_table[grid_x][grid_y]);
        }
        EdgeStrips[side].ResizeVertexCount(2, 1, 0, 0, this);
    }

    const unsigned short id_table[12] = {0, 3, 4, 1, 4, 2, 0, 4, 1, 3, 2, 4};
    IndexBuffer = mem_manager->AllocateIndexBuffer(this, PolyCountIncDegenerates * 6,
        NEngineInternal::GPResourceManager->GetIndexBufferUsage(), IB_FORMAT_16, PT_TRIANGLE_LIST);
    SharedIndexBuffer = false;
    unsigned short* ids = reinterpret_cast<unsigned short*>(IndexBuffer->Lock(0, 0, 0));
    memcpy(ids, id_table, sizeof(id_table));
    IndexBuffer->Unlock();
    IsDefaultSection = true;
}

static void check(unsigned width,unsigned height) {
    MapModel map;map.world=C3DVector(100.0f,200.0f,1.5f);CEngineLandscapeMap landscape={&map};Node parent={&landscape};
    CLandscapeBackgroundPatch patch;patch.Parent=&parent;patch.MapX=10;patch.MapY=20;patch.Width=width;patch.Height=height;
    patch.SharedIndexBuffer=true;patch.IsDefaultSection=false;
    unsigned n=NEngineInternal::memory.vb.locks;patch.BuildDefaultSection();
    assert(patch.IsDefaultSection && !patch.SharedIndexBuffer && patch.VertexCount==5 && patch.PolyCountIncDegenerates==4);
    assert(patch.VertexBuffer->locks==n+1 && patch.VertexBuffer->unlocks==n+1 && patch.IndexBuffer->locks==n+1 && patch.IndexBuffer->unlocks==n+1);
    unsigned x[5]={110,110+width,110+width,110,110+width/2};
    unsigned y[5]={220,220,220+height,220+height,220+height/2};
    static const unsigned u[5]={0,255,255,0,128},v[5]={0,0,255,255,128};
    static const int edges[4][2]={{0,3},{1,2},{0,1},{3,2}};
    for(int i=0;i<5;++i){const CTVertexLandscapeBackground& vert=patch.VertexBuffer->data[i];
        assert(vert.X==x[i] && vert.Y==y[i] && vert.Z==-2.5f && vert.U1==u[i] && vert.V1==v[i] && vert.NormalX==0.0f && vert.NormalY==0.0f && vert.NormalZ==1.0f);}
    for(int side=0;side<4;++side){const CPatchTesselationEdgeStrip& e=patch.EdgeStrips[side];
        assert(e.vertical==(side<2) && e.start==(side<2?220:110) && e.length==(side<2?height:width) && e.resized==1);
        assert(e.fan.WorldX==x[4] && e.fan.WorldY==y[4] && e.fan.WorldZ==-2.5f && e.fan.U==128 && e.fan.V==128 && e.fan.VertexBufferIndex==4 && e.fan.Normal==0x7fc00000UL);
        for(int i=0;i<2;++i){int j=edges[side][i];const CPatchTesselationEdgeStrip::CVertex& c=e.vertices[i];
            assert(c.WorldX==x[j] && c.WorldY==y[j] && c.WorldZ==-2.5f && c.Normal==0x7fc00000UL);
            assert(c.U==u[j] && c.V==v[j] && c.FanBaseIndex==0 && c.IndexBufferIndex==side*3 && c.VertexBufferIndex==j);}}
    static const unsigned short winding[12]={0,3,4,1,4,2,0,4,1,3,2,4};
    assert(!std::memcmp(winding,patch.IndexBuffer->data,sizeof(winding)));
}
int main(){CTVertexLandscapeBackground signed_normal;
    CLandscapeBackgroundPatch::SetTargetVertexNormal(signed_normal,0x80400000UL|0x401UL);
    assert(signed_normal.NormalX==-1.0f && signed_normal.NormalY==0.0f && signed_normal.NormalZ==-1.0f);
    check(9,7);check(8,6);check(0,0);std::puts("00be6880_TEST PASS");return 0;}