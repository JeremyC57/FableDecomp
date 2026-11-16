// 00c4aeb0 PKCBufferedReader::WaitForSlot
// Flags: /EHsc
// Leaf: ?WaitForSlot@PKCBufferedReader@@QAE_NK@Z
#define FABLE_RETAIL_PKCBUFFEREDREADER_RETURN_CONTEXT_PASS23
#define FABLE_RETAIL_PKCBUFFEREDREADER_WAIT_PASS23
#define FABLE_RETAIL_PKCBUFFEREDREADER_POLL_PASS23
#define FABLE_RETAIL_CSTRING_BUILDERS
#define FABLE_RETAIL_CSTRING_ALLOCCOPY
#define FABLE_RETAIL_CSTRING_DEFAULT_CTOR
#include "libpk/PKCBufferedReader.h"
#include "engine/CString.h"
// Inline-budget: native00c4af85 calls the actual13-byte CDebugBreak constructor.
// Its real imported DebugBreak body stays visible but is not expanded here.
#if defined(_MSC_VER)
extern "C" __declspec(dllimport) void __stdcall DebugBreak();
#pragma auto_inline(off)
#else
extern "C" void DebugBreak();
#endif
CDebugBreak::CDebugBreak(){DebugBreak();}
#if defined(_MSC_VER)
#pragma auto_inline(on)
#endif

// Complete genuine native C4AC70 context: assertion traces, optional ready out-flag,
// blocking mailbox result, modulo slot-owner clearing and outstanding decrement.
// This existing helper is context only; it is not an additional reserved row.
void PKCBufferedReader::ReturnBuffer(bool* more)
{
 if(m_outstanding==0){
  {CString line;line << ".\\PKCBufferedReader.cpp" << "(" << (long)56 << ") : " << "Error: There are no requests pending!" << "\n";
   if(g_pfnAssertHook)g_pfnAssertHook(line.m_pchData);}
  CDebugBreak();
 }
 if(more)*more=m_mailbox.GetCount()!=0;
 unsigned long request=m_mailbox.GetResult();
 if(request>=m_slotCount){
  {CString line;line << ".\\PKCBufferedReader.cpp" << "(" << (long)62 << ") : " << "Error: returned buffer out of scope" << "\n";
   if(g_pfnAssertHook)g_pfnAssertHook(line.m_pchData);}
  CDebugBreak();
 }
 if(m_slotOwners[request]!=request){
  {CString line;line << ".\\PKCBufferedReader.cpp" << "(" << (long)63 << ") : " << "Error: Request ID out of sync" << "\n";
   if(g_pfnAssertHook)g_pfnAssertHook(line.m_pchData);}
  CDebugBreak();
 }
 for(unsigned long i=0;i<m_slotCount;++i){
  unsigned char& owner=m_slotOwners[(i+request)%m_slotCount];
  if(owner==request)owner=0xff;else i=m_slotCount;
 }
 --m_outstanding;
}

// Naming: descriptive libpk method/locals; actual layout and ABI are documented.
bool PKCBufferedReader::WaitForSlot(unsigned long slot)
{
 const unsigned long slotCount=m_slotCount;
 bool immediate=true;
 if(slot>=slotCount) {
  {
   CString line;
   line << ".\\PKCBufferedReader.cpp" << "(" << (long)84 << ") : " << "Error: Request number (" << slot << ") out of scope (" << slotCount << ")" << "\n";
   if(g_pfnAssertHook)g_pfnAssertHook(line.m_pchData);
  }
  CDebugBreak();
 }
 {
 bool more;
 while(m_slotOwners[slot]!=0xff) {
  ReturnBuffer(&more);
  if(!more)immediate=false;
 }
 }
 return immediate;
}