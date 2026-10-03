package org.fablerecomp;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.RectF;
import android.os.Handler;
import android.os.Looper;
import android.util.SparseArray;
import android.view.HapticFeedbackConstants;
import android.view.MotionEvent;
import android.view.View;

import java.util.ArrayList;
import java.util.List;

/**
 * On-screen controls drawn over the game: a floating left stick (movement), Xbox-style buttons
 * (fed to the game as a virtual controller, mapped like a real one) and, everywhere else, a
 * touchpad: dragging moves the mouse (camera, menu cursor), a tap clicks, tap-and-hold holds the
 * left button. The round button at the top hides them; touches then reach the game as a mouse.
 * The button beside it shows the on-screen keyboard (profile names).
 */
public class TouchControls extends View {
    static native void nativeSetPad(boolean active, int buttons, float lx, float ly, float rx, float ry, float lt, float rt);
    static native void nativeMouseMotion(int dx, int dy);
    static native void nativeMouseButton(int button, boolean down);
    static native void nativeToggleKeyboard();

    // XINPUT_GAMEPAD_* bits
    static final int DUP = 0x1, DDOWN = 0x2, DLEFT = 0x4, DRIGHT = 0x8, START = 0x10, BACK = 0x20,
        LTHUMB = 0x40, RTHUMB = 0x80, LB = 0x100, RB = 0x200, A = 0x1000, B = 0x2000, X = 0x4000, Y = 0x8000;
    static final int LT = 0x10000, RT = 0x20000;  // triggers (sent as analog values)

    private static final class Button {
        final String label;
        final int bit;
        final int color;
        final boolean round;
        final RectF r = new RectF();
        int pressed;  // pointer count
        Button(String label, int bit, int color, boolean round) { this.label = label; this.bit = bit; this.color = color; this.round = round; }
    }

    private enum Role { STICK, BUTTON, PAD, TOGGLE, KEYBOARD }
    private static final class Touch {
        Role role;
        Button button;
        float x0, y0, x, y;
        long t0;
        boolean moved, holding;
        float fx, fy;  // fractional mouse motion
    }

    private final List<Button> buttons = new ArrayList<>();
    private final SparseArray<Touch> touches = new SparseArray<>();
    private final Paint fill = new Paint(Paint.ANTI_ALIAS_FLAG), stroke = new Paint(Paint.ANTI_ALIAS_FLAG), text = new Paint(Paint.ANTI_ALIAS_FLAG);
    private final RectF toggle = new RectF(), keyboard = new RectF();
    private final Handler handler = new Handler(Looper.getMainLooper());
    private final float dp;
    private boolean visible;
    private Touch stick;
    private float stickR;
    private int lastButtons = -1;
    private float lastLx = 2, lastLy = 2, lastLt = -1, lastRt = -1;

    public TouchControls(Context c, boolean visible) {
        super(c);
        this.visible = visible;
        dp = getResources().getDisplayMetrics().density;
        stroke.setStyle(Paint.Style.STROKE);
        stroke.setStrokeWidth(2 * dp);
        text.setTextAlign(Paint.Align.CENTER);
        text.setFakeBoldText(true);
        int a = Color.rgb(96, 176, 64), b = Color.rgb(200, 60, 50), x = Color.rgb(60, 110, 210), y = Color.rgb(220, 180, 40), g = Color.rgb(150, 150, 150);
        buttons.add(new Button("A", A, a, true));
        buttons.add(new Button("B", B, b, true));
        buttons.add(new Button("X", X, x, true));
        buttons.add(new Button("Y", Y, y, true));
        buttons.add(new Button("LB", LB, g, false));
        buttons.add(new Button("LT", LT, g, false));
        buttons.add(new Button("RB", RB, g, false));
        buttons.add(new Button("RT", RT, g, false));
        buttons.add(new Button("◀◀", BACK, g, false));
        buttons.add(new Button("☰", START, g, false));
        buttons.add(new Button("▲", DUP, g, true));
        buttons.add(new Button("▼", DDOWN, g, true));
        buttons.add(new Button("◀", DLEFT, g, true));
        buttons.add(new Button("▶", DRIGHT, g, true));
        buttons.add(new Button("L3", LTHUMB, g, true));
        buttons.add(new Button("R3", RTHUMB, g, true));
        post(this::send);
    }

    private Button find(int bit) {
        for (Button b : buttons) if (b.bit == bit) return b;
        return null;
    }

    private void place(int bit, float cx, float cy, float w, float h) {
        Button b = find(bit);
        b.r.set(cx - w / 2, cy - h / 2, cx + w / 2, cy + h / 2);
    }

    @Override
    protected void onSizeChanged(int w, int h, int ow, int oh) {
        float r = 30 * dp, off = 58 * dp;
        float cx = w - 120 * dp, cy = h - 110 * dp;  // face buttons
        place(A, cx, cy + off, 2 * r, 2 * r);
        place(B, cx + off, cy, 2 * r, 2 * r);
        place(X, cx - off, cy, 2 * r, 2 * r);
        place(Y, cx, cy - off, 2 * r, 2 * r);
        float sw = 72 * dp, sh = 40 * dp;
        place(LT, 56 * dp, 36 * dp, sw, sh);
        place(LB, 56 * dp, 86 * dp, sw, sh);
        place(RT, w - 56 * dp, 36 * dp, sw, sh);
        place(RB, w - 56 * dp, 86 * dp, sw, sh);
        place(BACK, w / 2f - 80 * dp, 30 * dp, 56 * dp, 34 * dp);
        place(START, w / 2f + 80 * dp, 30 * dp, 56 * dp, 34 * dp);
        float dr = 22 * dp, doff = 40 * dp, dx = 150 * dp, dy = h * 0.42f;  // d-pad
        place(DUP, dx, dy - doff, 2 * dr, 2 * dr);
        place(DDOWN, dx, dy + doff, 2 * dr, 2 * dr);
        place(DLEFT, dx - doff, dy, 2 * dr, 2 * dr);
        place(DRIGHT, dx + doff, dy, 2 * dr, 2 * dr);
        place(LTHUMB, 40 * dp, h - 40 * dp, 40 * dp, 40 * dp);
        place(RTHUMB, cx - off - 50 * dp, cy + off + 10 * dp, 40 * dp, 40 * dp);
        toggle.set(w / 2f - 20 * dp, 12 * dp, w / 2f + 20 * dp, 52 * dp);
        keyboard.set(w / 2f + 150 * dp, 12 * dp, w / 2f + 190 * dp, 52 * dp);
        stickR = 64 * dp;
        text.setTextSize(16 * dp);
    }

    private boolean inStickZone(float x, float y) {
        return x < getWidth() * 0.45f && y > getHeight() * 0.5f;
    }

    private Button hit(float x, float y) {
        float slop = 6 * dp;
        for (Button b : buttons)
            if (x >= b.r.left - slop && x <= b.r.right + slop && y >= b.r.top - slop && y <= b.r.bottom + slop) return b;
        return null;
    }

    @Override
    public boolean onTouchEvent(MotionEvent e) {
        int action = e.getActionMasked();
        int idx = e.getActionIndex();
        if (!visible) {
            // Only the toggle is live; everything else falls through to the game as a mouse.
            if (action == MotionEvent.ACTION_DOWN && toggle.contains(e.getX(), e.getY())) {
                setShown(true);
                return true;
            }
            if (action == MotionEvent.ACTION_DOWN && keyboard.contains(e.getX(), e.getY())) {
                toggleKeyboard();
                return true;
            }
            return false;
        }
        switch (action) {
            case MotionEvent.ACTION_DOWN:
            case MotionEvent.ACTION_POINTER_DOWN: {
                Touch t = new Touch();
                t.x0 = t.x = e.getX(idx);
                t.y0 = t.y = e.getY(idx);
                t.t0 = e.getEventTime();
                Button b = hit(t.x, t.y);
                if (toggle.contains(t.x, t.y)) t.role = Role.TOGGLE;
                else if (keyboard.contains(t.x, t.y)) t.role = Role.KEYBOARD;
                else if (b != null) {
                    t.role = Role.BUTTON;
                    t.button = b;
                    b.pressed++;
                    performHapticFeedback(HapticFeedbackConstants.VIRTUAL_KEY);
                } else if (stick == null && inStickZone(t.x, t.y)) {
                    t.role = Role.STICK;
                    stick = t;
                } else {
                    t.role = Role.PAD;
                    // tap-and-hold (without moving) holds the left mouse button
                    final int pid = e.getPointerId(idx);
                    handler.postDelayed(() -> {
                        if (touches.get(pid) == t && !t.moved) {
                            t.holding = true;
                            nativeMouseButton(0, true);
                            performHapticFeedback(HapticFeedbackConstants.LONG_PRESS);
                        }
                    }, 350);
                }
                touches.put(e.getPointerId(idx), t);
                break;
            }
            case MotionEvent.ACTION_MOVE:
                for (int i = 0; i < e.getPointerCount(); ++i) {
                    Touch t = touches.get(e.getPointerId(i));
                    if (t == null) continue;
                    float x = e.getX(i), y = e.getY(i);
                    if (Math.hypot(x - t.x0, y - t.y0) > 10 * dp) t.moved = true;
                    if (t.role == Role.PAD) {
                        // screen pixels -> mouse counts, independent of the display density
                        float k = 1.6f / dp;
                        t.fx += (x - t.x) * k;
                        t.fy += (y - t.y) * k;
                        int mx = (int) t.fx, my = (int) t.fy;
                        t.fx -= mx;
                        t.fy -= my;
                        if (mx != 0 || my != 0) nativeMouseMotion(mx, my);
                    } else if (t.role == Role.BUTTON) {
                        // sliding between buttons (e.g. across the face buttons) follows the finger
                        Button b = hit(x, y);
                        if (b != null && b != t.button) {
                            t.button.pressed--;
                            t.button = b;
                            b.pressed++;
                        }
                    }
                    t.x = x;
                    t.y = y;
                }
                break;
            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_POINTER_UP:
            case MotionEvent.ACTION_CANCEL: {
                if (action == MotionEvent.ACTION_CANCEL) {
                    for (int i = 0; i < touches.size(); ++i) release(touches.valueAt(i), false);
                    touches.clear();
                } else {
                    int id = e.getPointerId(idx);
                    Touch t = touches.get(id);
                    if (t != null) {
                        boolean tap = !t.moved && e.getEventTime() - t.t0 < 300;
                        if (t.role == Role.TOGGLE && toggle.contains(e.getX(idx), e.getY(idx))) setShown(false);
                        if (t.role == Role.KEYBOARD && keyboard.contains(e.getX(idx), e.getY(idx))) toggleKeyboard();
                        release(t, tap);
                        touches.remove(id);
                    }
                }
                break;
            }
            default:
                return true;
        }
        send();
        invalidate();
        return true;
    }

    private void release(Touch t, boolean tap) {
        if (t.role == Role.BUTTON) t.button.pressed--;
        else if (t.role == Role.STICK) stick = null;
        else if (t.role == Role.PAD) {
            if (t.holding) nativeMouseButton(0, false);
            else if (tap) {
                nativeMouseButton(0, true);
                handler.postDelayed(() -> nativeMouseButton(0, false), 60);
            }
        }
    }

    private void toggleKeyboard() {
        try {
            nativeToggleKeyboard();
        } catch (UnsatisfiedLinkError ignored) {
        }
    }

    private void setShown(boolean on) {
        for (int i = 0; i < touches.size(); ++i) release(touches.valueAt(i), false);
        touches.clear();
        stick = null;
        visible = on;
        lastButtons = -1;
        send();
        invalidate();
    }

    private void send() {
        int bits = 0;
        for (Button b : buttons) if (b.pressed > 0) bits |= b.bit;
        float lx = 0, ly = 0;
        if (stick != null) {
            lx = (stick.x - stick.x0) / stickR;
            ly = -(stick.y - stick.y0) / stickR;
            float m = (float) Math.hypot(lx, ly);
            if (m > 1) { lx /= m; ly /= m; }
        }
        float lt = (bits & LT) != 0 ? 1 : 0, rt = (bits & RT) != 0 ? 1 : 0;
        int pad = bits & 0xFFFF;
        if (pad == lastButtons && lx == lastLx && ly == lastLy && lt == lastLt && rt == lastRt) return;
        lastButtons = pad; lastLx = lx; lastLy = ly; lastLt = lt; lastRt = rt;
        try {
            nativeSetPad(visible, pad, lx, ly, 0, 0, lt, rt);
        } catch (UnsatisfiedLinkError ignored) {
            // libmain not loaded yet; the next change resends
            lastButtons = -1;
        }
    }

    @Override
    protected void onDraw(Canvas c) {
        // toggle
        fill.setColor(Color.argb(visible ? 110 : 60, 255, 255, 255));
        c.drawOval(toggle, fill);
        stroke.setColor(Color.argb(visible ? 200 : 90, 255, 255, 255));
        c.drawOval(toggle, stroke);
        text.setColor(Color.argb(visible ? 230 : 120, 255, 255, 255));
        drawLabel(c, "◎", toggle);
        c.drawOval(keyboard, fill);
        c.drawOval(keyboard, stroke);
        drawLabel(c, "⌨", keyboard);
        if (!visible) return;
        for (Button b : buttons) {
            int alpha = b.pressed > 0 ? 170 : 70;
            fill.setColor(Color.argb(alpha, Color.red(b.color), Color.green(b.color), Color.blue(b.color)));
            stroke.setColor(Color.argb(180, 255, 255, 255));
            if (b.round) {
                c.drawOval(b.r, fill);
                c.drawOval(b.r, stroke);
            } else {
                c.drawRoundRect(b.r, 10 * dp, 10 * dp, fill);
                c.drawRoundRect(b.r, 10 * dp, 10 * dp, stroke);
            }
            text.setColor(Color.argb(220, 255, 255, 255));
            drawLabel(c, b.label, b.r);
        }
        if (stick != null) {
            fill.setColor(Color.argb(50, 255, 255, 255));
            c.drawCircle(stick.x0, stick.y0, stickR, fill);
            c.drawCircle(stick.x0, stick.y0, stickR, stroke);
            float dx = stick.x - stick.x0, dy = stick.y - stick.y0, m = (float) Math.hypot(dx, dy);
            if (m > stickR) { dx *= stickR / m; dy *= stickR / m; }
            fill.setColor(Color.argb(140, 255, 255, 255));
            c.drawCircle(stick.x0 + dx, stick.y0 + dy, stickR * 0.45f, fill);
        } else {
            // hint where the stick lives
            fill.setColor(Color.argb(25, 255, 255, 255));
            c.drawCircle(getWidth() * 0.2f, getHeight() * 0.75f, stickR, fill);
        }
    }

    private void drawLabel(Canvas c, String s, RectF r) {
        Paint.FontMetrics fm = text.getFontMetrics();
        c.drawText(s, r.centerX(), r.centerY() - (fm.ascent + fm.descent) / 2, text);
    }
}
