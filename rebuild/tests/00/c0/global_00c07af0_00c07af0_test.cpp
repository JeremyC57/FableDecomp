#if !defined(_MSC_VER)
#define __cdecl
#define __declspec(x)
#endif
// 00c07af0 lzo1x_999_compress_internal
// Flags:
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

#include <stdio.h>
#include <stdlib.h>
extern "C" int __cdecl lzo_assert(int expr){return expr ? 1 : 0;}
typedef lzo_uint lzo_moff_t;
#define LZO_TEST_DECOMPRESS_OVERRUN
#define HAVE_MEMCPY
#define LZO_E_EOF_NOT_FOUND -7
#define LZO_E_INPUT_NOT_CONSUMED -8
#define LZO_E_INPUT_OVERRUN -4
#define LZO_E_OUTPUT_OVERRUN -5
#define LZO_E_LOOKBEHIND_OVERRUN -6
/* lzo1_d.ch -- common decompression stuff

   This file is part of the LZO real-time data compression library.

   Copyright (C) 1996-2002 Markus Franz Xaver Johannes Oberhumer
   All Rights Reserved.

   The LZO library is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   The LZO library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the LZO library; see the file COPYING.
   If not, write to the Free Software Foundation, Inc.,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

   Markus F.X.J. Oberhumer
   <markus@oberhumer.com>
 */



#if defined(LZO_TEST_DECOMPRESS_OVERRUN)
#  if !defined(LZO_TEST_DECOMPRESS_OVERRUN_INPUT)
#    define LZO_TEST_DECOMPRESS_OVERRUN_INPUT		2
#  endif
#  if !defined(LZO_TEST_DECOMPRESS_OVERRUN_OUTPUT)
#    define LZO_TEST_DECOMPRESS_OVERRUN_OUTPUT		2
#  endif
#  if !defined(LZO_TEST_DECOMPRESS_OVERRUN_LOOKBEHIND)
#    define LZO_TEST_DECOMPRESS_OVERRUN_LOOKBEHIND
#  endif
#endif


/***********************************************************************
// Overrun detection is internally handled by these macros:
//
//   TEST_IP    test input overrun at loop begin
//   NEED_IP    test input overrun at every input byte
//
//   TEST_OP    test output overrun at loop begin
//   NEED_OP    test output overrun at every output byte
//
//   TEST_LOOKBEHIND    test match postion
//
// The fastest decompressor results when testing for no overruns
// and using LZO_EOF_CODE.
************************************************************************/

#undef TEST_IP
#undef TEST_OP
#undef TEST_LOOKBEHIND
#undef NEED_IP
#undef NEED_OP
#undef HAVE_TEST_IP
#undef HAVE_TEST_OP
#undef HAVE_NEED_IP
#undef HAVE_NEED_OP
#undef HAVE_ANY_IP
#undef HAVE_ANY_OP


#if defined(LZO_TEST_DECOMPRESS_OVERRUN_INPUT)
#  if (LZO_TEST_DECOMPRESS_OVERRUN_INPUT >= 1)
#    define TEST_IP				(ip < ip_end)
#  endif
#  if (LZO_TEST_DECOMPRESS_OVERRUN_INPUT >= 2)
#    define NEED_IP(x) \
			if ((lzo_uint)(ip_end - ip) < (lzo_uint)(x))  goto input_overrun
#  endif
#endif

#if defined(LZO_TEST_DECOMPRESS_OVERRUN_OUTPUT)
#  if (LZO_TEST_DECOMPRESS_OVERRUN_OUTPUT >= 1)
#    define TEST_OP				(op <= op_end)
#  endif
#  if (LZO_TEST_DECOMPRESS_OVERRUN_OUTPUT >= 2)
#    undef TEST_OP				/* don't need both of the tests here */
#    define NEED_OP(x) \
			if ((lzo_uint)(op_end - op) < (lzo_uint)(x))  goto output_overrun
#  endif
#endif

#if defined(LZO_TEST_DECOMPRESS_OVERRUN_LOOKBEHIND)
#  define TEST_LOOKBEHIND(m_pos,out)	if (m_pos < out) goto lookbehind_overrun
#else
#  define TEST_LOOKBEHIND(m_pos,op)		((void) 0)
#endif


#if !defined(LZO_EOF_CODE) && !defined(TEST_IP)
   /* if we have no EOF code, we have to test for the end of the input */
#  define TEST_IP				(ip < ip_end)
#endif


#if defined(TEST_IP)
#  define HAVE_TEST_IP
#else
#  define TEST_IP				1
#endif
#if defined(TEST_OP)
#  define HAVE_TEST_OP
#else
#  define TEST_OP				1
#endif

#if defined(NEED_IP)
#  define HAVE_NEED_IP
#else
#  define NEED_IP(x)			((void) 0)
#endif
#if defined(NEED_OP)
#  define HAVE_NEED_OP
#else
#  define NEED_OP(x)			((void) 0)
#endif


#if defined(HAVE_TEST_IP) || defined(HAVE_NEED_IP)
#  define HAVE_ANY_IP
#endif
#if defined(HAVE_TEST_OP) || defined(HAVE_NEED_OP)
#  define HAVE_ANY_OP
#endif



/*
vi:ts=4:et
*/

#define SLOW_MEMCPY(a,b,l)		{ do *a++ = *b++; while (--l > 0); }
#if 1 && defined(HAVE_MEMCPY)
#  if !defined(__LZO_DOS16) && !defined(__LZO_WIN16)
#    define FAST_MEMCPY(a,b,l)	{ memcpy(a,b,l); a += l; }
#  endif
#endif

#if 1 && defined(FAST_MEMCPY)
#  define DICT_MEMMOVE(op,m_pos,m_len,m_off) \
		if (m_off >= (m_len)) \
			FAST_MEMCPY(op,m_pos,m_len) \
		else \
			SLOW_MEMCPY(op,m_pos,m_len)
#else
#  define DICT_MEMMOVE(op,m_pos,m_len,m_off) \
		SLOW_MEMCPY(op,m_pos,m_len)
#endif

#if !defined(FAST_MEMCPY)
#  define FAST_MEMCPY	SLOW_MEMCPY
#endif


#define COPY_DICT_DICT(m_len,m_off) \
	{ \
		register const lzo_byte *m_pos; \
		m_off -= (lzo_moff_t) (op - out); assert(m_off > 0); \
		if (m_off > dict_len) goto lookbehind_overrun; \
		m_pos = dict_end - m_off; \
		if (m_len > m_off) \
		{ \
			m_len -= m_off; \
			FAST_MEMCPY(op,m_pos,m_off) \
			m_pos = out; \
			SLOW_MEMCPY(op,m_pos,m_len) \
		} \
		else \
			FAST_MEMCPY(op,m_pos,m_len) \
	}

#define COPY_DICT(m_len,m_off) \
	assert(m_len >= 2); assert(m_off > 0); assert(op > out); \
	if (m_off <= (lzo_moff_t) (op - out)) \
	{ \
		register const lzo_byte *m_pos = op - m_off; \
		DICT_MEMMOVE(op,m_pos,m_len,m_off) \
	} \
	else \
		COPY_DICT_DICT(m_len,m_off)




/* lzo1x_d.ch -- implementation of the LZO1X decompression algorithm

   This file is part of the LZO real-time data compression library.

   Copyright (C) 1996-2002 Markus Franz Xaver Johannes Oberhumer
   All Rights Reserved.

   The LZO library is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   The LZO library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the LZO library; see the file COPYING.
   If not, write to the Free Software Foundation, Inc.,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

   Markus F.X.J. Oberhumer
   <markus@oberhumer.com>
 */





#undef __COPY4
#define __COPY4(dst,src) 	* (lzo_uint32p)(dst) = * (const lzo_uint32p)(src)

#undef COPY4
#if defined(LZO_UNALIGNED_OK_4)
#  define COPY4(dst,src)    __COPY4(dst,src)
#elif defined(LZO_ALIGNED_OK_4)
#  define COPY4(dst,src)    __COPY4((lzo_ptr_t)(dst),(lzo_ptr_t)(src))
#endif


/***********************************************************************
// decompress a block of data.
************************************************************************/


static int lzo1x_decompress_dict_safe(const lzo_byte* in,lzo_uint in_len,lzo_byte* out,lzo_uintp out_len,lzo_voidp wrkmem,const lzo_byte* dict,lzo_uint dict_len)
{
	register lzo_byte *op;
	register const lzo_byte *ip;
	register lzo_uint t;
#if defined(COPY_DICT)
	lzo_uint m_off;
	const lzo_byte *dict_end;
#else
	register const lzo_byte *m_pos;
#endif

	const lzo_byte * const ip_end = in + in_len;
#if defined(HAVE_ANY_OP)
	lzo_byte * const op_end = out + *out_len;
#endif
#if defined(LZO1Z)
	lzo_uint last_m_off = 0;
#endif

	LZO_UNUSED(wrkmem);

#if defined(__LZO_QUERY_DECOMPRESS)
	if (__LZO_IS_DECOMPRESS_QUERY(in,in_len,out,out_len,wrkmem))
		return __LZO_QUERY_DECOMPRESS(in,in_len,out,out_len,wrkmem,0,0);
#endif

#if defined(COPY_DICT)
	if (dict)
	{
		if (dict_len > M4_MAX_OFFSET)
		{
			dict += dict_len - M4_MAX_OFFSET;
			dict_len = M4_MAX_OFFSET;
		}
		dict_end = dict + dict_len;
	}
	else
	{
		dict_len = 0;
		dict_end = NULL;
	}
#endif /* COPY_DICT */

	*out_len = 0;

	op = out;
	ip = in;

	if (*ip > 17)
	{
		t = *ip++ - 17;
		if (t < 4)
			goto match_next;
		assert(t > 0); NEED_OP(t); NEED_IP(t+1);
		do *op++ = *ip++; while (--t > 0);
		goto first_literal_run;
	}

	while (TEST_IP && TEST_OP)
	{
		t = *ip++;
		if (t >= 16)
			goto match;
		/* a literal run */
		if (t == 0)
		{
			NEED_IP(1);
			while (*ip == 0)
			{
				t += 255;
				ip++;
				NEED_IP(1);
			}
			t += 15 + *ip++;
		}
		/* copy literals */
		assert(t > 0); NEED_OP(t+3); NEED_IP(t+4);
#if defined(LZO_UNALIGNED_OK_4) || defined(LZO_ALIGNED_OK_4)
#if !defined(LZO_UNALIGNED_OK_4)
		if (PTR_ALIGNED2_4(op,ip))
		{
#endif
		COPY4(op,ip);
		op += 4; ip += 4;
		if (--t > 0)
		{
			if (t >= 4)
			{
				do {
                    COPY4(op,ip);
					op += 4; ip += 4; t -= 4;
				} while (t >= 4);
				if (t > 0) do *op++ = *ip++; while (--t > 0);
			}
			else
				do *op++ = *ip++; while (--t > 0);
		}
#if !defined(LZO_UNALIGNED_OK_4)
		}
		else
#endif
#endif
#if !defined(LZO_UNALIGNED_OK_4)
		{
			*op++ = *ip++; *op++ = *ip++; *op++ = *ip++;
			do *op++ = *ip++; while (--t > 0);
		}
#endif


first_literal_run:


		t = *ip++;
		if (t >= 16)
			goto match;
#if defined(COPY_DICT)
#if defined(LZO1Z)
		m_off = (1 + M2_MAX_OFFSET) + (t << 6) + (*ip++ >> 2);
		last_m_off = m_off;
#else
		m_off = (1 + M2_MAX_OFFSET) + (t >> 2) + (*ip++ << 2);
#endif
		NEED_OP(3);
		t = 3; COPY_DICT(t,m_off)
#else /* !COPY_DICT */
#if defined(LZO1Z)
		t = (1 + M2_MAX_OFFSET) + (t << 6) + (*ip++ >> 2);
		m_pos = op - t;
		last_m_off = t;
#else
		m_pos = op - (1 + M2_MAX_OFFSET);
		m_pos -= t >> 2;
		m_pos -= *ip++ << 2;
#endif
		TEST_LOOKBEHIND(m_pos,out); NEED_OP(3);
		*op++ = *m_pos++; *op++ = *m_pos++; *op++ = *m_pos;
#endif /* COPY_DICT */
		goto match_done;


		/* handle matches */
		while (TEST_IP && TEST_OP)
		{
match:
			if (t >= 64)				/* a M2 match */
			{
#if defined(COPY_DICT)
#if defined(LZO1X)
				m_off = 1 + ((t >> 2) & 7) + (*ip++ << 3);
				t = (t >> 5) - 1;
#elif defined(LZO1Y)
				m_off = 1 + ((t >> 2) & 3) + (*ip++ << 2);
				t = (t >> 4) - 3;
#elif defined(LZO1Z)
				m_off = t & 0x1f;
				if (m_off >= 0x1c)
					m_off = last_m_off;
				else
				{
					m_off = 1 + (m_off << 6) + (*ip++ >> 2);
					last_m_off = m_off;
				}
				t = (t >> 5) - 1;
#endif
#else /* !COPY_DICT */
#if defined(LZO1X)
				m_pos = op - 1;
				m_pos -= (t >> 2) & 7;
				m_pos -= *ip++ << 3;
				t = (t >> 5) - 1;
#elif defined(LZO1Y)
				m_pos = op - 1;
				m_pos -= (t >> 2) & 3;
				m_pos -= *ip++ << 2;
				t = (t >> 4) - 3;
#elif defined(LZO1Z)
				{
					lzo_uint off = t & 0x1f;
					m_pos = op;
					if (off >= 0x1c)
					{
						assert(last_m_off > 0);
						m_pos -= last_m_off;
					}
					else
					{
						off = 1 + (off << 6) + (*ip++ >> 2);
						m_pos -= off;
						last_m_off = off;
					}
				}
				t = (t >> 5) - 1;
#endif
				TEST_LOOKBEHIND(m_pos,out); assert(t > 0); NEED_OP(t+3-1);
				goto copy_match;
#endif /* COPY_DICT */
			}
			else if (t >= 32)			/* a M3 match */
			{
				t &= 31;
				if (t == 0)
				{
					NEED_IP(1);
					while (*ip == 0)
					{
						t += 255;
						ip++;
						NEED_IP(1);
					}
					t += 31 + *ip++;
				}
#if defined(COPY_DICT)
#if defined(LZO1Z)
				m_off = 1 + (ip[0] << 6) + (ip[1] >> 2);
				last_m_off = m_off;
#else
				m_off = 1 + (ip[0] >> 2) + (ip[1] << 6);
#endif
#else /* !COPY_DICT */
#if defined(LZO1Z)
				{
					lzo_uint off = 1 + (ip[0] << 6) + (ip[1] >> 2);
					m_pos = op - off;
					last_m_off = off;
				}
#elif defined(LZO_UNALIGNED_OK_2) && (LZO_BYTE_ORDER == LZO_LITTLE_ENDIAN)
				m_pos = op - 1;
				m_pos -= (* (const lzo_ushortp) ip) >> 2;
#else
				m_pos = op - 1;
				m_pos -= (ip[0] >> 2) + (ip[1] << 6);
#endif
#endif /* COPY_DICT */
				ip += 2;
			}
			else if (t >= 16)			/* a M4 match */
			{
#if defined(COPY_DICT)
				m_off = (t & 8) << 11;
#else /* !COPY_DICT */
				m_pos = op;
				m_pos -= (t & 8) << 11;
#endif /* COPY_DICT */
				t &= 7;
				if (t == 0)
				{
					NEED_IP(1);
					while (*ip == 0)
					{
						t += 255;
						ip++;
						NEED_IP(1);
					}
					t += 7 + *ip++;
				}
#if defined(COPY_DICT)
#if defined(LZO1Z)
				m_off += (ip[0] << 6) + (ip[1] >> 2);
#else
				m_off += (ip[0] >> 2) + (ip[1] << 6);
#endif
				ip += 2;
				if (m_off == 0)
					goto eof_found;
				m_off += 0x4000;
#if defined(LZO1Z)
				last_m_off = m_off;
#endif
#else /* !COPY_DICT */
#if defined(LZO1Z)
				m_pos -= (ip[0] << 6) + (ip[1] >> 2);
#elif defined(LZO_UNALIGNED_OK_2) && (LZO_BYTE_ORDER == LZO_LITTLE_ENDIAN)
				m_pos -= (* (const lzo_ushortp) ip) >> 2;
#else
				m_pos -= (ip[0] >> 2) + (ip[1] << 6);
#endif
				ip += 2;
				if (m_pos == op)
					goto eof_found;
				m_pos -= 0x4000;
#if defined(LZO1Z)
				last_m_off = op - m_pos;
#endif
#endif /* COPY_DICT */
			}
			else							/* a M1 match */
			{
#if defined(COPY_DICT)
#if defined(LZO1Z)
				m_off = 1 + (t << 6) + (*ip++ >> 2);
				last_m_off = m_off;
#else
				m_off = 1 + (t >> 2) + (*ip++ << 2);
#endif
				NEED_OP(2);
				t = 2; COPY_DICT(t,m_off)
#else /* !COPY_DICT */
#if defined(LZO1Z)
				t = 1 + (t << 6) + (*ip++ >> 2);
				m_pos = op - t;
				last_m_off = t;
#else
				m_pos = op - 1;
				m_pos -= t >> 2;
				m_pos -= *ip++ << 2;
#endif
				TEST_LOOKBEHIND(m_pos,out); NEED_OP(2);
				*op++ = *m_pos++; *op++ = *m_pos;
#endif /* COPY_DICT */
				goto match_done;
			}

			/* copy match */
#if defined(COPY_DICT)

			NEED_OP(t+3-1);
			t += 3-1; COPY_DICT(t,m_off)

#else /* !COPY_DICT */

			TEST_LOOKBEHIND(m_pos,out); assert(t > 0); NEED_OP(t+3-1);
#if defined(LZO_UNALIGNED_OK_4) || defined(LZO_ALIGNED_OK_4)
#if !defined(LZO_UNALIGNED_OK_4)
			if (t >= 2 * 4 - (3 - 1) && PTR_ALIGNED2_4(op,m_pos))
			{
				assert((op - m_pos) >= 4);	/* both pointers are aligned */
#else
			if (t >= 2 * 4 - (3 - 1) && (op - m_pos) >= 4)
			{
#endif
                COPY4(op,m_pos);
				op += 4; m_pos += 4; t -= 4 - (3 - 1);
				do {
                    COPY4(op,m_pos);
					op += 4; m_pos += 4; t -= 4;
				} while (t >= 4);
				if (t > 0) do *op++ = *m_pos++; while (--t > 0);
			}
			else
#endif
			{
copy_match:
				*op++ = *m_pos++; *op++ = *m_pos++;
				do *op++ = *m_pos++; while (--t > 0);
			}

#endif /* COPY_DICT */

match_done:
#if defined(LZO1Z)
			t = ip[-1] & 3;
#else
			t = ip[-2] & 3;
#endif
			if (t == 0)
				break;

			/* copy literals */
match_next:
			assert(t > 0); NEED_OP(t); NEED_IP(t+1);
			do *op++ = *ip++; while (--t > 0);
			t = *ip++;
		}
	}

#if defined(HAVE_TEST_IP) || defined(HAVE_TEST_OP)
	/* no EOF code was found */
	*out_len = op - out;
	return LZO_E_EOF_NOT_FOUND;
#endif

eof_found:
	assert(t == 1);
	*out_len = op - out;
	return (ip == ip_end ? LZO_E_OK :
	       (ip < ip_end  ? LZO_E_INPUT_NOT_CONSUMED : LZO_E_INPUT_OVERRUN));


#if defined(HAVE_NEED_IP)
input_overrun:
	*out_len = op - out;
	return LZO_E_INPUT_OVERRUN;
#endif

#if defined(HAVE_NEED_OP)
output_overrun:
	*out_len = op - out;
	return LZO_E_OUTPUT_OVERRUN;
#endif

#if defined(LZO_TEST_DECOMPRESS_OVERRUN_LOOKBEHIND)
lookbehind_overrun:
	*out_len = op - out;
	return LZO_E_LOOKBEHIND_OVERRUN;
#endif
}


/*
vi:ts=4:et
*/

static unsigned test_callbacks, test_in_count, test_out_count;
static void check_test(bool value){if(!value){fprintf(stderr,"LZO fixture failure\n");exit(1);}}
static void __cdecl test_progress(lzo_uint input_count,lzo_uint output_count){
    check_test(input_count>=test_in_count && output_count>=test_out_count);
    test_in_count=input_count;test_out_count=output_count;++test_callbacks;
}
static void test_roundtrip(unsigned length,unsigned pattern,unsigned settings,unsigned dictionary_length){
    static unsigned char input[50032],original[50032],packed[110032],unpacked[50032],dictionary[70000];
    static unsigned char memory[sizeof(lzo_swd_t)+32];
    unsigned state=123456789U;
    for(unsigned i=0;i<70000;++i){state=state*1664525U+1013904223U;dictionary[i]=(unsigned char)(state>>24);}
    memset(input,0xa7,sizeof(input));memset(packed,0x6d,sizeof(packed));memset(unpacked,0x5c,sizeof(unpacked));memset(memory,0x93,sizeof(memory));
    unsigned char* data=input+16;
    for(unsigned j=0;j<length;++j){
        state=state*1664525U+1013904223U;
        if(pattern==0)data[j]=0;
        else if(pattern==1)data[j]=(unsigned char)(j%31);
        else if(pattern==2)data[j]=(unsigned char)(state>>24);
        else if(pattern==3)data[j]=(unsigned char)((j/257)%17);
        else if(pattern==4 && dictionary_length)data[j]=dictionary[(dictionary_length-1-j%dictionary_length)];
        else if(pattern==5 && j>=17000)data[j]=data[j-17000];
        else data[j]=(unsigned char)((state>>24)^(j%127));
    }
    if(pattern==4 && dictionary_length){
        unsigned available=dictionary_length<length?dictionary_length:length;
        memcpy(data,dictionary+dictionary_length-available,available);
    }
    memcpy(original,input,sizeof(input));
    const unsigned good[5]={0,4,8,32,32},lazy[5]={0,4,16,128,2048},nice[5]={0,8,32,128,2048},chain[5]={0,4,16,256,2048};
    const int tries[5]={-1,0,1,2,3};
    test_callbacks=test_in_count=test_out_count=0;
    lzo_uint packed_length=0xdeadbeefU;
    int result=lzo1x_999_compress_internal(data,length,packed+16,&packed_length,memory+16,
        dictionary_length?dictionary:0,dictionary_length,test_progress,tries[settings],good[settings],lazy[settings],nice[settings],chain[settings],settings&1U);
    check_test(result==0 && packed_length>=3 && packed_length<=length+length/16+64+3);
    check_test(test_callbacks>=1 && test_in_count==length && test_out_count==packed_length);
    check_test(memcmp(input,original,sizeof(input))==0);
    for(unsigned k=0;k<16;++k){check_test(packed[k]==0x6d && packed[16+packed_length+k]==0x6d);check_test(memory[k]==0x93 && memory[sizeof(memory)-16+k]==0x93);}
    check_test(packed[16+packed_length-3]==17 && packed[16+packed_length-2]==0 && packed[16+packed_length-1]==0);
    lzo_uint restored_length=50000;
    result=lzo1x_decompress_dict_safe(packed+16,packed_length,unpacked+16,&restored_length,0,dictionary_length?dictionary:0,dictionary_length);
    check_test(result==0 && restored_length==length && memcmp(data,unpacked+16,length)==0);
    for(unsigned p=0;p<16;++p)check_test(unpacked[p]==0x5c && unpacked[16+length+p]==0x5c);
    if(length==0)check_test(packed_length==3);
    if(length==1)check_test(packed_length==5 && packed[16]==18 && packed[17]==data[0]);
}
int main(){
    const unsigned lengths[10]={0,1,3,4,31,256,2048,4097,20000,50000};
    const unsigned dictionaries[3]={0,64,70000};
    for(unsigned size=0;size<10;++size)for(unsigned pattern=0;pattern<6;++pattern)
        for(unsigned settings=0;settings<5;++settings)for(unsigned dictionary=0;dictionary<3;++dictionary)
            test_roundtrip(lengths[size],pattern,settings,dictionaries[dictionary]);
    puts("00c07af0_TEST PASS");return 0;
}