// CUserProfileManager::VerifySignature(CWideString const &) -- validate profile/save signature, length, CRC and padding.
// Leaf: ?VerifySignature@CUserProfileManager@@SI_NABVCWideString@@@Z
// Grade: DIFFER(641v1078) -- inline budget: retail inlines the CDataInputStream readers (ReadType, Read with chunk overflow) and the CDiskFileWin32 destructor; this build calls them
// Context: grades against the Codex row context $S/codex11_dl/x/rows/00409730/selected_context/hdr (Codex per-feature macros; NOT rebased on rebuild/include); source = Codex selected; 00409730 probes in probe/00409730.log
// Flags: /GS
#define FABLE_RETAIL_PROFILE_SIGNATURE
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CFILEDATAINPUTSTREAM
#include "engine/CDiskFileWin32.h"
#include "engine/CFileDataInputStream.h"
#include "engine/CUserProfileManager.h"
#include <string.h>
#include <stdlib.h>
#pragma intrinsic(memcmp)



class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long,const void*,unsigned long);
};
extern const unsigned long CRC32TAB[];
#pragma auto_inline(off)
unsigned long __fastcall CCRC::Calc(unsigned long seed,const void* data,unsigned long length) {
    unsigned long result=seed;
    if(length!=0) {
        for(unsigned long index=0;index<length;++index)
            result=(result>>8)^CRC32TAB[((const unsigned char*)data)[index]^(result&255)];
    }
    return result;
}
#pragma auto_inline(on)
namespace std {
struct __true_type {};
inline void* __copy_trivial(const void* first,const void* last,void* result) {
    return last==first ? result : (char*)memmove(result,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);
}
inline void fill(char* first,char* last,const char& value) { char tmp=value;memset(first,(unsigned char)tmp,last-first); }
template<class Size> inline char* fill_n(char* first,Size count,const char& value) { fill(first,first+count,value);return first+count; }

template<class _Tp> class allocator {
public:
    allocator() {}
    allocator(const allocator<_Tp>&) {}
    ~allocator() {}
    _Tp* allocate(unsigned int count,const void* = 0) { return count ? (_Tp*)malloc(count*sizeof(_Tp)) : 0; }
    void deallocate(_Tp* p,unsigned int) { if(p!=0)free(p); }
};
template<class _Value,class _Tp,class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _STLP_alloc_proxy(const _Alloc& alloc,_Value p) : _Alloc(alloc),_M_data(p) {}
    _Value _M_data;
};
template<class _Tp,class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int count,const _Alloc& alloc)
        : _M_start(0),_M_finish(0),_M_end_of_storage(alloc,0) {
        _M_start=_M_end_of_storage.allocate(count);
        _M_finish=_M_start;
        _M_end_of_storage._M_data=_M_start+count;
    }
    ~_Vector_base() {
        if(_M_start!=0) _M_end_of_storage.deallocate(_M_start,(unsigned int)(_M_end_of_storage._M_data-_M_start));
    }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*,_Tp,_Alloc> _M_end_of_storage;
};
inline void _Destroy(char*,char*) {}
template<class _Tp,class _Alloc=allocator<_Tp> > class vector : public _Vector_base<_Tp,_Alloc> {
public:
    __declspec(noinline) explicit vector(unsigned int);
    ~vector() { _Destroy(this->_M_start,this->_M_finish); }
    __declspec(noinline) void resize(unsigned int count) { resize(count,_Tp()); }
    void resize(unsigned int count,_Tp value) {
        if(count<size()) erase(begin()+count,end());
        else _M_fill_insert(end(),count-size(),value);
    }
    _Tp* begin() {return this->_M_start;}
    _Tp* end() {return this->_M_finish;}
    _Tp* erase(_Tp* first,_Tp* last) {
        _Tp* i=(_Tp*)__copy_trivial(last,this->_M_finish,first);
        _Destroy(i,this->_M_finish);this->_M_finish=i;return first;
    }
protected:
    void _M_clear() {
        _Destroy(this->_M_start,this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start,(unsigned int)(this->_M_end_of_storage._M_data-this->_M_start));
    }
    void _M_set(_Tp* start,_Tp* finish,_Tp* end_storage) {
        this->_M_start=start;this->_M_finish=finish;this->_M_end_of_storage._M_data=end_storage;
    }
    void _M_insert_overflow(_Tp* position,const _Tp& value,const __true_type&,unsigned int fill_len,bool at_end=false) {
        unsigned int old_size=size();unsigned int length=old_size+(old_size<fill_len ? fill_len : old_size);
        _Tp* new_start=this->_M_end_of_storage.allocate(length);
        _Tp* new_finish=(_Tp*)__copy_trivial(this->_M_start,position,new_start);
        new_finish=fill_n(new_finish,fill_len,value);
        if(!at_end)new_finish=(_Tp*)__copy_trivial(position,this->_M_finish,new_finish);
        _M_clear();_M_set(new_start,new_finish,new_start+length);
    }
    void _M_fill_insert(_Tp* position,unsigned int count,const _Tp& value) {
        if(count!=0) {
            if((unsigned int)(this->_M_end_of_storage._M_data-this->_M_finish)>=count) {
                _Tp copy=value;unsigned int after=(unsigned int)(this->_M_finish-position);_Tp* old_finish=this->_M_finish;
                if(after>count) {
                    __copy_trivial(this->_M_finish-count,this->_M_finish,this->_M_finish);this->_M_finish+=count;
                    memmove(position+count,position,(old_finish-count-position)*sizeof(_Tp));
                    fill(position,position+count,copy);
                } else {
                    fill_n(this->_M_finish,count-after,copy);this->_M_finish+=count-after;
                    __copy_trivial(position,old_finish,this->_M_finish);this->_M_finish+=after;
                    fill(position,old_finish,copy);
                }
            } else _M_insert_overflow(position,value,__true_type(),count);
        }
    }
public:
    unsigned int size() const { return (unsigned int)(this->_M_finish-this->_M_start); }
    _Tp& front() { return *begin(); }
};
#pragma auto_inline(off)
template<class _Tp,class _Alloc>
__declspec(noinline) vector<_Tp,_Alloc>::vector(unsigned int count)
    : _Vector_base<_Tp,_Alloc>(count,_Alloc()) {
    memset(this->_M_start,0,count*sizeof(_Tp));
    this->_M_finish=this->_M_start+count;
}
#pragma auto_inline(on)

}
#define FABLE_RETAIL_CARRAY
#include "engine/CArray.h"
// Fidelity: ordinary stream readers remain out of line here; this candidate differs from retail.
bool __fastcall CUserProfileManager::VerifySignature(const CWideString& path_name)
{
    if(!CAFile::PathExists(path_name) || !CAFile::IsPathFile(path_name))return false;
    CDiskFileWin32 file(path_name,FILE_ACCESS_READ_WRITE,2);
    if(file.GetLength()<16)return false;
    char header[8];
    file.Read(header,8,0);
    bool binary_profile=memcmp(header,"FablePro",8)==0;
    bool binary_savegame=memcmp(header,"FableSav",8)==0;
    if(!binary_profile && !binary_savegame)return false;
    CFileDataInputStream file_is(file,16384);
    unsigned long version=file_is.ReadULONG();
    if(binary_savegame && version!=0x2165)return false;
    unsigned long file_size=file_is.ReadULONG();
    if(file_size+4>(unsigned long)file.GetLength())return false;
    CArray<char> save_data(file_size);
    file_is.SetPosition(0);
    file_is.Read(save_data.GetAsCArray(),file_size);
    unsigned long saved_crc=file_is.ReadULONG();
    if(CCRC::Calc(0,save_data.GetAsCArray(),file_size)==saved_crc) {
        unsigned long correct_size=binary_profile ? 16384 : 307200;
        if(correct_size<file_is.GetPosition())correct_size=file_is.GetPosition();
        if((unsigned long)file.GetLength()==correct_size) {
            char padding_char=binary_profile ? 32 : 0;
            long remaining_bytes=correct_size-file_is.GetPosition();
            save_data.resize(16384);
            for(;remaining_bytes>0;) {
                long read_size=remaining_bytes;
                if(read_size>16384)read_size=16384;
                char* data=save_data.GetAsCArray();
                file_is.Read(data,read_size);
                long c_char=0;
                for(;;) {
                    if(c_char>=read_size)break;
                    if(data[c_char]!=padding_char)return false;
                    ++c_char;
                }
                remaining_bytes-=read_size;
            }
            return true;
        }
    }
    return false;
}
