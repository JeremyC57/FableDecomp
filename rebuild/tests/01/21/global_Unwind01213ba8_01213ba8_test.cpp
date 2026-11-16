// Standalone source and shared-object parser behaviour test.
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
// 01213ba8 CSegmentFile::CSegmentFile parent00c2da20
// Leaf: ??0CSegmentFile@@QAE@PAXK@Z
// Flags: /EHsc /GS
#define FABLE_RETAIL_CSEGMENTFILE_PASS23
#include "libpk/CSegmentFile.h"
#include <string.h>
CSegmentFile::CSegmentFile(void* source,unsigned long chunkSize)
    : Reader(source, chunkSize), ErrorCount(0)
{
    if (Reader.SetRange(0, 8) < 0) {
        ++ErrorCount;
        Reader.SetRange(0, 0);
        return;
    }
    char signature[9];
    signature[8] = 0;
    if (Reader.Read(signature, 8) < 8) {
        ++ErrorCount;
        Reader.SetRange(0, 0);
        return;
    }
    if (strcmp(signature, "LiOnHeAd") != 0) {
        ++ErrorCount;
        Reader.SetRange(0, 0);
        return;
    }
    unsigned long pos = 8;
    if (Reader.GetCount() > pos) {
        do {
            if (Reader.SetRange(pos, 0x24) < 0) {
                ++ErrorCount;
                break;
            }
            char name[0x21];
            name[0x20] = 0;
            if (Reader.Read(name, 0x20) < 0x20) {
                ++ErrorCount;
                Reader.SetRange(0, 0);
                return;
            }
            long segmentSize;
            if (Reader.Read(&segmentSize, 4) < 4) {
                ++ErrorCount;
                Reader.SetRange(0, 0);
                return;
            }
            if (pos + 0x24 + segmentSize > Reader.GetCount()) {
                ++ErrorCount;
                break;
            }
            CSegment* segment = new CSegment;
            segment->Offset = pos + 0x24;
            segment->Size = segmentSize;
            segment->Name = name;
            Segments.Add(segment);
            pos = segment->Offset + segment->Size;
        } while (pos < Reader.GetCount());
    }
    Reader.SetRange(0, 0);
}

#undef private
#undef protected
static char Empty[1]={0};char* _afxPchNil=Empty;
PFNASSERTHOOK g_pfnAssertHook=0;CDebugBreak::CDebugBreak(){assert(false);}
static unsigned char Input[512];static unsigned InputSize,RangeCalls,ReadCalls,FailRange,FailRead,Added,ReaderDtors,TreeDtors;
static unsigned ExpectedChunk,ThrowPoint;static CSegment* Entries[8];static CSegmentFile* Active;
static unsigned Decode32(const unsigned char*p){return (unsigned)p[0]|((unsigned)p[1]<<8)|((unsigned)p[2]<<16)|((unsigned)p[3]<<24);}
static void Encode32(unsigned char*p,unsigned n){for(unsigned i=0;i<4;++i)p[i]=(unsigned char)(n>>(8*i));}
static void MaybeThrow(unsigned point){
#ifndef _MSC_VER
 if(ThrowPoint==point)throw point;
#else
 assert(!ThrowPoint);
#endif
}
PKCCriticalSection::PKCCriticalSection(){memset(m_section,0,sizeof(m_section));}PKCCriticalSection::~PKCCriticalSection(){}
void PKCCriticalSection::Enter(){}void PKCCriticalSection::Leave(){}
PKCSemaphore::PKCSemaphore(long,long):m_hSemaphore(0){}PKCSemaphore::~PKCSemaphore(){}
void PKCSemaphore::Wait(){}void PKCSemaphore::Release(){}
PKCMailbox::PKCMailbox(void*q,long n):m_pQueue(q),m_semaphore(0,n){}
PKCBufferedReader::PKCBufferedReader(void* source,const PKCBufferedReaderParams&p):m_mailbox(0,(long)(p.ExtraBuffers+p.BaseBuffers)),m_outstanding(0),m_completedReadOffset(0){assert(source==Input&&p.Granularity==ExpectedChunk&&p.ExtraBuffers==0&&p.BaseBuffers==2);memset(_unknown00,0,sizeof(_unknown00));}
CBufferedRangeReader::CBufferedRangeReader(void* source,unsigned long chunkSize):Reader(source,PKCBufferedReaderParams(chunkSize,0,2)),RangeStart(0),RangeLength(0),ReadOffset(0){assert((void*)this==(void*)Active);MaybeThrow(1);}
CBufferedRangeReader::~CBufferedRangeReader(){++ReaderDtors;}
unsigned long CBufferedRangeReader::GetCount()const{return InputSize;}
long CBufferedRangeReader::SetRange(unsigned long first,unsigned long count){++RangeCalls;if(count&&RangeCalls==FailRange)return -1;if(first>InputSize||count>InputSize-first)return -1;RangeStart=first;RangeLength=count;ReadOffset=0;return 0;}
long CBufferedRangeReader::Read(void* dest,long count){++ReadCalls;MaybeThrow(3);assert(count>=0);if(ReadCalls==FailRead)return -1;unsigned n=(unsigned)count;if(n>RangeLength-ReadOffset)n=(unsigned)(RangeLength-ReadOffset);assert(RangeStart+ReadOffset+n<=InputSize);if(count==4&&n==4){/* Logical wire DWORD to native long: the fixture remains valid when host long is64bits. */unsigned value=Decode32(Input+RangeStart+ReadOffset);*(long*)dest=(value&0x80000000U)?(long)value-2147483647L-2147483647L-2:(long)value;}else memcpy(dest,Input+RangeStart+ReadOffset,n);ReadOffset+=n;return n;}
const CString& CString::operator=(const char* text){if(m_pchData!=Empty)free(m_pchData);unsigned n=(unsigned)strlen(text);m_pchData=(char*)malloc(n+1);assert(m_pchData);memcpy(m_pchData,text,n+1);return *this;}
CString::~CString(){if(m_pchData!=Empty)free(m_pchData);}
template<class T> void PKCTRList<T>::Add(T* p){assert(Added<8&&this==&Active->Segments);Entries[Added++]=p;}
template<class T> void PKCTRList<T>::Remove(T*){assert(false);}
template<class T> void PKCTRList<T>::MorphToArray(){assert(false);}
template<class T> void PKCTRList<T>::MorphToTree(){assert(false);}
template<class T> T* PKCTRList<T>::Find(const T*){assert(false);return 0;}
template<class T> void PKCTRList<T>::ForEach(void*){assert(false);}
template<class T> T* PKCTRList<T>::GetFirst(){assert(false);return 0;}
template<class T> T* PKCTRList<T>::GetLast(){assert(false);return 0;}
template<class T> T* PKCTRList<T>::GetItemByIndex(long){assert(false);return 0;}
template<class T> void PKCTRList<T>::Reserve(long){assert(false);}
template<class T> long PKCTRList<T>::GetCount(){assert(false);return 0;}
#define CONTAINER(K) \
 int K::IsArray(){return 0;} int K::IsTree(){return 1;} \
 void K::MorphToArray(void*){assert(false);} void K::MorphToTree(void*){assert(false);} \
 void K::Reserve(long){assert(false);} void K::Add(void*,void*){assert(false);} void K::Remove(void*,void*){assert(false);} \
 void* K::Find(void*,const void*){assert(false);return 0;} void* K::FindByKey(void*,const void*){assert(false);return 0;} \
 void* K::GetItemByIndex(long){assert(false);return 0;} long K::GetCount(){return 0;} \
 void* K::GetFirst(){return 0;} void* K::GetLast(){return 0;} void K::ForEach(void*){assert(false);}
CONTAINER(PKCContainer_00c17b70)
CONTAINER(PKCTRRedblackTree)
PKCContainer_00c17b70::~PKCContainer_00c17b70(){}
PKCTRRedblackTree::PKCTRRedblackTree(PKCTRAllocator*p):PAllocator(p),POwnedAllocator(0),Root(&Sentinel),Count(0){assert(!p);Sentinel.Left=Sentinel.Right=&Sentinel;Sentinel.Parent=0;Sentinel.Colour=0;Sentinel.Key=0;MaybeThrow(2);}
PKCTRRedblackTree::~PKCTRRedblackTree(){++TreeDtors;}
int CSegmentList::IsEqual(const void*a,const void*b)const{return strcmp(((const CSegment*)a)->Name.m_pchData,((const CSegment*)b)->Name.m_pchData)==0;}
int CSegmentList::IsLess(const void*a,const void*b)const{return strcmp(((const CSegment*)a)->Name.m_pchData,((const CSegment*)b)->Name.m_pchData)<0;}
int CSegmentList::IsKeyEqual(const void*a,const void*b)const{return strcmp(((const CString*)a)->m_pchData,((const CSegment*)b)->Name.m_pchData)==0;}
int CSegmentList::IsKeyLess(const void*a,const void*b)const{return strcmp(((const CString*)a)->m_pchData,((const CSegment*)b)->Name.m_pchData)<0;}
void CSegmentList::TransferAllFrom(PKCTRList<CSegment>*){assert(false);}CSegment* CSegmentList::FindByKey(const CString&){assert(false);return 0;}
CSegmentFile::~CSegmentFile(){for(unsigned i=0;i<Added;++i)delete Entries[i];}
static void Build(unsigned mode){memset(Input,0,sizeof(Input));memcpy(Input,"LiOnHeAd",8);InputSize=8;if(mode==0){InputSize=0;return;}if(mode==1){InputSize=7;return;}if(mode==2){Input[0]='X';return;}if(mode==3)return;unsigned records=mode>=6?2:1,pos=8;for(unsigned n=0;n<records;++n){unsigned payload=mode==4?0:4+4*n;for(unsigned k=0;k<32;++k)Input[pos+k]=(unsigned char)('A'+n);if(mode!=5)Input[pos+(n?31:0)]=0;Encode32(Input+pos+32,payload);pos+=36;for(unsigned j=0;j<payload;++j)Input[pos+j]=(unsigned char)(n+j);pos+=payload;}InputSize=pos;if(mode==7)--InputSize;if(mode==8)Encode32(Input+8+32,500);}
struct Expected{unsigned Count,Errors,Range,Read,Offset[8],Size[8];char Name[8][33];};
static Expected Oracle(){Expected e;memset(&e,0,sizeof(e));++e.Range;if(FailRange==e.Range||InputSize<8){e.Errors=1;++e.Range;return e;}++e.Read;if(FailRead==e.Read){e.Errors=1;++e.Range;return e;}if(memcmp(Input,"LiOnHeAd",8)){e.Errors=1;++e.Range;return e;}unsigned cursor=8;while(cursor<InputSize){++e.Range;if(FailRange==e.Range||InputSize-cursor<36){e.Errors=1;break;}++e.Read;if(FailRead==e.Read){e.Errors=1;break;}++e.Read;if(FailRead==e.Read){e.Errors=1;break;}unsigned length=Decode32(Input+cursor+32);if(length>InputSize-cursor-36){e.Errors=1;break;}e.Offset[e.Count]=cursor+36;e.Size[e.Count]=length;memcpy(e.Name[e.Count],Input+cursor,32);e.Name[e.Count][32]=0;++e.Count;cursor+=36+length;}++e.Range;return e;}
int main(){unsigned cases=0;ThrowPoint=0;const unsigned chunks[]={1,2048,8192};for(unsigned mode=0;mode<9;++mode)for(unsigned fail=0;fail<5;++fail)for(unsigned read=0;read<8;++read)for(unsigned chunk=0;chunk<3;++chunk){Build(mode);FailRange=fail;FailRead=read;ExpectedChunk=chunks[chunk];Expected expected=Oracle();RangeCalls=ReadCalls=Added=ReaderDtors=TreeDtors=0;union Storage{double align;unsigned char bytes[sizeof(CSegmentFile)];}storage;memset(storage.bytes,0xa5,sizeof(storage.bytes));Active=(CSegmentFile*)(void*)storage.bytes;CSegmentFile* f=::new((void*)storage.bytes)CSegmentFile(Input,ExpectedChunk);assert(f==Active&&f->ErrorCount==(long)expected.Errors&&Added==expected.Count&&RangeCalls==expected.Range&&ReadCalls==expected.Read);assert(!f->Reader.RangeStart&&!f->Reader.RangeLength&&!f->Reader.ReadOffset);for(unsigned k=0;k<Added;++k){assert(Entries[k]->Offset==expected.Offset[k]&&Entries[k]->Size==(long)expected.Size[k]);assert(strcmp(Entries[k]->Name.m_pchData,expected.Name[k])==0);}assert(!ReaderDtors&&!TreeDtors);
#ifdef _MSC_VER
assert(sizeof(CSegmentFile)==0xb0&&((unsigned char*)&f->Segments-(unsigned char*)f)==0x80);
#endif
f->~CSegmentFile();assert(ReaderDtors==1&&TreeDtors==1);++cases;}
assert(cases==1080);
#ifndef _MSC_VER
for(unsigned point=1;point<=3;++point){Build(6);FailRange=FailRead=0;ThrowPoint=point;ExpectedChunk=2048;RangeCalls=ReadCalls=Added=ReaderDtors=TreeDtors=0;union Storage{double align;unsigned char bytes[sizeof(CSegmentFile)];}storage;Active=(CSegmentFile*)(void*)storage.bytes;bool caught=false;try{::new((void*)storage.bytes)CSegmentFile(Input,ExpectedChunk);}catch(unsigned value){assert(value==point);caught=true;}assert(caught&&Added==0&&ReaderDtors==(point>=2?1U:0U)&&TreeDtors==(point>=3?1U:0U));}
#endif
puts("01213ba8_TEST PASS");return 0;}