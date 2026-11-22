#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#include "engine/C3DVector.h"
#define FABLE_RETAIL_ASSERT(x) ((void)0)

typedef std::string CCharString;
template<class T> class CArray : public std::vector<T> {
public:
    const T* GetAsCArray() const { return this->empty() ? (const T*)this : &this->front(); }
    T& At(unsigned int index) { return (*this)[index]; }
};

static int FinishedStreams;
class CMemoryDataInputStream {
public:
    CMemoryDataInputStream(const void* data, unsigned long length) : Current((const char*)data), End(Current+length) {}
    ~CMemoryDataInputStream() { FinishedStreams += Current == End; }
    float ReadFloat() { return Read<float>(); }
    unsigned long ReadULONG() { return Read<unsigned long>(); }
    C3DVector Read3DVector() {
        float x=ReadFloat(), y=ReadFloat(), z=ReadFloat();
        return C3DVector(x,y,z);
    }
    CCharString ReadNullTerminatedString() {
        const char* start=Current;
        while (Current!=End && *Current)
            ++Current;
        if (Current==End)
            abort();
        return CCharString(start,Current++);
    }
private:
    template<class T> T Read() {
        if (End-Current < (int)sizeof(T))
            abort();
        T result;
        memcpy(&result,Current,sizeof(result));
        Current += sizeof(result);
        return result;
    }
    const char* Current;
    const char* End;
};
struct C3DBoundingBox {
    void SetMinMax(const C3DVector& min, const C3DVector& max) { Min=min; Max=max; }
    C3DVector Min, Max;
};
struct C3DBoundingSphere {
    C3DBoundingSphere(const C3DVector& center=C3DVector(0.0f,0.0f,0.0f), float radius=0.0f) : Center(center), Radius(radius) {}
    C3DVector Center;
    float Radius;
};
struct CMeshDataBankEntry {};
struct C3DMeshInfo : CMeshDataBankEntry {
    void SetPhysicsMeshIndex(unsigned long i) { PhysicsMeshIndex=i; }
    void SetBoundingVolumes(const C3DBoundingSphere& sphere, const C3DBoundingBox& box) {
        BoundingSphere=sphere;
        BoundingBox=box;
    }
    C3DBoundingSphere BoundingSphere;
    C3DBoundingBox BoundingBox;
    long PhysicsMeshIndex;
    float SafeBoundingRadius;
    unsigned long LODDataSizes[4];
    float LODErrors[4];
};
struct C3DAnimationInfo : CMeshDataBankEntry {
    float Duration, NonLoopingDuration, Rotation;
    C3DVector MovementVector;
};
class CMeshDataBank {
public:
    struct CLipSyncEntry {
        CLipSyncEntry(const CCharString& phoneme, unsigned long index) : PhonemeName(phoneme), AnimationIndex(index) {}
        CCharString PhonemeName;
        unsigned long AnimationIndex;
    };
    static bool IsRenderMesh(unsigned long type) { return type==1 || type==2 || type==4 || type==5; }
    static bool IsAnimation(unsigned long type) { return type==6 || type==10 || type==7 || type==8 || type==9; }
    static bool IsStaticMesh(unsigned long type) { return type==1; }
    static bool IsAnimatedMesh(unsigned long type) { return type==5; }
    void InitialiseEntry(unsigned long datatype, unsigned long index, const CArray<char>& info_buffer);
    CArray<CMeshDataBankEntry*> MeshesAndAnimations;
    std::map<CCharString, CArray<CLipSyncEntry> > LipSyncAnimations;
};

void CMeshDataBank::InitialiseEntry(unsigned long datatype, unsigned long index, const CArray<char>& info_buffer)
{
    CMemoryDataInputStream info_stream(info_buffer.GetAsCArray(), info_buffer.size());
    CMeshDataBankEntry* mesh_entry = MeshesAndAnimations.At(index);
    FABLE_RETAIL_ASSERT(mesh_entry);
    if (IsRenderMesh(datatype)) {
        C3DMeshInfo* meshinfo = static_cast<C3DMeshInfo*>(mesh_entry);
        unsigned long physics_mesh_index = info_stream.ReadULONG();
        if ((IsStaticMesh(datatype) || IsAnimatedMesh(datatype)) && physics_mesh_index)
            meshinfo->SetPhysicsMeshIndex(physics_mesh_index);
        C3DVector center;
        center = info_stream.Read3DVector();
        float radius = info_stream.ReadFloat();
        C3DBoundingSphere bsphere(center, radius);
        C3DVector bbox_min = info_stream.Read3DVector();
        C3DVector bbox_max = info_stream.Read3DVector();
        C3DBoundingBox bbox;
        bbox.SetMinMax(bbox_min, bbox_max);
        float safe_radius = radius;
        if (IsAnimatedMesh(datatype))
            safe_radius += bsphere.Center.GetMagnitude();
        meshinfo->SetBoundingVolumes(bsphere, bbox);
        meshinfo->SafeBoundingRadius = safe_radius;
        unsigned long no_lods = info_stream.ReadULONG();
        FABLE_RETAIL_ASSERT(no_lods <= 4);
        std::fill_n(meshinfo->LODDataSizes, 4L, 0);
        for (unsigned long c_lod = 0; c_lod < no_lods; ++c_lod)
            meshinfo->LODDataSizes[c_lod] = info_stream.ReadULONG();
        std::fill_n(meshinfo->LODErrors, 4L, 0.0f);
        for (unsigned long c_lod = 0; c_lod < no_lods; ++c_lod)
            meshinfo->LODErrors[c_lod] = info_stream.ReadFloat();
    } else if (IsAnimation(datatype)) {
        C3DAnimationInfo* animinfo = static_cast<C3DAnimationInfo*>(mesh_entry);
        animinfo->Duration = info_stream.ReadFloat();
        animinfo->NonLoopingDuration = info_stream.ReadFloat();
        animinfo->MovementVector = info_stream.Read3DVector();
        animinfo->Rotation = info_stream.ReadFloat();
        if ((datatype & 0xf) == 8) {
            CCharString anim_name = info_stream.ReadNullTerminatedString();
            CCharString phoneme_name = info_stream.ReadNullTerminatedString();
            CLipSyncEntry lipsync(phoneme_name, index);
            LipSyncAnimations[anim_name].push_back(lipsync);
        }
    }
}

template<class T> static void append(CArray<char>& buffer, const T& x) {
    const char* first=(const char*)&x;
    buffer.insert(buffer.end(),first,first+sizeof(T));
}
static void appendPoint(CArray<char>& buffer,float x,float y,float z) {
    append(buffer,x); append(buffer,y); append(buffer,z);
}
static void appendString(CArray<char>& buffer,const char* value) {
    buffer.insert(buffer.end(),value,value+strlen(value)+1);
}
static bool same(const C3DVector& point,float x,float y,float z) {
    return point.X==x && point.Y==y && point.Z==z;
}
int main() {
    const unsigned long meshTypes[]={1,2,4,5};
    for (unsigned int m=0;m!=4;++m) {
        const unsigned long type=meshTypes[m];
        for (unsigned long physics=0;physics!=2;++physics) {
            for (unsigned long lods=0;lods!=6;lods+=2) {
                CArray<char> buffer;
                append(buffer,physics ? 17UL : 0UL);
                appendPoint(buffer,3.0f,4.0f,0.0f);
                append(buffer,2.0f);
                appendPoint(buffer,-1.0f,-2.0f,-3.0f);
                appendPoint(buffer,1.0f,2.0f,3.0f);
                append(buffer,lods);
                for (unsigned long i=0;i!=lods;++i)
                    append(buffer,10UL+i);
                for (unsigned long i=0;i!=lods;++i)
                    append(buffer,0.125f*(i+1));
                CMeshDataBank bank;
                C3DMeshInfo info;
                info.PhysicsMeshIndex=99;
                for (unsigned int j=0;j!=4;++j) {
                    info.LODDataSizes[j]=99;
                    info.LODErrors[j]=99.0f;
                }
                bank.MeshesAndAnimations.push_back(&info);
                bank.InitialiseEntry(type,0,buffer);
                const long expected=physics && (type==1 || type==5) ? 17 : 99;
                if (info.PhysicsMeshIndex!=expected || info.SafeBoundingRadius!=(type==5?7.0f:2.0f)
                    || info.BoundingSphere.Radius!=2.0f || !same(info.BoundingSphere.Center,3.0f,4.0f,0.0f)
                    || !same(info.BoundingBox.Min,-1.0f,-2.0f,-3.0f) || !same(info.BoundingBox.Max,1.0f,2.0f,3.0f))
                    return 1;
                for (unsigned int j=0;j!=4;++j)
                    if (info.LODDataSizes[j]!=(j<lods?10UL+j:0UL) || info.LODErrors[j]!=(j<lods?0.125f*(j+1):0.0f))
                        return 2;
            }
        }
    }
    CMeshDataBank bank;
    C3DAnimationInfo entries[2];
    bank.MeshesAndAnimations.push_back(&entries[0]);
    bank.MeshesAndAnimations.push_back(&entries[1]);
    for (unsigned long type=6;type!=11;++type) {
        CArray<char> buffer;
        append(buffer,10.0f);
        append(buffer,8.0f);
        appendPoint(buffer,6.0f,-4.0f,2.0f);
        append(buffer,-3.0f);
        if (type==8) {
            appendString(buffer,"dialogue");
            appendString(buffer,"phoneme");
        }
        bank.InitialiseEntry(type,1,buffer);
        if (entries[1].Duration!=10.0f || entries[1].NonLoopingDuration!=8.0f || entries[1].Rotation!=-3.0f
            || !same(entries[1].MovementVector,6.0f,-4.0f,2.0f) || bank.LipSyncAnimations.size()!=(type>=8?1:0))
            return 3;
    }
    if (bank.LipSyncAnimations["dialogue"].size()!=1 || bank.LipSyncAnimations["dialogue"][0].PhonemeName!="phoneme"
       || bank.LipSyncAnimations["dialogue"][0].AnimationIndex!=1)
        return 4;
    CArray<char> empty;
    bank.InitialiseEntry(11,1,empty);
    if (entries[1].Duration!=10.0f || FinishedStreams!=30 || bank.LipSyncAnimations.size()!=1)
        return 5;
    puts("00a276d0_TEST PASS");
    return 0;
}