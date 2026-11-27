#include <stdio.h>
#include <vector>
struct C2DExtentsI {long X,Y;};
struct CPixelFormat {long bits;};
enum ESurfacePool { POOL };
enum ESurfaceMultisampleType { SAMPLES };
struct Record {
    static int allocations,releases,calls,fail_at,error;
    static bool Check(const C2DExtentsI& d,const CPixelFormat& f) {
        ++calls;
        if(d.X!=19||d.Y!=31||f.bits!=7)error=1;
        return calls!=fail_at;
    }
};
int Record::allocations; int Record::releases; int Record::calls; int Record::fail_at; int Record::error;
struct CTexture : Record {
    CTexture() {++allocations;}
    ~CTexture() {++releases;}
    bool Initialise(const C2DExtentsI& d,long levels,const CPixelFormat& f,unsigned long flags,ESurfacePool pool,bool first,bool second) {
        if(levels!=5||flags!=17||pool!=POOL||first||!second)error=1;
        return Check(d,f);
    }
};
struct CSurface : Record {
    CSurface() {++allocations;}
    virtual ~CSurface(){++releases;}
    bool InitialiseAsDepthBuffer(const C2DExtentsI& d,const CPixelFormat& f,ESurfaceMultisampleType samples,bool enabled) {
        if(samples!=SAMPLES||!enabled)error=1;return Check(d,f);
    }
};
struct CGPUScratchBufferTextureManager {
    enum EResourceType {RESOURCE_TYPE_NOT_ALLOCATED,RESOURCE_TYPE_TEXTURE,RESOURCE_TYPE_DEPTH_BUFFER_SURFACE};
    struct CResourceEntry{EResourceType ResourceType;CTexture* Texture;CSurface* Surface;C2DExtentsI Dimensions;long NoLevels;
        CPixelFormat PixelFormat;ESurfaceMultisampleType MultiSampleType;unsigned long UseageFlags;ESurfacePool MemoryPool;};
    std::vector<CResourceEntry> Resources;bool ShutdownFlag;bool EndShutdown();
};
bool CGPUScratchBufferTextureManager::EndShutdown()
{
    bool succeded = true;
    if (ShutdownFlag) {
        for (long c_res = 0; c_res < Resources.size(); ++c_res) {
            CResourceEntry& resource = Resources[c_res];
            if (resource.ResourceType == RESOURCE_TYPE_TEXTURE) {
                resource.Texture = new CTexture;
                if (!resource.Texture->Initialise(resource.Dimensions, resource.NoLevels, resource.PixelFormat,
                    resource.UseageFlags, resource.MemoryPool, false, true)) {
                    succeded = false;
                    break;
                }
            } else if (resource.ResourceType == RESOURCE_TYPE_DEPTH_BUFFER_SURFACE) {
                resource.Surface = new CSurface;
                if (!resource.Surface->InitialiseAsDepthBuffer(resource.Dimensions, resource.PixelFormat,
                    resource.MultiSampleType, true)) {
                    succeded = false;
                    break;
                }
            }
        }
        if (!succeded) {
            for (long c_res = 0; c_res < Resources.size(); ++c_res) {
                CResourceEntry& resource = Resources[c_res];
                delete resource.Texture;
                delete resource.Surface;
                resource.Texture = 0;
                resource.Surface = 0;
            }
        }
        if (succeded)
            ShutdownFlag = false;
    }
    return succeded;
}

int main() {
    typedef CGPUScratchBufferTextureManager Manager;
    for(int count=0;count<=4;++count){
        int patterns=1;for(int n=0;n<count;++n)patterns*=3;
        for(int pattern=0;pattern<patterns;++pattern)for(int state=0;state<2;++state)for(int failure=0;failure<=count+1;++failure){
            Manager x;x.ShutdownFlag=state!=0;
            int seq=pattern,active=0;
            for(int i=0;i<count;++i){Manager::CResourceEntry e;e.ResourceType=(Manager::EResourceType)(seq%3);seq/=3;
                if(e.ResourceType!=Manager::RESOURCE_TYPE_NOT_ALLOCATED)++active;
                e.Dimensions.X=19;e.Dimensions.Y=31;e.PixelFormat.bits=7;e.NoLevels=5;e.UseageFlags=17;e.MemoryPool=POOL;e.MultiSampleType=SAMPLES;
                e.Texture=0;e.Surface=0;x.Resources.push_back(e);}
            Record::allocations=Record::releases=Record::calls=Record::error=0;Record::fail_at=failure;
            bool wanted=!state||!failure||failure>active;
            if(x.EndShutdown()!=wanted||x.ShutdownFlag==wanted||Record::error)return 1;
            int attempts=!state?0:wanted?active:failure;
            if(Record::allocations!=attempts||Record::calls!=attempts||Record::releases!=(wanted?0:attempts))return 2;
            for(unsigned j=0;j<x.Resources.size();++j){Manager::CResourceEntry& e=x.Resources[j];
                if(!!e.Texture!=(wanted&&state&&e.ResourceType==Manager::RESOURCE_TYPE_TEXTURE))return 3;
                if(!!e.Surface!=(wanted&&state&&e.ResourceType==Manager::RESOURCE_TYPE_DEPTH_BUFFER_SURFACE))return 4;
                delete e.Texture;delete e.Surface;}
        }
    }
    puts("00f36d30_TEST PASS");return 0;
}