#include <cstdio>
#include <cmath>
// Model of SummonPillars' scheduling: one pillar per elapsed interval, dead pillars retried, stop on terminate.
struct Pillar { bool alive; bool enabled; float angle; float x,y,z; };
static float AngleXY(float x,float y){ float a=(float)atan2(x,y)*0.15915494f; if(a<0.0f||a>=1.0f){a=(float)fmod((double)a,1.0); if(a<0.0f)a+=1.0f;} return a; }
static int Run(Pillar* p,long count,float interval,long fps,const long* frames,int nframes,int terminate_at,float cx,float cy,bool centre_alive,int* actions)
{
    long index=0,last=0; int i=0; int result;
    if(count==0)return 0;
    do{
        long frame=frames[i<nframes?i:nframes-1];
        if((float)((frame-last)*fps)>interval){
            last=frame;
            if(p[index].alive){
                p[index].enabled=true;
                if(centre_alive){
                    float dx=cx-p[index].x,dy=cy-p[index].y;
                    p[index].angle=AngleXY(dx,dy);
                    p[index].y=0.0f+0.1f;
                }
                ++index;
            }
        }
        ++*actions; result=(i==terminate_at)?1:0; ++i;
    }while(index<count&&result!=1);
    return (int)index;
}
int main()
{
    Pillar p[3]={{true,false,0,1,0,0},{true,false,0,0,1,0},{true,false,0,-1,0,0}};
    long frames[]={1,2,3,4,5,6,7,8,9,10};
    int actions=0;
    int n=Run(p,3,1.5f,1,frames,10,-1,0,0,true,&actions);
    if(n!=3||!p[0].enabled||!p[2].enabled||actions!=6){ std::printf("00973dc0_TEST FAIL\n"); return 1; }
    if(fabs(p[0].angle-0.75f)>1e-4f||fabs(p[1].angle-0.5f)>1e-4f||fabs(p[0].y-0.1f)>1e-6f){ std::printf("00973dc0_TEST FAIL\n"); return 1; }
    Pillar q[2]={{true,false,0,1,0,0},{true,false,0,2,0,0}};
    actions=0;
    n=Run(q,2,1.5f,1,frames,10,0,0,0,true,&actions);
    if(n!=0||actions!=1){ std::printf("00973dc0_TEST FAIL\n"); return 1; }
    actions=0;
    if(Run(q,0,1.5f,1,frames,10,-1,0,0,true,&actions)!=0||actions!=0){ std::printf("00973dc0_TEST FAIL\n"); return 1; }
    std::printf("00973dc0_TEST PASS\n");
    return 0;
}