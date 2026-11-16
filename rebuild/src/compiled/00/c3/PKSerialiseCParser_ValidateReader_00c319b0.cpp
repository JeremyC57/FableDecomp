// 00c319b0 PKSerialiseCParser::ValidateReader
// Flags: /EHsc
// Leaf: ?ValidateReader@PKSerialiseCParser@@QAEPAUIDataStream@@XZ
#define FABLE_RETAIL_PKSERIALISECPARSER_READER_PASS23
#define FABLE_RETAIL_CSTRING_BUILDERS
#define FABLE_RETAIL_CSTRING_ALLOCCOPY
#define FABLE_RETAIL_CSTRING_DEFAULT_CTOR
#include "engine/PKSerialiseCParser.h"
#include "engine/CString.h"
// Naming: ValidateReader is descriptive; original libpk symbols are unavailable.
IDataStream* PKSerialiseCParser::ValidateReader()
{
 if(!pReader) {
  {
   CString line;
   line << ".\\PKSerialiseCParser.cpp" << "(" << (long)11 << ") : " << "Null reader" << "\n";
   if(g_pfnAssertHook)g_pfnAssertHook(line.m_pchData);
  }
  CDebugBreak();
 }
 return pReader;
}