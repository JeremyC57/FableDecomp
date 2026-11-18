#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __int64 long long
#endif
#define FABLE_RETAIL_STLP_VECTOR_C
#include "engine/CArray.h"
#define private public
#include "engine/CRangeCompressor.h"
#undef private
static unsigned int costs[5];
static unsigned char bytes[16];
static unsigned int read_column(const unsigned char *address, unsigned int width) {
    unsigned int value=0;
    for(unsigned int i=0;i<width;++i) value |= (unsigned int)address[i]<<(8*i);
    return value;
}
static bool inputs_ok;
long CRangeCompressor::CalcBestCompressionForBlock(const CArray<unsigned long>& values,long width,CScriptEntry& entry)
{
    entry.Flags=width;
    entry.BitCount=0;entry.RangeBias=0;entry.RangeShift=0;entry.ShuffleMask=0;entry.OrMask=0;
    if(values.size()) {
        const unsigned int start=(values[0]&255)-1;
        entry.RangeBias=start;
        if(start+width>8 || values.size()!=2 ||
           values[0]!=read_column(bytes+start,width) ||
           values[1]!=read_column(bytes+8+start,width)) inputs_ok=false;
    }
    return costs[width];
}
long CRangeCompressor::CalcCompressionScript(CArray<CScriptEntry>& script, const unsigned char* block_ptr, long element_count, long element_size, long block_size)
{
    long count = 0;
    if (block_size > 4) {
        long left = block_size;
        for (long n = 0; n < block_size; n += 4) {
            count += CalcCompressionScript(script, block_ptr + n, element_count, element_size, left > 4 ? 4 : left);
            left -= 4;
        }
        return count;
    }
    if (block_size == 3)
        return CalcCompressionScript(script, block_ptr + 2, element_count, element_size, 1)
             + CalcCompressionScript(script, block_ptr, element_count, element_size, 2);
    CArray<unsigned long> block_buffer(element_count);
    for (long n = 0; n < element_count; n++) {
        block_buffer[n] = block_size == 4 ? *(const fable_u32*)(block_ptr + n * element_size)
                        : block_size == 2 ? *(const unsigned short*)(block_ptr + n * element_size)
                        : *(block_ptr + n * element_size);
    }
    CScriptEntry script_entry;
    long count1 = CalcBestCompressionForBlock(block_buffer, block_size, script_entry);
    if (block_size == 1) {
        script.push_back(script_entry);
        return count1;
    }
    CArray<CScriptEntry> sub_script;
    long count2 = CalcCompressionScript(sub_script, block_ptr, element_count, element_size, block_size / 2);
    count2 += CalcCompressionScript(sub_script, block_ptr + block_size / 2, element_count, element_size, block_size / 2);
    if (count1 <= count2) {
        script.push_back(script_entry);
        return count1;
    }
    for (long n = 0; n < sub_script.size(); n++)
        script.push_back(sub_script[n]);
    return count2;
}

static bool scenario(unsigned int width,int n,unsigned int c1,unsigned int c2,unsigned int c4,
                     long score,const char *columns) {
    CRangeCompressor compressor;
    CArray<CRangeCompressor::CScriptEntry> script;
    costs[1]=c1;costs[2]=c2;costs[4]=c4;inputs_ok=true;
    if(compressor.CalcCompressionScript(script,bytes,n,8,width)!=score || !inputs_ok) return false;
    if(script.size()!=strlen(columns)/2) return false;
    for(unsigned int i=0;i<script.size();++i) {
        if(script[i].Flags!=(unsigned)(columns[2*i]-'0')) return false;
        if(n && script[i].RangeBias!=(unsigned)(columns[2*i+1]-'0')) return false;
    }
    return true;
}
int main() {
    for(unsigned int i=0;i<sizeof(bytes);++i) bytes[i]=(unsigned char)(i+1);
    if(!scenario(1,0,2,3,4,2,"10"))return 1;
    if(!scenario(1,2,2,3,4,2,"10"))return 2;
    if(!scenario(2,2,2,4,9,4,"20"))return 3;
    if(!scenario(2,2,1,4,9,2,"1011"))return 4;
    if(!scenario(4,2,2,3,9,6,"2022"))return 5;
    if(!scenario(4,2,2,3,5,5,"40"))return 6;
    if(!scenario(5,2,2,3,5,7,"4014"))return 7;
    if(!scenario(8,2,2,3,5,10,"4044"))return 8;
    // The two calls in a sum have no C++03 evaluation-order guarantee.
    if(!scenario(3,2,2,3,5,5,"1220") && !scenario(3,2,2,3,5,5,"2012"))return 9;
    if(!scenario(7,2,2,3,5,10,"401624") && !scenario(7,2,2,3,5,10,"402416"))return 10;
    puts("00f3abf0_TEST PASS");return 0;
}
