// CGraphicDataBank::CloseGraphics() -- free the graphic list and the graphic frame buffer, then close the bank file
// Leaf: ?CloseGraphics@CGraphicDataBank@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FEB80, 61 bytes, thiscall, tail `jmp CBankFile::Close` (no direct callers: VC7.1
//   inlined every call; this is the TU's out-of-line copy).
// Identity (proven): Ego_r and FableWin both define the private ?CloseGraphics@CGraphicDataBank@@AAEXXZ (Ego_r 0x00B5CE10, 0x50
//   bytes, between ??0CGraphicDataBank 0x00B5CCB0 and ??1 0x00B5D260; FableWin 0x03117BB0 right after Close); retail keeps the same
//   order ??0 0x009FEA20 < 0x009FEB80 < ??1 0x009FEBC0 < Close 0x009FECE0, and both the landed ~CGraphicDataBank 0x009FEBC0 and
//   Close 0x009FECE0 (the byte twin) contain this exact body inline. The catalog label "CGraphicDataBank::Close" is the twin's name.
// Callees: std::vector<CGraphicInfo>::erase(iterator, iterator) 0x00A00E60 and std::vector<CGraphicInfoFrame>::erase 0x00A00ED0 (the
//   STLport spellings bound by the landed resize rows 0x00A01790 / 0x00A016F0; result dropped), CBankFile::Close 0x009CE360
//   (slot 5 of CBankFile's vtable 0x0129B54C, ?Close@CBankFile@@UAEXXZ), CBankFileAsync::Close 0x009D58A0 (landed, UAE).
// Layout (retail, the landed ctor 0x009FEA20 / dtor 0x009FEBC0): CGraphicDataBank : CBankFileAsync (0x164) , CResourceBank (+0x164,
//   0x7C); GraphicList +0x1E0 and GraphicFrameBuffer +0x1EC are Lionhead CArray<T> (= STLport vector<T>, 12 bytes), Initialised +0x1F8.
//   engine/CBankFile.h is on the engine-header hold (flattened vptr) and engine/CGraphicDataBank.h exists only under _quarantine, so
//   the bank hierarchy is declared here (RTTI bases CBankFileAsync : CBankFile : CBaseClassNonCopyable, CResourceBank :
//   CBaseClassNonCopyable; vtable slots 1-5 with the Ego_r spellings, as the landed CBankFileAsync::Close 0x009D58A0).
// Shape: CloseGraphics is a plain private member defined above Close in the same TU; VC7.1 keeps its out-of-line copy (0x009FEB80)
//   and expands it inside Close (0x009FECE0) and ~CGraphicDataBank (0x009FEBC0): two erase(begin, end) clears, the qualified
//   CBankFile::Close() call (a tail jump in the out-of-line copy), then Close's own qualified CBankFileAsync::Close() tail.
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x0
    _Tp* _M_finish;                                             // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x8
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    iterator erase(iterator __first, iterator __last);         // out of line (0x00A00E60 / 0x00A00ED0)
    void clear() { erase(begin(), end()); }
};
}   // namespace std

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };   // Lionhead CArray over the STLport vector

class CGraphicDataBank;
class CGraphicInfoFrame;
class CGraphicInfo {                                            // 44 bytes in retail (Ego_r 44; the landed resize 0x00A01790)
public:
    unsigned char      Header[0x1c];                            // +0x00 CGraphicHeader
    long               TextureType;                             // +0x1c ETextureType
    CGraphicDataBank*  Bank;                                    // +0x20
    long               Index;                                   // +0x24
    CGraphicInfoFrame* Frames;                                  // +0x28
};
class CGraphicInfoFrame : public CBaseClass {                   // RTTI CGraphicInfoFrame : CBaseClass, vtable 0x0129C878, 16 bytes
public:
    unsigned short FrameNum;                                    // +0x04
    signed char    SmallMipmapLevel;                            // +0x06
    void*          Data;                                        // +0x08 CIVCountedPointer<CGraphicInfoFrame::CFrameData>
    CGraphicInfo*  GraphicInfo;                                 // +0x0c
};
class CCharString;
class CWideString;

class CBankFile : public CBaseClassNonCopyable {                // vtable 0x0129B54C, retail 0x110 bytes
public:
    virtual bool OpenReadOnly(const CCharString& name, unsigned long flags);                          // slot 1
    virtual void OpenReadWrite(const CCharString& name, unsigned long flags, unsigned long alignment); // slot 2
    virtual bool OpenFileReadOnly(const CWideString& name, unsigned long flags);                      // slot 3
    virtual void OpenFileReadWrite(const CWideString& name, unsigned long flags, unsigned long alignment); // slot 4
    virtual void Close();                                       // slot 5 0x009CE360
    unsigned char _body04[0x110 - 0x04];                        // Size .. ChecksumCache (engine/CBankFile.h member list)
};

class CBankFileAsync : public CBankFile {                       // vtable 0x0129B8D4, retail 0x164 bytes
public:
    virtual void Close();                                       // slot 5 0x009D58A0
    unsigned char _body110[0x164 - 0x110];                      // ThreadedFile +0x110 ..
};

class CResourceBank : public CBaseClassNonCopyable {            // retail 0x7C bytes (secondary vtable 0x0129C938 in CGraphicDataBank)
public:
    unsigned char _body04[0x7c - 0x04];
};

class CGraphicDataBank : public CBankFileAsync, public CResourceBank {   // vtable 0x0129C8CC
public:
    virtual void Close();                                       // slot 5
private:
    void CloseGraphics();
public:
    CArray<CGraphicInfo>      GraphicList;                      // +0x1e0
    CArray<CGraphicInfoFrame> GraphicFrameBuffer;               // +0x1ec
    bool                      Initialised;                      // +0x1f8
};

void CGraphicDataBank::CloseGraphics()
{
    GraphicList.clear();
    GraphicFrameBuffer.clear();
    CBankFile::Close();
}

void CGraphicDataBank::Close()
{
    CloseGraphics();
    CBankFileAsync::Close();
}