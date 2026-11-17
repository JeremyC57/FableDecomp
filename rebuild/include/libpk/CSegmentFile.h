#ifndef FABLE_LIBPK_CSEGMENTFILE_H
#define FABLE_LIBPK_CSEGMENTFILE_H
#ifdef FABLE_RETAIL_CSEGMENTFILE_PASS23
#define FABLE_RETAIL_PKCBUFFEREDREADER_POLL_PASS23
#define FABLE_RETAIL_LIBPK_EVENT_SCRIPT_CTOR_PASS23
#define FABLE_RETAIL_CSTRING_BUILDERS
#define FABLE_RETAIL_CSTRING_ALLOCCOPY
#define FABLE_RETAIL_CSTRING_DEFAULT_CTOR
#include "libpk/PKCBufferedReader.h"
#include "libpk/PKCTRRedblackTree.h"
#include "engine/CString.h"
#include "rebuild_abi.h"
// Descriptive names: original libpk symbols unavailable; see NAMING_EVIDENCE.
// Layout: native C3AAC0 constructs PKCBufferedReader at0; C3AA50/C3A9E0
// access range start+70,length+74,position+78; C3A9D0 destroys that first member.
class CBufferedRangeReader {
public:
 CBufferedRangeReader(void* source,unsigned long size);
 ~CBufferedRangeReader();
 long SetRange(unsigned long first,unsigned long count);
#ifdef FABLE_RETAIL_CBUFFEREDRANGEREADER_RESET
 // NF56 opt-in (row 0x00C3AA50 SetRange): the range reset out of line at 0x00C3A9C0 (zeroes RangeStart / RangeLength / ReadOffset,
 // +0x70/+0x74/+0x78). Descriptive name (no libpk symbol): the landed self row's method name Reset (there on the address-named stand-in
 // CReaderBase_00c4b300; NAME_FIXES). Declaration only.
 void Reset();
#endif
 long Read(void* buffer,long count);
 unsigned long GetCount() const;
 PKCBufferedReader Reader;
 unsigned long RangeStart,RangeLength,ReadOffset;
};
// Layout: allocated12bytes byC2DA20; all four C2DF40..DFBF comparators use
// CString at+8. Offset/Size also read by C2D9C0/C2DA10 segment-selection helpers.
struct CSegment {
 unsigned long Offset;
 long Size;
 CString Name;
};
// Layout: native012b47d0 has the shared16-slot list prefix and two keyed-list
// methods; C2E030 destroys the40-byte tree at+8 then the true list root.
class CSegmentList:public PKCTRList<CSegment> {
public:
 CSegmentList():PKCTRList<CSegment>(&Tree),Tree(0){}
#ifdef FABLE_RETAIL_CSEGMENTLIST_CTOR_ALLOCATOR
 // Native C2DFE0 (ret 4): impl4=&Tree8, vtable012b47d0, tree constructed C194F0 from the argument.
 CSegmentList(PKCTRAllocator* allocator);
#endif
#ifdef FABLE_RETAIL_CSEGMENTLIST_DTOR_OUTOFLINE
 // Native C2E030: out-of-line copy of the inline destructor (??_G C2E080 calls it).
 virtual ~CSegmentList();
#else
 virtual ~CSegmentList(){}
#endif
 virtual int IsEqual(const void*,const void*)const;
 virtual int IsLess(const void*,const void*)const;
 virtual int IsKeyEqual(const void*,const void*)const;
 virtual int IsKeyLess(const void*,const void*)const;
 virtual void TransferAllFrom(PKCTRList<CSegment>*);
 virtual CSegment* FindByKey(const CString&);
 PKCTRRedblackTree Tree;
};
// Layout: C2DA20/C2DBE0 construct/destroy reader0,error7c,list80/tree88.
// Reader is represented by composition: no invented virtual slots or suppressed
// vtable stores. Native wrapper constructor/destructor act on its first member.
class CSegmentFile {
public:
 CSegmentFile(void* source,unsigned long size);
 ~CSegmentFile();
#ifdef FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
 // Native C2D9C0/C2DA10/C2DA00; bank wrapper opt-in only.
 long OpenSegment(const CString& name);
 unsigned long GetPosition();
 int Read(void* buffer,int size);
#endif
 CBufferedRangeReader Reader;
 long ErrorCount;
 CSegmentList Segments;
};
FABLE_STATIC_ASSERT_AT(58, sizeof(PKCBufferedReader)==0x70);
FABLE_STATIC_ASSERT_AT(59, sizeof(CBufferedRangeReader)==0x7c);
FABLE_STATIC_ASSERT_AT(60, sizeof(CSegment)==12);
FABLE_STATIC_ASSERT_AT(61, sizeof(CSegmentList)==48);
FABLE_STATIC_ASSERT_AT(62, sizeof(CSegmentFile)==0xb0);
#endif
#endif
