// lzo1x_999_compress_internal(...) -- Compress with the LZO1X sliding-window matcher.
// Leaf: _lzo1x_999_compress_internal
// Vendor: LZO 1.08 lzo1x_9x.c with original match helpers from lzo_mchw.ch.
// Layout: vendor lzo1x_999_t and lzo1x_999_swd_t; native member offsets corroborate.
typedef unsigned char  lzo_byte;
typedef unsigned int   lzo_uint;
typedef unsigned long  lzo_uint32;
typedef unsigned short swd_uint;
typedef void (__cdecl *lzo_progress_callback_t)(lzo_uint, lzo_uint);

#define SWD_N          0xbfff          /* M4_MAX_OFFSET: size of the ring buffer */
#define SWD_F          2048            /* upper limit for match length */
#define SWD_HSIZE      16384
#define SWD_MAX_CHAIN  2048
#define SWD_BEST_OFF   (34 + 1 - 1)    /* LZO_MAX3(M2_MAX_LEN, M3_MAX_LEN, M4_MAX_LEN) + 1, minus one unused */
#define NIL2           0xffffu

typedef struct
{
    int init;                  // +0x00
    lzo_uint look;             // +0x04
    lzo_uint m_len;            // +0x08
    lzo_uint m_off;            // +0x0c
    lzo_uint last_m_len;       // +0x10
    lzo_uint last_m_off;       // +0x14
    const lzo_byte* bp;        // +0x18
    const lzo_byte* ip;        // +0x1c
    const lzo_byte* in;        // +0x20
    const lzo_byte* in_end;    // +0x24
    lzo_byte* out;             // +0x28
    lzo_progress_callback_t cb;// +0x2c
    lzo_uint textsize;         // +0x30
    lzo_uint codesize;         // +0x34
    lzo_uint printcount;       // +0x38
    unsigned long lit_bytes;   // +0x3c
    unsigned long match_bytes; // +0x40
    unsigned long rep_bytes;   // +0x44
    unsigned long lazy;        // +0x48
    lzo_uint r1_lit;           // +0x4c
    lzo_uint r1_m_len;         // +0x50
    unsigned long m1a_m, m1b_m, m2_m, m3_m, m4_m;   // +0x54..+0x64
    unsigned long lit1_r, lit2_r, lit3_r;           // +0x68..+0x70
} lzo1x_999_t;                 // 0x74

typedef struct
{
    lzo_uint n;                // +0x000
    lzo_uint f;                // +0x004
    lzo_uint threshold;        // +0x008
    lzo_uint max_chain;        // +0x00c
    lzo_uint nice_length;      // +0x010
    int use_best_off;          // +0x014
    lzo_uint lazy_insert;      // +0x018
    lzo_uint m_len;            // +0x01c
    lzo_uint m_off;            // +0x020
    lzo_uint look;             // +0x024
    int b_char;                // +0x028
    lzo_uint best_off[34];     // +0x02c
    lzo1x_999_t* c;            // +0x0b4
    lzo_uint m_pos;            // +0x0b8
    lzo_uint best_pos[34];     // +0x0bc
    const lzo_byte* dict;      // +0x144
    const lzo_byte* dict_end;  // +0x148
    lzo_uint dict_len;         // +0x14c
    lzo_uint ip;               // +0x150
    lzo_uint bp;               // +0x154
    lzo_uint rp;               // +0x158
    lzo_uint b_size;           // +0x15c
    lzo_byte* b_wrap;          // +0x160
    lzo_uint node_count;       // +0x164
    lzo_uint first_rp;         // +0x168
    lzo_byte b[SWD_N + SWD_F + SWD_F];   // +0x16c
    swd_uint head3[SWD_HSIZE];           // +0xd16c
    swd_uint succ3[SWD_N + SWD_F];       // +0x1516c
    swd_uint best3[SWD_N + SWD_F];       // +0x2e16a
    swd_uint llen3[SWD_HSIZE];           // +0x47168
    swd_uint head2[65536L];              // +0x4f168
} lzo1x_999_swd_t;                       // 0x6f168
typedef lzo1x_999_swd_t* lzo_swd_p;

#define HEAD3(b,p) \
    (((0x9f5f*(((((lzo_uint32)b[p]<<5)^b[p+1])<<5)^b[p+2]))>>5) & (SWD_HSIZE-1))
#define HEAD2(b,p)      (* (unsigned short*) &(b[p]))   /* LZO_UNALIGNED_OK_2 */

#define LZO_BYTE(x) ((unsigned char) (x))
#define M1_MAX_OFFSET 0x0400
#define M2_MIN_LEN 3
#define M2_MAX_LEN 8
#define M2_MAX_OFFSET 0x0800
#define MX_MAX_OFFSET (M1_MAX_OFFSET+M2_MAX_OFFSET)
#define M3_MAX_LEN 33
#define M3_MAX_OFFSET 0x4000
#define M4_MAX_LEN 9
#define M4_MAX_OFFSET 0xbfff
#define SWD_THRESHOLD 1
#define LZO_E_OK 0
#define getbyte(c)  ((c).ip < (c).in_end ? *((c).ip)++ : (-1))


#include <string.h>
#define LZO1X
#define NDEBUG
#define LZO_EOF_CODE
#define M1_MARKER 0
#define M2_MARKER 64
#define M3_MARKER 32
#define M4_MARKER 16
#define LZO_E_ERROR (-1)
#define LZO1X_999_MEM_COMPRESS (14*16384L*sizeof(short))
#define LZO_MIN(a,b) ((a) <= (b) ? (a) : (b))
#define lzo_sizeof(x) ((lzo_uint)sizeof(x))
#define LZO_PUBLIC(t) t __cdecl
#define assert(x) ((void)0)
#define THRESHOLD 1
#define SWD_UINT(x) ((swd_uint)(x))
#define SWD_UINT_MAX 65535U
#define s_head3(s,key) s->head3[key]
#define LZO_UNUSED(x) ((void)(x))
#define __inline__ __inline
typedef int lzo_bool;
typedef int lzo_int;
typedef lzo_uint* lzo_uintp;
typedef void* lzo_voidp;
typedef lzo1x_999_t LZO_COMPRESS_T;
typedef lzo1x_999_swd_t lzo_swd_t;
extern "C" {
int __cdecl lzo_assert(int);
static
void swd_initdict(lzo_swd_t *s, const lzo_byte *dict, lzo_uint dict_len)
{
	s->dict = s->dict_end = NULL;
	s->dict_len = 0;

	if (!dict || dict_len <= 0)
		return;
	if (dict_len > s->n)
	{
		dict += dict_len - s->n;
		dict_len = s->n;
	}

	s->dict = dict;
	s->dict_len = dict_len;
	s->dict_end = dict + dict_len;
	memcpy(s->b,dict,dict_len);
	s->ip = dict_len;
}


static
void swd_insertdict(lzo_swd_t *s, lzo_uint node, lzo_uint len)
{
	lzo_uint key;

	s->node_count = s->n - len;
	s->first_rp = node;

	while (len-- > 0)
	{
		key = HEAD3(s->b,node);
		s->succ3[node] = s_head3(s,key);
		s->head3[key] = SWD_UINT(node);
		s->best3[node] = SWD_UINT(s->f + 1);
		s->llen3[key]++;
		assert(s->llen3[key] <= SWD_N);

#ifdef HEAD2
		key = HEAD2(s->b,node);
		s->head2[key] = SWD_UINT(node);
#endif

		node++;
	}
}


/***********************************************************************
//
************************************************************************/

static
int swd_init(lzo_swd_t *s, const lzo_byte *dict, lzo_uint dict_len)
{
	lzo_uint i = 0;
	int c = 0;

#if defined(__LZO_CHECKER)
	s->b = malloc(SWD_N + SWD_F + SWD_F);
	s->head3 = malloc(sizeof(swd_uint) * SWD_HSIZE);
	s->succ3 = malloc(sizeof(swd_uint) * (SWD_N + SWD_F));
	s->best3 = malloc(sizeof(swd_uint) * (SWD_N + SWD_F));
	s->llen3 = malloc(sizeof(swd_uint) * SWD_HSIZE);
#ifdef HEAD2
	s->head2 = malloc(sizeof(swd_uint) * 65536L);
#endif
#endif

	s->n = SWD_N;
	s->f = SWD_F;
	s->threshold = SWD_THRESHOLD;

	/* defaults */
	s->max_chain = SWD_MAX_CHAIN;
	s->nice_length = SWD_F;
	s->use_best_off = 0;
	s->lazy_insert = 0;

	s->b_size = s->n + s->f;
	if (2 * s->f >= s->n || s->b_size + s->f >= SWD_UINT_MAX)
		return LZO_E_ERROR;
	s->b_wrap = s->b + s->b_size;
	s->node_count = s->n;

	memset(s->llen3, 0, sizeof(s->llen3[0]) * SWD_HSIZE);
#ifdef HEAD2
#if 1
	memset(s->head2, 0xff, sizeof(s->head2[0]) * 65536L);
	assert(s->head2[0] == NIL2);
#else
	for (i = 0; i < 65536L; i++)
		s->head2[i] = NIL2;
#endif
#endif

	s->ip = 0;
	swd_initdict(s,dict,dict_len);
	s->bp = s->ip;
	s->first_rp = s->ip;

	assert(s->ip + s->f <= s->b_size);
#if 1
	s->look = (lzo_uint) (s->c->in_end - s->c->ip);
	if (s->look > 0)
	{
		if (s->look > s->f)
			s->look = s->f;
		memcpy(&s->b[s->ip],s->c->ip,s->look);
		s->c->ip += s->look;
		s->ip += s->look;
	}
#else
	s->look = 0;
	while (s->look < s->f)
	{
		if ((c = getbyte(*(s->c))) < 0)
			break;
		s->b[s->ip] = LZO_BYTE(c);
		s->ip++;
		s->look++;
	}
#endif
	if (s->ip == s->b_size)
		s->ip = 0;

	if (s->look >= 2 && s->dict_len > 0)
		swd_insertdict(s,0,s->dict_len);

	s->rp = s->first_rp;
	if (s->rp >= s->node_count)
		s->rp -= s->node_count;
	else
		s->rp += s->b_size - s->node_count;

#if defined(__LZO_CHECKER)
	/* initialize memory for the first few HEAD3 (if s->ip is not far
	 * enough ahead to do this job for us). The value doesn't matter. */
	if (s->look < 3)
		memset(&s->b[s->bp+s->look],0,3);
#endif

	LZO_UNUSED(i);
	LZO_UNUSED(c);
	return LZO_E_OK;
}


static
void swd_exit(lzo_swd_t *s)
{
#if defined(__LZO_CHECKER)
	/* free in reverse order of allocations */
#ifdef HEAD2
	free(s->head2); s->head2 = NULL;
#endif
	free(s->llen3); s->llen3 = NULL;
	free(s->best3); s->best3 = NULL;
	free(s->succ3); s->succ3 = NULL;
	free(s->head3); s->head3 = NULL;
	free(s->b); s->b = NULL;
#else
	LZO_UNUSED(s);
#endif
}


#define swd_pos2off(s,pos) \
	(s->bp > (pos) ? s->bp - (pos) : s->b_size - ((pos) - s->bp))


/***********************************************************************
//
************************************************************************/

static __inline__
void swd_getbyte(lzo_swd_t *s)
{
	int c;

	if ((c = getbyte(*(s->c))) < 0)
	{
		if (s->look > 0)
			--s->look;
#if defined(__LZO_CHECKER)
		/* initialize memory - value doesn't matter */
		s->b[s->ip] = 0;
		if (s->ip < s->f)
			s->b_wrap[s->ip] = 0;
#endif
	}
	else
	{
		s->b[s->ip] = LZO_BYTE(c);
		if (s->ip < s->f)
			s->b_wrap[s->ip] = LZO_BYTE(c);
	}
	if (++s->ip == s->b_size)
		s->ip = 0;
	if (++s->bp == s->b_size)
		s->bp = 0;
	if (++s->rp == s->b_size)
		s->rp = 0;
}


/***********************************************************************
// remove node from lists
************************************************************************/

static __inline__
void swd_remove_node(lzo_swd_t *s, lzo_uint node)
{
	if (s->node_count == 0)
	{
		lzo_uint key;

#ifdef LZO_DEBUG
		if (s->first_rp != LZO_UINT_MAX)
		{
			if (node != s->first_rp)
				printf("Remove %5d: %5d %5d %5d %5d  %6d %6d\n",
				        node, s->rp, s->ip, s->bp, s->first_rp,
				        s->ip - node, s->ip - s->bp);
			assert(node == s->first_rp);
			s->first_rp = LZO_UINT_MAX;
		}
#endif

		key = HEAD3(s->b,node);
		assert(s->llen3[key] > 0);
		--s->llen3[key];

#ifdef HEAD2
		key = HEAD2(s->b,node);
		assert(s->head2[key] != NIL2);
		if ((lzo_uint) s->head2[key] == node)
			s->head2[key] = NIL2;
#endif
	}
	else
		--s->node_count;
}


/***********************************************************************
//
************************************************************************/

static
void swd_accept(lzo_swd_t *s, lzo_uint n)
{
	assert(n <= s->look);

	while (n--)
	{
		lzo_uint key;

		swd_remove_node(s,s->rp);

		/* add bp into HEAD3 */
		key = HEAD3(s->b,s->bp);
		s->succ3[s->bp] = s_head3(s,key);
		s->head3[key] = SWD_UINT(s->bp);
		s->best3[s->bp] = SWD_UINT(s->f + 1);
		s->llen3[key]++;
		assert(s->llen3[key] <= SWD_N);

#ifdef HEAD2
		/* add bp into HEAD2 */
		key = HEAD2(s->b,s->bp);
		s->head2[key] = SWD_UINT(s->bp);
#endif

		swd_getbyte(s);
	}
}


/***********************************************************************
//
************************************************************************/

static
void swd_search(lzo_swd_t *s, lzo_uint node, lzo_uint cnt)
{
const unsigned char *p1;
const unsigned char *p2;
const unsigned char *px;
	lzo_uint m_len = s->m_len;
	const unsigned char * b  = s->b;
	const unsigned char * bp = s->b + s->bp;
	const unsigned char * bx = s->b + s->bp + s->look;
	unsigned char scan_end1;

	assert(s->m_len > 0);

	scan_end1 = bp[m_len - 1];
	for ( ; cnt-- > 0; node = s->succ3[node])
	{
		p1 = bp;
		p2 = b + node;
		px = bx;

		assert(m_len < s->look);

		if (
#if 1
		    p2[m_len - 1] == scan_end1 &&
		    p2[m_len] == p1[m_len] &&
#endif
		    p2[0] == p1[0] &&
		    p2[1] == p1[1])
		{
			lzo_uint i;
			assert(memcmp(bp,&b[node],3) == 0);

#if 0 && defined(LZO_UNALIGNED_OK_4)
			p1 += 3; p2 += 3;
			while (p1 < px && * (const lzo_uint32p) p1 == * (const lzo_uint32p) p2)
				p1 += 4, p2 += 4;
			while (p1 < px && *p1 == *p2)
				p1 += 1, p2 += 1;
#else
			p1 += 2; p2 += 2;
			do {} while (++p1 < px && *p1 == *++p2);
#endif
			i = p1 - bp;

#ifdef LZO_DEBUG
			if (memcmp(bp,&b[node],i) != 0)
				printf("%5ld %5ld %02x%02x %02x%02x\n",
				        (long)s->bp, (long) node,
				        bp[0], bp[1], b[node], b[node+1]);
#endif
			assert(memcmp(bp,&b[node],i) == 0);

#if defined(SWD_BEST_OFF)
			if (i < SWD_BEST_OFF)
			{
				if (s->best_pos[i] == 0)
					s->best_pos[i] = node + 1;
			}
#endif
			if (i > m_len)
			{
				s->m_len = m_len = i;
				s->m_pos = node;
				if (m_len == s->look)
					return;
				if (m_len >= s->nice_length)
					return;
				if (m_len > (lzo_uint) s->best3[node])
					return;
				scan_end1 = bp[m_len - 1];
			}
		}
	}
}


/***********************************************************************
//
************************************************************************/

#ifdef HEAD2

static
lzo_bool swd_search2(lzo_swd_t *s)
{
	lzo_uint key;

	assert(s->look >= 2);
	assert(s->m_len > 0);

	key = s->head2[ HEAD2(s->b,s->bp) ];
	if (key == NIL2)
		return 0;
#ifdef LZO_DEBUG
	if (memcmp(&s->b[s->bp],&s->b[key],2) != 0)
		printf("%5ld %5ld %02x%02x %02x%02x\n", (long)s->bp, (long)key,
		        s->b[s->bp], s->b[s->bp+1], s->b[key], s->b[key+1]);
#endif
	assert(memcmp(&s->b[s->bp],&s->b[key],2) == 0);
#if defined(SWD_BEST_OFF)
	if (s->best_pos[2] == 0)
		s->best_pos[2] = key + 1;
#endif

	if (s->m_len < 2)
	{
		s->m_len = 2;
		s->m_pos = key;
	}
	return 1;
}

#endif


/***********************************************************************
//
************************************************************************/

static
void swd_findbest(lzo_swd_t *s)
{
	lzo_uint key;
	lzo_uint cnt, node;
	lzo_uint len;

	assert(s->m_len > 0);

	/* get current head, add bp into HEAD3 */
	key = HEAD3(s->b,s->bp);
	node = s->succ3[s->bp] = s_head3(s,key);
	cnt = s->llen3[key]++;
	assert(s->llen3[key] <= SWD_N + SWD_F);
	if (cnt > s->max_chain && s->max_chain > 0)
		cnt = s->max_chain;
	s->head3[key] = SWD_UINT(s->bp);

	s->b_char = s->b[s->bp];
	len = s->m_len;
	if (s->m_len >= s->look)
	{
		if (s->look == 0)
			s->b_char = -1;
		s->m_off = 0;
		s->best3[s->bp] = SWD_UINT(s->f + 1);
	}
	else
	{
#ifdef HEAD2
		if (swd_search2(s))
#endif
			if (s->look >= 3)
				swd_search(s,node,cnt);
		if (s->m_len > len)
			s->m_off = swd_pos2off(s,s->m_pos);
		s->best3[s->bp] = SWD_UINT(s->m_len);

#if defined(SWD_BEST_OFF)
		if (s->use_best_off)
		{
			int i;
			for (i = 2; i < SWD_BEST_OFF; i++)
				if (s->best_pos[i] > 0)
					s->best_off[i] = swd_pos2off(s,s->best_pos[i]-1);
				else
					s->best_off[i] = 0;
		}
#endif
	}

	swd_remove_node(s,s->rp);

#ifdef HEAD2
	/* add bp into HEAD2 */
	key = HEAD2(s->b,s->bp);
	s->head2[key] = SWD_UINT(s->bp);
#endif
}


static __declspec(noinline) int init_match(LZO_COMPRESS_T*,lzo_swd_t*,const lzo_byte*,lzo_uint,lzo_uint32);
static int find_match(LZO_COMPRESS_T*,lzo_swd_t*,lzo_uint,lzo_uint);
static __declspec(noinline) lzo_byte* code_match(LZO_COMPRESS_T*,lzo_byte*,lzo_uint,lzo_uint);
static __declspec(noinline) lzo_byte* STORE_RUN(LZO_COMPRESS_T*,lzo_byte*,const lzo_byte*,lzo_uint);
static __declspec(noinline) int len_of_coded_match(lzo_uint,lzo_uint,lzo_uint);
static __declspec(noinline) void better_match(const lzo_swd_t*,lzo_uint*,lzo_uint*);
static int
init_match ( LZO_COMPRESS_T *c, lzo_swd_t *s,
			 const lzo_byte *dict, lzo_uint dict_len,
			 lzo_uint32 flags )
{
	int r;

	assert(!c->init);
	c->init = 1;

	s->c = c;

	c->last_m_len = c->last_m_off = 0;

	c->textsize = c->codesize = c->printcount = 0;
	c->lit_bytes = c->match_bytes = c->rep_bytes = 0;
	c->lazy = 0;

	r = swd_init(s,dict,dict_len);
	if (r != 0)
		return r;

	s->use_best_off = (flags & 1) ? 1 : 0;
	return r;
}


/***********************************************************************
//
************************************************************************/

static int
find_match ( LZO_COMPRESS_T *c, lzo_swd_t *s,
			 lzo_uint this_len, lzo_uint skip )
{
	assert(c->init);

	if (skip > 0)
	{
		assert(this_len >= skip);
		swd_accept(s, this_len - skip);
		c->textsize += this_len - skip + 1;
	}
	else
	{
		assert(this_len <= 1);
		c->textsize += this_len - skip;
	}

	s->m_len = 1;
	s->m_len = THRESHOLD;
#ifdef SWD_BEST_OFF
	if (s->use_best_off)
		memset(s->best_pos,0,sizeof(s->best_pos));
#endif
	swd_findbest(s);
	c->m_len = s->m_len;
	c->m_off = s->m_off;

	swd_getbyte(s);

	if (s->b_char < 0)
	{
		c->look = 0;
		c->m_len = 0;
		swd_exit(s);
	}
	else
	{
		c->look = s->look + 1;
	}
	c->bp = c->ip - c->look;

#if 0
	/* brute force match search */
	if (c->m_len > THRESHOLD && c->m_len + 1 <= c->look)
	{
		const lzo_byte *ip = c->bp;
		const lzo_byte *m  = c->bp - c->m_off;
		const lzo_byte *in = c->in;

		if (ip - in > N)
			in = ip - N;
		for (;;)
		{
			while (*in != *ip)
				in++;
			if (in == ip)
				break;
			if (in != m)
				if (memcmp(in,ip,c->m_len+1) == 0)
					printf("%p %p %p %5d\n",in,ip,m,c->m_len);
			in++;
		}
	}
#endif

	if (c->cb && c->textsize > c->printcount)
	{
		(*c->cb)(c->textsize,c->codesize);
		c->printcount += 1024;
	}

	return LZO_E_OK;
}


static lzo_byte *
code_match ( LZO_COMPRESS_T *c, lzo_byte *op, lzo_uint m_len, lzo_uint m_off )
{
	lzo_uint x_len = m_len;
	lzo_uint x_off = m_off;

	c->match_bytes += m_len;

#if 0
/*
	static lzo_uint last_m_len = 0, last_m_off = 0;
	static lzo_uint prev_m_off[4];
	static int prev_m_off_ptr = 0;
	int i;

	//if (m_len >= 3 && m_len <= M2_MAX_LEN && m_off <= M2_MAX_OFFSET)
	if (m_len >= 3 && m_len <= M2_MAX_LEN)
	{
	//if (m_len == last_m_len && m_off == last_m_off)
		//printf("last_m_len + last_m_off\n");
	//else
	if (m_off == last_m_off)
		printf("last_m_off\n");
	else
	{
		for (i = 0; i < 4; i++)
			if (m_off == prev_m_off[i])
				printf("prev_m_off %d: %5ld\n",i,(long)m_off);
	}
	}
	last_m_len = m_len;
	last_m_off = prev_m_off[prev_m_off_ptr] = m_off;
	prev_m_off_ptr = (prev_m_off_ptr + 1) & 3;
*/
#endif

	assert(op > c->out);
	if (m_len == 2)
	{
		assert(m_off <= M1_MAX_OFFSET);
		assert(c->r1_lit > 0); assert(c->r1_lit < 4);
		m_off -= 1;
#if defined(LZO1Z)
		*op++ = LZO_BYTE(M1_MARKER | (m_off >> 6));
		*op++ = LZO_BYTE(m_off << 2);
#else
		*op++ = LZO_BYTE(M1_MARKER | ((m_off & 3) << 2));
		*op++ = LZO_BYTE(m_off >> 2);
#endif
		c->m1a_m++;
	}
#if defined(LZO1Z)
	else if (m_len <= M2_MAX_LEN && (m_off <= M2_MAX_OFFSET || m_off == c->last_m_off))
#else
	else if (m_len <= M2_MAX_LEN && m_off <= M2_MAX_OFFSET)
#endif
	{
		assert(m_len >= 3);
#if defined(LZO1X)
		m_off -= 1;
		*op++ = LZO_BYTE(((m_len - 1) << 5) | ((m_off & 7) << 2));
		*op++ = LZO_BYTE(m_off >> 3);
		assert(op[-2] >= M2_MARKER);
#elif defined(LZO1Y)
		m_off -= 1;
		*op++ = LZO_BYTE(((m_len + 1) << 4) | ((m_off & 3) << 2));
		*op++ = LZO_BYTE(m_off >> 2);
		assert(op[-2] >= M2_MARKER);
#elif defined(LZO1Z)
		if (m_off == c->last_m_off)
			*op++ = LZO_BYTE(((m_len - 1) << 5) | (0x700 >> 6));
		else
		{
			m_off -= 1;
			*op++ = LZO_BYTE(((m_len - 1) << 5) | (m_off >> 6));
			*op++ = LZO_BYTE(m_off << 2);
		}
#endif
		c->m2_m++;
	}
	else if (m_len == M2_MIN_LEN && m_off <= MX_MAX_OFFSET && c->r1_lit >= 4)
	{
		assert(m_len == 3);
		assert(m_off > M2_MAX_OFFSET);
		m_off -= 1 + M2_MAX_OFFSET;
#if defined(LZO1Z)
		*op++ = LZO_BYTE(M1_MARKER | (m_off >> 6));
		*op++ = LZO_BYTE(m_off << 2);
#else
		*op++ = LZO_BYTE(M1_MARKER | ((m_off & 3) << 2));
		*op++ = LZO_BYTE(m_off >> 2);
#endif
		c->m1b_m++;
	}
	else if (m_off <= M3_MAX_OFFSET)
	{
		assert(m_len >= 3);
		m_off -= 1;
		if (m_len <= M3_MAX_LEN)
			*op++ = LZO_BYTE(M3_MARKER | (m_len - 2));
		else
		{
			m_len -= M3_MAX_LEN;
			*op++ = M3_MARKER | 0;
			while (m_len > 255)
			{
				m_len -= 255;
				*op++ = 0;
			}
			assert(m_len > 0);
			*op++ = LZO_BYTE(m_len);
		}
#if defined(LZO1Z)
		*op++ = LZO_BYTE(m_off >> 6);
		*op++ = LZO_BYTE(m_off << 2);
#else
		*op++ = LZO_BYTE(m_off << 2);
		*op++ = LZO_BYTE(m_off >> 6);
#endif
		c->m3_m++;
	}
	else
	{
		lzo_uint k;

		assert(m_len >= 3);
		assert(m_off > 0x4000); assert(m_off <= 0xbfff);
		m_off -= 0x4000;
		k = (m_off & 0x4000) >> 11;
		if (m_len <= M4_MAX_LEN)
			*op++ = LZO_BYTE(M4_MARKER | k | (m_len - 2));
		else
		{
			m_len -= M4_MAX_LEN;
			*op++ = LZO_BYTE(M4_MARKER | k | 0);
			while (m_len > 255)
			{
				m_len -= 255;
				*op++ = 0;
			}
			assert(m_len > 0);
			*op++ = LZO_BYTE(m_len);
		}
#if defined(LZO1Z)
		*op++ = LZO_BYTE(m_off >> 6);
		*op++ = LZO_BYTE(m_off << 2);
#else
		*op++ = LZO_BYTE(m_off << 2);
		*op++ = LZO_BYTE(m_off >> 6);
#endif
		c->m4_m++;
	}

	c->last_m_len = x_len;
	c->last_m_off = x_off;
	return op;
}


static lzo_byte *
STORE_RUN ( LZO_COMPRESS_T *c, lzo_byte *op, const lzo_byte *ii, lzo_uint t )
{
	c->lit_bytes += t;

	if (op == c->out && t <= 238)
	{
		*op++ = LZO_BYTE(17 + t);
	}
	else if (t <= 3)
	{
#if defined(LZO1Z)
		op[-1] |= LZO_BYTE(t);
#else
		op[-2] |= LZO_BYTE(t);
#endif
		c->lit1_r++;
	}
	else if (t <= 18)
	{
		*op++ = LZO_BYTE(t - 3);
		c->lit2_r++;
	}
	else
	{
		lzo_uint tt = t - 18;

		*op++ = 0;
		while (tt > 255)
		{
			tt -= 255;
			*op++ = 0;
		}
		assert(tt > 0);
		*op++ = LZO_BYTE(tt);
		c->lit3_r++;
	}
	do *op++ = *ii++; while (--t > 0);

	return op;
}


static lzo_byte *
code_run ( LZO_COMPRESS_T *c, lzo_byte *op, const lzo_byte *ii,
           lzo_uint lit, lzo_uint m_len )
{
	if (lit > 0)
	{
		assert(m_len >= 2);
		op = STORE_RUN(c,op,ii,lit);
		c->r1_m_len = m_len;
		c->r1_lit = lit;
	}
	else
	{
		assert(m_len >= 3);
		c->r1_m_len = 0;
		c->r1_lit = 0;
	}

	return op;
}


/***********************************************************************
//
************************************************************************/

static int
len_of_coded_match ( lzo_uint m_len, lzo_uint m_off, lzo_uint lit )
{
	int n = 4;

	if (m_len < 2)
		return -1;
	if (m_len == 2)
		return (m_off <= M1_MAX_OFFSET && lit > 0 && lit < 4) ? 2 : -1;
	if (m_len <= M2_MAX_LEN && m_off <= M2_MAX_OFFSET)
		return 2;
	if (m_len == M2_MIN_LEN && m_off <= MX_MAX_OFFSET && lit >= 4)
		return 2;
	if (m_off <= M3_MAX_OFFSET)
	{
		if (m_len <= M3_MAX_LEN)
			return 3;
		m_len -= M3_MAX_LEN;
		while (m_len > 255)
		{
			m_len -= 255;
			n++;
		}
		return n;
	}
	if (m_off <= M4_MAX_OFFSET)
	{
		if (m_len <= M4_MAX_LEN)
			return 3;
		m_len -= M4_MAX_LEN;
		while (m_len > 255)
		{
			m_len -= 255;
			n++;
		}
		return n;
	}
	return -1;
}


static lzo_int
min_gain(lzo_uint ahead, lzo_uint lit1, lzo_uint lit2, int l1, int l2, int l3)
{
	lzo_int lazy_match_min_gain = 0;

	assert (ahead >= 1);
	lazy_match_min_gain += ahead;

#if 0
	if (l3 > 0)
		lit2 -= ahead;
#endif

	if (lit1 <= 3)
		lazy_match_min_gain += (lit2 <= 3) ? 0 : 2;
	else if (lit1 <= 18)
		lazy_match_min_gain += (lit2 <= 18) ? 0 : 1;

	lazy_match_min_gain += (l2 - l1) * 2;
	if (l3 > 0)
		lazy_match_min_gain -= (ahead - l3) * 2;

	if (lazy_match_min_gain < 0)
		lazy_match_min_gain = 0;

#if 0
	if (l1 == 2)
		if (lazy_match_min_gain == 0)
			lazy_match_min_gain = 1;
#endif

	return lazy_match_min_gain;
}


/***********************************************************************
//
************************************************************************/

#if !defined(NDEBUG)
static
void assert_match( const lzo_swd_t *swd, lzo_uint m_len, lzo_uint m_off )
{
	const LZO_COMPRESS_T *c = swd->c;
	lzo_uint d_off;

	assert(m_len >= 2);
	if (m_off <= (lzo_uint) (c->bp - c->in))
	{
		assert(c->bp - m_off + m_len < c->ip);
		assert(lzo_memcmp(c->bp, c->bp - m_off, m_len) == 0);
	}
	else
	{
		assert(swd->dict != NULL);
		d_off = m_off - (lzo_uint) (c->bp - c->in);
		assert(d_off <= swd->dict_len);
		if (m_len > d_off)
		{
			assert(lzo_memcmp(c->bp, swd->dict_end - d_off, d_off) == 0);
			assert(c->in + m_len - d_off < c->ip);
			assert(lzo_memcmp(c->bp + d_off, c->in, m_len - d_off) == 0);
		}
		else
		{
			assert(lzo_memcmp(c->bp, swd->dict_end - d_off, m_len) == 0);
		}
	}
}
#else
#  define assert_match(a,b,c)	((void)0)
#endif


#if defined(SWD_BEST_OFF)

static void
better_match ( const lzo_swd_t *swd, lzo_uint *m_len, lzo_uint *m_off )
{
#if defined(LZO1Z)
	const LZO_COMPRESS_T *c = swd->c;
#endif

	if (*m_len <= M2_MIN_LEN)
		return;
#if defined(LZO1Z)
	if (*m_off == c->last_m_off && *m_len <= M2_MAX_LEN)
		return;
#if 1
	if (*m_len >= M2_MIN_LEN + 1 && *m_len <= M2_MAX_LEN + 1 &&
		c->last_m_off && swd->best_off[*m_len-1] == c->last_m_off)
	{
		*m_len = *m_len - 1;
		*m_off = swd->best_off[*m_len];
		return;
	}
#endif
#endif

	if (*m_off <= M2_MAX_OFFSET)
		return;

#if 1
	/* M3/M4 -> M2 */
	if (*m_off > M2_MAX_OFFSET &&
	    *m_len >= M2_MIN_LEN + 1 && *m_len <= M2_MAX_LEN + 1 &&
	    swd->best_off[*m_len-1] && swd->best_off[*m_len-1] <= M2_MAX_OFFSET)
	{
		*m_len = *m_len - 1;
		*m_off = swd->best_off[*m_len];
		return;
	}
#endif

#if 1
	/* M4 -> M2 */
	if (*m_off > M3_MAX_OFFSET &&
	    *m_len >= M4_MAX_LEN + 1 && *m_len <= M2_MAX_LEN + 2 &&
	    swd->best_off[*m_len-2] && swd->best_off[*m_len-2] <= M2_MAX_OFFSET)
	{
		*m_len = *m_len - 2;
		*m_off = swd->best_off[*m_len];
		return;
	}
#endif

#if 1
	/* M4 -> M3 */
	if (*m_off > M3_MAX_OFFSET &&
	    *m_len >= M4_MAX_LEN + 1 && *m_len <= M3_MAX_LEN + 1 &&
	    swd->best_off[*m_len-1] && swd->best_off[*m_len-1] <= M3_MAX_OFFSET)
	{
		*m_len = *m_len - 1;
		*m_off = swd->best_off[*m_len];
	}
#endif
}

#endif


/***********************************************************************
//
************************************************************************/

LZO_PUBLIC(int)
lzo1x_999_compress_internal ( const lzo_byte *in , lzo_uint  in_len,
                                    lzo_byte *out, lzo_uintp out_len,
                                    lzo_voidp wrkmem,
                              const lzo_byte *dict, lzo_uint dict_len,
                                    lzo_progress_callback_t cb,
                                    int try_lazy,
                                    lzo_uint good_length,
                                    lzo_uint max_lazy,
                                    lzo_uint nice_length,
                                    lzo_uint max_chain,
                                    lzo_uint32 flags )
{
	lzo_byte *op;
	const lzo_byte *ii;
	lzo_uint lit;
	lzo_uint m_len, m_off;
	LZO_COMPRESS_T cc;
	LZO_COMPRESS_T * const c = &cc;
	lzo_swd_t * const swd = (lzo_swd_t *) wrkmem;
	int r;

#if defined(__LZO_QUERY_COMPRESS)
	if (__LZO_IS_COMPRESS_QUERY(in,in_len,out,out_len,wrkmem))
		return __LZO_QUERY_COMPRESS(in,in_len,out,out_len,wrkmem,1,lzo_sizeof(lzo_swd_t));
#endif

	/* sanity check */
	if (!lzo_assert(LZO1X_999_MEM_COMPRESS >= lzo_sizeof(lzo_swd_t)))
		return LZO_E_ERROR;

/* setup parameter defaults */
	/* number of lazy match tries */
	if (try_lazy < 0)
		try_lazy = 1;
	/* reduce lazy match search if we already have a match with this length */
	if (good_length <= 0)
		good_length = 32;
	/* do not try a lazy match if we already have a match with this length */
	if (max_lazy <= 0)
		max_lazy = 32;
	/* stop searching for longer matches than this one */
	if (nice_length <= 0)
		nice_length = 0;
	/* don't search more positions than this */
	if (max_chain <= 0)
		max_chain = SWD_MAX_CHAIN;

	c->init = 0;
	c->ip = c->in = in;
	c->in_end = in + in_len;
	c->out = out;
	c->cb = cb;
	c->m1a_m = c->m1b_m = c->m2_m = c->m3_m = c->m4_m = 0;
	c->lit1_r = c->lit2_r = c->lit3_r = 0;

	op = out;
	ii = c->ip;				/* point to start of literal run */
	lit = 0;
	c->r1_lit = c->r1_m_len = 0;

	r = init_match(c,swd,dict,dict_len,flags);
	if (r != 0)
		return r;
	if (max_chain > 0)
		swd->max_chain = max_chain;
	if (nice_length > 0)
		swd->nice_length = nice_length;

	r = find_match(c,swd,0,0);
	if (r != 0)
		return r;
	while (c->look > 0)
	{
		lzo_uint ahead;
		lzo_uint max_ahead;
		int l1, l2, l3;

		c->codesize = op - out;

		m_len = c->m_len;
		m_off = c->m_off;

		assert(c->bp == c->ip - c->look);
		assert(c->bp >= in);
		if (lit == 0)
			ii = c->bp;
		assert(ii + lit == c->bp);
		assert(swd->b_char == *(c->bp));

		if ( m_len < 2 ||
		    (m_len == 2 && (m_off > M1_MAX_OFFSET || lit == 0 || lit >= 4)) ||
#if 1
			/* Do not accept this match for compressed-data compatibility
			 * with LZO v1.01 and before
			 * [ might be a problem for decompress() and optimize() ]
			 */
		    (m_len == 2 && op == out) ||
#endif
			(op == out && lit == 0))
		{
			/* a literal */
			m_len = 0;
		}
		else if (m_len == M2_MIN_LEN)
		{
			/* compression ratio improves if we code a literal in some cases */
			if (m_off > MX_MAX_OFFSET && lit >= 4)
				m_len = 0;
		}

		if (m_len == 0)
		{
	/* a literal */
			lit++;
			swd->max_chain = max_chain;
			r = find_match(c,swd,1,0);
			assert(r == 0);
			continue;
		}

	/* a match */
#if defined(SWD_BEST_OFF)
		if (swd->use_best_off)
			better_match(swd,&m_len,&m_off);
#endif
		assert_match(swd,m_len,m_off);



		/* shall we try a lazy match ? */
		ahead = 0;
		if (try_lazy <= 0 || m_len >= max_lazy)
		{
			/* no */
			l1 = 0;
			max_ahead = 0;
		}
		else
		{
			/* yes, try a lazy match */
			l1 = len_of_coded_match(m_len,m_off,lit);
			assert(l1 > 0);
#if 1
			max_ahead = LZO_MIN(try_lazy, l1 - 1);
#else
			max_ahead = LZO_MIN3(try_lazy, l1, m_len - 1);
#endif
		}


		while (ahead < max_ahead && c->look > m_len)
		{
			lzo_int lazy_match_min_gain;

			if (m_len >= good_length)
				swd->max_chain = max_chain >> 2;
			else
				swd->max_chain = max_chain;
			r = find_match(c,swd,1,0);
			ahead++;

			assert(r == 0);
			assert(c->look > 0);
			assert(ii + lit + ahead == c->bp);

#if defined(LZO1Z)
			if (m_off == c->last_m_off && c->m_off != c->last_m_off)
				if (m_len >= M2_MIN_LEN && m_len <= M2_MAX_LEN)
					c->m_len = 0;
#endif
			if (c->m_len < m_len)
				continue;
#if 1
			if (c->m_len == m_len && c->m_off >= m_off)
				continue;
#endif
#if defined(SWD_BEST_OFF)
			if (swd->use_best_off)
				better_match(swd,&c->m_len,&c->m_off);
#endif
			l2 = len_of_coded_match(c->m_len,c->m_off,lit+ahead);
			if (l2 < 0)
				continue;
#if 0
			if (c->m_len == m_len && l2 >= l1)
				continue;
#endif


#if 1
			/* compressed-data compatibility [see above] */
			l3 = (op == out) ? -1 : len_of_coded_match(ahead,m_off,lit);
#else
			l3 = len_of_coded_match(ahead,m_off,lit);
#endif

			lazy_match_min_gain = min_gain(ahead,lit,lit+ahead,l1,l2,l3);
			if (c->m_len >= m_len + lazy_match_min_gain)
			{
				c->lazy++;
				assert_match(swd,c->m_len,c->m_off);

				if (l3 > 0)
				{
					/* code previous run */
					op = code_run(c,op,ii,lit,ahead);
					lit = 0;
					/* code shortened match */
					op = code_match(c,op,ahead,m_off);
				}
				else
				{
					lit += ahead;
					assert(ii + lit == c->bp);
				}
				goto lazy_match_done;
			}
		}


		assert(ii + lit + ahead == c->bp);

		/* 1 - code run */
		op = code_run(c,op,ii,lit,m_len);
		lit = 0;

		/* 2 - code match */
		op = code_match(c,op,m_len,m_off);
		swd->max_chain = max_chain;
		r = find_match(c,swd,m_len,1+ahead);
		assert(r == 0);

lazy_match_done: ;
	}


	/* store final run */
	if (lit > 0)
		op = STORE_RUN(c,op,ii,lit);

#if defined(LZO_EOF_CODE)
	*op++ = M4_MARKER | 1;
	*op++ = 0;
	*op++ = 0;
#endif

	c->codesize = op - out;
	assert(c->textsize == in_len);

	*out_len = op - out;

	if (c->cb)
		(*c->cb)(c->textsize,c->codesize);

#if 0
	printf("%ld %ld -> %ld  %ld: %ld %ld %ld %ld %ld  %ld: %ld %ld %ld  %ld\n",
		(long) c->textsize, (long) in_len, (long) c->codesize,
		c->match_bytes, c->m1a_m, c->m1b_m, c->m2_m, c->m3_m, c->m4_m,
		c->lit_bytes, c->lit1_r, c->lit2_r, c->lit3_r, c->lazy);
#endif
	assert(c->lit_bytes + c->match_bytes == in_len);

	return LZO_E_OK;
}


/***********************************************************************
//
************************************************************************/


}