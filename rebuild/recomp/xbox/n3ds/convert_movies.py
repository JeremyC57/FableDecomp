#!/usr/bin/env python3
"""Converts Fable's XMV movies into the New 3DS build's native movie files.

    convert_movies.py <extracted disc>/Data/Video <out folder> [--crf 26] [--audio-rate 22050]

Copy the output folder to sdmc:/3ds/fable/movies/. Each .fmv holds:
  - video: H.264 baseline, 400x240 rotated to the top screen's 240x400 framebuffer layout, constant
    29.97 fps, one access unit per frame (the New 3DS hardware decoder writes it straight to the
    framebuffer, as in devkitPro's mvd example)
  - audio: IMA ADPCM (as the XMVs carry it), resampled
  - a frame table, so the player streams from the SD card
Only one copy of movies that exist as both NAME.xmv and NAME_PAL.xmv is converted (the game asks
for the NTSC names); the player falls back from NAME_PAL to NAME.

Needs ffmpeg with the xmv demuxer and libx264 on the PATH.
"""
import argparse
import os
import re
import struct
import subprocess
import sys
import tempfile

MAGIC = b"FMV3"
FPS_NUM, FPS_DEN = 30000, 1001
HEADER = struct.Struct("<4s15I")  # magic + 15 words (see the player, n3ds/movie.cpp)
# The player sizes the decoder's work buffer for 5 reference frames: keep ref= in step with it.


def run(cmd, optional=False):
    r = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    # XMVs end with padding the demuxer reports as an I/O error after the last packet.
    if r.returncode != 0 and not os.path.exists(cmd[-1]):
        if optional:
            return False
        sys.exit("ffmpeg failed: " + r.stderr.decode(errors="replace"))
    return True


def wav_chunks(data):
    if data[:4] != b"RIFF" or data[8:12] != b"WAVE":
        raise ValueError("not a WAV file")
    pos, fmt, body = 12, None, None
    while pos + 8 <= len(data):
        cid, size = data[pos:pos + 4], struct.unpack_from("<I", data, pos + 4)[0]
        if cid == b"fmt ":
            fmt = data[pos + 8:pos + 8 + size]
        elif cid == b"data":
            body = data[pos + 8:pos + 8 + size]
        pos += 8 + size + (size & 1)
    return fmt, body


def convert(src, dst, crf, audio_rate):
    with tempfile.TemporaryDirectory() as tmp:
        h264, wav = os.path.join(tmp, "v.h264"), os.path.join(tmp, "a.wav")
        run(["ffmpeg", "-v", "error", "-y", "-i", src, "-an", "-r", f"{FPS_NUM}/{FPS_DEN}",
             "-vf", "scale=400:240:flags=lanczos,transpose=clock", "-c:v", "libx264", "-profile:v", "baseline",
             "-level", "3.1", "-preset", "slow", "-crf", str(crf), "-x264-params", "aud=1:keyint=60:bframes=0:ref=5",
             "-pix_fmt", "yuv420p", "-f", "h264", h264])
        has_audio = run(["ffmpeg", "-v", "error", "-y", "-i", src, "-vn", "-ar", str(audio_rate), "-ac", "2",
                         "-c:a", "adpcm_ima_wav", wav], optional=True)  # some movies are silent
        video = open(h264, "rb").read()
        fmt, audio = wav_chunks(open(wav, "rb").read()) if has_audio else (None, b"")
    # One access unit per frame: split at the access unit delimiters (aud=1).
    starts = [m.start() for m in re.finditer(b"\x00\x00\x00\x01\x09", video)]
    if not starts:
        raise ValueError("no access unit delimiters in the H.264 stream")
    frames = [(s, (starts[i + 1] if i + 1 < len(starts) else len(video)) - s) for i, s in enumerate(starts)]
    tag, channels, rate, _, block_align = struct.unpack_from("<HHIIH", fmt, 0) if fmt else (0x11, 0, 0, 0, 0)
    if tag != 0x11:
        raise ValueError(f"unexpected audio format 0x{tag:X}")
    table_off = HEADER.size
    video_off = table_off + 8 * len(frames)
    audio_off = video_off + len(video)
    with open(dst, "wb") as f:
        f.write(HEADER.pack(MAGIC, 1, 240, 400, FPS_NUM, FPS_DEN, len(frames), rate, channels, block_align,
                            table_off, video_off, len(video), audio_off, len(audio), 0))
        for off, size in frames:
            f.write(struct.pack("<II", off, size))
        f.write(video)
        f.write(audio)
    return len(frames), os.path.getsize(dst)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("video_dir")
    ap.add_argument("out_dir")
    ap.add_argument("--crf", type=int, default=26, help="x264 quality (higher = smaller, default 26)")
    ap.add_argument("--audio-rate", type=int, default=22050, help="audio sample rate (default 22050)")
    args = ap.parse_args()
    os.makedirs(args.out_dir, exist_ok=True)
    names = sorted(n for n in os.listdir(args.video_dir) if n.lower().endswith(".xmv"))
    lower = {n.lower() for n in names}
    total_in = total_out = 0
    for n in names:
        stem = n[:-4]
        if re.search(r"_pal$", stem, re.I) and (re.sub(r"_pal$", "", stem, flags=re.I) + ".xmv").lower() in lower:
            continue  # the NTSC copy is converted instead
        src, dst = os.path.join(args.video_dir, n), os.path.join(args.out_dir, stem + ".fmv")
        if os.path.exists(dst) and os.path.getmtime(dst) >= os.path.getmtime(src):
            total_in += os.path.getsize(src)
            total_out += os.path.getsize(dst)
            continue  # converted by an earlier run
        frames, size = convert(src, dst, args.crf, args.audio_rate)
        total_in += os.path.getsize(src)
        total_out += size
        print(f"{n}: {frames} frames, {os.path.getsize(src) // 1024} KB -> {size // 1024} KB", flush=True)
    print(f"total: {total_in / 2**20:.0f} MB -> {total_out / 2**20:.0f} MB")


if __name__ == "__main__":
    main()
