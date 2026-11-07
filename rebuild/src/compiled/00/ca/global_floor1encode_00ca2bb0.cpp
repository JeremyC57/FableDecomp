// floor1_encode(vorbis_block*,vorbis_look_floor1*,int*,int*) -- Encode quantized floor posts and render their integer curve.
// Leaf: @floor1_encode@16
// Flags: /TC /Gr
#include "engine/Vorbis.h"
#include <string.h>
#include <stdlib.h>
void __fastcall oggpack_write(oggpack_buffer*,unsigned long,int);
int __fastcall vorbis_book_encode(codebook*,int,oggpack_buffer*);
// Fidelity: Original C file-local helpers retain their source names and local linkage.
static __declspec(noinline) int ilog(unsigned int v){
  int ret=0;
  while(v){
    ret++;
    v>>=1;
  }
  return(ret);
}
static __declspec(noinline) int render_point(int x0,int x1,int y0,int y1,int x){
  y0&=0x7fff; /* mask off flag */
  y1&=0x7fff;
    
  {
    int dy=y1-y0;
    int adx=x1-x0;
    int ady=abs(dy);
    int err=ady*(x-x0);
    
    int off=err/adx;
    if(dy<0)return(y0-off);
    return(y0+off);
  }
}
static void render_line0(int x0,int x1,int y0,int y1,int *d){
  int dy=y1-y0;
  int adx=x1-x0;
  int ady=abs(dy);
  int base=dy/adx;
  int sy=(dy<0?base-1:base+1);
  int x=x0;
  int y=y0;
  int err=0;

  ady-=abs(base*adx);

  d[x]=y;
  while(++x<x1){
    err=err+ady;
    if(err>=adx){
      err-=adx;
      y+=sy;
    }else{
      y+=base;
    }
    d[x]=y;
  }
}
int floor1_encode(vorbis_block *vb,vorbis_look_floor1 *look,
		  int *post,int *ilogmask){

  long i,j;
  vorbis_info_floor1 *info=look->vi;
  long n=look->n;
  long posts=look->posts;
  codec_setup_info *ci=(codec_setup_info*)vb->vd->vi->codec_setup;
  int out[VIF_POSIT+2];
  static_codebook **sbooks=ci->book_param;
  codebook *books=ci->fullbooks;
  static long seq=0; 

  /* quantize values to multiplier spec */
  if(post){
    for(i=0;i<posts;i++){
      int val=post[i]&0x7fff;
      switch(info->mult){
      case 1: /* 1024 -> 256 */
	val>>=2;
	break;
      case 2: /* 1024 -> 128 */
	val>>=3;
	break;
      case 3: /* 1024 -> 86 */
	val/=12;
	break;
      case 4: /* 1024 -> 64 */
	val>>=4;
	break;
      }
      post[i]=val | (post[i]&0x8000);
    }

    out[0]=post[0];
    out[1]=post[1];

    /* find prediction values for each post and subtract them */
    for(i=2;i<posts;i++){
      int ln=look->loneighbor[i-2];
      int hn=look->hineighbor[i-2];
      int x0=info->postlist[ln];
      int x1=info->postlist[hn];
      int y0=post[ln];
      int y1=post[hn];
      
      int predicted=render_point(x0,x1,y0,y1,info->postlist[i]);
      
      if((post[i]&0x8000) || (predicted==post[i])){
	post[i]=predicted|0x8000; /* in case there was roundoff jitter
				     in interpolation */
	out[i]=0;
      }else{
	int headroom=(look->quant_q-predicted<predicted?
		      look->quant_q-predicted:predicted);
	
	int val=post[i]-predicted;
	
	/* at this point the 'deviation' value is in the range +/- max
	   range, but the real, unique range can always be mapped to
	   only [0-maxrange).  So we want to wrap the deviation into
	   this limited range, but do it in the way that least screws
	   an essentially gaussian probability distribution. */
	
	if(val<0)
	  if(val<-headroom)
	    val=headroom-val-1;
	  else
	    val=-1-(val<<1);
	else
	  if(val>=headroom)
	    val= val+headroom;
	  else
	    val<<=1;
	
	out[i]=val;
	post[ln]&=0x7fff;
	post[hn]&=0x7fff;
      }
    }
    
    /* we have everything we need. pack it out */
    /* mark nontrivial floor */
    oggpack_write(&vb->opb,1,1);
      
    /* beginning/end post */
    look->frames++;
    look->postbits+=ilog(look->quant_q-1)*2;
    oggpack_write(&vb->opb,out[0],ilog(look->quant_q-1));
    oggpack_write(&vb->opb,out[1],ilog(look->quant_q-1));
      
      
    /* partition by partition */
    for(i=0,j=2;i<info->partitions;i++){
      int class=info->partitionclass[i];
      int cdim=info->class_dim[class];
      int csubbits=info->class_subs[class];
      int csub=1<<csubbits;
      int bookas[8]={0,0,0,0,0,0,0,0};
      int cval=0;
      int cshift=0;
      int k,l;

      /* generate the partition's first stage cascade value */
      if(csubbits){
	int maxval[8];
	for(k=0;k<csub;k++){
	  int booknum=info->class_subbook[class][k];
	  if(booknum<0){
	    maxval[k]=1;
	  }else{
	    maxval[k]=sbooks[info->class_subbook[class][k]]->entries;
	  }
	}
	for(k=0;k<cdim;k++){
	  for(l=0;l<csub;l++){
	    int val=out[j+k];
	    if(val<maxval[l]){
	      bookas[k]=l;
	      break;
	    }
	  }
	  cval|= bookas[k]<<cshift;
	  cshift+=csubbits;
	}
	/* write it */
	look->phrasebits+=
	  vorbis_book_encode(books+info->class_book[class],cval,&vb->opb);
	

      }
	
      /* write post values */
      for(k=0;k<cdim;k++){
	int book=info->class_subbook[class][bookas[k]];
	if(book>=0){
	  /* hack to allow training with 'bad' books */
	  if(out[j+k]<(books+book)->entries)
	    look->postbits+=vorbis_book_encode(books+book,
					       out[j+k],&vb->opb);
	  /*else
	    fprintf(stderr,"+!");*/
	  

	}
      }
      j+=cdim;
    }
    
    {
      /* generate quantized floor equivalent to what we'd unpack in decode */
      /* render the lines */
      int hx=0;
      int lx=0;
      int ly=post[0]*info->mult;
      for(j=1;j<look->posts;j++){
	int current=look->forward_index[j];
	int hy=post[current]&0x7fff;
	if(hy==post[current]){
	  
	  hy*=info->mult;
	  hx=info->postlist[current];
	
	  render_line0(lx,hx,ly,hy,ilogmask);
	
	  lx=hx;
	  ly=hy;
	}
      }
      for(j=hx;j<vb->pcmend/2;j++)ilogmask[j]=ly; /* be certain */    
      seq++;
      return(1);
    }
  }else{
    oggpack_write(&vb->opb,0,1);
    memset(ilogmask,0,vb->pcmend/2*sizeof(*ilogmask));
    seq++;
    return(0);
  }
}