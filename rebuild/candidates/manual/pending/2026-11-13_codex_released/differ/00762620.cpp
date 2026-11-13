// NShape::CShape::StatOnSerialise(CTCShapeManager& owner, CPersistContext& persist, long shape_number, const C2DCoordI& map_pos, C2DBoxF& bounds) -- restores a saved polygon or camera track
// Leaf: ?StatOnSerialise@CShape@NShape@@SIXAAVCTCShapeManager@@AAVCPersistContext@@JABVC2DCoordI@@AAVC2DBoxF@@@Z
// Grade: DIFFER(1182v1160) -- argument evaluation order of the keyed type Transfer: retail builds the '.Type' key (CCharString temp + operator+ + c_str) and zeroes the points vector before pushing &type / the default, VC7.1 RTM pushes the cheap reference arguments first; the inlined CTrack length loop is also 22 bytes longer. 10 shapes (points/size_guy declaration placement, named key local, const char* operator+, size_guy=0, default through a const reference) leave or worsen it. Graded in Codex's header context (Codex CCharString.h second model + FABLE_RETAIL_SHAPE_*/VECTOR_* per-feature macros = header debt)
// Flags: /Gr
#define FABLE_RETAIL_SHAPE_ALLOCATOR_LIFETIME
#define FABLE_RETAIL_SHAPE_PERSIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_VECTOR_SPLINE_COPY
#define FABLE_RETAIL_VECTOR_GEOMETRY
#define FABLE_RETAIL_VECTOR_COPY_INITIALISE
#define FABLE_STLPORT_HTTP
#define FABLE_STLPORT_UI
#include "engine/C2DCoordI.h"
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
#pragma auto_inline(off)
namespace std {
template<> vector<C3DVector>::vector(const std::vector<C3DVector>& other):_Vector_base<C3DVector,std::allocator<C3DVector> >(other.size(),other.get_allocator())
{
    _M_finish=std::__uninitialized_copy((const C3DVector*)other._M_start,(const C3DVector*)other._M_finish,_M_start,std::__false_type());
}
template<> void vector<C3DVector>::_M_insert_overflow(iterator position,const C3DVector& x,const std::__false_type&,size_type fillLength,bool atEnd)
{
    const size_type oldSize=size();const size_type len=oldSize+(std::max)(oldSize,fillLength);
    C3DVector* newStart=_M_end_of_storage.allocate(len);C3DVector* newFinish=newStart;
    newFinish=std::__uninitialized_copy(_M_start,position,newStart,std::__false_type());
    if(fillLength==1){std::_Construct(newFinish,x);++newFinish;}
    else newFinish=std::__uninitialized_fill_n(newFinish,fillLength,x,std::__false_type());
    if(!atEnd)newFinish=std::__uninitialized_copy(position,_M_finish,newFinish,std::__false_type());
    _M_clear();_M_set(newStart,newFinish,newStart+len);
}
}
#pragma auto_inline(on)
#include "engine/CBasicShape.h"
#include "engine/CTrack.h"
#include "engine/CTCShapeManager.h"
#include "engine/CPersistContext.h"
#include "engine/CCharString.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CPersistContext_TransferableComponent.h"
#include <math.h>
#pragma intrinsic(sqrt)
CCharString __fastcall GFIntToCharString(long);
CCharString __fastcall operator+(const CCharString&,const CCharString&);
CCharString __fastcall operator+(const CCharString&,const char*);
namespace {
extern CCharString CLOSED_STRING_NAME;
extern CCharString LINE_STRING_NAME;
extern CCharString LEGACY_SHAPE_TYPE;
}
#pragma auto_inline(off)
NShape::CTrack::CTrack(CTCShapeManager& owner,const std::vector<C3DVector>& points):CShape(owner,points),TrackTraverser(0),TrackLength(0.0f)
{
    if(Points.size()>1) {
        C3DVector previous=Points[0];
        for(std::vector<C3DVector>::iterator it=Points.begin()+1;it!=Points.end();++it) {
            TrackLength+=(*it-previous).GetMagnitude();
            previous=*it;
        }
    }
}
template<> void CPersistContext::Transfer<float>(const char* name,float& value,const float& default_value)
{
    CheckCRC(name);
    switch(Mode) {
    case MODE_CLEAR:value=default_value;break;
    case MODE_SAVE_BINARY:PSaveStream->WriteFloat(value);break;
    case MODE_LOAD_BINARY:{CPersistTraits<float> traits;traits.TransferBinaryIn(*PLoadStream,value);break;}
    case MODE_SAVE_TEXT:{CPersistContext_TransferableComponent<float> item(value);TransferObjectSaveText(name,item);break;}
    case MODE_LOAD_TEXT:{if(UseDefaults)value=default_value;CPersistContext_TransferableComponent<float> item(value);TransferObjectLoadText(name,item);break;}
    default:__assume(0);
    }
}
template<> void CPersistContext::Transfer<long>(const char* name,long& value,const long& default_value)
{
    CheckCRC(name);
    switch(Mode) {
    case MODE_CLEAR:value=default_value;break;
    case MODE_SAVE_BINARY:PSaveStream->WriteSLONG(value);break;
    case MODE_LOAD_BINARY:{CPersistTraits<long> traits;traits.TransferBinaryIn(*PLoadStream,value);break;}
    case MODE_SAVE_TEXT:{CPersistContext_TransferableComponent<long> item(value);TransferObjectSaveText(name,item);break;}
    case MODE_LOAD_TEXT:{if(UseDefaults)value=default_value;CPersistContext_TransferableComponent<long> item(value);TransferObjectLoadText(name,item);break;}
    default:__assume(0);
    }
}
void NShape::CShape::GetBounds(C2DBoxF& bounds) const
{
    std::vector<C3DVector>::const_iterator it=Points.begin();
    bounds.TLX=bounds.BRX=it->X;
    bounds.TLY=bounds.BRY=it->Y;
    for(++it;it!=Points.end();++it) {
        if(it->X<bounds.TLX)bounds.TLX=it->X;
        if(it->Y<bounds.TLY)bounds.TLY=it->Y;
        if(it->X>bounds.BRX)bounds.BRX=it->X;
        if(it->Y>bounds.BRY)bounds.BRY=it->Y;
    }
}
template<> long __fastcall CBasicString<char>::Compare(const char* first,const char* second)
{
    while(*first || *second) {
        if(*first<*second)return -1;
        if(*first>*second)return 1;
        ++first;++second;
    }
    return 0;
}
#pragma auto_inline(on)
void __fastcall NShape::CShape::StatOnSerialise(CTCShapeManager& owner,CPersistContext& persist,long shape_number,const C2DCoordI& map_pos,C2DBoxF& bounds)
{
    CCharString shape_def=CCharString("Shape[")+GFIntToCharString(shape_number)+CCharString("]");
    CCharString type;
    persist.Transfer(shape_def+CCharString(".Type"),type,LEGACY_SHAPE_TYPE);
    std::vector<C3DVector> points;
    long size_guy;
    persist.Transfer(shape_def+CCharString(".size()"),size_guy);
    for(long c1=0;c1!=size_guy;++c1) {
        C3DVector pos;
        float reader;
        CCharString searcher=shape_def+CCharString(".pos[")+GFIntToCharString(c1)+CCharString("]");
        persist.Transfer(searcher+".X",reader);
        pos.X=map_pos.X+reader;
        persist.Transfer(searcher+".Y",reader);
        pos.Y=map_pos.Y+reader;
        persist.Transfer(searcher+".Z",reader);
        pos.Z=reader;
        points.push_back(pos);
    }
    CCountedPointer<CShape> pshape;
    if(type==CLOSED_STRING_NAME) {
        pshape.Reset(new CBasicShape(owner,points));
        C2DBoxF this_bounds;
        pshape->GetBounds(this_bounds);
        if(this_bounds.TLX<bounds.TLX)bounds.TLX=this_bounds.TLX;
        if(this_bounds.TLY<bounds.TLY)bounds.TLY=this_bounds.TLY;
        if(this_bounds.BRX>bounds.BRX)bounds.BRX=this_bounds.BRX;
        if(this_bounds.BRY>bounds.BRY)bounds.BRY=this_bounds.BRY;
    } else if(type==LINE_STRING_NAME) {
        pshape.Reset(new CTrack(owner,points));
    } else return;
    owner.AddNewShape(pshape);
}
