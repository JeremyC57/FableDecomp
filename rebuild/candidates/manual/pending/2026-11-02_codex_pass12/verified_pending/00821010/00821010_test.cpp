// Production dependency closure uses the ordinary original CDefPointer opt-in; fixture dependency models and target behavior are unchanged.
// Standalone C++03 behavior fixture: exact selected target body, complete CMap
// storage, packed records, CArray and ReadVar implementations are pasted below.
// External world/definition/thing dependencies are explicit behavior models;
// their production layouts and virtual slots are checked in the COFF audit.
// The added CMap constructor is fixture-only initialization, not retail evidence.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <new>
#ifdef _MSC_VER
typedef unsigned __int64 FixtureUInt64;
#else
typedef unsigned long FixtureUInt64;
#define __fastcall
#define __cdecl
#define __stdcall
#define __declspec(x)
#define __forceinline inline
#define long int
#endif
#define private public
#define protected public



#define FABLE_RETAIL_MAP_BRUSHES
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_DEFINITION_MANAGER
#define FABLE_ENGINE_CCHARSTRING_H


class CCharStringData;
class CCharString {
public:
    const CCharString& operator=(const char*);
    CCharString();
    CCharString(const char* text, long length = -1);
    CCharString(const CCharString& other);
    ~CCharString();
protected:
    CCharStringData* PStringData;
};
#define FABLE_ENGINE_CMAP_H
#define FABLE_ENGINE_CBASECLASSNONCOPYABLE_H



#define FABLE_ENGINE_CBASECLASS_H








class CBaseClass {
public:
    CBaseClass();                                                        
    virtual ~CBaseClass();                                               
};


class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();                                    
private:
    CBaseClassNonCopyable(const CBaseClassNonCopyable&);
    CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);
};

#define FABLE_ENGINE_C2DBOXI_H

#define FABLE_ENGINE_C2DCOORDI_H

class C2DCoordI { public: long X,Y;C2DCoordI(){}
C2DCoordI(long x,long y):X(x),Y(y){} };
class C2DBoxI { public: long TLX,TLY,BRX,BRY; C2DBoxI() {}
C2DCoordI GetTLPos()const{return C2DCoordI(TLX,TLY);}
C2DCoordI GetBRPos()const{return C2DCoordI(BRX,BRY);}
};


class CIDrawMap {public:virtual ~CIDrawMap();};
class CGameDefinitionManager;class CWorld;
class CAFile;class CScriptedMapBrush;class CScriptedMapBrushInit;

class CMap:public CIDrawMap,public CBaseClassNonCopyable {
protected:
    unsigned char CachedVolumes[4],ScriptedMapBrushes[4],ScriptedMapBrushUpdateAreas[12];
    const CGameDefinitionManager& DefinitionManager;
    CWorld& World;
    unsigned long* WorldSeed;
    float RippleBuffer[41][41];
    unsigned char ThemePaletteEntryList[12],GameMap[4],HeightMap[4];
    C2DBoxI ChangedHeightAreas[20],ChangedGameAreas[20];
    long NoChangedHeightAreas,NoChangedGameAreas;
    float SeaLevel;
    long WorldSizeX,WorldSizeY,HeightMapSizeX,HeightMapSizeY,GameMapSizeX,GameMapSizeY;
    C2DCoordI WorldPos;
    unsigned long MapUID;
    FixtureUInt64 ThingUIDCount;
    bool InitialisePassabilityFromThemes;
public:
    void LoadBrushesFromFile(CAFile&);
    CScriptedMapBrush& CreateScriptedMapBrush(const CScriptedMapBrushInit&);
    CMap(const CGameDefinitionManager& d,CWorld& w):DefinitionManager(d),World(w){}
    bool IsPassableAt(const C2DCoordI&)const;
    bool IsPrefNavAt(const C2DCoordI&)const;
};
#define FIXTURE_MODEL_engine_CWorld_h
class CThingManager;class CWorldMap;
class CWorld {public:virtual ~CWorld(){}virtual CThingManager&GetThingManager()=0;virtual const CWorldMap&PeekWorldMap()const=0;};
#define FIXTURE_MODEL_engine_CWorldMap_h
class CWorldMap {public:virtual ~CWorldMap(){}virtual long GetMapNumberAt(const C2DCoordI&)const=0;};
#define FIXTURE_MODEL_engine_CThing_h
class CThing {public:void SetAsActivated(bool);};
#define FIXTURE_MODEL_engine_CThingManager_h
class CThingManager {public:CThing*GetPThingWithUID(FixtureUInt64);};
#define FIXTURE_MODEL_engine_CGameDefinitionManager_h
class CDefinitionManager {public:long GetDefGlobalIndexFromName(const CCharString&)const;};class CGameDefinitionManager:public CDefinitionManager {};
#define FABLE_ENGINE_CSCRIPTEDMAPBRUSH_H
#define FABLE_ENGINE_CBASEOBJECT_H

















class CBaseObjectPointer;
class CBaseObject : public CBaseClass {
protected:
    CBaseObject();                                                       
    CBaseObject(const CBaseObject&);                                     
    CBaseObject& operator=(const CBaseObject&);                          
public:
    void ClearIntelligentPointers();                                     
    virtual void Validate() const;                                       
protected:
    virtual ~CBaseObject();                                              
    void InternalValidate() const;                                       
public:
    CBaseObjectPointer* IntelligentPointer;                              
};
#define FABLE_ENGINE_CSCRIPTEDMAPBRUSHINIT_H



class CScriptedMapBrushInit {public:C2DBoxI GameArea;CCharString Name;float CreationTimeSeconds;long CreationPattern;};
#define FABLE_ENGINE_C3DVECTOR_H


#define FABLE_VECTOR_INLINE inline
class C3DVector {
public:
    float X,Y,Z;
    C3DVector() {}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
    FABLE_VECTOR_INLINE void Initialise(float x,float y,float z){X=x;Y=y;Z=z;}
    FABLE_VECTOR_INLINE const C3DVector operator-(const C3DVector& other)const{return C3DVector(X-other.X,Y-other.Y,Z-other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator+(const C3DVector& other)const{return C3DVector(X+other.X,Y+other.Y,Z+other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
    FABLE_VECTOR_INLINE bool operator==(const C3DVector& other)const{return X==other.X&&Y==other.Y&&Z==other.Z;}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return X*X+Z*Z+Y*Y;}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude2D()const{return X*X+Y*Y;}
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{return X*other.X+Y*other.Y+Z*other.Z;}
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
    C3DVector& AccurateNormalise();
    void Rotate(const C3DVector&,float);
};
#undef FABLE_VECTOR_INLINE
#define FABLE_ENGINE_C3DANGLES_H

class C3DAngles {public:float AngleXY,AngleYZ,AngleXZ;C3DAngles(){}};

class CScriptedMapBrush:public CBaseObject {
 class CScriptedMapCell;class CThingPosAndOrientation;
 unsigned char Cells[12]; 
 unsigned char ThingsToDelete[12]; 
 unsigned char ThingsToMove[12]; 
 unsigned char ThingsToAdd[12]; 
 C2DBoxI GameArea;CCharString BrushName;float CreationTimeSeconds;long CreationPattern;
public:
 virtual ~CScriptedMapBrush();
 void SetGameHeight(long,long,float);
 void SetTheme(long,long,unsigned long,unsigned long);
 void SetThemeBlend(long,long,unsigned char,unsigned long);
 void SetThingToMove(FixtureUInt64,const C3DVector&,const C3DAngles&);
 void SetThingToDelete(FixtureUInt64);
 void SetThingToAdd(FixtureUInt64);
};
#define FABLE_ENGINE_CFILEFORMATBRUSH_H
#define FIXTURE_MODEL_engine_CAFile_h
#define FABLE_ENGINE_CARRAY_H
#define FABLE_STLPORT_CONTAINERS_H



#include <stddef.h>
#include <stdlib.h>
#include <string.h>
namespace std {
inline void fill(unsigned char* first,unsigned char* last,const unsigned char& value) {
    unsigned char tmp=value;memset(first,tmp,last-first);
}
template<class Size> inline unsigned char* fill_n(unsigned char* first,Size n,const unsigned char& value) {
    fill(first,first+n,value);return first+n;
}
struct __true_type { };
struct __false_type { };
template<class T> struct __type_traits { typedef __false_type is_POD_type; };
template<> struct __type_traits<unsigned char> { typedef __true_type is_POD_type; };
template<class Iterator> struct iterator_traits;
template<class T> struct iterator_traits<T*> { typedef T value_type; };
template<class ForwardIterator,class Size,class T>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first,Size n,const T& value,const __true_type&) {
    return std::fill_n(first,n,value);
}
template<class ForwardIterator,class Size,class T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first,Size n,const T& value) {
    typedef typename iterator_traits<ForwardIterator>::value_type Value;
    typedef typename __type_traits<Value>::is_POD_type IsPOD;
    return __uninitialized_fill_n(first,n,value,IsPOD());
}
template<class _Tp> class allocator {
public:
    void deallocate(_Tp* p, size_t) {
        free(p);
    }
};
template<class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& a, _Value p)
        : _MaybeReboundAlloc(a), _M_data(p) { }
};
template<class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) { }
    _Vector_base(size_t n,const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) {
        _M_start=n?(_Tp*)malloc(n*sizeof(_Tp)):0;
        _M_finish=_M_start;
        _M_end_of_storage._M_data=_M_start+n;
    }
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data-_M_start);
    }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*,_Tp,_Alloc> _M_end_of_storage;
};
template<class _Tp, class _Alloc=allocator<_Tp> >
class vector : public _Vector_base<_Tp,_Alloc> {
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef _Tp value_type;
    typedef size_t size_type;
    explicit vector(const _Alloc& a=_Alloc()) : _Vector_base<_Tp,_Alloc>(a) { }
    explicit vector(size_type n) : _Vector_base<_Tp,_Alloc>(n,_Alloc()) {
        this->_M_finish=uninitialized_fill_n(this->_M_start,n,_Tp());
    }
    _Tp& front() { return *this->_M_start; }
    iterator begin() { return this->_M_start; }
    const_iterator begin() const { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish-this->_M_start); }
    bool empty() const { return this->_M_start==this->_M_finish; }
    _Tp& operator[](size_type n) { return *(begin()+n); }
    const _Tp& operator[](size_type n) const { return *(begin()+n); }
    void push_back(const _Tp&);
};
template<class _T1,class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template<class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template<class _Tp> struct _Nonconst_traits { typedef _Tp* pointer; typedef _Tp& reference; };
template<class _Tp> struct _Const_traits { typedef const _Tp* pointer; typedef const _Tp& reference; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* p) : _M_node(p) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node=_M_node->_M_next; }
    bool operator==(const _List_iterator_base& y) const { return _M_node==y._M_node; }
    bool operator!=(const _List_iterator_base& y) const { return _M_node!=y._M_node; }
};
template<class _Tp,class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator() { }
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    pointer operator->() const { return &operator*(); }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};
template<class _Tp,class _Alloc> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<_Tp>*,_List_node<_Tp>,allocator<_List_node<_Tp> > > _M_node;
    void clear();
};
template<class _Tp,class _Alloc=allocator<_Tp> > class list : public _List_base<_Tp,_Alloc> {
public:
    typedef _Tp value_type;
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp,_Const_traits<_Tp> > const_iterator;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    bool empty() const { return this->_M_node._M_data->_M_next==this->_M_node._M_data; }
};
template<size_t _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t n) { return n/(8*sizeof(_WordT)); }
    static size_t _S_whichbit(size_t n) { return n%(8*sizeof(_WordT)); }
    static _WordT _S_maskbit(size_t n) { return _WordT(1)<<_S_whichbit(n); }
    _WordT _M_getword(size_t n) const { return _M_w[_S_whichword(n)]; }
};
void __stl_throw_out_of_range(const char*);
template<size_t _Nb> class bitset : public _Base_bitset<(_Nb+8*sizeof(unsigned long)-1)/(8*sizeof(unsigned long))> {
public:
    bool _Unchecked_test(size_t n) const { return (this->_M_getword(n)&this->_S_maskbit(n))!=0; }
    bool test(size_t n) const { if(n>=_Nb) __stl_throw_out_of_range("bitset"); return _Unchecked_test(n); }
};
template<class _Arg,class _Result> struct unary_function {
    typedef _Arg argument_type;
    typedef _Result result_type;
};
template<class _RandomAccessIter,class _RandomNumberGenerator>
void __fastcall random_shuffle(_RandomAccessIter,_RandomAccessIter,_RandomNumberGenerator&);
}

template<class T>class CArray:public std::vector<T>{public:
 CArray():std::vector<T>(){}
 explicit CArray(unsigned int n):std::vector<T>(n){}
 T&operator[](unsigned int n){return this->_M_start[n];}
 const T&operator[](unsigned int n)const{return this->_M_start[n];}
};
class CASuspendableProcess;
class CAFile {public:virtual ~CAFile(){}virtual void Read(void*,long,CASuspendableProcess*)=0;virtual void SetPosition(long)=0;virtual long GetPosition()const=0;virtual long GetAlignment()const=0;
    template<class T>void ReadVar(T& var,CASuspendableProcess* psuspendable) {
        if(GetAlignment()<=1)Read(&var,sizeof(T),psuspendable);
        else {
            unsigned long buffer_length=GetAlignment();
            buffer_length=((sizeof(T)+buffer_length-1)/buffer_length)*buffer_length;
            CArray<unsigned char> buffer(buffer_length);
            Read(&buffer[0],sizeof(T),psuspendable);
            var=*(T*)&buffer[0];
        }
    }

};






#pragma pack(push,1)
template<class T,unsigned short Version>class CBaseSerialise {public:
 unsigned long ByteLength;unsigned char SerialiseVersionNumber;
 CBaseSerialise():SerialiseVersionNumber(Version){}
 bool ReadFromFile(CAFile&file,CASuspendableProcess*psuspendable_process){
  unsigned long length;file.ReadVar(length,psuspendable_process);
  long old_pos=file.GetPosition();unsigned char version;
  file.ReadVar(version,psuspendable_process);file.SetPosition(old_pos);
  ByteLength=length;file.Read((char*)this+sizeof(ByteLength),length-sizeof(ByteLength),psuspendable_process);
  return true;
 }
};
template<class T,class Old>class CReplaceSerialise {public:
 unsigned long ByteLength;unsigned char SerialiseVersionNumber;
 void ReadFromFile(CAFile&,CASuspendableProcess*);
};
class COldFileFormatLevel3;class COldFileFormatBrush;
class CFileFormatLevel:public CReplaceSerialise<CFileFormatLevel,COldFileFormatLevel3>{public:
 long MapDataOffset,ThingDataOffset,BrushDataOffset,NavigationDataOffset,NavigationSectionsOffset;
};
class CFileLargeThemePaletteEntry {public:CFileLargeThemePaletteEntry();char ThemePaletteEntryName[128];long ThemePaletteThemeDefIndex;};
class CFileLargeThemePalette {CFileLargeThemePaletteEntry FileThemePaletteEntry[256];public:
 CFileLargeThemePalette();
 CFileLargeThemePaletteEntry&GetThemePaletteEntry(long index){return FileThemePaletteEntry[index];}
 const CFileLargeThemePaletteEntry&PeekThemePaletteEntry(long index)const{return FileThemePaletteEntry[index];}
};
class CFileFormatBrushHeader:public CBaseSerialise<CFileFormatBrushHeader,1>{public:unsigned long NoBrushes;CFileLargeThemePalette LargeThemePalette;};
class CFileFormatBrush:public CReplaceSerialise<CFileFormatBrush,COldFileFormatBrush>{public:
 C2DBoxI GameArea;char ScriptName[32];float CreationTimeSeconds;long CreationPattern;
 long NumberOfThingsToMove,NumberOfThingsToDelete,NumberOfThingsToAdd;
};
class CFileFormatBrushCell:public CBaseSerialise<CFileFormatBrushCell,1>{public:float Height;unsigned char Theme[3],ThemeBlend[2];};
class CFileFormatBrushThingToMove:public CBaseSerialise<CFileFormatBrushThingToMove,1>{public:FixtureUInt64 UniqueID;C3DVector Position;C3DAngles FacingAngles;};
class CFileFormatBrushThingToDelete:public CBaseSerialise<CFileFormatBrushThingToDelete,1>{public:FixtureUInt64 UniqueID;};
class CFileFormatBrushThingToAdd:public CBaseSerialise<CFileFormatBrushThingToAdd,1>{public:FixtureUInt64 UniqueID;};
#pragma pack(pop)
void CMap::LoadBrushesFromFile(CAFile& file)
{
    file.SetPosition(0);
    CFileFormatLevel level_header;
    level_header.ReadFromFile(file,0);
    CScriptedMapBrushInit init;
    CFileFormatBrushHeader file_brush_header;
    CFileFormatBrush file_brush;
    CFileFormatBrushCell file_brush_cell;
    CFileFormatBrushThingToMove file_brush_thing_to_move;
    CFileFormatBrushThingToDelete file_brush_thing_to_delete;
    CFileFormatBrushThingToAdd file_brush_thing_to_add;
    CThingManager& thing_manager=World.GetThingManager();
    const long map_number=World.PeekWorldMap().GetMapNumberAt(WorldPos);
    if(!level_header.BrushDataOffset)return;
    file.SetPosition(level_header.BrushDataOffset);
    file_brush_header.ReadFromFile(file,0);
    const CGameDefinitionManager& def_manager=DefinitionManager;
    CFileLargeThemePalette& file_palette=file_brush_header.LargeThemePalette;
    for(long c1=0;c1<256;++c1) {
        if(file_palette.GetThemePaletteEntry(c1).ThemePaletteThemeDefIndex) {
            long new_theme_def_index=def_manager.GetDefGlobalIndexFromName(file_palette.GetThemePaletteEntry(c1).ThemePaletteEntryName);
            if(new_theme_def_index)file_palette.GetThemePaletteEntry(c1).ThemePaletteThemeDefIndex=new_theme_def_index;
            else file_palette.GetThemePaletteEntry(c1).ThemePaletteThemeDefIndex=1;
        }
    }
    for(long c1=0;c1<file_brush_header.NoBrushes;++c1) {
        file_brush.ReadFromFile(file,0);
        init.GameArea=file_brush.GameArea;
        init.Name=file_brush.ScriptName;
        init.CreationTimeSeconds=file_brush.CreationTimeSeconds;
        init.CreationPattern=file_brush.CreationPattern;
        CScriptedMapBrush& brush=CreateScriptedMapBrush(init);
        C2DCoordI tl_pos=file_brush.GameArea.GetTLPos();
        C2DCoordI br_pos=file_brush.GameArea.GetBRPos();
        for(long y=tl_pos.Y;y<br_pos.Y;++y) {
            for(long x=tl_pos.X;x<br_pos.X;++x) {
                file_brush_cell.ReadFromFile(file,0);
                brush.SetGameHeight(x,y,file_brush_cell.Height);
                for(long cblend=0;cblend<2;++cblend)brush.SetThemeBlend(x,y,file_brush_cell.ThemeBlend[cblend],cblend);
                for(long ctheme=0;ctheme<3;++ctheme)brush.SetTheme(x,y,file_palette.GetThemePaletteEntry(file_brush_cell.Theme[ctheme]).ThemePaletteThemeDefIndex,ctheme);
            }
        }
        for(long cthing=0;cthing<file_brush.NumberOfThingsToMove;++cthing) {
            file_brush_thing_to_move.ReadFromFile(file,0);
            brush.SetThingToMove(file_brush_thing_to_move.UniqueID,file_brush_thing_to_move.Position,file_brush_thing_to_move.FacingAngles);
        }
        for(long cthing=0;cthing<file_brush.NumberOfThingsToDelete;++cthing) {
            file_brush_thing_to_delete.ReadFromFile(file,0);
            brush.SetThingToDelete(file_brush_thing_to_delete.UniqueID);
        }
        for(long cthing=0;cthing<file_brush.NumberOfThingsToAdd;++cthing) {
            file_brush_thing_to_add.ReadFromFile(file,0);
            brush.SetThingToAdd(file_brush_thing_to_add.UniqueID);
            CThing* pthing=thing_manager.GetPThingWithUID(file_brush_thing_to_add.UniqueID);
            if(pthing)pthing->SetAsActivated(false);
        }
    }
}

class CCharStringData {public:char* text;unsigned refs;};
static unsigned strings=0,palette_ctors=0,definition_calls=0,world_calls=0,map_calls=0,lookup_calls=0;
static const char* string_text(const CCharString&s){return s.PStringData?s.PStringData->text:"";}
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const char*p,long n):PStringData(0){if(n<0)n=(long)strlen(p);PStringData=new CCharStringData;PStringData->text=(char*)malloc(n+1);memcpy(PStringData->text,p,n);PStringData->text[n]=0;PStringData->refs=1;++strings;}
CCharString::CCharString(const CCharString&s):PStringData(s.PStringData){if(PStringData)++PStringData->refs;}
CCharString::~CCharString(){if(PStringData&&!--PStringData->refs){free(PStringData->text);delete PStringData;--strings;}}
const CCharString&CCharString::operator=(const char*p){CCharString temp(p);CCharStringData*old=PStringData;PStringData=temp.PStringData;temp.PStringData=old;return *this;}
CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CBaseObject::CBaseObject():IntelligentPointer(0){}CBaseObject::~CBaseObject(){}void CBaseObject::Validate()const{}
CIDrawMap::~CIDrawMap(){}
CScriptedMapBrush::~CScriptedMapBrush(){}
CFileLargeThemePaletteEntry::CFileLargeThemePaletteEntry(){memset(ThemePaletteEntryName,0,sizeof(ThemePaletteEntryName));ThemePaletteThemeDefIndex=0;}
CFileLargeThemePalette::CFileLargeThemePalette(){++palette_ctors;}
template<class T,class Old>void CReplaceSerialise<T,Old>::ReadFromFile(CAFile&file,CASuspendableProcess*p){
 // Version conversion is an external callee. This fixture supplies current-version records.
 unsigned long n;file.Read(&n,4,p);assert(n==sizeof(T));ByteLength=n;file.Read((char*)this+4,n-4,p);assert(SerialiseVersionNumber==(sizeof(T)==25?4:2));
}
struct Event {unsigned kind;long a,b,c,d;FixtureUInt64 uid;float values[6];char name[40];};
static Event expected[2048],observed[2048];static unsigned expected_count,observed_count,current_brush;
static CScriptedMapBrush brushes[4];static CThing things[16];static FixtureUInt64 thing_ids[16];static unsigned thing_count;
static Event make_event(unsigned kind){Event e;memset(&e,0,sizeof(e));e.kind=kind;return e;}
static void expect(const Event&e){assert(expected_count<2048);expected[expected_count++]=e;}
static void observe(const Event&e){assert(observed_count<2048);observed[observed_count++]=e;}
static unsigned brush_id(const CScriptedMapBrush*p){for(unsigned i=0;i<4;++i)if(p==brushes+i)return i;assert(false);return 0;}
CScriptedMapBrush&CMap::CreateScriptedMapBrush(const CScriptedMapBrushInit&i){Event e=make_event(1);e.a=i.GameArea.TLX;e.b=i.GameArea.TLY;e.c=i.GameArea.BRX;e.d=i.GameArea.BRY;e.values[0]=i.CreationTimeSeconds;e.values[1]=(float)i.CreationPattern;strcpy(e.name,string_text(i.Name));observe(e);assert(current_brush<4);return brushes[current_brush++];}
void CScriptedMapBrush::SetGameHeight(long x,long y,float h){Event e=make_event(2);e.a=x;e.b=y;e.c=brush_id(this);e.values[0]=h;observe(e);}
void CScriptedMapBrush::SetThemeBlend(long x,long y,unsigned char value,unsigned long index){Event e=make_event(3);e.a=x;e.b=y;e.c=value;e.d=index;e.uid=brush_id(this);observe(e);}
void CScriptedMapBrush::SetTheme(long x,long y,unsigned long value,unsigned long index){Event e=make_event(4);e.a=x;e.b=y;e.c=value;e.d=index;e.uid=brush_id(this);observe(e);}
void CScriptedMapBrush::SetThingToMove(FixtureUInt64 id,const C3DVector&p,const C3DAngles&a){Event e=make_event(5);e.a=brush_id(this);e.uid=id;e.values[0]=p.X;e.values[1]=p.Y;e.values[2]=p.Z;e.values[3]=a.AngleXY;e.values[4]=a.AngleYZ;e.values[5]=a.AngleXZ;observe(e);}
void CScriptedMapBrush::SetThingToDelete(FixtureUInt64 id){Event e=make_event(6);e.a=brush_id(this);e.uid=id;observe(e);}
void CScriptedMapBrush::SetThingToAdd(FixtureUInt64 id){Event e=make_event(7);e.a=brush_id(this);e.uid=id;observe(e);}
CThing*CThingManager::GetPThingWithUID(FixtureUInt64 id){++lookup_calls;Event e=make_event(8);e.uid=id;observe(e);for(unsigned i=0;i<thing_count;++i)if(thing_ids[i]==id)return things+i;return 0;}
void CThing::SetAsActivated(bool value){assert(!value);Event e=make_event(9);for(unsigned i=0;i<thing_count;++i)if(this==things+i){e.uid=thing_ids[i];observe(e);return;}assert(false);}
long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&s)const{++definition_calls;unsigned i;assert(sscanf(string_text(s),"THEME_%u",&i)==1&&i<256);return i%3?1000+(long)i:0;}
class FixtureWorldMap:public CWorldMap {public:virtual long GetMapNumberAt(const C2DCoordI&p)const{++map_calls;assert(p.X==17&&p.Y==-9);return 37;}};
class FixtureWorld:public CWorld {public:CThingManager manager;FixtureWorldMap map;virtual CThingManager&GetThingManager(){++world_calls;return manager;}virtual const CWorldMap&PeekWorldMap()const{return map;}};
class FixtureFile:public CAFile {public:
 unsigned char bytes[40000];long size,position,alignment;mutable unsigned alignment_calls;unsigned var_records;
 FixtureFile(long a):size(0),position(0),alignment(a),alignment_calls(0),var_records(0){memset(bytes,0,sizeof(bytes));}
 void Read(void*p,long n,CASuspendableProcess*process){assert(!process&&n>=0&&position>=0&&position+n<=size);memcpy(p,bytes+position,n);position+=n;}
 void SetPosition(long p){assert(p>=0&&p<=size);position=p;}
 long GetPosition()const{return position;}
 long GetAlignment()const{++alignment_calls;return alignment;}
 void raw(const void*p,unsigned n){assert(size+(long)n<=40000);memcpy(bytes+size,p,n);size+=n;}
 void word(unsigned long value){raw(&value,4);}
 void quad(FixtureUInt64 value){raw(&value,8);}
 void byte(unsigned char value){raw(&value,1);}
 void real(float value){raw(&value,4);}
 void fixed(const char*value,unsigned n){char b[128];assert(n<=128&&strlen(value)<n);memset(b,0,n);strcpy(b,value);raw(b,n);}
 void tag(unsigned n,unsigned version){word(n);byte((unsigned char)version);}
};
static FixtureUInt64 uid(unsigned brush,unsigned i,unsigned kind){return (FixtureUInt64(0x12345678u)<<32)|FixtureUInt64(1000+brush*100+i*3+kind);}
static long palette_result(unsigned index){if(index%4==0)return 0;return index%3?1000+(long)index:1;}
static void run(long alignment,unsigned mode,unsigned count,unsigned version){
 expected_count=observed_count=current_brush=thing_count=0;definition_calls=world_calls=map_calls=lookup_calls=0;
 unsigned starting_strings=strings,starting_palette=palette_ctors,expected_lookups=0,cells=0,records=0;
 FixtureFile file(alignment);bool no_offset=mode==7;
 file.tag(25,4);file.word(0);file.word(0);file.word(no_offset?0:32);file.word(0);file.word(0);
 while(file.size<32)file.byte(0x5a);
 file.tag(33801,version);file.word(count);
 for(unsigned index=0;index<256;++index){char name[40];sprintf(name,"THEME_%u",index);file.fixed(name,128);file.word(index%4==0?0:9999);}
 for(unsigned b=0;b<count;++b){
  long left=-2+(long)b,top=-1-(long)b,width=(long)(mode%3),height=(long)((mode/2)%3);
  long moves=(long)(mode%3),deletes=(long)((mode+1)%3),adds=(long)((mode+2)%3);
  if(mode==6){moves=-2;deletes=-1;adds=-3;}
  char name[32];sprintf(name,"BRUSH_%u_%u",mode,b);
  file.tag(73,2);file.word(left);file.word(top);file.word(left+width);file.word(top+height);file.fixed(name,32);file.real((float)b+0.5f);file.word(17+b);file.word(moves);file.word(deletes);file.word(adds);
  Event e=make_event(1);e.a=left;e.b=top;e.c=left+width;e.d=top+height;e.values[0]=(float)b+0.5f;e.values[1]=(float)(17+b);strcpy(e.name,name);expect(e);
  for(long cell=0;cell<width*height;++cell){long x=left+cell%width,y=top+cell/width;unsigned theme[3]={(unsigned)((cell+b)%256),(unsigned)((cell+3+b)%256),(unsigned)((cell+253+b)%256)};unsigned blend[2]={(unsigned)(cell+1),(unsigned)(255-cell)};float h=(float)(cell*2-(long)b)+0.5f;
   file.tag(14,version);file.real(h);for(unsigned j=0;j<3;++j)file.byte((unsigned char)theme[j]);for(unsigned j=0;j<2;++j)file.byte((unsigned char)blend[j]);
   e=make_event(2);e.a=x;e.b=y;e.c=b;e.values[0]=h;expect(e);
   for(unsigned j=0;j<2;++j){e=make_event(3);e.a=x;e.b=y;e.c=blend[j];e.d=j;e.uid=b;expect(e);}
   for(unsigned j=0;j<3;++j){e=make_event(4);e.a=x;e.b=y;e.c=palette_result(theme[j]);e.d=j;e.uid=b;expect(e);}
   ++cells;++records;
  }
  for(long i=0;i<moves;++i){FixtureUInt64 id=uid(b,i,0);file.tag(37,version);file.quad(id);e=make_event(5);e.a=b;e.uid=id;for(unsigned j=0;j<6;++j){e.values[j]=(float)(i*4+(long)j)-0.5f;file.real(e.values[j]);}expect(e);++records;}
  for(long i=0;i<deletes;++i){FixtureUInt64 id=uid(b,i,1);file.tag(13,version);file.quad(id);e=make_event(6);e.a=b;e.uid=id;expect(e);++records;}
  for(long i=0;i<adds;++i){FixtureUInt64 id=uid(b,i,2);file.tag(13,version);file.quad(id);e=make_event(7);e.a=b;e.uid=id;expect(e);e=make_event(8);e.uid=id;expect(e);++expected_lookups;
   if((i+b)%2==0){assert(thing_count<16);thing_ids[thing_count++]=id;e=make_event(9);e.uid=id;expect(e);}++records;
  }
 }
 unsigned data_end=(unsigned)file.size;file.word(0x11223344);file.word(0x55667788);
 FixtureWorld world;CGameDefinitionManager definitions;CMap map(definitions,world);map.WorldPos=C2DCoordI(17,-9);
 memset(map.CachedVolumes,0x11,sizeof(map.CachedVolumes));memset(map.ScriptedMapBrushes,0x22,sizeof(map.ScriptedMapBrushes));memset(map.ScriptedMapBrushUpdateAreas,0x33,sizeof(map.ScriptedMapBrushUpdateAreas));
 map.LoadBrushesFromFile(file);
 assert(world_calls==1&&map_calls==1&&palette_ctors==starting_palette+1&&strings==starting_strings);
 if(no_offset){assert(file.position==25&&observed_count==0&&definition_calls==0&&lookup_calls==0&&file.alignment_calls==0);}
 else {assert((unsigned)file.position==data_end&&observed_count==expected_count&&current_brush==count);assert(definition_calls==192&&lookup_calls==expected_lookups);assert(file.alignment_calls==2*(records+1)*(alignment>1?2:1));for(unsigned i=0;i<expected_count;++i)assert(memcmp(expected+i,observed+i,sizeof(Event))==0);}
 for(unsigned i=0;i<sizeof(map.CachedVolumes);++i)assert(map.CachedVolumes[i]==0x11);
 for(unsigned i=0;i<sizeof(map.ScriptedMapBrushes);++i)assert(map.ScriptedMapBrushes[i]==0x22);
 for(unsigned i=0;i<sizeof(map.ScriptedMapBrushUpdateAreas);++i)assert(map.ScriptedMapBrushUpdateAreas[i]==0x33);
}
int main(){
 assert(sizeof(CFileFormatLevel)==25&&sizeof(CFileFormatBrushHeader)==33801&&sizeof(CFileFormatBrush)==73&&sizeof(CFileFormatBrushCell)==14&&sizeof(CFileFormatBrushThingToMove)==37&&sizeof(CFileFormatBrushThingToDelete)==13&&sizeof(CFileFormatBrushThingToAdd)==13);
 assert(offsetof(CFileFormatLevel,BrushDataOffset)==13&&offsetof(CFileFormatBrush,GameArea)==5&&offsetof(CFileFormatBrush,ScriptName)==21&&offsetof(CFileFormatBrushThingToMove,UniqueID)==5&&offsetof(CFileFormatBrushThingToMove,Position)==13);
#ifdef _MSC_VER
 assert(sizeof(CMap)==0x1d40&&sizeof(CScriptedMapBrush)==84);
#endif
 const long alignments[]={0,1,2,3,4,8,16};unsigned cases=0;
 for(unsigned a=0;a<7;++a)for(unsigned mode=0;mode<8;++mode)for(unsigned count=0;count<4;++count)for(unsigned version=1;version<=9;version+=8){run(alignments[a],mode,count,version);++cases;}
 assert(cases==448&&strings==0);puts("00821010_TEST PASS");return 0;
}
