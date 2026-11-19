// Behaviour test for CVtable_012bedb8::RebuildGain (0x00C6EBE0): the row body over a portable model of libpk's FtoL (the x87 default
// rounding: nearest, ties to even; out of range -> 0x80000000) and a logging lock. Kills: the lock not taken / not released / the
// work outside it, the flag not stored, the records swapped (alternate -> previous +0x34, else current +0x30), both records written,
// the clamp at 0x7fff missing or wrong, the halving (gain >> 1) or the step (gain >> 7) wrong, and the scale (32767) wrong.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
static void check(bool v){if(!v){puts("FAIL");exit(1);}}
static int log_pos;static char log_buf[16];
static void EnterCriticalSection(void*){log_buf[log_pos++]='E';}
static void LeaveCriticalSection(void*){log_buf[log_pos++]='L';}
static long FtoL(float f){double v=(double)f;if(!(v>=-2147483648.0&&v<2147483647.5))return (long)0x80000000;
 double fl=floor(v);double d=v-fl;long r=(long)fl;if(d>0.5||(d==0.5&&(r&1)))++r;return r;}
struct SCriticalSection { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
struct CVtable_012beda4 { void* _vtable; void* _lockVtable; SCriticalSection m_lock; float m_volume; bool m_alternate; };
struct SGainRamp { unsigned short m_gain; short m_step; };
static int compute_calls;
struct CVtable_012bedb8 : CVtable_012beda4 {
    SGainRamp  m_first;
    SGainRamp  m_second;
    SGainRamp* m_pCurrent;
    SGainRamp* m_pPrevious;
    void ComputeGainRamp(SGainRamp* ramp);
    void RebuildGain(bool alternate);
};
void CVtable_012bedb8::ComputeGainRamp(SGainRamp* ramp)
{
    log_buf[log_pos++]='C';
    unsigned long level = FtoL(m_volume * 32767.0f);
    if (level > 0x7fff)
        level = 0x7fff;
    unsigned short gain = (unsigned short)((unsigned short)level >> 1);
    ramp->m_gain = gain;
    ramp->m_step = (short)(gain >> 7);
}
void CVtable_012bedb8::RebuildGain(bool alternate)
{
    EnterCriticalSection(&m_lock);
    m_alternate = alternate;
    if (alternate)
        ComputeGainRamp(m_pPrevious);
    else
        ComputeGainRamp(m_pCurrent);
    LeaveCriticalSection(&m_lock);
}
int main(){
 const float vol[]={0.0f,1.0f,0.5f,0.25f,0.001f,1.5f,2.0f,-0.5f,0.75f,1.0f/32767.0f,2.5f/32767.0f,3.5f/32767.0f,100.0f,1e10f};
 long cases=0;
 for(int a=0;a!=2;++a)for(int v=0;v!=(int)(sizeof vol/sizeof vol[0]);++v){
  CVtable_012bedb8 e;e.m_volume=vol[v];e.m_alternate=!a;e.m_first.m_gain=0x1111;e.m_first.m_step=0x2222;e.m_second.m_gain=0x3333;e.m_second.m_step=0x4444;
  e.m_pCurrent=&e.m_first;e.m_pPrevious=&e.m_second;log_pos=0;
  e.RebuildGain(a!=0);
  check(log_pos==3&&log_buf[0]=='E'&&log_buf[1]=='C'&&log_buf[2]=='L');
  check(e.m_alternate==(a!=0));
  unsigned long lv=(unsigned long)FtoL(vol[v]*32767.0f);if(lv>0x7fff)lv=0x7fff;unsigned short g=(unsigned short)(lv>>1);
  SGainRamp& hit=a?e.m_second:e.m_first;SGainRamp& miss=a?e.m_first:e.m_second;
  check(hit.m_gain==g&&hit.m_step==(short)(g>>7));
  check(a?(miss.m_gain==0x1111&&miss.m_step==0x2222):(miss.m_gain==0x3333&&miss.m_step==0x4444));
  ++cases;
 }
 // spot values: full volume -> 0x7fff >> 1 = 0x3fff, step 0x7f; ties round to even (2.5 -> 2 -> gain 1, 3.5 -> 4 -> gain 2)
 {CVtable_012bedb8 e;SGainRamp r;e.m_pCurrent=&r;e.m_pPrevious=&r;e.m_volume=1.0f;log_pos=0;e.RebuildGain(false);check(r.m_gain==0x3fff&&r.m_step==0x7f);
  e.m_volume=2.5f/32767.0f;log_pos=0;e.RebuildGain(true);check(r.m_gain==1);e.m_volume=3.5f/32767.0f;log_pos=0;e.RebuildGain(true);check(r.m_gain==2);
  e.m_volume=-0.5f;log_pos=0;e.RebuildGain(false);check(r.m_gain==0x3fff);}   // negative: the unsigned compare clamps
 check(cases==28);puts("00c6ebe0_TEST PASS");return 0;
}