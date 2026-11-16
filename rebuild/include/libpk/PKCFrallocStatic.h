// PKCFrallocStatic<T> -- libpk's fixed pool (new shared header, chunk 1023; the model of the landed accessor rows 0x00C13700 /
// 0x00C13620): the element count at +4 and the element block at +0x10; Get(index) is the out-of-line bounds-checked accessor
// (0x00C13700 for CPlaybackSlot, 0x00C13620 for CSoundEvent). The class name is the warning's own header file
// "libpk\PKCFrallocStatic.h"; member spellings are the landed accessor's.
#ifndef FABLE_LIBPK_PKCFRALLOCSTATIC_H
#define FABLE_LIBPK_PKCFRALLOCSTATIC_H
template<class T> class PKCFrallocStatic {
public:
    long m_0;
    unsigned long m_count;                      // +4
    long m_8;
    long m_c;
    T* m_pData;                                 // +0x10
    T* Get(unsigned long index);                // 0x00C13700 <CPlaybackSlot>
};
#ifdef FABLE_RETAIL_LISTENER_POOL_BODY_PASS23
#define FABLE_RETAIL_CSTRING_BUILDERS
#define FABLE_RETAIL_CSTRING_ALLOCCOPY
#define FABLE_RETAIL_CSTRING_DEFAULT_CTOR
#include "engine/CString.h"
template<class T> T*PKCFrallocStatic<T>::Get(unsigned long index){
 if(index>=m_count){
  {CString line;
   line << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCFrallocStatic.h" << "(" << (long)31 << ") : " << "Error: Fralloc overflow" << "\n";
   if(g_pfnAssertHook)g_pfnAssertHook(line.m_pchData);
  }
  CDebugBreak();
 }
 return &m_pData[index];
}
#endif
#endif

