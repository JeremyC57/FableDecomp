// 00c52840 AudioEnvironment::InitialiseReverb
// Flags: /O2 /Oy /W3
// Leaf: InitialiseReverb
// Layout: 27-dword acoustic defaults copied to receiver+8, property interface+0xe4; exact constants from retail. Manifest Resize label is propagated.
namespace N00c52840 {
struct ReverbProperties {float P0;float P1;float P2;float P3;float P4;float P5;int P6;int P7;float P8;float P9;float P10;float P11;int P12;float P13;int P14;float P15;float P16;float P17;float P18;float P19;float P20;float P21;float P22;int P23;int P24;float P25;int P26;__forceinline ReverbProperties(){P0=0.0f;P1=0.0f;P2=0.0f;P3=0.0f;P4=0.0f;P5=0.0f;P6=-1000;P7=-100;P8=0.0f;P9=1.49f;P10=0.83f;P11=1.0f;P12=-2602;P13=0.007f;P14=200;P15=0.011f;P16=7.5f;P17=1.0f;P18=1.0f;P19=0.25f;P20=0.0f;P21=0.25f;P22=0.0f;P23=5000;P24=250;P25=0.0f;P26=-5;}};

struct PropertyInterface {int Set(unsigned int,unsigned int,const void*,unsigned int,unsigned int*);};
struct AudioEnvironment {void* Vtable;unsigned int State;ReverbProperties Current;unsigned char Padding[0x70];PropertyInterface* Properties;int Probe();void Apply(unsigned int,unsigned int);int InitialiseReverb();};
int AudioEnvironment::InitialiseReverb(){
 ReverbProperties defaults;Current=defaults;
 int result=Probe();
 if(!result){unsigned int handle=0;int attenuation=-10000;Properties->Set(0,5,&attenuation,4,&handle);Apply(handle,0);return 0;}
 return result;
}
}

#include <cstdio>
#include <cstring>
namespace N00c52840 {

static int probe;static int sets;static int applies;
int AudioEnvironment::Probe(){return probe;}void AudioEnvironment::Apply(unsigned int h,unsigned int f){if(h==23&&f==0)++applies;}
int PropertyInterface::Set(unsigned int a,unsigned int b,const void* p,unsigned int size,unsigned int* out){if(a==0&&b==5&&size==4&&*(const int*)p==-10000)++sets;*out=23;return 0;}
}
int main(){using namespace N00c52840;AudioEnvironment e;PropertyInterface p;e.Properties=&p;probe=7;if(e.InitialiseReverb()!=7||sets)return 1;if(e.Current.P6!=-1000||e.Current.P9!=1.49f||e.Current.P23!=5000||e.Current.P26!=-5)return 2;probe=0;if(e.InitialiseReverb()!=0||sets!=1||applies!=1)return 3;std::printf("00c52840_TEST PASS\n");return 0;}