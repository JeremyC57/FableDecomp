// Leaf: ?SetTexturePointersValidForFrames@CGraphicDataBank@@QAEXK@Z
class CDataInputStream;
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICDATABANK_ORIGINAL_METHODS_PASS22
#include "engine/CGraphicDataBank.h"

void CGraphicDataBank::SetTexturePointersValidForFrames(unsigned long frames)
{
    TexturePointersValidForFrames = frames;
}

#include <cstdio>
#include <cstring>
#include <cassert>
int main(){unsigned long memory[(sizeof(CGraphicDataBank)+3)/4]; const unsigned long input[]={0,1,0xFFFFFFFFul,0xDEADBEEFul};for(int k=0;k<4;++k){std::memset(memory,0x55,sizeof(memory));((CGraphicDataBank*)memory)->SetTexturePointersValidForFrames(input[k]);for(unsigned j=0;j<sizeof(memory)/4;++j)assert(memory[j]==(j==0x270/4?input[k]:0x55555555ul));}std::puts("009fd120_TEST PASS");return 0;}