// 00c4b160 PKCBufferedReader::PollReadCompletion
// Flags: /EHsc
// Leaf: ?PollReadCompletion@PKCBufferedReader@@QAE_NXZ
#define FABLE_RETAIL_PKCBUFFEREDREADER_POLL_PASS23
#define FABLE_RETAIL_CSTRING_BUILDERS
#define FABLE_RETAIL_CSTRING_ALLOCCOPY
#define FABLE_RETAIL_CSTRING_DEFAULT_CTOR
#include "libpk/PKCBufferedReader.h"
#include "engine/CString.h"
// Naming: PollReadCompletion is descriptive; libpk original symbols unavailable.
bool PKCBufferedReader::PollReadCompletion()
{
 while(m_outstanding!=0) {
  if(m_mailbox.GetCount()==0)break;
  bool more;
  ReturnBuffer(&more);
  if(!more) {
   {
    CString line;
    line << ".\\PKCBufferedReader.cpp" << "(" << (long)482 << ") : " << "Error: Should not be blocked here..." << "\n";
    if(g_pfnAssertHook)g_pfnAssertHook(line.m_pchData);
   }
   CDebugBreak();
  }
  Refill();
 }
 return m_outstanding==0;
}