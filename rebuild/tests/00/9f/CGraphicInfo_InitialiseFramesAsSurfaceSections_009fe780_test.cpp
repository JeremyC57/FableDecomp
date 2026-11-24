#include <stdio.h>
struct CTexture {};
template<class T> struct CCountedPointer {T* p;};
struct CManagedTexture {};
static int made, destroyed;
struct CGraphicFrame {
    CGraphicFrame():texture(0){++made;}
    ~CGraphicFrame(){++destroyed;}
    const CCountedPointer<CTexture>* texture;
    float x,y; long w,h; bool alpha,boolean_alpha;
    bool InitialiseAsSurfaceSection(const CCountedPointer<CTexture>& t,float u,float v,long width,long height,bool a,bool b) {
        texture=&t;x=u;y=v;w=width;h=height;alpha=a;boolean_alpha=b;return true;
    }
};
struct CGraphicInfoFrame {
    float x,y;long w,h;bool alpha,boolean_alpha;signed char mip;
    const CManagedTexture* man;const CCountedPointer<CTexture>* texture;
    void InitialiseFrame(CGraphicFrame& f,CManagedTexture& m,signed char level) {
        x=f.x;y=f.y;w=f.w;h=f.h;alpha=f.alpha;boolean_alpha=f.boolean_alpha;
        mip=level;man=&m;texture=f.texture;
    }
};
struct CGraphicInfo {
    struct {unsigned short FrameWidth,FrameHeight,Width,Height,FrameCount;} Header;
    CGraphicInfoFrame* Frames;
    void InitialiseFramesAsSurfaceSections(CManagedTexture&,signed char,CCountedPointer<CTexture>&,bool,bool);
};
void CGraphicInfo::InitialiseFramesAsSurfaceSections(CManagedTexture& managed_texture,
    signed char mipmap_level, CCountedPointer<CTexture>& texture, bool is_alpha, bool is_boolean_alpha)
{
    long tlx = 0;
    long tly = 0;
    long frame_width = Header.FrameWidth;
    long frame_height = Header.FrameHeight;
    long width = Header.Width;
    long height = Header.Height;
    for (unsigned long c_frame = 0; c_frame < Header.FrameCount; ++c_frame) {
        if (tlx + frame_width > width) {
            tlx -= width;
            tly += frame_height;
        }
        CGraphicFrame frame;
        frame.InitialiseAsSurfaceSection(texture, (float)tlx / width, (float)tly / height,
            frame_width, frame_height, is_alpha, is_boolean_alpha);
        CGraphicInfoFrame& info_frame = Frames[c_frame];
        info_frame.InitialiseFrame(frame, managed_texture, mipmap_level);
        tlx += frame_width;
    }
}

int main() {
    CManagedTexture managed;CTexture texture;CCountedPointer<CTexture> ptr={&texture};
    const float xs[]={0,0.5f,0,0.5f,0},ys[]={0,0,0.25f,0.25f,0.5f};
    for(unsigned flags=0;flags<4;++flags)for(unsigned count=0;count<=5;++count) {
        CGraphicInfoFrame frames[5]={0};CGraphicInfo info={{4,2,8,8,(unsigned short)count},frames};
        made=destroyed=0;info.InitialiseFramesAsSurfaceSections(managed,-2,ptr,(flags&1)!=0,(flags&2)!=0);
        if(made!=(int)count||destroyed!=(int)count)return 1;
        for(unsigned i=0;i<count;++i) {
            const CGraphicInfoFrame& f=frames[i];
            if(f.x!=xs[i]||f.y!=ys[i]||f.w!=4||f.h!=2||f.mip!=-2||
               f.man!=&managed||f.texture!=&ptr||f.alpha!=((flags&1)!=0)||f.boolean_alpha!=((flags&2)!=0))return 1;
        }
    }
    puts("009fe780_TEST PASS");
}