#include <stdio.h>
#include <string.h>
struct CSalSwBuffer { void* m_pvBuffer; unsigned objects,bytes;
 unsigned GetNumObjects()const{return objects;} unsigned GetObjectSize()const{return bytes;}
 void CopyChannel(unsigned int,unsigned int); };
void CSalSwBuffer::CopyChannel(unsigned int nDestChannel, unsigned int nSrcChannel)
{
    unsigned int channelSize=GetNumObjects()*GetObjectSize();
    memcpy((unsigned char*)m_pvBuffer+nDestChannel*channelSize,
           (const unsigned char*)m_pvBuffer+nSrcChannel*channelSize,channelSize);
}
int main(){
    unsigned char data[96],before[96];
    for(unsigned width=1;width<=3;++width)for(unsigned n=0;n<=8;++n)
    for(unsigned src=0;src<3;++src)for(unsigned dst=0;dst<3;++dst)if(src!=dst){
        for(unsigned j=0;j<96;++j) before[j]=data[j]=(unsigned char)j;
        CSalSwBuffer b={data,n,width};b.CopyChannel(dst,src);
        unsigned stride=n*width;
        for(unsigned j=0;j<96;++j){
            unsigned k=j>=dst*stride&&j<(dst+1)*stride?j-dst*stride+src*stride:j;
            if(data[j]!=before[k]) return 1;
        }
    }
    puts("00c5cf90_TEST PASS");return 0;
}
