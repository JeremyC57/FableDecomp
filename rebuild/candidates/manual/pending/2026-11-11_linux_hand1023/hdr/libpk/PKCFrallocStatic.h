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
#endif
