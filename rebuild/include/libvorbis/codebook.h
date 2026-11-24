#ifndef FABLE_LIBVORBIS_CODEBOOK_H
#define FABLE_LIBVORBIS_CODEBOOK_H
#ifdef FABLE_RETAIL_LIBVORBIS_CODEBOOK_PASS27

/*
Copyright (c) 2002, Xiph.org Foundation

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

- Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

- Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

- Neither the name of the Xiph.org Foundation nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION
OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// Xiph libvorbis 1.0.1 codebook.h types for sharedbook.c.
typedef unsigned int ogg_uint32_t;
typedef struct static_codebook {                   // libvorbis 1.0.1 codebook.h, 0x34 bytes
    long  dim;                                     // +0x00
    long  entries;                                 // +0x04
    long* lengthlist;                              // +0x08
    int   maptype;                                 // +0x0c
    long  q_min;                                   // +0x10
    long  q_delta;                                 // +0x14
    int   q_quant;                                 // +0x18
    int   q_sequencep;                             // +0x1c
    long* quantlist;                               // +0x20
    struct encode_aux_nearestmatch* nearest_tree;  // +0x24
    struct encode_aux_threshmatch*  thresh_tree;   // +0x28
    struct encode_aux_pigeonhole*   pigeon_tree;   // +0x2c
    int   allocedp;                                // +0x30
} static_codebook;
typedef struct codebook {                          // libvorbis 1.0.1 codebook.h, 0x2c bytes
    long                   dim;                    // +0x00
    long                   entries;                // +0x04
    long                   used_entries;           // +0x08
    const static_codebook* c;                      // +0x0c
    float*                 valuelist;              // +0x10
    ogg_uint32_t*          codelist;               // +0x14
    int*                   dec_index;              // +0x18
    char*                  dec_codelengths;        // +0x1c
    ogg_uint32_t*          dec_firsttable;         // +0x20
    int                    dec_firsttablen;        // +0x24
    int                    dec_maxlength;          // +0x28
} codebook;

#endif
#endif
