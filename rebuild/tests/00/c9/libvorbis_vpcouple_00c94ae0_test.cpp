// Behaviour test for libvorbis _vp_couple (0x00C94AE0): the 1.0.1 psy.c body (with its static helpers couple_lossless,
// precomputed_couple_point, unitnorm and the stereo_threshholds / hypot / FLOOR1 tables) on one coupling step -- lines at
// or above the lowpass are zeroed, lines with both magnitudes under the pre-point threshold (or above the point limit and
// under the post-point threshold) take the point-stereo path, the rest the lossless square-polar path; both channels are
// marked nonzero, and an all-zero pair is left untouched.
#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdlib>
#define rint(x) (std::floor((x)+0.5f))
#define P_BANDS 17
#define P_NOISECURVES 3
#define NOISE_COMPAND_LEVELS 40
#define PACKETBLOBS 15
typedef unsigned int ogg_uint32_t;
typedef struct { int blockflag; int normal_point_p; int normal_partition; double normal_thresh; } vorbis_info_psy;
typedef struct { int n; vorbis_info_psy* vi; } vorbis_look_psy;
typedef struct { int coupling_pointlimit[2][PACKETBLOBS]; int coupling_prepointamp[PACKETBLOBS]; int coupling_postpointamp[PACKETBLOBS]; } vorbis_info_psy_global;
typedef struct { int coupling_steps; int coupling_mag[256]; int coupling_ang[256]; } vorbis_info_mapping0;
static float unitnorm(float x){
  ogg_uint32_t *ix=(ogg_uint32_t *)&x;
  *ix=(*ix&0x80000000UL)|(0x3f800000UL);
  return(x);
}
static double stereo_threshholds[]={0.0, .5, 1.0, 1.5, 2.5, 4.5, 8.5, 16.5, 9e10};
static float FLOOR1_fromdB_INV_LOOKUP[256]={
  0.F, 8.81683e+06F, 8.27882e+06F, 7.77365e+06F, 
  7.29930e+06F, 6.85389e+06F, 6.43567e+06F, 6.04296e+06F, 
  5.67422e+06F, 5.32798e+06F, 5.00286e+06F, 4.69759e+06F, 
  4.41094e+06F, 4.14178e+06F, 3.88905e+06F, 3.65174e+06F, 
  3.42891e+06F, 3.21968e+06F, 3.02321e+06F, 2.83873e+06F, 
  2.66551e+06F, 2.50286e+06F, 2.35014e+06F, 2.20673e+06F, 
  2.07208e+06F, 1.94564e+06F, 1.82692e+06F, 1.71544e+06F, 
  1.61076e+06F, 1.51247e+06F, 1.42018e+06F, 1.33352e+06F, 
  1.25215e+06F, 1.17574e+06F, 1.10400e+06F, 1.03663e+06F, 
  973377.F, 913981.F, 858210.F, 805842.F, 
  756669.F, 710497.F, 667142.F, 626433.F, 
  588208.F, 552316.F, 518613.F, 486967.F, 
  457252.F, 429351.F, 403152.F, 378551.F, 
  355452.F, 333762.F, 313396.F, 294273.F, 
  276316.F, 259455.F, 243623.F, 228757.F, 
  214798.F, 201691.F, 189384.F, 177828.F, 
  166977.F, 156788.F, 147221.F, 138237.F, 
  129802.F, 121881.F, 114444.F, 107461.F, 
  100903.F, 94746.3F, 88964.9F, 83536.2F, 
  78438.8F, 73652.5F, 69158.2F, 64938.1F, 
  60975.6F, 57254.9F, 53761.2F, 50480.6F, 
  47400.3F, 44507.9F, 41792.0F, 39241.9F, 
  36847.3F, 34598.9F, 32487.7F, 30505.3F, 
  28643.8F, 26896.0F, 25254.8F, 23713.7F, 
  22266.7F, 20908.0F, 19632.2F, 18434.2F, 
  17309.4F, 16253.1F, 15261.4F, 14330.1F, 
  13455.7F, 12634.6F, 11863.7F, 11139.7F, 
  10460.0F, 9821.72F, 9222.39F, 8659.64F, 
  8131.23F, 7635.06F, 7169.17F, 6731.70F, 
  6320.93F, 5935.23F, 5573.06F, 5232.99F, 
  4913.67F, 4613.84F, 4332.30F, 4067.94F, 
  3819.72F, 3586.64F, 3367.78F, 3162.28F, 
  2969.31F, 2788.13F, 2617.99F, 2458.24F, 
  2308.24F, 2167.39F, 2035.14F, 1910.95F, 
  1794.35F, 1684.85F, 1582.04F, 1485.51F, 
  1394.86F, 1309.75F, 1229.83F, 1154.78F, 
  1084.32F, 1018.15F, 956.024F, 897.687F, 
  842.910F, 791.475F, 743.179F, 697.830F, 
  655.249F, 615.265F, 577.722F, 542.469F, 
  509.367F, 478.286F, 449.101F, 421.696F, 
  395.964F, 371.803F, 349.115F, 327.812F, 
  307.809F, 289.026F, 271.390F, 254.830F, 
  239.280F, 224.679F, 210.969F, 198.096F, 
  186.008F, 174.658F, 164.000F, 153.993F, 
  144.596F, 135.773F, 127.488F, 119.708F, 
  112.404F, 105.545F, 99.1046F, 93.0572F, 
  87.3788F, 82.0469F, 77.0404F, 72.3394F, 
  67.9252F, 63.7804F, 59.8885F, 56.2341F, 
  52.8027F, 49.5807F, 46.5553F, 43.7144F, 
  41.0470F, 38.5423F, 36.1904F, 33.9821F, 
  31.9085F, 29.9614F, 28.1332F, 26.4165F, 
  24.8045F, 23.2910F, 21.8697F, 20.5352F, 
  19.2822F, 18.1056F, 17.0008F, 15.9634F, 
  14.9893F, 14.0746F, 13.2158F, 12.4094F, 
  11.6522F, 10.9411F, 10.2735F, 9.64662F, 
  9.05798F, 8.50526F, 7.98626F, 7.49894F, 
  7.04135F, 6.61169F, 6.20824F, 5.82941F, 
  5.47370F, 5.13970F, 4.82607F, 4.53158F, 
  4.25507F, 3.99542F, 3.75162F, 3.52269F, 
  3.30774F, 3.10590F, 2.91638F, 2.73842F, 
  2.57132F, 2.41442F, 2.26709F, 2.12875F, 
  1.99885F, 1.87688F, 1.76236F, 1.65482F, 
  1.55384F, 1.45902F, 1.36999F, 1.28640F, 
  1.20790F, 1.13419F, 1.06499F, 1.F
};
static void couple_lossless(float A, float B, 
			    float *qA, float *qB){
  int test1=fabs(*qA)>fabs(*qB);
  test1-= fabs(*qA)<fabs(*qB);
  
  if(!test1)test1=((fabs(A)>fabs(B))<<1)-1;
  if(test1==1){
    *qB=(*qA>0.f?*qA-*qB:*qB-*qA);
  }else{
    float temp=*qB;  
    *qB=(*qB>0.f?*qA-*qB:*qB-*qA);
    *qA=temp;
  }

  if(*qB>fabs(*qA)*1.9999f){
    *qB= -fabs(*qA)*2.f;
    *qA= -*qA;
  }
}
static float hypot_lookup[32]={
  -0.009935, -0.011245, -0.012726, -0.014397, 
  -0.016282, -0.018407, -0.020800, -0.023494, 
  -0.026522, -0.029923, -0.033737, -0.038010, 
  -0.042787, -0.048121, -0.054064, -0.060671, 
  -0.068000, -0.076109, -0.085054, -0.094892, 
  -0.105675, -0.117451, -0.130260, -0.144134, 
  -0.159093, -0.175146, -0.192286, -0.210490, 
  -0.229718, -0.249913, -0.271001, -0.292893};
static void precomputed_couple_point(float premag,
				     int floorA,int floorB,
				     float *mag, float *ang){
  
  int test=(floorA>floorB)-1;
  int offset=31-abs(floorA-floorB);
  float floormag=hypot_lookup[((offset<0)-1)&offset]+1.f;

  floormag*=FLOOR1_fromdB_INV_LOOKUP[(floorB&test)|(floorA&(~test))];

  *mag=premag*floormag;
  *ang=0.f;
}
static void _vp_couple(int blobno,
		vorbis_info_psy_global *g,
		vorbis_look_psy *p,
		vorbis_info_mapping0 *vi,
		float **res,
		float **mag_memo,
		int   **mag_sort,
		int   **ifloor,
		int   *nonzero,
		int  sliding_lowpass){

  int i,j,k,n=p->n;

  /* perform any requested channel coupling */
  /* point stereo can only be used in a first stage (in this encoder)
     because of the dependency on floor lookups */
  for(i=0;i<vi->coupling_steps;i++){

    /* once we're doing multistage coupling in which a channel goes
       through more than one coupling step, the floor vector
       magnitudes will also have to be recalculated an propogated
       along with PCM.  Right now, we're not (that will wait until 5.1
       most likely), so the code isn't here yet. The memory management
       here is all assuming single depth couplings anyway. */

    /* make sure coupling a zero and a nonzero channel results in two
       nonzero channels. */
    if(nonzero[vi->coupling_mag[i]] ||
       nonzero[vi->coupling_ang[i]]){
     

      float *rM=res[vi->coupling_mag[i]];
      float *rA=res[vi->coupling_ang[i]];
      float *qM=rM+n;
      float *qA=rA+n;
      int *floorM=ifloor[vi->coupling_mag[i]];
      int *floorA=ifloor[vi->coupling_ang[i]];
      float prepoint=stereo_threshholds[g->coupling_prepointamp[blobno]];
      float postpoint=stereo_threshholds[g->coupling_postpointamp[blobno]];
      int partition=(p->vi->normal_point_p?p->vi->normal_partition:p->n);
      int limit=g->coupling_pointlimit[p->vi->blockflag][blobno];
      int pointlimit=limit;

      nonzero[vi->coupling_mag[i]]=1; 
      nonzero[vi->coupling_ang[i]]=1; 

      for(j=0;j<p->n;j+=partition){
	float acc=0.f;

	for(k=0;k<partition;k++){
	  int l=k+j;

	  if(l<sliding_lowpass){
	    if((l>=limit && fabs(rM[l])<postpoint && fabs(rA[l])<postpoint) ||
	       (fabs(rM[l])<prepoint && fabs(rA[l])<prepoint)){


	      precomputed_couple_point(mag_memo[i][l],
				       floorM[l],floorA[l],
				       qM+l,qA+l);

	      if(rint(qM[l])==0.f)acc+=qM[l]*qM[l];
	    }else{
	      couple_lossless(rM[l],rA[l],qM+l,qA+l);
	    }
	  }else{
	    qM[l]=0.;
	    qA[l]=0.;
	  }
	}
	
	if(p->vi->normal_point_p){
	  for(k=0;k<partition && acc>=p->vi->normal_thresh;k++){
	    int l=mag_sort[i][j+k];
	    if(l<sliding_lowpass && l>=pointlimit && rint(qM[l])==0.f){
	      qM[l]=unitnorm(qM[l]);
	      acc-=1.f;
	    }
	  } 
	}
      }
    }
  }
}


int main()
{
    vorbis_info_psy pvi; std::memset(&pvi, 0, sizeof(pvi)); pvi.normal_point_p = 0;
    vorbis_look_psy p = { 8, &pvi };
    vorbis_info_psy_global g; std::memset(&g, 0, sizeof(g));
    g.coupling_pointlimit[0][3] = 4; g.coupling_prepointamp[3] = 2; g.coupling_postpointamp[3] = 4;   // 1.0 / 2.5
    static vorbis_info_mapping0 vi; vi.coupling_steps = 1; vi.coupling_mag[0] = 0; vi.coupling_ang[0] = 1;
    float m[16], a[16]; float* res[2] = { m, a };
    float in_m[8] = { 0.5f, 3.f, -0.2f, 2.f, 1.5f, 0.4f, 7.f, 7.f }, in_a[8] = { 0.25f, -1.f, 0.3f, 1.f, 2.f, -0.3f, 7.f, 7.f };
    for (int i = 0; i < 16; i++) { m[i] = i < 8 ? in_m[i] : 9.f; a[i] = i < 8 ? in_a[i] : 9.f; }
    float memo[8]; for (int i = 0; i < 8; i++) memo[i] = 10.f + i; float* mag_memo[1] = { memo };
    int fl0[8] = { 10, 20, 30, 40, 50, 60, 70, 80 }, fl1[8] = { 12, 18, 30, 45, 49, 61, 70, 80 }; int* ifloor[2] = { fl0, fl1 };
    int nonzero[2] = { 1, 0 };
    _vp_couple(3, &g, &p, &vi, res, mag_memo, 0, ifloor, nonzero, 6);
    int bad = 0;
    if (nonzero[0] != 1 || nonzero[1] != 1) bad = 1;
    for (int l = 0; l < 8; l++) {
        float qm = 9.f, qa = 9.f;
        if (l >= 6) { qm = 0.f; qa = 0.f; }
        else if ((l >= 4 && std::fabs(in_m[l]) < 2.5f && std::fabs(in_a[l]) < 2.5f) || (std::fabs(in_m[l]) < 1.f && std::fabs(in_a[l]) < 1.f))
            precomputed_couple_point(memo[l], fl0[l], fl1[l], &qm, &qa);
        else couple_lossless(in_m[l], in_a[l], &qm, &qa);
        if (std::fabs(m[8 + l] - qm) > 1e-5f || std::fabs(a[8 + l] - qa) > 1e-5f) bad = 2;
    }
    if (m[8 + 2] == 9.f || a[8 + 1] == 9.f) bad = 3;                       // both paths ran
    // all-zero channels: nothing happens
    nonzero[0] = nonzero[1] = 0; for (int i = 8; i < 16; i++) m[i] = 5.f;
    _vp_couple(3, &g, &p, &vi, res, mag_memo, 0, ifloor, nonzero, 6);
    if (m[8] != 5.f || nonzero[0] || nonzero[1]) bad = 4;
    if (bad) { std::printf("00c94ae0_TEST FAIL %d\n", bad); return 1; }
    std::printf("00c94ae0_TEST PASS\n"); return 0;
}